<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_hp_callout_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'homepage_callout';
  $context->description = 'Homepage callout slideshow placement';
  $context->tag = 'wn_hp_callout';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-wn_homepage_callout-block_1' => array(
          'module' => 'views',
          'delta' => 'wn_homepage_callout-block_1',
          'region' => 'callout',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Homepage callout slideshow placement');
  t('wn_hp_callout');

  $export['homepage_callout'] = $context;
  return $export;
}
