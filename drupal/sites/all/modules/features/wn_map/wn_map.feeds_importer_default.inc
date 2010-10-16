<?php

/**
 * Implementation of hook_feeds_importer_default().
 */
function wn_map_feeds_importer_default() {
  $export = array();
  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'feature_import';
  $feeds_importer->config = array(
    'name' => 'feature_import',
    'description' => 'Import of map feature data',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => 'TAB',
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'map_feature',
        'input_format' => '2',
        'update_existing' => '0',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'wkt',
            'target' => 'field_wkt',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'location type',
            'target' => 'taxonomy:1',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'description',
            'target' => 'field_description',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'address',
            'target' => 'field_address',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'hours',
            'target' => 'field_hours',
            'unique' => FALSE,
          ),
        ),
        'author' => '1',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );

  $export['feature_import'] = $feeds_importer;
  return $export;
}
