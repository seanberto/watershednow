<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wn_map_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
  elseif ($module == "openlayers" && $api == "openlayers_layers") {
    return array("version" => 1);
  }
  elseif ($module == "openlayers" && $api == "openlayers_presets") {
    return array("version" => 1);
  }
  elseif ($module == "openlayers" && $api == "openlayers_styles") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function wn_map_imagecache_default_presets() {
  $items = array(
    'homepage_thumb' => array(
      'presetname' => 'homepage_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '75',
            'height' => '75',
          ),
        ),
      ),
    ),
    'main_map' => array(
      'presetname' => 'main_map',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '',
            'height' => '150',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'map_gallery_slide' => array(
      'presetname' => 'map_gallery_slide',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '350',
            'height' => '219',
          ),
        ),
      ),
    ),
    'map_gallery_thumb' => array(
      'presetname' => 'map_gallery_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '85',
            'height' => '58',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function wn_map_node_info() {
  $items = array(
    'map_feature' => array(
      'name' => t('Feature'),
      'module' => 'features',
      'description' => t('A <em>Watershed Feature</em> represents a point of interest on a map of a watershed. A watershed feature can be a point, a polyline or a polygon - or a collection of any of the three.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'map_kml' => array(
      'name' => t('KML layer'),
      'module' => 'features',
      'description' => t('KML layer nodes are used to manage KML files presented on OpenLayers map presets.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => t('Upload a KML file to be presented on OpenLayer\'s map presets.'),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function wn_map_views_api() {
  return array(
    'api' => '2',
  );
}
