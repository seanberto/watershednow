<?php

/**
 * Implementation of hook_default_box().
 */
function wn_showcase_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'showcase_1';
  $box->plugin_key = 'simple';
  $box->title = 'Showcase 1';
  $box->description = 'Neuse Showcase 1';
  $box->options = array(
    'body' => 'This is a callout block that appears on the homepage page with the Neuse themes. You can use this callout to emphasize where folks can take action to support your mission. You can also simply delete the title and body of this block to remove this, it is up to you.',
    'format' => '3',
  );

  $export['showcase_1'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'showcase_2';
  $box->plugin_key = 'simple';
  $box->title = 'Showcase 2';
  $box->description = 'Neuse Showcase 2';
  $box->options = array(
    'body' => 'This is a callout block that appears on the homepage page with the Neuse themes. You can use this callout to emphasize where folks can take action to support your mission. You can also simply delete the title and body of this block to remove this, it is up to you.',
    'format' => '3',
  );

  $export['showcase_2'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'showcase_3';
  $box->plugin_key = 'simple';
  $box->title = 'Showcase 3';
  $box->description = 'Neuse Showcase 3';
  $box->options = array(
    'body' => 'This is a callout block that appears on the homepage page with the Neuse themes. You can use this callout to emphasize where folks can take action to support your mission. You can also simply delete the title and body of this block to remove this, it is up to you.',
    'format' => '3',
  );

  $export['showcase_3'] = $box;
  return $export;
}
