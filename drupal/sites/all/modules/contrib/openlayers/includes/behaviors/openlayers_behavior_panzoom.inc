<?php
// $Id: openlayers_behavior_panzoom.inc,v 1.1.2.3 2010/05/18 19:25:57 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Pan Zoom Bar Behavior
 */
class openlayers_behavior_panzoom extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'panzoom' => '',
    );
  }

  function options_form($defaults) {
    return array(
      'spec' => openlayers_dependency_widget('OpenLayers.Control.PanZoom')
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_panzoom.js');
    return $this->options;
  }
}
