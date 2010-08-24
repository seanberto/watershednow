<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function drupalrivers_action_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'action_content';
  $context->description = 'Context triggered when viewing a node';
  $context->tag = 'section';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'donation' => 'donation',
        'event' => 'event',
        'map_point' => 'map_point',
        'page' => 'page',
        'take_action' => 'take_action',
        'update' => 'update',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-related_action_content-block_1' => array(
          'module' => 'views',
          'delta' => 'related_action_content-block_1',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context triggered when viewing a node');
  t('section');

  $export['action_content'] = $context;
  return $export;
}
