<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function drupalrivers_map_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'map_point_add_edit';
  $context->description = 'Instructions when creating or editing an map point node.';
  $context->tag = 'section';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'map_point' => 'map_point',
      ),
      'options' => array(
        'node_form' => '2',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-map_point_add_edit' => array(
          'module' => 'boxes',
          'delta' => 'map_point_add_edit',
          'region' => 'message_block',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Instructions when creating or editing an map point node.');
  t('section');

  $export['map_point_add_edit'] = $context;
  return $export;
}
