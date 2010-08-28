<?php
// $Id: openlayers_geocoder_zoomtolocation.inc,v 1.1.2.1 2010/04/26 08:58:48 antoniodemarco Exp $

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Zoom to a location
 */
class openlayers_geocoder_zoomtolocation extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'query' => '',
    );
  }

  function options_form($defaults = array()) {

    $form['query'] = array(
      '#type' => 'textfield',
      '#title' => t('Geocoder query'),
      '#default_value' => isset($defaults['query']) ? $defaults['query'] : '',
      '#description' => t('Query submitted to the geocoder. You can use replacement patterns below.'),
    );
    $form['query_tokens'] = array(
      '#title' => t('Query replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['query_tokens']['help']['#value'] = theme('token_help', 'openlayers data');

    return $form;
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_geocoder') .'/includes/behaviors/js/openlayers_geocoder_zoomtolocation.js');

    $return = array();
    $query = token_replace($this->options['query'], 'openlayers data', $this->map);
    if($response = openlayers_geocoder_response($query)) {
      $return = $response[0]['bounds'];
    }

    return $return;
  }
}

