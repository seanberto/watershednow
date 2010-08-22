<?php
// $Id: FeedsBatch.inc,v 1.12 2010/06/20 01:10:29 alexb Exp $

/**
 * A FeedsBatch object holds the state of an import or clear batch.
 *
 * Used in FeedsSource class. Counter variables are public for easier access.
 */
class FeedsBatch {
  // Maximum number of items in this batch. This is not necessarily the current
  // number of items in this batch
  public $total;
  // Number of items created.
  public $created;
  // Number of items updated or replaced.
  public $updated;
  // Number of items deleted.
  public $deleted;
  public function __construct() {
    $this->total = 0;
    $this->created = 0;
    $this->updated = 0;
    $this->deleted = 0;
  }
}

/**
 * A FeedsImportBatch wraps the actual content retrieved from a FeedsSource. On
 * import, it is created on the fetching stage and passed through the parsing
 * and processing stage where it is normalized and consumed.
 *
 * A Fetcher must return a FeedsImportBatch object on fetch(). To that end it
 * can use one of the existing FeedsImportBatch classes (FeedsImportBatch,
 * FeedsFileBatch or FeedsHTTPBatch) or provide its own as a direct or indirect
 * extension of FeedsImportBatch.
 *
 * A Parser must populate a FeedsImportBatch object through the set methods upon
 * parse(). For instance:
 *
 * @code
 * $batch->setItems($parsed_rows);
 * $batch->setTitle('My imported document');
 * @endcode
 *
 * Finally, a processor can work off the information produced on the parsing
 * stage by consuming items with $batch->shiftItem().
 *
 * @code
 * while ($item = $batch->shiftItem()) {
 *   $object = $this->map($item);
 *   $object->save();
 * }
 * @endcode
 *
 * Note: Knowledge of the internal structure of a single item in the $items
 * array is managed by the mapping API specified in FeedsParser class and
 * FeedsProcessor class.
 *
 * @see FeedsFileBatch
 * @see FeedsHTTPBatch
 */
class FeedsImportBatch extends FeedsBatch {
  protected $title;
  protected $description;
  protected $link;
  protected $items;
  protected $raw;

  public function __construct($raw = '') {
    $this->raw = $raw;
    $this->title = '';
    $this->description = '';
    $this->link = '';
    $this->items = array();
  }

  /**
   * @return
   *   The raw content from the source as a string.
   *
   * @throws Exception
   *   Extending classes MAY throw an exception if a problem occurred.
   */
  public function getRaw() {
    return $this->raw;
  }

  /**
   * @return
   *   A path to a file containing the raw content as a source.
   *
   * @throws Exception
   *   If an unexpected problem occurred.
   */
  public function getFilePath() {
    if (!isset($this->file_path)) {
      $dir = file_directory_path() .'/feeds/';
      if (!file_check_directory($dir, TRUE)) {
        throw new Exception(t('Feeds directory either cannot be created or is not writable.'));
      }
      $dest = file_destination($dir . get_class($this) .'_'. drupal_get_token($this->url) .'_'. time(), FILE_EXISTS_RENAME);
      $this->file_path = file_save_data($this->getRaw(), $dest);
      if($this->file_path === 0) {
        throw new Exception(t('Cannot write content to %dest', array('%dest' => $dest)));
      }
    }
    return $this->file_path;
  }

  /**
   * @return
   *   A string that is the feed's title.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @return
   *   A string that is the feed's description.
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * @return
   *   A string that is the link to the feed's site (not the actual URL of the
   *   feed). Falls back to URL if not available.
   */
  public function getLink() {
    return $this->link;
  }

  /**
   * @return
   *   Next available item or NULL if there is none. Every returned item is
   *   removed from the internal array.
   */
  public function shiftItem() {
    return array_shift($this->items);
  }

  /**
   * Set title.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Set description.
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * Set link.
   */
  public function setLink($link) {
    $this->link = $link;
  }

  /**
   * Set items.
   *
   * @param $items
   *   An array of the items in the feed. Cannot be NULL.
   */
  public function setItems($items) {
    $this->items = $items;
    $this->total = count($this->items);
  }

  /**
   * Add an item.
   */
  public function addItem($item) {
    $this->items[] = $item;
    $this->total = count($this->items);
  }
}
