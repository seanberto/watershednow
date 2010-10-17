<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_donations_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'donation_pages';
  $context->description = 'Context triggered when viewing donation pages.';
  $context->tag = 'content_types';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'donation' => 'donation',
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
          'region' => 'footer_second',
          'weight' => 0,
        ),
        'views-ce3c6f72a6fff470fd9f1ade8c7b668c' => array(
          'module' => 'views',
          'delta' => 'ce3c6f72a6fff470fd9f1ade8c7b668c',
          'region' => 'footer_second',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context triggered when viewing donation pages.');
  t('content_types');

  $export['donation_pages'] = $context;
  return $export;
}
