<?php

/**
 * Implementation of hook_default_box().
 */
function wn_donations_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'donate';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Donation callout';
  $box->options = array(
    'body' => '<p>This is a callout block that appears on your organization\'s "Donate" page. You can use this callout to make quick updates or to describe why people should give to your organization. You can also simply delete the title and body of this block to remove this sidebar. It\'s up to you. You can always add this content back from the <a href="/admin/dashboard">Dashboard</a>.</p>',
    'format' => '3',
  );

  $export['donate'] = $box;
  return $export;
}
