<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_blog_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'blog_description';
  $context->description = 'Descriptive text found on the blog listing';
  $context->tag = 'views';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'updates:page_1' => 'updates:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-blog_description' => array(
          'module' => 'boxes',
          'delta' => 'blog_description',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Descriptive text found on the blog listing');
  t('views');

  $export['blog_description'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'update_pages';
  $context->description = 'Context triggered when viewing update pages.';
  $context->tag = 'content_types';
  $context->conditions = array(
    'node' => array(
      'values' => array(
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
        'views-updates-block_1' => array(
          'module' => 'views',
          'delta' => 'updates-block_1',
          'region' => 'right',
          'weight' => 0,
        ),
        'views-ce3c6f72a6fff470fd9f1ade8c7b668c' => array(
          'module' => 'views',
          'delta' => 'ce3c6f72a6fff470fd9f1ade8c7b668c',
          'region' => 'right',
          'weight' => 1,
        ),
        'wn_common-0' => array(
          'module' => 'wn_common',
          'delta' => 0,
          'region' => 'right',
          'weight' => 2,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context triggered when viewing update pages.');
  t('content_types');

  $export['update_pages'] = $context;
  return $export;
}
