<?php

/**
 * Implementation of hook_node_info().
 */
function drupalrivers_map_node_info() {
  module_load_include('inc', 'drupalrivers_map', 'drupalrivers_map.features.node');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_map_node_info', $args);
}
