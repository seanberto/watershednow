<?php

/**
 * Implementation of hook_default_box().
 */
function drupalrivers_map_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'map_point_add_edit';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Add or edit a map point';
  $box->options = array(
    'body' => 'Use the form below to add or edit a map point on the watershed map.',
    'format' => '1',
  );

  $export['map_point_add_edit'] = $box;
  return $export;
}
