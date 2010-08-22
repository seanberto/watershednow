<?php

/**
 * Implementation of hook_strongarm().
 */
function drupalrivers_map_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_map_point';
  $strongarm->value = '4';

  $export['comment_default_mode_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_map_point';
  $strongarm->value = '1';

  $export['comment_default_order_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_map_point';
  $strongarm->value = '0';

  $export['comment_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_map_point';
  $strongarm->value = '1';

  $export['comment_preview_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_map_point';
  $strongarm->value = '1';

  $export['comment_subject_field_map_point'] = $strongarm;
  return $export;
}
