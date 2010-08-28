<?php
// $Id: openlayers_behavior_mapformvalues.inc,v 1.1.2.4 2010/02/04 17:43:12 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Map Form Values Behavior
 */
class openlayers_behavior_mapformvalues extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'center_form' => '#edit-center-initial-centerpoint',
      'zoom_form' => '#edit-center-initial-zoom'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_mapformvalues.js');
    return $this->options;
  }
}
