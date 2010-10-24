<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_map_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'main_map';
  $context->description = 'Context triggered when viewing the main map';
  $context->tag = 'wn_map';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'map' => 'map',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-main_map' => array(
          'module' => 'boxes',
          'delta' => 'main_map',
          'region' => 'content_top',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context triggered when viewing the main map');
  t('wn_map');

  $export['main_map'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'map_features';
  $context->description = 'Context triggered when viewing map features.';
  $context->tag = 'content_types';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'map_feature' => 'map_feature',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-sidebar_map-block_1' => array(
          'module' => 'views',
          'delta' => 'sidebar_map-block_1',
          'region' => 'right',
          'weight' => 0,
        ),
        'views-sidebar_multimedia-block_1' => array(
          'module' => 'views',
          'delta' => 'sidebar_multimedia-block_1',
          'region' => 'right',
          'weight' => 1,
        ),
        'views-sidebar_multimedia-block_2' => array(
          'module' => 'views',
          'delta' => 'sidebar_multimedia-block_2',
          'region' => 'right',
          'weight' => 2,
        ),
        'views-sidebar_multimedia-block_3' => array(
          'module' => 'views',
          'delta' => 'sidebar_multimedia-block_3',
          'region' => 'right',
          'weight' => 3,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context triggered when viewing map features.');
  t('content_types');

  $export['map_features'] = $context;
  return $export;
}
