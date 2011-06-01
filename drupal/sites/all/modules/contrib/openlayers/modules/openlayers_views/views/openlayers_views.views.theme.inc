<?php

/**
 * @file
 * This file holds views theme function for style plugin.
 *
 * @ingroup openlayers
 */

/**
 * Implementation of theme function
 */
function theme_openlayers_views_map($view, $options = array(), $map = array(), $title = FALSE) {
  $title = ($title) ? '<h3>'. $title .'</h3>' : '';
  $output = '
    '. $title .'
    <div class="openlayers-views-map">
      '. $map['themed'] .'
    </div>
  ';

  return $output;
}
