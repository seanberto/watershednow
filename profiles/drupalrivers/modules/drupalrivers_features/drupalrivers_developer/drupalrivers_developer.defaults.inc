<?php

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _drupalrivers_developer_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}