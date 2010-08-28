<?php
// $Id: openlayers_views.views.theme.inc,v 1.3.2.1 2009/09/27 21:24:09 phayes Exp $

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
