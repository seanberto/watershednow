<?php

/**
 * Implementation of hook_openlayers_layers().
 */
function openlayers_test_features_openlayers_layers() {
  $export = array();
  $openlayers_layers = new stdClass;
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'openlayers_test_features_layer_google';
  $openlayers_layers->title = 'Test Features Layer: Google';
  $openlayers_layers->description = 'This is a test layer for Features, providing a Google layer.';
  $openlayers_layers->data = array(
    'type' => 'hybrid',
    'layer_type' => 'openlayers_layer_type_google',
    'numZoomLevels' => '15',
  );

  $export['openlayers_test_features_layer_google'] = $openlayers_layers;
  return $export;
}
