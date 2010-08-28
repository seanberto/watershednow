<?php
// $Id: openlayers_behavior_mouseposition.inc,v 1.1.2.4 2010/05/18 19:25:57 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Mouse Position Behavior
 */
class openlayers_behavior_mouseposition extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'mouseposition' => '',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.MousePosition');
  }

  function options_form($defaults) {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_mouseposition.js');
    return $this->options;
  }
}
