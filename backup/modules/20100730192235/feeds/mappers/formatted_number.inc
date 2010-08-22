<?php
// $Id: formatted_number.inc,v 1.1 2010/07/18 19:06:10 alexb Exp $

/**
 * @file
 * On behalf implementation of Feeds mapping API for Formatted Number CCK.
 */

/**
 * Implementation of feeds_node_processor_target_alter().
 */
function formatted_number_feeds_node_processor_targets_alter(&$targets, $content_type) {
  if (module_exists('formatted_number')) {
    $info = content_types($content_type);
    $fields = array();
    if (isset($info['fields']) && count($info['fields'])) {
      foreach ($info['fields'] as $field_name => $field) {
        if (in_array($field['type'], array_keys(formatted_number_get_fields_info()))) {
          $fields[$field_name] = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
        }
      }
    }
    foreach ($fields as $k => $name) {
      $targets[$k] = array(
        'name' => $name,
        'callback' => 'formatted_number_feeds_set_target',
        'description' => t('The CCK !name field of the node.', array('!name' => $name)),
      );
    }
  }
}

/**
 * Set the CCK field target after import.
 */
function formatted_number_feeds_set_target($node, $target, $value) {
  $field = isset($node->$target) ? $node->$target : array();

  // Handle multiple value fields.
  if (is_array($value)) {
    $i = 0;
    foreach ($value as $v) {
      if (!is_array($v) && !is_object($v)) {
        $field[$i]['value'] = parse_formatted_number($v, TRUE);
      }
      $i++;
    }
  }
  else {
    $field[0]['value'] = parse_formatted_number($value, TRUE);
  }

  $node->$target = $field;
}
