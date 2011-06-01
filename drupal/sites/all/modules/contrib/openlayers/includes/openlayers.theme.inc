<?php

/**
 * @file
 * This file holds the theme functions for openlayers module
 *
 * @ingroup openlayers
 */

/**
 * Theme function for openlayers_map
 */
function theme_openlayers_map($map = array(), $preset_name = '') {
  // To ensure that any controls are on the map correctly, we need to
  // wrap the map in a container, and take into account percentage dimensions
  $container_width = $map['width'];
  $container_height = $map['height'];
  $map['width'] = (strpos($map['width'], '%') > 0) ? '100%' : $map['width'];
  $map['height'] = (strpos($map['height'], '%') > 0) ? '100%' : $map['height'];

  // Create output
  $output = '
    <div style="width: ' . $container_width . '; height: ' . $container_height . ';" id="openlayers-container-'. $map['id'] .'" class="openlayers-container openlayers-container-preset-' . $preset_name .'">
      <div style="width: ' . $map['width'] . '; height: ' . $map['height'] . ';" id="'. $map['id'] .'" class="openlayers-map openlayers-preset-' . $preset_name .'"></div>
    </div>
  ';
  return $output;
}

/**
 * Theme function to be able to override styles
 */
function theme_openlayers_styles($styles = array(), $map = array()) {
  // Default is to just send the processed styles back
  return $styles;
}
