<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_hp_main_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'homepage';
  $context->description = 'Homepage context specific to the WN Custom Homepage feature.';
  $context->tag = 'wn_hp_main';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'home' => 'home',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-homepage_secondary' => array(
          'module' => 'boxes',
          'delta' => 'homepage_secondary',
          'region' => 'right',
          'weight' => 30,
        ),
        'boxes-homepage_promo' => array(
          'module' => 'boxes',
          'delta' => 'homepage_promo',
          'region' => 'content_bottom',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Homepage context specific to the WN Custom Homepage feature.');
  t('wn_hp');

  $export['homepage'] = $context;
  return $export;
}
