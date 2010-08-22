<?php
// $Id: FeedsProcessor.inc,v 1.14 2010/07/18 18:33:43 alexb Exp $


// Update mode for existing items.
define('FEEDS_SKIP_EXISTING', 0);
define('FEEDS_REPLACE_EXISTING', 1);
define('FEEDS_UPDATE_EXISTING', 2);

/**
 * Abstract class, defines interface for processors.
 */
abstract class FeedsProcessor extends FeedsPlugin {

  /**
   * Process the result of the parser or previous processors.
   * Extending classes must implement this method.
   *
   * @param FeedsImportBatch $batch
   *   The current feed import data passed in from the parsing stage.
   * @param FeedsSource $source
   *   Source information about this import.
   *
   * @return
   *   FEEDS_BATCH_COMPLETE if all items have been processed, a float between 0
   *   and 0.99* indicating progress otherwise.
   */
  public abstract function process(FeedsImportBatch $batch, FeedsSource $source);

  /**
   * Remove all stored results or stored results up to a certain time for this
   * configuration/this source.
   *
   * @param FeedsBatch $batch
   *   A FeedsBatch object for tracking information such as how many
   *   items have been deleted total between page loads.
   * @param FeedsSource $source
   *   Source information for this expiry. Implementers should only delete items
   *   pertaining to this source. The preferred way of determining whether an
   *   item pertains to a certain souce is by using $source->feed_nid. It is the
   *   processor's responsibility to store the feed_nid of an imported item in
   *   the processing stage.
   *
   * @return
   *   FEEDS_BATCH_COMPLETE if all items have been processed, a float between 0
   *   and 0.99* indicating progress otherwise.
   */
  public abstract function clear(FeedsBatch $batch, FeedsSource $source);

  /**
   * Delete feed items younger than now - $time. Do not invoke expire on a
   * processor directly, but use FeedsImporter::expire() instead.
   *
   * @see FeedsImporter::expire().
   * @see FeedsDataProcessor::expire().
   *
   * @param $time
   *   If implemented, all items produced by this configuration that are older
   *   than FEEDS_REQUEST_TIME - $time should be deleted.
   *   If $time === NULL processor should use internal configuration.
   *
   * @return
   *   FEEDS_BATCH_COMPLETE if all items have been processed, a float between 0
   *   and 0.99* indicating progress otherwise.
   */
  public function expire($time = NULL) {
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Execute mapping on an item.
   *
   * This method encapsulates the central mapping functionality. When an item is
   * processed, it is passed through map() where the properties of $source_item
   * are mapped onto $target_item following the processor's mapping
   * configuration.
   *
   * For each mapping FeedsParser::getSourceElement() is executed to retrieve
   * the source element, then FeedsProcessor::setTargetElement() is invoked
   * to populate the target item properly. Alternatively a
   * hook_x_targets_alter() may have specified a callback for a mapping target
   * in which case the callback is asked to populate the target item instead of
   * FeedsProcessor::setTargetElement().
   *
   * @ingroup mappingapi
   *
   * @see hook_feeds_data_processor_targets_alter()
   * @see hook_feeds_node_processor_targets_alter()
   * @see hook_feeds_term_processor_targets_alter()
   * @see hook_feeds_user_processor_targets_alter()
   */
  protected function map($source_item, $target_item = NULL) {

    // Static cache $targets as getMappingTargets() may be an expensive method.
    static $targets;
    if (!isset($targets[$this->id])) {
      $targets[$this->id] = $this->getMappingTargets();
    }
    $parser = feeds_importer($this->id)->parser;
    if (empty($target_item)) {
      $target_item = array();
    }

    // Many mappers add to existing fields rather than replacing them. Hence we
    // need to clear target elements of each item before mapping in case we are
    // mapping on a prepopulated item such as an existing node.
    if (is_array($target_item)) {
      $target_item = (object)$target_item;
      $convert_to_array = TRUE;
    }
    foreach ($this->config['mappings'] as $mapping) {
      if (isset($targets[$mapping['target']]['real_target'])) {
        unset($target_item->{$targets[$mapping['target']]['real_target']});
      }
      elseif (isset($target_item->{$mapping['target']})) {
        unset($target_item->{$mapping['target']});
      }
    }
    if ($convert_to_array) {
      $target_item = (array)$target_item;
    }

    /*
    This is where the actual mapping happens: For every mapping we envoke
    the parser's getSourceElement() method to retrieve the value of the source
    element and pass it to the processor's setTargetElement() to stick it
    on the right place of the target item.

    If the mapping specifies a callback method, use the callback instead of
    setTargetElement().
    */
    foreach ($this->config['mappings'] as $mapping) {
      $value = $parser->getSourceElement($source_item, $mapping['source']);

      if (is_array($targets[$this->id][$mapping['target']]) && isset($targets[$this->id][$mapping['target']]['callback']) && function_exists($targets[$this->id][$mapping['target']]['callback'])) {
        $callback = $targets[$this->id][$mapping['target']]['callback'];
        $callback($target_item, $mapping['target'], $value);
      }
      else {
        $this->setTargetElement($target_item, $mapping['target'], $value);
      }
    }
    return $target_item;
  }

  /**
   * Per default, don't support expiry. If processor supports expiry of imported
   * items, return the time after which items should be removed.
   */
  public function expiryTime() {
    return FEEDS_EXPIRE_NEVER;
  }

  /**
   * Declare default configuration.
   */
  public function configDefaults() {
    return array('mappings' => array());
  }

  /**
   * Add a mapping to existing mappings.
   *
   * @param $source
   *   A string that identifies a source element.
   * @param $target
   *   A string that identifies a target element.
   * @param $unique
   *   A boolean that defines whether the target value should be unique. If
   *   TRUE only one item with a given target value can exist on the local
   *   system. Compare with existingItemId() and uniqueTargets().
   */
  public function addMapping($source, $target, $unique = FALSE) {
    if (!empty($source) && !empty($target)) {
      $this->config['mappings'][] = array(
        'source' => $source,
        'target' => $target,
        'unique' => $unique,
      );
    }
  }

  /**
   * Set unique state of a mapping target.
   */
  public function setUnique($source, $target, $unique) {
    if (!empty($source) && !empty($target)) {
      foreach ($this->config['mappings'] as $k => $mapping) {
        if ($mapping['source'] == $source && $mapping['target'] == $target) {
          $this->config['mappings'][$k]['unique'] = $unique;
        }
      }
    }
  }

  /**
   * Remove a mapping.
   */
  public function removeMapping($source, $target) {
    foreach ($this->config['mappings'] as $k => $mapping) {
      if ($mapping['source'] == $source && $mapping['target'] == $target) {
        unset($this->config['mappings'][$k]);
      }
    }
    // Keep or keys clean.
    $this->config['mappings'] = array_values($this->config['mappings']);
  }

  /**
   * Get mappings.
   */
  public function getMappings() {
    return isset($this->config['mappings']) ? $this->config['mappings'] : array();
  }

  /**
   * Declare possible mapping targets that this processor exposes.
   *
   * @ingroup mappingapi
   *
   * @return
   *   An array of mapping targets. Keys are paths to targets
   *   separated by ->, values are TRUE if target can be unique,
   *   FALSE otherwise.
   */
  public function getMappingTargets() {
    return array();
  }

  /**
   * Set a concrete target element. Invoked from FeedsProcessor::map().
   *
   * @ingroup mappingapi
   */
  public function setTargetElement(&$target_item, $target_element, $value) {
    $target_item[$target_element] = $value;
  }

  /**
   * Retrieve the target item's existing id if available. Otherwise return 0.
   *
   * @ingroup mappingapi
   *
   * @param $source_item
   *   A single item that has been aggregated from a feed.
   * @param FeedsSource $source
   *   The source information about this import.
   */
  protected function existingItemId($source_item, FeedsSource $source) {
    return 0;
  }

  /**
   * Utility function that iterates over a target array and retrieves all
   * sources that are unique.
   *
   * @param $source_item
   *   A feed item from a FeedsImportBatch.
   *
   * @return
   *   An array where the keys are target field names and the values are the
   *   elements from the source item mapped to these targets.
   */
  protected function uniqueTargets($source_item) {
    $parser = feeds_importer($this->id)->parser;
    $targets = array();
    foreach ($this->config['mappings'] as $mapping) {
      if ($mapping['unique']) {
        // Invoke the parser's getSourceElement to retrieve the value for this
        // mapping's source.
        $targets[$mapping['target']] = $parser->getSourceElement($source_item, $mapping['source']);
      }
    }
    return $targets;
  }

  /**
   * Loads on-behalf implementations from mappers/ directory.
   *
   * FeedsProcessor::map() does not load from mappers/ as only node and user
   * processor ship with on-behalf implementations.
   *
   * @see FeedsNodeProcessor::map()
   * @see FeedsUserProcessor::map()
   */
  protected static function loadMappers() {
    static $loaded = FALSE;
    if (!$loaded) {
      $path = drupal_get_path('module', 'feeds') .'/mappers';
      $files = drupal_system_listing('.*\.inc$', $path, 'name', 0);
      foreach ($files as $file) {
        if (strstr($file->filename, '/mappers/')) {
          require_once("./$file->filename");
        }
      }
      // Rebuild cache.
      module_implements('', FALSE, TRUE);
    }
    $loaded = TRUE;
  }
}
