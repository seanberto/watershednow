<?php

/**
 * Implementation of hook_openlayers_presets().
 */
function wn_map_openlayers_presets() {
  $export = array();
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'main_map';
  $openlayers_presets->title = 'Watershed map';
  $openlayers_presets->description = 'The main watershed map';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '700px',
    'image_path' => '/sites/all/modules/features/map/openlayers_themes/james/',
    'css_path' => '/sites/all/modules/features/map/openlayers_themes/james/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-77.475585934425, 37.65773212499',
        'zoom' => '8',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_popup' => array(
        'layers' => array(
          'openlayers_kml_layer_node_2' => 'openlayers_kml_layer_node_2',
        ),
      ),
      'openlayers_behavior_fullscreen' => array(
        'activated' => 0,
      ),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 1,
        'roundedCorner' => 0,
        'roundedCornerColor' => 'darkblue',
      ),
      'openlayers_behavior_mouseposition' => array(),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_permalink' => array(),
      'openlayers_behavior_scaleline' => array(),
    ),
    'default_layer' => 'google_physical',
    'layers' => array(
      'google_satellite' => 'google_satellite',
      'google_hybrid' => 'google_hybrid',
      'google_normal' => 'google_normal',
      'google_physical' => 'google_physical',
      'openlayers_kml_layer_node_2' => 'openlayers_kml_layer_node_2',
    ),
    'layer_styles' => array(
      'watershed_feature_data_openlayers_1' => 'features',
      'watershed_report_data_openlayers_1' => 'reports',
    ),
    'layer_activated' => array(
      'openlayers_kml_layer_node_2' => 'openlayers_kml_layer_node_2',
    ),
    'layer_switcher' => array(
      'openlayers_kml_layer_node_2' => 'openlayers_kml_layer_node_2',
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'features',
      'select' => 'default',
      'temporary' => 'default',
    ),
  );

  $export['main_map'] = $openlayers_presets;
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'picker_map';
  $openlayers_presets->title = 'Picker map';
  $openlayers_presets->description = 'A watershed location picker map';
  $openlayers_presets->data = array(
    'width' => 'auto',
    'height' => '350px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-77.475585934425, 37.65773212499',
        'zoom' => '8',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_mouseposition' => array(),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_drawfeatures' => array(
        'feature_types' => array(
          'point' => 'point',
          'path' => 'path',
          'polygon' => 'polygon',
        ),
        'feature_limit' => '0',
        'element_id' => '',
      ),
    ),
    'default_layer' => 'google_physical',
    'layers' => array(
      'google_satellite' => 'google_satellite',
      'google_hybrid' => 'google_hybrid',
      'google_normal' => 'google_normal',
      'google_physical' => 'google_physical',
    ),
    'layer_styles' => array(),
    'layer_activated' => array(),
    'layer_switcher' => array(),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default',
      'temporary' => 'default',
    ),
  );

  $export['picker_map'] = $openlayers_presets;
  $openlayers_presets = new stdClass;
  $openlayers_presets->disabled = FALSE; /* Edit this to true to make a default openlayers_presets disabled initially */
  $openlayers_presets->api_version = 1;
  $openlayers_presets->name = 'sidebar_map';
  $openlayers_presets->title = 'Sidebar map';
  $openlayers_presets->description = 'Sidbar map';
  $openlayers_presets->data = array(
    'width' => '350px',
    'height' => '300px',
    'image_path' => '/sites/all/modules/features/map/openlayers_themes/wn/',
    'css_path' => '/sites/all/modules/features/map/openlayers_themes/wn/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '-77.47559, 37.65773',
        'zoom' => '8',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 0,
      ),
      'openlayers_behavior_panzoom' => array(),
    ),
    'default_layer' => 'google_physical',
    'layers' => array(
      'google_physical' => 'google_physical',
      'watershed_feature_data_with_args_openlayers_1' => 'watershed_feature_data_with_args_openlayers_1',
    ),
    'layer_styles' => array(
      'watershed_feature_data_openlayers_1' => 'features',
      'watershed_feature_data_with_args_openlayers_1' => 'sidebar_features',
      'watershed_report_data_with_args_openlayers_1' => 'sidebar_reports',
    ),
    'layer_activated' => array(
      'watershed_feature_data_with_args_openlayers_1' => 'watershed_feature_data_with_args_openlayers_1',
      'watershed_report_data_with_args_openlayers_1' => 'watershed_report_data_with_args_openlayers_1',
    ),
    'layer_switcher' => array(),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default',
      'temporary' => 'default',
    ),
  );

  $export['sidebar_map'] = $openlayers_presets;
  return $export;
}
