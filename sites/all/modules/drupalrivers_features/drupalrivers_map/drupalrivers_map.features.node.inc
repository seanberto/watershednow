<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _drupalrivers_map_node_info() {
  $items = array(
    'map_point' => array(
      'name' => t('Map point'),
      'module' => 'features',
      'description' => t('A map point represents a point of interest on a map of a watershed.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
