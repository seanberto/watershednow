<?php

/**
 * Implementation of hook_default_box().
 */
function wn_blog_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'blog_description';
  $box->plugin_key = 'simple';
  $box->title = 'About our updates';
  $box->description = 'Description of our blog';
  $box->options = array(
    'body' => 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.',
    'format' => '1',
  );

  $export['blog_description'] = $box;
  return $export;
}
