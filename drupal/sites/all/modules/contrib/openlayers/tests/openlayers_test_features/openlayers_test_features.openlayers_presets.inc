<?php

/**
 * Implementation of hook_openlayers_presets().
 */
function openlayers_test_features_openlayers_presets() {
  $export = array();
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'openlayers_test_features_map';
  $openlayers_presets->title = 'Test Features Map: Example';
  $openlayers_presets->description = 'This is a test map for Features, providing an example.';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '300px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_argparser' => array(),
      'openlayers_behavior_fullscreen' => array(
        'activated' => 0,
      ),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'roundedCorner' => 1,
        'roundedCornerColor' => 'darkblue',
      ),
      'openlayers_behavior_mouseposition' => array(),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 1,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_permalink' => array(),
      'openlayers_behavior_scaleline' => array(),
      'openlayers_behavior_zoombox' => array(),
    ),
    'default_layer' => 'osm_mapnik',
    'layers' => array(
      'openlayers_test_features_layer_google' => 'openlayers_test_features_layer_google',
      'osm_mapnik' => 'osm_mapnik',
    ),
    'layer_styles' => array(),
    'layer_activated' => array(),
    'layer_switcher' => array(),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'openlayers_test_features_style_example',
      'select' => 'openlayers_test_features_style_example',
      'temporary' => 'openlayers_test_features_style_example',
    ),
  );

  $export['openlayers_test_features_map'] = $openlayers_presets;
  return $export;
}
