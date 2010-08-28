<?php
// $Id: openlayers_behavior_keyboarddefaults.inc,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Keyboard Defaults Behavior
 */
class openlayers_behavior_keyboarddefaults extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'keyboarddefaults' => '',
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_keyboarddefaults.js');
    return $this->options;
  }
}
