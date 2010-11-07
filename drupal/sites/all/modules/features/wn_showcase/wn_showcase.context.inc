<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_showcase_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'wn_showcase';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
    'wn_showcase_context_condition' => array(
      'values' => array(
        'wn_neuse' => 'wn_neuse',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-showcase_1' => array(
          'module' => 'boxes',
          'delta' => 'showcase_1',
          'region' => 'footer_first',
          'weight' => 0,
        ),
        'boxes-showcase_2' => array(
          'module' => 'boxes',
          'delta' => 'showcase_2',
          'region' => 'footer_first',
          'weight' => 1,
        ),
        'boxes-showcase_3' => array(
          'module' => 'boxes',
          'delta' => 'showcase_3',
          'region' => 'footer_first',
          'weight' => 2,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  $export['wn_showcase'] = $context;
  return $export;
}
