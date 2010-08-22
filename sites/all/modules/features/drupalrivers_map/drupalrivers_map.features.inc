<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalrivers_map_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function drupalrivers_map_node_info() {
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
