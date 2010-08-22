<?php

/**
 * Implementation of hook_default_box().
 */
function drupalrivers_map_default_box() {
  module_load_include('inc', 'drupalrivers_map', 'drupalrivers_map.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_map_default_box', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function drupalrivers_map_context_default_contexts() {
  module_load_include('inc', 'drupalrivers_map', 'drupalrivers_map.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_map_context_default_contexts', $args);
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalrivers_map_ctools_plugin_api() {
  module_load_include('inc', 'drupalrivers_map', 'drupalrivers_map.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_map_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_node_info().
 */
function drupalrivers_map_node_info() {
  module_load_include('inc', 'drupalrivers_map', 'drupalrivers_map.features.node');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_map_node_info', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function drupalrivers_map_strongarm() {
  module_load_include('inc', 'drupalrivers_map', 'drupalrivers_map.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_map_strongarm', $args);
}
