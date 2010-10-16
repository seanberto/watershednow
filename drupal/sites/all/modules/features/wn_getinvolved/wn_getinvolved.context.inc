<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function wn_getinvolved_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'action_content';
  $context->description = 'Context triggered when viewing a donation, take action, event, update or page node.';
  $context->tag = 'section';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'donation' => 'donation',
        'event' => 'event',
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
  t('Context triggered when viewing a donation, take action, event, update or page node.');
  t('section');

  $export['action_content'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'donate_header';
  $context->description = 'Header content on the donation listing page.';
  $context->tag = 'section';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'donation_opportunities:page_1' => 'donation_opportunities:page_1',
        'take_action:page_1' => 'take_action:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-donate' => array(
          'module' => 'boxes',
          'delta' => 'donate',
          'region' => 'preface',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Header content on the donation listing page.');
  t('section');

  $export['donate_header'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events_header';
  $context->description = 'Header content on the events listing page.';
  $context->tag = 'section';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'events:page_1' => 'events:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-events' => array(
          'module' => 'boxes',
          'delta' => 'events',
          'region' => 'preface',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Header content on the events listing page.');
  t('section');

  $export['events_header'] = $context;
  return $export;
}
