<?php
// $Id: FeedsSource.inc,v 1.15 2010/07/11 13:53:17 alexb Exp $

/**
 * @file
 * Definition of FeedsSourceInterface and FeedsSource class.
 */

/**
 * Declares an interface for a class that defines default values and form
 * descriptions for a FeedSource.
 */
interface FeedsSourceInterface {

  /**
   * Crutch: for ease of use, we implement FeedsSourceInterface for every
   * plugin, but then we need to have a handle which plugin actually implements
   * a source.
   *
   * @see FeedsPlugin class.
   *
   * @return
   *   TRUE if a plugin handles source specific configuration, FALSE otherwise.
   */
  public function hasSourceConfig();

  /**
   * Return an associative array of default values.
   */
  public function sourceDefaults();

  /**
   * Return a Form API form array that defines a form configuring values. Keys
   * correspond to the keys of the return value of sourceDefaults().
   */
  public function sourceForm($source_config);

  /**
   * Validate user entered values submitted by sourceForm().
   */
  public function sourceFormValidate(&$source_config);

  /**
   * A source is being deleted.
   */
  public function sourceSave(FeedsSource $source);

  /**
   * A source is being saved.
   */
  public function sourceDelete(FeedsSource $source);
}

/**
 * This class encapsulates a source of a feed. It stores where the feed can be
 * found and how to import it.
 *
 * Information on how to import a feed is encapsulated in a FeedsImporter object
 * which is identified by the common id of the FeedsSource and the
 * FeedsImporter. More than one FeedsSource can use the same FeedsImporter
 * therefore a FeedsImporter never holds a pointer to a FeedsSource object, nor
 * does it hold any other information for a particular FeedsSource object.
 *
 * Classes extending FeedsPlugin can implement a sourceForm to expose
 * configuration for a FeedsSource object. This is for instance how FeedsFetcher
 * exposes a text field for a feed URL or how FeedsCSVParser exposes a select
 * field for choosing between colon or semicolon delimiters.
 *
 * It is important that a FeedsPlugin does not directly hold information about
 * a source but leave all storage up to FeedsSource. An instance of a
 * FeedsPlugin class only exists once per FeedsImporter configuration, while an
 * instance of a FeedsSource class exists once per feed_nid to be imported.
 *
 * As with FeedsImporter, the idea with FeedsSource is that it can be used
 * without actually saving the object to the database.
 */
class FeedsSource extends FeedsConfigurable {

  // Contains the node id of the feed this source info object is attached to.
  // Equals 0 if not attached to any node - i. e. if used on a
  // standalone import form within Feeds or by other API users.
  protected $feed_nid;

  // The FeedsImporter object that this source is expected to be used with.
  protected $importer;

  // A FeedsBatch object. NULL if there is no active batch.
  protected $batch;

  /**
   * Instantiate a unique object per class/id/feed_nid. Don't use
   * directly, use feeds_source() instead.
   */
  public static function instance($importer_id, $feed_nid = 0) {
    $class = variable_get('feeds_source_class', 'FeedsSource');
    static $instances = array();
    if (!isset($instances[$class][$importer_id][$feed_nid])) {
      $instances[$class][$importer_id][$feed_nid] = new $class($importer_id, $feed_nid);
    }
    return $instances[$class][$importer_id][$feed_nid];
  }

  /**
   * Constructor.
   */
  protected function __construct($importer_id, $feed_nid) {
    $this->feed_nid = $feed_nid;
    $this->importer = feeds_importer($importer_id);
    parent::__construct($importer_id);
    $this->load();
  }

  /**
   * Preview = fetch and parse a feed.
   *
   * @return
   *   FeedsImportBatch object, fetched and parsed.
   *
   * @throws
   *   Throws Exception if an error occurs when fetching or parsing.
   */
  public function preview() {
    $batch = $this->importer->fetcher->fetch($this);
    $this->importer->parser->parse($batch, $this);
    return $batch;
  }

  /**
   * Import a feed: execute fetching, parsing and processing stage.
   *
   * @return
   *   FEEDS_BATCH_COMPLETE if the import process finished. A decimal between
   *   0.0 and 0.9 periodic if import is still in progress.
   *
   * @throws
   *   Throws Exception if an error occurs when importing.
   */
  public function import() {
    try {
      if (!$this->batch || !($this->batch instanceof FeedsImportBatch)) {
        $this->batch = $this->importer->fetcher->fetch($this);
        $this->importer->parser->parse($this->batch, $this);
      }
      $result = $this->importer->processor->process($this->batch, $this);
      if ($result == FEEDS_BATCH_COMPLETE) {
        unset($this->batch);
        module_invoke_all('feeds_after_import', $this->importer, $this);
      }
    }
    catch (Exception $e) {
      unset($this->batch);
      $this->save();
      throw $e;
    }
    $this->save();
    return $result;
  }

  /**
   * Remove all items from a feed.
   *
   * @return
   *   FEEDS_BATCH_COMPLETE if the clearing process finished. A decimal between
   *   0.0 and 0.9 periodic if clearing is still in progress.
   *
   * @throws
   *   Throws Exception if an error occurs when clearing.
   */
  public function clear() {
    try {
      $this->importer->fetcher->clear($this);
      $this->importer->parser->clear($this);
      if (!$this->batch) {
        $this->batch = new FeedsBatch();
      }
      $result = $this->importer->processor->clear($this->batch, $this);
      if ($result == FEEDS_BATCH_COMPLETE) {
        unset($this->batch);
      }
    }
    catch (Exception $e) {
      unset($this->batch);
      $this->save();
      throw $e;
    }
    $this->save();
    return $result;
  }

  /**
   * Save configuration.
   */
  public function save() {
    $config = $this->getConfig();
    // Alert implementers of FeedsSourceInterface to the fact that we're saving.
    foreach ($this->importer->plugin_types as $type) {
      $this->importer->$type->sourceSave($this);
    }
    // Store the source property of the fetcher in a separate column so that we
    // can do fast lookups on it.
    $source = '';
    if (isset($config[get_class($this->importer->fetcher)]['source'])) {
      $source = $config[get_class($this->importer->fetcher)]['source'];
    }
    $object = array(
      'id' => $this->id,
      'feed_nid' => $this->feed_nid,
      'config' => $config,
      'source' => $source,
      'batch' => isset($this->batch) ? $this->batch : FALSE,
    );
    if (db_result(db_query_range("SELECT 1 FROM {feeds_source} WHERE id = '%s' AND feed_nid = %d", $this->id, $this->feed_nid, 0, 1))) {
      drupal_write_record('feeds_source', $object, array('id', 'feed_nid'));
    }
    else {
      drupal_write_record('feeds_source', $object);
    }
  }

  /**
   * Load configuration and unpack.
   *
   * @todo Patch CTools to move constants from export.inc to ctools.module.
   */
  public function load() {
    if ($record = db_fetch_object(db_query("SELECT config, batch FROM {feeds_source} WHERE id = '%s' AND feed_nid = %d", $this->id, $this->feed_nid))) {
      // While FeedsSource cannot be exported, we still use CTool's export.inc
      // export definitions.
      ctools_include('export');
      $this->export_type = EXPORT_IN_DATABASE;
      $this->config = unserialize($record->config);
      $this->batch = unserialize($record->batch);
    }
  }

  /**
   * Delete configuration. Removes configuration information
   * from database, does not delete configuration itself.
   */
  public function delete() {
    // Alert implementers of FeedsSourceInterface to the fact that we're
    // deleting.
    foreach ($this->importer->plugin_types as $type) {
      $this->importer->$type->sourceDelete($this);
    }
    db_query("DELETE FROM {feeds_source} WHERE id = '%s' AND feed_nid = %d", $this->id, $this->feed_nid);
  }

  /**
   * Convenience function. Returns the configuration for a specific class.
   *
   * @param FeedsSourceInterface $client
   *   An object that is an implementer of FeedsSourceInterface.
   *
   * @return
   *   An array stored for $client.
   */
  public function getConfigFor(FeedsSourceInterface $client) {
    return $this->config[get_class($client)];
  }

  /**
   * Return defaults for feed configuration.
   */
  public function configDefaults() {
    // Collect information from plugins.
    $defaults = array();
    foreach ($this->importer->plugin_types as $type) {
      if ($this->importer->$type->hasSourceConfig()) {
        $defaults[get_class($this->importer->$type)] = $this->importer->$type->sourceDefaults();
      }
    }
    return $defaults;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    // Collect information from plugins.
    $form = array();
    foreach ($this->importer->plugin_types as $type) {
      if ($this->importer->$type->hasSourceConfig()) {
        $class = get_class($this->importer->$type);
        $form[$class] = $this->importer->$type->sourceForm($this->config[$class]);
        $form[$class]['#tree'] = TRUE;
      }
    }
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    foreach ($this->importer->plugin_types as $type) {
      $class = get_class($this->importer->$type);
      if (isset($values[$class]) && $this->importer->$type->hasSourceConfig()) {
        $this->importer->$type->sourceFormValidate($values[$class]);
      }
    }
  }
}
