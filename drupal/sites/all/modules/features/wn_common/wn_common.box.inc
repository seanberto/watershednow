<?php

/**
 * Implementation of hook_default_box().
 */
function wn_common_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'contact_form';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Contact form';
  $box->options = array(
    'body' => 'This is editable sidebar content on the contact form.',
    'format' => '3',
  );

  $export['contact_form'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'customizable_footer';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Customizable footer';
  $box->options = array(
    'body' => '[Place your own footer text here.]
© 2010. Drupal for Rivers is an open source product developed by <a href="http://thinkshout.com" target="_blank">ThinkShout.com</a>.</div>',
    'format' => '3',
  );

  $export['customizable_footer'] = $box;
  return $export;
}
