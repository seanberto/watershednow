<?php

/**
 * Implementation of hook_openlayers_styles().
 */
function openlayers_test_features_openlayers_styles() {
  $export = array();
  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'openlayers_test_features_style_example';
  $openlayers_styles->title = 'Test Features Style: Example';
  $openlayers_styles->description = 'This is a test style for Features, providing a basic style.';
  $openlayers_styles->data = array(
    'pointRadius' => 10,
    'fillColor' => '#BBBBBB',
    'strokeColor' => '#222222',
    'strokeWidth' => 3,
    'fillOpacity' => 0.2,
    'strokeOpacity' => 0.8,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );

  $export['openlayers_test_features_style_example'] = $openlayers_styles;
  return $export;
}
