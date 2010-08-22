<?php
// $Id: FeedsScheduler.inc,v 1.20 2010/06/19 19:40:43 alexb Exp $

/**
 * @file
 * FeedsScheduler class and related.
 */

/**
 * Describe a scheduler.
 */
interface FeedsSchedulerInterface {

  /**
   * Run Drupal cron.
   */
  public function cron();

  /**
   * Add a feed to the schedule.
   *
   * @param $importer_id
   *   Id of a FeedsImporter object.
   * @param $callback
   *   The callback to invoke on importer. Either 'import' or 'expire'.
   * @param $feed_nid
   *   Feed nid that identifies the source for this configuration.
   */
  public function add($importer_id, $callback, $feed_nid = 0);

  /**
   * Remove a feed from the schedule.
   *
   * @param $feed_nid
   *   Feed nid that identifies the source for this configuration.
   */
  public function remove($importer_id, $callback, $feed_nid = 0);

  /**
   * Work off a given feed identified by $job.
   *
   * @param $job
   *   Array where 'id' key is the id of a FeedsImporter object,
   *   and 'feed_nid' is the feed node id that identifies the
   *   source of a FeedsSource object.
   */
  public function work($job);
}

/**
 * Implementation of FeedsSchedulerInterface.
 *
 * This scheduler uses the last_refreshed_time paradigm: By storing the time
 * when a particular feed was refreshed last rather than storing when a feed
 * should be refreshed next, we gain two advantages:
 *
 * 1) If a feed's import_period setting changes, it has immediate effects -
 *    without batch updating an existing schedule.
 * 2) The time between refreshes will always be scheduled based on when it
 *    has been scheduled last. Less drift occurs.
 *
 * This behavior may change soon: http://drupal.org/node/721428
 */
class FeedsScheduler implements FeedsSchedulerInterface {

  /**
   * Create a single instance of FeedsScheduler.
   */
  public static function instance() {
    static $instance;
    if (!isset($instance)) {
      $class = variable_get('feeds_scheduler_class', 'FeedsScheduler');
      $instance = new $class();
    }
    return $instance;
  }

  /**
   * Protect constructor.
   */
  protected function __construct() {}

  /**
   * Implementation of FeedsSchedulerInterface::cron().
   *
   * Refreshes scheduled feeds.
   *
   * If drupal_queue is present, only pushes refresh tasks to queue and
   * returns. If drupal_queue is not available, works off tasks.
   */
  public function cron() {
    // Check and set scheduler semaphore, take time.
    if (variable_get('feeds_scheduler_cron', FALSE)) {
      watchdog('FeedsScheduler', 'Last cron process did not finish.', array(), WATCHDOG_ERROR);
    }
    variable_set('feeds_scheduler_cron', TRUE);
    $start = time();

    // Release schedule lock where the lock is older than 1 hour.
    db_query("UPDATE {feeds_schedule} SET scheduled = 0 WHERE scheduled < %d", FEEDS_REQUEST_TIME - 3600);

    // Iterate over feed importers, pick $num jobs for each of them and
    // schedule them.
    if ($importers = feeds_importer_load_all()) {
      $num = $this->queue() ? variable_get('feeds_schedule_queue_num', 200) : variable_get('feeds_schedule_num', 5);
      foreach ($importers as $importer) {
        foreach ($importer->getScheduleCallbacks() as $callback) {
          $period = $importer->getSchedulePeriod($callback);
          if ($period != FEEDS_SCHEDULE_NEVER) {
            $result = db_query_range("SELECT feed_nid, id, callback, last_executed_time FROM {feeds_schedule} WHERE id = '%s' AND callback = '%s' AND scheduled = 0 AND (last_executed_time < %d OR last_executed_time = 0) ORDER BY last_executed_time ASC", $importer->id, $callback, FEEDS_REQUEST_TIME - $period, 0, $num);
            while ($job = db_fetch_array($result)) {
              $this->schedule($job);
              // @todo Add time limit.
            }
          }
        }
      }
    }
    // Unflag and post a message that we're done.
    variable_set('feeds_scheduler_cron', FALSE);
    watchdog('FeedsScheduler', 'Finished processing schedule after !time.', array('!time' => format_interval(time() - $start)));
  }

  /**
   * Implementation of FeedsSchedulerInterface::add().
   *
   * Add a feed to the scheduler.
   *
   * @todo Create optional parameter $last_executed_time to pass in. Set this
   *   value if a feed is refreshed on creation.
   */
  public function add($importer_id, $callback, $feed_nid = 0) {
    $save = array(
      'id' => $importer_id,
      'callback' => $callback,
      'feed_nid' => $feed_nid,
      'last_executed_time' => 0,
      'scheduled' => 0, // Means NOT scheduled at the moment.
    );
    if (db_result(db_query_range("SELECT 1 FROM {feeds_schedule} WHERE id = '%s' AND callback = '%s' AND feed_nid = %d", $importer_id, $callback, $feed_nid, 0, 1))) {
      drupal_write_record('feeds_schedule', $save, array('id', 'callback', 'feed_nid'));
    }
    else {
      drupal_write_record('feeds_schedule', $save);
    }
  }

  /**
   * Implementation of FeedsSchedulerInterface::remove().
   */
  public function remove($importer_id, $callback, $feed_nid = 0) {
    db_query("DELETE FROM {feeds_schedule} WHERE id = '%s' AND callback = '%s' AND feed_nid = %d", $importer_id, $callback, $feed_nid);
  }

  /**
   * Implementation of FeedsSchedulerInterface::work().
   *
   * Refresh a feed.
   *
   * Used as worker callback invoked from feeds_scheduler_refresh() or
   * if drupal_queue is not enabled, directly from $this->cron().
   */
  public function work($job) {
    $importer = feeds_importer($job['id']);
    try {
      if (FEEDS_BATCH_COMPLETE == $importer->existing()->work($job)) {
        $this->finished($job);
      }
    }
    catch (Exception $e) {
      watchdog('FeedsScheduler', $e->getMessage(), array(), WATCHDOG_ERROR);
      $this->finished($job);
    }
    // Make sure that job is not scheduled after this method has executed.
    $this->unschedule($job);
  }

  /**
   * @return
   *   Drupal Queue if available, NULL otherwise.
   */
  protected function queue() {
    if (module_exists('drupal_queue')) {
      drupal_queue_include();
      return drupal_queue_get(FEEDS_SCHEDULER_QUEUE);
    }
  }

  /**
   * Attempt to reserve a job. If successful work it off or - if Drupal Queue is
   * available - queue it.
   *
   * The lock/release mechanism makes sure that an item does not get queued
   * twice. It has a different purpose than the FeedsSource level locking
   * which is in place to avoid concurrent import/clear operations on a source.
   *
   * @param $job
   *   A job array.
   */
   protected function schedule($job) {
     db_query("UPDATE {feeds_schedule} SET scheduled = %d WHERE id = '%s' AND feed_nid = %d AND callback = '%s'", FEEDS_REQUEST_TIME, $job['id'], $job['feed_nid'], $job['callback']);
     if (db_affected_rows()) {
       if ($this->queue()) {
         if (!$this->queue()->createItem($job)) {
           $this->unschedule($job);
           watchdog('FeedsScheduler', 'Error adding item to queue.', WATCHDOG_CRITICAL);
           return;
         }
       }
       else {
         $this->work($job);
       }
     }
   }

  /**
   * Remove a job from schedule.
   *
   * This function sets the source's scheduled bit to 0 and thus makes
   * it eligible for being added to the queue again.
   *
   * @param $job
   *   A job array.
   */
  protected function unschedule($job) {
    unset($job['last_executed_time']);
    $job = array(
      'scheduled' => 0,
    ) + $job;
    drupal_write_record('feeds_schedule', $job, array('id', 'callback', 'feed_nid'));
  }

  /**
   * Release a job and set its last_executed_time flag.
   *
   * @param $job
   *   A job array.
   */
  protected function finished($job) {
    $job = array(
      'scheduled' => 0,
      'last_executed_time' => FEEDS_REQUEST_TIME,
    ) + $job;
    drupal_write_record('feeds_schedule', $job, array('id', 'callback', 'feed_nid'));
  }
}
