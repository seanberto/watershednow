<?php

/**
 * Implementation of hook_strongarm().
 */
function wn_takeaction_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'webform_node_types';
  $strongarm->value = array(
    0 => 'take_action',
  );

  $export['webform_node_types'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_take_action';
  $strongarm->value = array(
    'webform' => '1',
    'title' => '-5',
    'revision_information' => '3',
    'author' => '4',
    'options' => '5',
    'comment_settings' => '6',
    'menu' => '2',
    'path' => '7',
  );

  $export['content_extra_weights_take_action'] = $strongarm;
  
  return $export;
}
