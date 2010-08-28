<?php

/**
 * Implementation of hook_strongarm().
 */
function watershednow_map_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_map_feature';
  $strongarm->value = 0;

  $export['comment_anonymous_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_map_feature';
  $strongarm->value = '3';

  $export['comment_controls_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_map_feature';
  $strongarm->value = '4';

  $export['comment_default_mode_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_map_feature';
  $strongarm->value = '1';

  $export['comment_default_order_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_map_feature';
  $strongarm->value = '50';

  $export['comment_default_per_page_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_map_feature';
  $strongarm->value = '0';

  $export['comment_form_location_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_map_feature';
  $strongarm->value = '0';

  $export['comment_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_map_feature';
  $strongarm->value = '1';

  $export['comment_preview_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_map_feature';
  $strongarm->value = '1';

  $export['comment_subject_field_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_map_feature';
  $strongarm->value = array(
    'title' => '-3',
    'body_field' => '-2',
    'revision_information' => '3',
    'author' => '4',
    'options' => '5',
    'comment_settings' => '7',
    'menu' => '2',
    'taxonomy' => '-4',
    'path' => '6',
  );

  $export['content_extra_weights_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'enable_revisions_page_map_feature';
  $strongarm->value = 0;

  $export['enable_revisions_page_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_map_feature';
  $strongarm->value = array(
    0 => 'status',
  );

  $export['node_options_map_feature'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_map_feature_pattern';
  $strongarm->value = 'watershed/feature/[title-raw]';

  $export['pathauto_node_map_feature_pattern'] = $strongarm;
  return $export;
}
