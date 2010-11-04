<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_common_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'contact_form';
  $context->description = 'This context is triggered on the site-wide contact form.';
  $context->tag = 'page-specific';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'contact' => 'contact',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-contact_form' => array(
          'module' => 'boxes',
          'delta' => 'contact_form',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('This context is triggered on the site-wide contact form.');
  t('page-specific');

  $export['contact_form'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'pages';
  $context->description = 'Add sidebar content to applicable nodes.';
  $context->tag = 'content_types';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'page' => 'page',
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
  t('Add sidebar content to applicable nodes.');
  t('content_types');

  $export['pages'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'sitewide';
  $context->description = 'Sitewide context triggered on all pages';
  $context->tag = 'sitewide';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu_block-1' => array(
          'module' => 'menu_block',
          'delta' => 1,
          'region' => 'right',
          'weight' => -9,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sitewide context triggered on all pages');
  t('sitewide');

  $export['sitewide'] = $context;
  return $export;
}
