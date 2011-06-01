<?php

/**
 * @file
 * This file holds the theme functions for openlayers_views module
 *
 * @ingroup openlayers
 */

/**
 * Theme function for openlayers_views_group_display_item
 *
 * Output of OpenLayers View diplay plugin for grouped items.
 */
function theme_openlayers_views_group_display_item($name = '', $description = '') {
  $output = '';

  // Build output
  if (!empty($name)) {
    $output .= '<h2 class="point-title openlayers-views-group-feature-name">' . $name . '</h2>';
  } 
  if (!empty($description)) {
    $output .= '<div class="point-content openlayers-views-group-feature-description">' . $description . '</div>';
  } 

  return $output;
}

/**
 * Theme function for openlayers_views_render_feature
 */
function theme_openlayers_views_render_feature($feature ='', $record = '') {
  return $feature;
}
