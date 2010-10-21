<?php

/**
 * Implementation of hook_default_box().
 */
function wn_hp_main_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'homepage_promo';
  $box->plugin_key = 'simple';
  $box->title = 'Lorem ipsum lorem ipsum lorem ipsum';
  $box->description = 'Homepage promo';
  $box->options = array(
    'body' => '<p>Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>',
    'format' => '3',
  );

  $export['homepage_promo'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'homepage_secondary';
  $box->plugin_key = 'simple';
  $box->title = 'Sidebar callout';
  $box->description = 'Homepage secondary callout';
  $box->options = array(
    'body' => '<p>This is a secondary callout on the homepage. Users can enter rich HTML blah blah blah blah blah. lorem lorem lorem....</p>',
    'format' => '3',
  );

  $export['homepage_secondary'] = $box;
  return $export;
}
