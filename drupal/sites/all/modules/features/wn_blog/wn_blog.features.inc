<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wn_blog_ctools_plugin_api() {
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
function wn_blog_node_info() {
  $items = array(
    'update' => array(
      'name' => t('Update'),
      'module' => 'features',
      'description' => t('An <em>Update</em> is a blog post. By default, comments are enabled for update posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('body'),
      'min_word_count' => '0',
      'help' => t('"Updates" are blog posts that keep your community informed about what your organization is doing.'),
    ),
    'update_feed' => array(
      'name' => t('Update feed'),
      'module' => 'features',
      'description' => t('<em>Update feed</em> nodes are used to pull in feeds as update posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => t('An "Update feed" is the URL of an RSS feed that links to content that you would like to use to populate your blog. Any RSS feed will work, but we recommend you choose RSS feeds that contain content that you have created, such as Posterous.com feeds, Facebook feeds, or feeds from other blogs or sites that you maintain.'),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function wn_blog_views_api() {
  return array(
    'api' => '2',
  );
}
