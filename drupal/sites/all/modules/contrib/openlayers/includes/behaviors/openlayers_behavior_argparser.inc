<?php
// $Id: openlayers_behavior_argparser.inc,v 1.1.2.1 2010/05/30 19:02:01 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Attribution Behavior
 */
class openlayers_behavior_argparser extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'attribution' => '',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.ArgParser');
  }

  function options_form($defaults) {
    return array(
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_argparser.js');
    return $this->options;
  }
}
