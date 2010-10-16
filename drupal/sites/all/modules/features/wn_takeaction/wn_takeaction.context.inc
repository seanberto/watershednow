<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_takeaction_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'take_action_header';
  $context->description = 'Header content on the "Take Action" listing page.';
  $context->tag = 'section';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'take_action:page_1' => 'take_action:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-take_action' => array(
          'module' => 'boxes',
          'delta' => 'take_action',
          'region' => 'preface',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Header content on the "Take Action" listing page.');
  t('section');

  $export['take_action_header'] = $context;
  return $export;
}
