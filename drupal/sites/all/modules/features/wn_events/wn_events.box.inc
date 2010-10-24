<?php

/**
 * Implementation of hook_default_box().
 */
function wn_events_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'events';
  $box->plugin_key = 'simple';
  $box->title = 'Events listing callout';
  $box->description = 'Events callout';
  $box->options = array(
    'body' => '<p>This is a callout block that appears on the main events listing page. You can use this callout to make quick updates or to describe the types of events that your organization hosts. You can also simply delete the title and body of this block to remove this sidebar. It\'s up to you. You can always add this content back from the <a href="/admin/dashboard">Dashboard</a>.</p>',
    'format' => '3',
  );

  $export['events'] = $box;
  return $export;
}
