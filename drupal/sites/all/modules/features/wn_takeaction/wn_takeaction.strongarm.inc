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
  return $export;
}
