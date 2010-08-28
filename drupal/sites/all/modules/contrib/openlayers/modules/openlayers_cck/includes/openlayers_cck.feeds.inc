<?php
// $Id: openlayers_cck.feeds.inc,v 1.1.4.3 2010/03/22 23:55:10 zzolo Exp $

/**
 * @file
 * File for feeds module integration
 */

/**
 * Private function for hook of same name.
 *
 * Enable mapping to openlayer_wkt fields for feeds
 */
function _openlayers_cck_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  $fields = array();
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if ($field['type'] == 'openlayers_wkt') {
        $fields[$field_name] = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
      }
    }
  }
  
  foreach ($fields as $k => $name) {
    $targets[$k] = array(
      'name' => $name,
      'callback' => 'openlayers_cck_feeds_set_target',
      'description' => t('The CCK !name field of the node.', array('!name' => $name)),
    );
  }
}

/**
 * Set Feeds Target
 * 
 * Callback for mapping. Here we map feeds data to the actual field of the node.
 */
function openlayers_cck_feeds_set_target(&$node, $target, $value) {
  
  // The field name is the target
  $field_name = $target;
  
  // Load the field from the node
  $field = isset($node->$field_name) ? $node->$field_name : array();
  
  // Handle multiple value fields.
  if (is_array($value)) {
    $i = 0;
    foreach ($value as $v) {
      if (!is_array($v) && !is_object($v)) {
        $field[$i]['openlayers_wkt'] = $v;
      }
      $i++;
    }
  }
  else {
    $field[0]['openlayers_wkt'] = $value;
  }

  $node->$field_name = $field;
}