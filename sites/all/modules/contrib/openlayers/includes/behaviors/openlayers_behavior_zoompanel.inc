<?php
// $Id: openlayers_behavior_zoompanel.inc,v 1.1.2.2 2010/05/28 15:37:30 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Zoom Panel Behavior
 */
class openlayers_behavior_zoompanel extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'panzoom' => '',
    );
  }

  function js_dependency() {
    return array(
      'OpenLayers.Control.ZoomPanel'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_zoompanel.js');
    return $this->options;
  }
}
