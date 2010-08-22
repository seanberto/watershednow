<?php

/**
 * Helper to implementation of hook_default_box().
 */
function _drupalrivers_map_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'map_point_add_edit';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Add or edit a map point';
  $box->options = array(
    'body' => 'Use the form below to add or edit a map point on the watershed map.',
    'format' => '1',
  );

  $export['map_point_add_edit'] = $box;
  return $export;
}

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _drupalrivers_map_context_default_contexts() {
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

  $translatables['map_point_add_edit'] = array(
    t('Instructions when creating or editing an map point node.'),
    t('section'),
  );

  $export['map_point_add_edit'] = $context;
  return $export;
}

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _drupalrivers_map_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Helper to implementation of hook_strongarm().
 */
function _drupalrivers_map_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_map_point';
  $strongarm->value = '4';

  $export['comment_default_mode_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_map_point';
  $strongarm->value = '1';

  $export['comment_default_order_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_map_point';
  $strongarm->value = '0';

  $export['comment_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_map_point';
  $strongarm->value = '1';

  $export['comment_preview_map_point'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_map_point';
  $strongarm->value = '1';

  $export['comment_subject_field_map_point'] = $strongarm;
  return $export;
}
