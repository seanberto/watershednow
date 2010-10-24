<?php

/**
 * Implementation of hook_default_box().
 */
function wn_takeaction_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'take_action';
  $box->plugin_key = 'simple';
  $box->title = 'Take Action page callout';
  $box->description = 'Take Action description';
  $box->options = array(
    'body' => '<p>This is a callout block that appears on the "Take Action" listing page. You can use this callout to emphasize where folks can take action to support your mission. You can also simply delete the title and body of this block to remove this sidebar. It\'s up to you. You can always add this content back from the <a href="/admin/dashboard">Dashboard</a>.</p>',
    'format' => '3',
  );

  $export['take_action'] = $box;
  return $export;
}
