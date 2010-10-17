<?php

/**
 * Implementation of hook_strongarm().
 */
function wn_donations_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_donation';
  $strongarm->value = 0;

  $export['comment_anonymous_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_donation';
  $strongarm->value = '3';

  $export['comment_controls_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_donation';
  $strongarm->value = '4';

  $export['comment_default_mode_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_donation';
  $strongarm->value = '1';

  $export['comment_default_order_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_donation';
  $strongarm->value = '50';

  $export['comment_default_per_page_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_donation';
  $strongarm->value = '0';

  $export['comment_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_donation';
  $strongarm->value = '0';

  $export['comment_form_location_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_donation';
  $strongarm->value = '1';

  $export['comment_preview_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_donation';
  $strongarm->value = '1';

  $export['comment_subject_field_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_donation';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '2',
    'author' => '3',
    'options' => '4',
    'comment_settings' => '5',
    'menu' => '1',
    'path' => '6',
  );

  $export['content_extra_weights_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'enable_revisions_page_donation';
  $strongarm->value = 0;

  $export['enable_revisions_page_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_donation';
  $strongarm->value = array(
    0 => 'status',
  );

  $export['node_options_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_donation_pattern';
  $strongarm->value = 'donations/[title-raw]';

  $export['pathauto_node_donation_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'simple_donations_allowed_donation';
  $strongarm->value = 1;

  $export['simple_donations_allowed_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'simple_donations_button_donation';
  $strongarm->value = 'Donate now!';

  $export['simple_donations_button_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'simple_donations_description_donation';
  $strongarm->value = 'Enter the amount you wish to donate.';

  $export['simple_donations_description_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'simple_donations_teaser_donation';
  $strongarm->value = 1;

  $export['simple_donations_teaser_donation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'simple_donations_title_donation';
  $strongarm->value = 'Amount';

  $export['simple_donations_title_donation'] = $strongarm;
  return $export;
}
