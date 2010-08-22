<?php
// $Id: taxonomy.inc,v 1.2 2010/07/08 17:19:16 alexb Exp $

/**
 * @file
 * Mapper that exposes a node's taxonomy vocabularies as mapping targets.
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function taxonomy_feeds_node_processor_targets_alter(&$targets, $content_type) {
  foreach (taxonomy_get_vocabularies($content_type) as $vocab) {
    $description = t('The !name vocabulary of the node. If this is a "Tags" vocabulary, any new terms will be created on import. Otherwise only existing terms will be used. If this is not a "Tags" vocabulary and not a "Multiple select" vocabulary, only the first available term will be created. See !settings.', array('!name' => $vocab->name, '!settings' => l(t('vocabulary settings'), 'admin/content/taxonomy/edit/vocabulary/'. $vocab->vid, array('query' => 'destination='. $_GET['q']))));

    $targets['taxonomy:'. $vocab->vid] = array(
      'name' => "Taxonomy: ". $vocab->name,
      'callback' => 'taxonomy_feeds_set_target',
      'description' => $description,
      'real_target' => 'taxonomy',
    );
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * @param $node
 *   Reference to the node object we are working on.
 *
 * @param $key
 *   A key as added to the $targets array by
 *   taxonomy_feeds_node_processor_targets_alter().
 *
 * @param $terms
 *   Given terms as array.
 *
 * Add the given terms to the node object so the taxonomy module can add them
 * on node_save().
 */
function taxonomy_feeds_set_target(&$node, $key, $terms) {

  // Return if there are no terms.
  if (empty($terms)) {
    return;
  }

  // Load target vocabulary to check, if it has the "tags" flag.
  $vocab_id = (int) str_replace('taxonomy:', '', $key);
  $vocab = taxonomy_vocabulary_load($vocab_id);

  // Cast a given single string to an array so we can use it.
  if (!is_array($terms)) {
    $terms = array($terms);
  }

  if ($vocab->tags) {
    // Simply add a comma separated list to the node for a "tags" vocabulary.
    $node->taxonomy['tags'][$vocab->vid] = implode(',', $terms);
  }
  else {
    foreach ($terms as $term_name) {
      // Check if a term already exists.
      if ($terms_found = taxonomy_get_term_by_name_vid($term_name, $vocab->vid)) {
        // If any terms are found add them to the node's taxonomy by found tid.
        foreach ($terms_found AS $term_found) {
          $node->taxonomy[$vocab->vid][$term_found->tid] = $term_found->tid;
          if (!$vocab->multiple) {
            break;
          }
        }
        // If the vocab is not for multiple tags break after the first hit.
        if (!$vocab->multiple) {
          break;
        }

      }
    }
  }
}

/**
 * Try to map a string to an existing term by name and vocabulary id.
 *
 * Provides a case-insensitive and trimmed mapping, to maximize the likelihood
 * of a successful match limited by a vocabulary id.
 *
 * @param $name
 *   Name of the term to search for.
 *
 * @param $vid
 *   The vocabulary's ID.
 *
 * @return
 *   An array of matching term objects.
 */
function taxonomy_get_term_by_name_vid($name, $vid) {
  $db_result = db_query(db_rewrite_sql("SELECT t.tid, t.name FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s') AND vid=%d", 't', 'tid'), trim($name), $vid);
  $result = array();
  while ($term = db_fetch_object($db_result)) {
    $result[] = $term;
  }
  return $result;
}
