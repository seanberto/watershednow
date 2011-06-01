<?php

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
      'ascending' => TRUE,
      'roundedCorner' => TRUE,
      'roundedCornerColor' => 'darkblue',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.LayerSwitcher');
  }

  function options_form($defaults) {
    return array(
      'ascending' => array(
        '#type' => 'checkbox',
        '#title' => t('Show layers in ascending order'),
        '#description' => t('Higher layers shown toward the bottom'),
        '#default_value' => isset($defaults['ascending']) ? $defaults['ascending'] : TRUE
      ),
      'roundedCorner' => array(
        '#type' => 'checkbox',
        '#title' => t('Round corners'),
        '#description' => t('If true the Rico library is used for rounding the corners of the layer switcher div.'),
        '#default_value' => isset($defaults['roundedCorner']) ? $defaults['roundedCorner'] : TRUE
      ),
      'roundedCornerColor' => array(
        '#type' => 'textfield',
        '#title' => t('Round corners color'),
        '#description' => t('The color of the rounded corners, only applies if roundedCorner is true.'),
        '#default_value' => isset($defaults['roundedCornerColor']) ? $defaults['roundedCornerColor'] : 'darkblue' ,
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_layerswitcher.js');
    return $this->options;
  }
}
