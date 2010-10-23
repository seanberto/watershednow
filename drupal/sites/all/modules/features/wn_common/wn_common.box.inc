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
  $box->title = 'Contact form callout';
  $box->description = 'Contact form';
  $box->options = array(
    'body' => '<p>This is a callout block that appears on your primary contact form. You can use this callout to say a little bit more about how to folks can get in touch with your organization. You can also simply delete the title and body of this block to remove this sidebar. It\'s up to you. You can always add this content back from the <a href="/admin/dashboard">Dashboard</a>.</p>',
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
    'body' => '<div class="footer">© 2010. Drupal for Rivers is an open source product developed by <a href="http://thinkshout.com" target="_blank">ThinkShout.com</a>.</div>',
    'format' => '3',
  );

  $export['customizable_footer'] = $box;
  return $export;
}
