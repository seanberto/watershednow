<?php
// $Id: openlayers_behavior_fullscreen.inc,v 1.1.2.1 2010/02/02 18:44:45 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Fullscreen Behavior
 */
class openlayers_behavior_fullscreen extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'input_fields' => '',
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_fullscreen.css');
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_fullscreen.js');
    return $this->options;
  }
}

