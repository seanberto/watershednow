<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalrivers_blog_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function drupalrivers_blog_node_info() {
  $items = array(
    'update' => array(
      'name' => t('Update'),
      'module' => 'features',
      'description' => t('An <em>Update</em> is a blog post. By default, comments are enabled for update posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'update_feed' => array(
      'name' => t('Update feed'),
      'module' => 'features',
      'description' => t('Update feed nodes are used to pull in feeds as update posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function drupalrivers_blog_views_api() {
  return array(
    'api' => '2',
  );
}
