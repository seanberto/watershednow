<?php

/**
 * Implementation of hook_strongarm().
 */
function wn_hp_callout_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_wn_hp_callout';
  $strongarm->value = 0;

  $export['comment_anonymous_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_wn_hp_callout';
  $strongarm->value = '3';

  $export['comment_controls_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_wn_hp_callout';
  $strongarm->value = '4';

  $export['comment_default_mode_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_wn_hp_callout';
  $strongarm->value = '1';

  $export['comment_default_order_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_wn_hp_callout';
  $strongarm->value = '50';

  $export['comment_default_per_page_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_wn_hp_callout';
  $strongarm->value = '0';

  $export['comment_form_location_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_wn_hp_callout';
  $strongarm->value = '1';

  $export['comment_preview_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_wn_hp_callout';
  $strongarm->value = '1';

  $export['comment_subject_field_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_wn_hp_callout';
  $strongarm->value = '2';

  $export['comment_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_wn_hp_callout';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '-2',
    'author' => '-1',
    'options' => '0',
    'comment_settings' => '1',
    'menu' => '-3',
    'path' => '2',
  );

  $export['content_extra_weights_wn_hp_callout'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_wn_hp_callout';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );

  $export['node_options_wn_hp_callout'] = $strongarm;
  return $export;
}
