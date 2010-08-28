<?php
// $Id: openlayers_behavior_boxselect.inc,v 1.1.2.4 2010/05/18 19:25:57 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * BoxSelect Behavior
 */
class openlayers_behavior_boxselect extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'input_fields' => '',
    );
  }

  function js_dependency() {
    return array(
      'OpenLayers.Control.DrawFeature',
      'OpenLayers.Layer.Vector',
      'OpenLayers.Handler.RegularPolygon'
    );
  }

  function options_form() {
    return array(
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_boxselect.js');
    return $this->options;
  }
}
