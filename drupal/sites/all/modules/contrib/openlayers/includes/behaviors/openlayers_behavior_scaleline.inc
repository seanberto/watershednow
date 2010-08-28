<?php
// $Id: openlayers_behavior_scaleline.inc,v 1.1.2.4 2010/05/18 19:25:57 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Scaleline Behavior
 */
class openlayers_behavior_scaleline extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'scaleline' => '',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.ScaleLine');
  }

  function options_form($defaults) {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_scaleline.js');
    return $this->options;
  }
}
