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
  $box->title = '';
  $box->description = 'Events page header';
  $box->options = array(
    'body' => '<p>This is a placeholder for the event listing page. As you build online campaigns, they will be listed here. Edit this header to provide an overview of your campaigns. You can also simply delete this header all together.</p>',
    'format' => '3',
  );

  $export['events'] = $box;
  return $export;
}
