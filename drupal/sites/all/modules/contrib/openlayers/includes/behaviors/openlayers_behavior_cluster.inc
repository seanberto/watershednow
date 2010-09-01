<?php
// $Id: openlayers_behavior_cluster.inc,v 1.1.2.2 2010/06/07 20:41:40 tmcw Exp $

/**
 * @file
 * Implementation of OpenLayers Cluster behavior.
 */

/**
 * Cluster behavior
 */
class openlayers_behavior_cluster extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'distance' => '20',
      'threshold' => NULL,
    );
  }

  /**
   * OpenLayers library dependency.
   */
  function js_dependency() {
    return array('OpenLayers.Strategy.Cluster');
  }

  /**
   * Provide form for configurations per map.
   */
  function options_form($defaults) {
    // Only prompt for vector layers
    $vector_layers = array();
    foreach ($this->map['layers'] as $id => $name) {
      $layer = openlayers_layer_load($id);
      if ($layer->data['layer_type'] == 'openlayers_views_vector') {
        $vector_layers[$id] = $name;
      }
    }

    return array(
      'clusterlayer' => array(
        '#type' => 'select',
        '#options' => $vector_layers,
        '#description' => t('Select layer to cluster'),
        '#default_value' => isset($defaults['clusterlayer']) ?
        $defaults['clusterlayer'] : NULL,
      ),
      'distance' => array(
        '#type' => 'textfield',
        '#default_value' => (isset($defaults['distance'])) ?
        $defaults['distance'] : 20,
        '#size' => 5,
        '#title' => t('Distance'),
        '#description' => t('Pixel distance between features that should ' .
        'be considered a single cluster'),
      ),
      'threshold' => array(
        '#type' => 'textfield',
        '#default_value' => (isset($defaults['threshold'])) ?
        $defaults['threshold'] : NULL,
        '#size' => 5,
        '#title' => t('Threshold'),
        '#description' => t('Optional threshold below which original ' .
        'features will be added to the layer instead of clusters'),
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') . 
      '/includes/behaviors/js/openlayers_behavior_cluster.js');
    return $this->options;
  }
}
