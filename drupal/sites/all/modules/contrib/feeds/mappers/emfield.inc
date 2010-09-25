<?php
// $Id: emfield.inc,v 1.1 2010/09/16 19:20:04 alexb Exp $

/**
 * @file
 * On behalf implementation of Feeds mapping API for emfield.module (Embedded
 * Media Field).
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function emfield_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  $fields = array();
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if (in_array($field['type'], array('emvideo', 'emaudio', 'emimage'))) {
        $fields[$field_name] = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
      }
    }
  }
  foreach ($fields as $k => $name) {
    $targets[$k] = array(
      'name' => $name,
      'callback' => 'emfield_feeds_set_target',
      'description' => t('The Embedded !name field of the node. Map a URL of a page containing media to this field.', array('!name' => $name)),
    );
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * When the callback is invoked, $target contains the name of the field the
 * user has decided to map to and $value contains the value of the feed item
 * element the user has picked as a source.
 */
function emfield_feeds_set_target(&$node, $target, $value) {
  $field = isset($node->$target) ? $node->$target : array();

  // Handle multiple value fields.
  if (is_array($value)) {
    $i = 0;
    foreach ($value as $v) {
      if (!is_array($v) && !is_object($v)) {
        $field[$i]['embed'] = $v;
      }
      $i++;
    }
  }
  else {
    $field[0]['embed'] = $value;
  }

  $node->$target = $field;
}