<?php
// $Id: draggableviews_handler_cck.inc,v 1.1.2.12 2009/10/15 21:39:50 sevi Exp $

/**
 * @file
 * The default implementation for draggableviews.
 */

/*
 * Default Implementation with CCK-fields
 */
class draggableviews_handler_cck extends draggableviews_handler {
  function init($field_name, &$view) {
    parent::init($field_name, $view);

    $this->options         = $view->display['default']->display_options['style_options'];
    $this->fields          = $view->field;
    $this->field           = $this->fields[$field_name];

    $this->field_alias     = $this->fields[$field_name]->field_alias;

    $this->real_field_name = $this->fields[$field_name]->content_field['field_name'];
    $this->field_type      = $this->fields[$field_name]->content_field['type'];

    $this->allowed_values  = content_allowed_values($this->field->content_field);
  }

  function save($nid, $value) {
    $node = node_load(array('nid' => $nid));
    $type = content_types($node->type);

    // Get field.
    if (isset($type['fields'][$this->real_field_name])) {
      $field = &$node->{$this->real_field_name};
    }
    else {
      // This note doesn't this field.
      return;
    }

    if (!isset($value)) {
      // Sometimes there is no value available (e.g. a root node without a parent).
      $value = 0;
    }

    // Differ between certain field types.
    switch ($this->field_type) {
      case 'nodereference':
        $field[0]['nid'] = $value;
        break;

      default:
      case 'number_integer':
        $field[0]['value'] = $value;
        break;
    }

    // If we didn't set $node->date then node_submit() would set it to the current timestamp.
    $node->date = format_date($node->created, 'custom', 'Y-m-d H:i:s O');

    // Submit node: Set teaser, format, author and date.
    node_submit($node);
    // Finally save the node.
    node_save($node);
  }
}
