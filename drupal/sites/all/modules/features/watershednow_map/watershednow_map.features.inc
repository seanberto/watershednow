<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function watershednow_map_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "openlayers" && $api == "openlayers_presets") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function watershednow_map_node_info() {
  $items = array(
    'map_feature' => array(
      'name' => t('Watershed feature'),
      'module' => 'features',
      'description' => t('A map feature represents a point of interest, polyline, and/or polygon on a map of a watershed.'),
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

/**
 * Implementation of hook_views_api().
 */
function watershednow_map_views_api() {
  return array(
    'api' => '2',
  );
}
