<?php
// $Id: openlayers_taxonomy_vector.inc,v 1.1 2010/10/18 16:29:33 loubabe Exp $

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
  function get_features($term_id, $view_name, $view_display = 'openlayers_1') {
    if ($view = views_get_view($view_name)) {
      $features = $view->execute_display($view_display, array($term_id));

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
    $features = $this->get_features($this->data['term_id'], $this->data['view_name'], $this->data['view_display']);
    $this->data['features'] = $features;
    return $this->options;
  }
}
