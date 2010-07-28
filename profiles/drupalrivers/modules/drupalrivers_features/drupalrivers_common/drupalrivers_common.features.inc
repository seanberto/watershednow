<?php

/**
 * Implementation of hook_default_box().
 */
function drupalrivers_common_default_box() {
  module_load_include('inc', 'drupalrivers_common', 'drupalrivers_common.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_common_default_box', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function drupalrivers_common_context_default_contexts() {
  module_load_include('inc', 'drupalrivers_common', 'drupalrivers_common.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_common_context_default_contexts', $args);
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalrivers_common_ctools_plugin_api() {
  module_load_include('inc', 'drupalrivers_common', 'drupalrivers_common.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_common_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_node_info().
 */
function drupalrivers_common_node_info() {
  module_load_include('inc', 'drupalrivers_common', 'drupalrivers_common.features.node');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_common_node_info', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function drupalrivers_common_strongarm() {
  module_load_include('inc', 'drupalrivers_common', 'drupalrivers_common.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_common_strongarm', $args);
}
