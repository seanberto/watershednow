<?php
// $Id: openlayers_cck.theme.inc,v 1.1.2.7 2010/05/13 23:07:37 zzolo Exp $

/**
 * @file
 * Theme functions for openlayers_cck
 */

/**
 * Theme function for openlayers_wkt_widget element
 */
function theme_openlayers_wkt_widget($element) {
  return $element['#children'];
}

/**
 * Theme function for WKT CCK formatter
 */
function theme_openlayers_cck_formatter_openlayers_wkt($element) {
  $features = array();

  // Create array of $features
  foreach (element_children($element) as $delta) {
    $features[$delta] = $element[$delta]['#item']['openlayers_wkt'];
  }

  // Clean features array of empty and null values
  $features = array_filter($features);
  return array_pop($features);
}

/**
 * Theme function for openlayers cck formatters
 */
function theme_openlayers_cck_formatter_map($element) {
  $features = array();
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $widget = $field['widget'];

  // Create array of $features
  foreach (element_children($element) as $delta) {
    $features[] = array(
      // First look for geo_field data, then for openlayers wkt data
      'wkt' => $element[$delta]['#item']['openlayers_wkt'],
      'projection' => '4326',
    );
  }

  // Get preset name from formatter. Strip off 'openlayers_cck_formatter_'
  // and 'openlayers_map_' from the beginning.
  // If none found, use default map.
  // TODO: Better way to do this?
  if (drupal_substr($element['#formatter'], 25) == 'openlayers_cck_formatter_') {
    $preset_name = drupal_substr($element['#formatter'], 25);
  }
  elseif (drupal_substr($element['#formatter'], 0, 15) == 'openlayers_map_') {
    $preset_name = drupal_substr($element['#formatter'], 15);
  }
  else {
    $preset_name = variable_get('openlayers_default_preset', 'default');
  }

  // Get map preset
  $preset = openlayers_preset_load($preset_name);

  // Add vector layer behavior
  $preset->data['behaviors']['openlayers_cck_vector_layer'] =
    array('features' => $features);

  // Add widget/formatter behaviors
  if (isset($widget['openlayers_behaviors']) && is_array($widget['openlayers_behaviors'])) {
    foreach ($widget['openlayers_behaviors'] as $key => $behavior) {
      if ($behavior['enabled']) {
        $preset->data['behaviors'][$key] = $behavior['options'];
      }
    }
  }

  // Render map
  return openlayers_render_map($preset->data, $preset->name);
}

/**
 * Theme function for openlayers_cck_map
 */
function theme_openlayers_cck_map($field = array(), $map = array()) {
  // TODO: fix
  return $map;
}
