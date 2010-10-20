<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_takeaction_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'take_action_description';
  $context->description = 'Sidebar content on the "Take Action" listing page.';
  $context->tag = 'views';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'take_action:page_1' => 'take_action:page_1',
        'take_action:page_2' => 'take_action:page_2',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-take_action' => array(
          'module' => 'boxes',
          'delta' => 'take_action',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sidebar content on the "Take Action" listing page.');
  t('views');

  $export['take_action_description'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'takeaction_pages';
  $context->description = 'Context triggered when viewing take action pages.';
  $context->tag = 'content_types';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'take_action' => 'take_action',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wn_common-0' => array(
          'module' => 'wn_common',
          'delta' => 0,
          'region' => 'right',
          'weight' => 0,
        ),
        'views-ce3c6f72a6fff470fd9f1ade8c7b668c' => array(
          'module' => 'views',
          'delta' => 'ce3c6f72a6fff470fd9f1ade8c7b668c',
          'region' => 'right',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context triggered when viewing take action pages.');
  t('content_types');

  $export['takeaction_pages'] = $context;
  return $export;
}
