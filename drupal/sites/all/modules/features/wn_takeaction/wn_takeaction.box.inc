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
  $box->title = '';
  $box->description = 'take action page header';
  $box->options = array(
    'body' => '<p>This is a placeholder for the listing of opportunities to take action. As you build online campaigns, they will be listed here. Edit this header to provide an overview of your campaigns. You can also simply delete this header all together.</p>',
    'format' => '3',
  );

  $export['take_action'] = $box;
  return $export;
}
