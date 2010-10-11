<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wn_hp_callout_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function wn_hp_callout_node_info() {
  $items = array(
    'wn_hp_callout' => array(
      'name' => t('Homepage Callout'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function wn_hp_callout_views_api() {
  return array(
    'api' => '2',
  );
}
