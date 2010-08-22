<?php

/**
 * Implementation of hook_default_box().
 */
function drupalrivers_common_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'page_add_edit';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Add or edit a page';
  $box->options = array(
    'body' => '<p>Please add or edit the <em>Page</em> below. Pages contain general information about your watershed organization.</p>',
    'format' => '3',
  );

  $export['page_add_edit'] = $box;
  return $export;
}
