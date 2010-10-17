<?php
// $Id: openlayers_views_vector.inc,v 1.1.2.8 2010/05/14 15:13:52 tmcw Exp $

/**
 * @file
 * OpenLayers Views Vector Layer Type
 */
class openlayers_layer_type_openlayers_taxonomy_vector extends openlayers_layer_type {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layer_type' => 'openlayers_layer_type_openlayers_taxonomy_vector',
      'layer_handler' => 'openlayers_taxonomy_vector',
      'vector' => TRUE,
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    return array(
    );
  }

  /**
   * Get features for this view
   *
   * @param $view_name
   *   the view name
   * @param $display_name
   *   the view display name
   * @return
   *   array of features
   */
  function get_features($term_id, $view_name = 'watershed_feature_data') {

    if ($view = views_get_view($view_name)) {
      $features = $view->execute_display('openlayers_1', array($term_id));

      // Get dynamic title from view
      $term = taxonomy_get_term($term_id);
      $this->title = $term->name;

      $view->destroy();
    }
    return isset($features) ? $features : array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_taxonomy') .
      '/includes/layer_types/openlayers_taxonomy_vector.js');
    $features = $this->get_features($this->data['term_id']);
    $this->data['features'] = $features;
    return $this->options;
  }
}
