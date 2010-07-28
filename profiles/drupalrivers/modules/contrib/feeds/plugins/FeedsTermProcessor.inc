<?php
// $Id: FeedsTermProcessor.inc,v 1.10 2010/07/13 19:32:06 alexb Exp $

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

      if (!($tid = $this->existingItemId($item, $source)) || $this->config['update_existing']) {

        // Map item to a term.
        $term = $this->map($item);

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
    $vocabularies = taxonomy_get_vocabularies();
    $vocabulary = $vocabularies[$this->config['vocabulary']];
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

    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    $deleted = 0;

    $result = db_query("SELECT tid FROM {term_data} WHERE vid = %d", $this->config["vocabulary"]);
    while ($term = db_fetch_object($result)) {
      if (taxonomy_del_term($term->tid) == SAVED_DELETED) {
        $deleted++;
      }
    }

    // Set messages.
    $vocabularies = taxonomy_get_vocabularies();
    $vocabulary = $vocabularies[$this->config['vocabulary']];
    if ($deleted) {
      drupal_set_message(format_plural($deleted, 'Deleted @number term from !vocabulary.', 'Deleted @number terms from !vocabulary.', array('@number' => $deleted, '!vocabulary' => $vocabulary->name)));
    }
    else {
      drupal_set_message(t('No terms to be deleted.'));
    }
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Execute mapping on an item.
   */
  protected function map($source_item) {
    // Prepare term object.
    $target_term = array();
    $target_term['vid'] = $this->config['vocabulary'];

    // Have parent class do the iterating.
    return parent::map($source_item, $target_term);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'vocabulary' => 0,
      'update_existing' => 0,
      'mappings' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $options = array(0 => t('Select a vocabulary'));
    foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
      $options[$vid] = $vocab->name;
    }
    $form = array();
    $form['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Import to vocabulary'),
      '#description' => t('Choose the vocabulary to import into. <strong>CAUTION:</strong> when deleting terms through the "Delete items" tab, Feeds will delete <em>all</em> terms from this vocabulary.'),
      '#options' => $options,
      '#default_value' => $this->config['vocabulary'],
    );
    // @todo Implement true updating.
    $form['update_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace existing terms'),
      '#description' => t('If an existing term is found for an imported term, replace it. Existing terms will be determined using mappings that are a "unique target".'),
      '#default_value' => $this->config['update_existing'],
    );
    return $form;
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
    );
    // Let implementers of hook_feeds_term_processor_targets() add their targets.
    drupal_alter('feeds_term_processor_targets', $targets, $this->config['vocabulary']);
    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingItemId($source_item, FeedsSource $source) {

    // The only possible unique target is name.
    foreach ($this->uniqueTargets($source_item) as $target => $value) {
      if ($target == 'name') {
        if ($tid = db_result(db_query("SELECT tid FROM {term_data} WHERE name = '%s' AND vid = %d", $value, $this->config["vocabulary"]))) {
          return $tid;
        }
      }
    }
    return 0;
  }
}
