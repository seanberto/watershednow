<?php
// $Id: feeds_defaults.features.inc,v 1.1 2009/10/23 22:37:15 alexb Exp $

/**
 * @file
 * Default hook definitions. This code is generated with Features module but it
 * has been tweaked manually. Do not attempt to reexport.
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function feeds_defaults_ctools_plugin_api() {
  module_load_include('inc', 'feeds_defaults', 'feeds_defaults.defaults');
  $args = func_get_args();
  return call_user_func_array('_feeds_defaults_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_data_default().
 */
function feeds_defaults_data_default() {
  module_load_include('inc', 'feeds_defaults', 'feeds_defaults.defaults');
  $args = func_get_args();
  return call_user_func_array('_feeds_defaults_data_default', $args);
}

/**
 * Implementation of hook_feeds_importer_default().
 */
function feeds_defaults_feeds_importer_default() {
  module_load_include('inc', 'feeds_defaults', 'feeds_defaults.defaults');
  $args = func_get_args();
  return call_user_func_array('_feeds_defaults_feeds_importer_default', $args);
}
