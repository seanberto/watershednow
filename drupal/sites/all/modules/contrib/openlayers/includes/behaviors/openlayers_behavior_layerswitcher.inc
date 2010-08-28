<?php
// $Id: openlayers_behavior_layerswitcher.inc,v 1.1.2.4 2010/05/18 19:25:57 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Layer Switcher Behavior
 */
class openlayers_behavior_layerswitcher extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layerswitcher' => '',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.LayerSwitcher');
  }

  function options_form($defaults) {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_layerswitcher.js');
    return $this->options;
  }
}
