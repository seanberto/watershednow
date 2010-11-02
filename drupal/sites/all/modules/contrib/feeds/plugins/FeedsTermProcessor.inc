<?php
// $Id: FeedsTermProcessor.inc,v 1.20.2.2 2010/10/28 20:48:10 alexb Exp $

/**
 * @file
 * FeedsTermProcessor class.
 */

/**
 * Feeds processor plugin. Create taxonomy terms from feed items.
 */
class FeedsTermProcessor extends FeedsProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsImportBatch $batch, FeedsSource $source) {

    if (empty($this->config['vocabulary'])) {
      throw new Exception(t('You must define a vocabulary for Taxonomy term processor before importing.'));
    }

    // Count number of created and updated nodes.
    $created  = $updated = $no_name = 0;

    while ($item = $batch->shiftItem()) {

      if (!($tid = $this->existingItemId($batch, $source)) || $this->config['update_existing'] != FEEDS_SKIP_EXISTING) {

        // Map item to a term.
        $term = array();
        if ($tid && $this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
          $term = (array) taxonomy_get_term($tid, TRUE);
          $term = module_invoke_all('feeds_taxonomy_load', $term);
        }
        $term = $this->map($batch, $term, $source->feed_nid);

        // Check if term name is set, otherwise continue.
        if (empty($term['name'])) {
          $no_name++;
          continue;
        }

        // Add term id if available.
        if (!empty($tid)) {
          $term['tid'] = $tid;
        }

        // Save the term.
        $term['importer_id'] = $this->id;
        $term['feed_nid'] = $source->feed_nid;
        taxonomy_save_term($term);
        if ($tid) {
          $updated++;
        }
        else {
          $created++;
        }
      }
    }

    // Set messages.
    $vocabulary = $this->vocabulary();
    if ($no_name) {
      drupal_set_message(
        format_plural(
          $no_name,
          'There was @number term that could not be imported because their name was empty. Check mapping settings on Taxomy term processor.',
          'There were @number terms that could not be imported because their name was empty. Check mapping settings on Taxomy term processor.',
          array('@number' => $no_name)
        ),
        'error'
      );
    }
    if ($created) {
      drupal_set_message(format_plural($created, 'Created @number term in !vocabulary.', 'Created @number terms in !vocabulary.', array('@number' => $created, '!vocabulary' => $vocabulary->name)));
    }
    elseif ($updated) {
      drupal_set_message(format_plural($updated, 'Updated @number term in !vocabulary.', 'Updated @number terms in !vocabulary.', array('@number' => $updated, '!vocabulary' => $vocabulary->name)));
    }
    else {
      drupal_set_message(t('There are no new terms.'));
    }
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    $deleted = 0;
    $vocabulary = $this->vocabulary();
    $result = db_query("SELECT td.tid
                        FROM {term_data} td
                        JOIN {feeds_term_item} ft ON td.tid = ft.tid
                        WHERE td.vid = %d
                        AND ft.id = '%s'
                        AND ft.feed_nid = %d",
                        $vocabulary->vid, $this->id, $source->feed_nid);
    while ($term = db_fetch_object($result)) {
      if (taxonomy_del_term($term->tid) == SAVED_DELETED) {
        $deleted++;
      }
    }
    // Set messages.
    if ($deleted) {
      drupal_set_message(format_plural($deleted, 'Deleted @number term from !vocabulary.', 'Deleted @number terms from !vocabulary.', array('@number' => $deleted, '!vocabulary' => $vocabulary->name)));
    }
    else {
      drupal_set_message(t('No terms to be deleted.'));
    }
  }

  /**
   * Execute mapping on an item.
   */
  protected function map(FeedsImportBatch $batch, $target_term = NULL) {
    // Prepare term object, have parent class do the iterating.
    if (!$target_term) {
      $target_term = array();
    }
    if (!$vocabulary = $this->vocabulary()) {
      throw new Exception(t('No vocabulary specified for term processor'));
    }
    $target_term['vid'] = $vocabulary->vid;
    $target_term = parent::map($batch, $target_term);
    // Taxonomy module expects synonyms to be supplied as a single string.
    if (isset($target_term['synonyms']) && is_array($target_term['synonyms'])) {
      $target_term['synonyms'] = implode("\n", $target_term['synonyms']);
    }
    return $target_term;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'vocabulary' => 0,
      'update_existing' => FEEDS_SKIP_EXISTING,
      'mappings' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $options = array(0 => t('Select a vocabulary'));
    foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
      if (strpos($vocab->module, 'features_') === 0) {
        $options[$vocab->module] = $vocab->name;
      }
      else {
        $options[$vid] = $vocab->name;
      }
    }
    $form = array();
    $form['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Import to vocabulary'),
      '#description' => t('Choose the vocabulary to import into. <strong>CAUTION:</strong> when deleting terms through the "Delete items" tab, Feeds will delete <em>all</em> terms from this vocabulary.'),
      '#options' => $options,
      '#default_value' => $this->config['vocabulary'],
    );
    $form['update_existing'] = array(
      '#type' => 'radios',
      '#title' => t('Update existing terms'),
      '#description' => t('Select how existing terms should be updated. Existing terms will be determined using mappings that are a "unique target".'),
      '#options' => array(
        FEEDS_SKIP_EXISTING => 'Do not update existing terms',
        FEEDS_REPLACE_EXISTING => 'Replace existing terms',
        FEEDS_UPDATE_EXISTING => 'Update existing terms (slower than replacing them)',
      ),
      '#default_value' => $this->config['update_existing'],
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if (empty($values['vocabulary'])) {
      form_set_error('vocabulary', t('Choose a vocabulary'));
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = array(
      'name' => array(
        'name' => t('Term name'),
        'description' => t('Name of the taxonomy term.'),
        'optional_unique' => TRUE,
       ),
      'description' => array(
        'name' => t('Term description'),
        'description' => t('Description of the taxonomy term.'),
       ),
      'synonyms' => array(
        'name' => t('Term synonyms'),
        'description' => t('One synonym or an array of synonyms of the taxonomy term.'),
       ),
    );
    // Let implementers of hook_feeds_term_processor_targets() add their targets.
    $vocabulary = $this->vocabulary();
    drupal_alter('feeds_term_processor_targets', $targets, $vocabulary->vid);
    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {

    // The only possible unique target is name.
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      if ($target == 'name') {
        $vocabulary = $this->vocabulary();
        if ($tid = db_result(db_query("SELECT tid FROM {term_data} WHERE name = '%s' AND vid = %d", $value, $vocabulary->vid))) {
          return $tid;
        }
      }
    }
    return 0;
  }

  /**
   * Return vocabulary to map to.
   *
   * Feeds supports looking up vocabularies by their module name as part of an
   * effort to use the vocabulary.module field as machine name to make
   * vocabularies exportable.
   */
  public function vocabulary() {
    $vocabularies = taxonomy_get_vocabularies();
    if (is_numeric($this->config['vocabulary'])) {
      return $vocabularies[$this->config['vocabulary']];
    }
    else {
      foreach ($vocabularies as $vocabulary) {
        if ($vocabulary->module == $this->config['vocabulary']) {
          return $vocabulary;
        }
      }
    }
  }
}
