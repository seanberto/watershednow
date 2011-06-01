<?php

/**
 * @file
 * Test 
 */

// Define plugin for CTools
$plugin = array(
  'title' => t('TEST: random integer'),
  'description' => t('Example style plugin for context styling. '
    . 'Provides random integers.'),
  'style_plugin' => array(
    'class' => 'openlayers_test_rnd_int',
    'parent' => 'openlayers_style_plugin',
  ),
);

/**
 * Style Plugin for testing purposes.
 */
class openlayers_test_rnd_int extends 
  openlayers_style_plugin {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'low' => 1,
      'high' => 10,
    );
  }
  
  /**
   * Options form.
   */
  function options_form($defaults = array()) {
    $form = array();
    
    // Allow use to pick the highest and lowest for random
    // point radius
    $form['low'] = array(
      '#type' => 'textfield',
      '#title' => t('Lowest value'),
      '#description' => t('Lowest value for the random integer.'),
      '#default_value' => isset($defaults['low']) ?
        $defaults['low'] : 2,
    );
    $form['high'] = array(
      '#type' => 'textfield',
      '#title' => t('Highest value'),
      '#description' => t('Highest value for the random integer.'),
      '#default_value' => isset($defaults['high']) ?
        $defaults['high'] : 10,
    );
    
    return $form;
  }

  /**
   * Get an array of style property callbacks
   */
  function get_context_properties() {
    return array(
      'pointRadius' => 'getInt',
      'strokeWidth' => 'getInt',
      'graphicWidth' => 'getInt',
      'graphicHeight' => 'getInt',
      'graphicXOffset' => 'getInt',
      'graphicYOffset' => 'getInt',
      'rotation' => 'getInt',
      'labelXOffset' => 'getInt',
      'labelYOffset' => 'getInt',
      'fontSize' => 'getInt',
    );
  }


  /**
   * Render function
   */
  function render() {
    // Add JS
    drupal_add_js(drupal_get_path('module', 'openlayers_test') .
      '/plugins/style_plugin/openlayers_test_rnd_int.js');
  }
}
