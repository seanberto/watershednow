<?php
// $Id: openlayers_cck.generate.inc,v 1.1.2.2 2010/03/22 23:55:10 zzolo Exp $

/**
 * @file
 * Implementations of content generate for openlayers_cck
 */

/**
 * Private callback for same hook.
 */
function _openlayers_cck_content_generate(&$node, $field) {
  $node_field = array();
  $types = array();
  $value = NULL;
  $multiple_count = $field['multiple'];

  // Check types
  foreach ($field['openlayers_cck_feature_types'] as $type => $available) {
    if ($available) {
      $types[] = $type;
    }
  }

  // Check multiple
  if ($field['multiple'] == 0) {
    // Only one
    $multiple_count = 1;
  }
  elseif ($field['multiple'] == 1) {
    // Unlimited
    $multiple_count = rand(3, 11);
  }

  // Create WKT
  for ($i = 0; $i < $multiple_count; $i++) {
    $type = $types[array_rand($types)];
    switch ($type) {
      case 'point':
        $value = 'POINT('. _openlayers_cck_generate_pair() . ')';
        break;

      case 'path':
        // Make random number of points
        $line_array = array();
        for ($j = 1; $j <= rand(1, 12); $j++) {
          $line_array[] = _openlayers_cck_generate_pair();
        }
        $value = 'LINESTRING(' . implode(',', $line_array) . ')';
        break;

      case 'polygon':
        // Make random number of points
        $poly_array = array();
        for ($j = 1; $j <= rand(1, 12); $j++) {
          $poly_array[] = _openlayers_cck_generate_pair();
        }
        $value = 'POLYGON((' . implode(',', $poly_array) . '))';
        break;
    }
    $node_field[$i] = array(
      'openlayers_wkt' => $value,
    );
  }

  // Check values
  if (count($node_field) > 0) {
    return $node_field;
  }
}

/**
 * Generate Random Lat and Lon
 *
 * @return
 *   A space separated longitude and latitiude
 */
function _openlayers_cck_generate_pair() {
  $lat = (float)((mt_rand(0, 120000) - 60000)/1000);
  $lon = (float)((mt_rand(0, 360000) - 180000)/1000);
  return ((string) $lon) . ' ' . ((string) $lat);
}
