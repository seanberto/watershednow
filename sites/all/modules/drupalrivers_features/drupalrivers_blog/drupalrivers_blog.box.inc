<?php

/**
 * Implementation of hook_default_box().
 */
function drupalrivers_blog_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'update_add_edit';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Add or edit an update';
  $box->options = array(
    'body' => '<p>Add or edit the <em>Update</em> below. Updates are blog posts on your site.</p>',
    'format' => '3',
  );

  $export['update_add_edit'] = $box;
  return $export;
}
