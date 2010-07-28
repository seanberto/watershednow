<?php

/**
 * Implementation of hook_default_box().
 */
function drupalrivers_blog_default_box() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_default_box', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function drupalrivers_blog_context_default_contexts() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_context_default_contexts', $args);
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalrivers_blog_ctools_plugin_api() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_feeds_importer_default().
 */
function drupalrivers_blog_feeds_importer_default() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_feeds_importer_default', $args);
}

/**
 * Implementation of hook_node_info().
 */
function drupalrivers_blog_node_info() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.features.node');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_node_info', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function drupalrivers_blog_strongarm() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_strongarm', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function drupalrivers_blog_views_default_views() {
  module_load_include('inc', 'drupalrivers_blog', 'drupalrivers_blog.features.views');
  $args = func_get_args();
  return call_user_func_array('_drupalrivers_blog_views_default_views', $args);
}
