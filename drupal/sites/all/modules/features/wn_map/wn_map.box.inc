<?php

/**
 * Implementation of hook_default_box().
 */
function wn_map_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'main_map';
  $box->plugin_key = 'simple';
  $box->title = 'Your Conservation Map';
  $box->description = 'Map callout';
  $box->options = array(
    'body' => '<p>This is a header that you can customize at the top of your conservation map. You can add <a href="/node/add/map-feature">points, lines and polygons</a> on the map below. If you are a web admin, you can manage your own set of <a href="/admin/content/taxonomy/taxonomy_image">map point icons</a>. And, you can <a href="/node/add/map-kml">upload KML files</a> that will automatically display on this map. Once you have created some content, the map will automatically zoom in for the optimal display of your content.</p>',
    'format' => '3',
  );

  $export['main_map'] = $box;
  return $export;
}
