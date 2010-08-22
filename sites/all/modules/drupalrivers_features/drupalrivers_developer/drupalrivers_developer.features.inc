<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalrivers_developer_ctools_plugin_api() {
  module_load_include('inc', 'drupalrivers_developer', 'drupalrivers_developer.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_developer_ctools_plugin_api', $args);
}
