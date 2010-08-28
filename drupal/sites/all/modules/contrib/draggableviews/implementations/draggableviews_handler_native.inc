<?php
// $Id: draggableviews_handler_native.inc,v 1.1.2.13 2009/10/15 21:39:49 sevi Exp $

/**
 * @file
 * The default implementation of draggableviews handlers.
 */

/*
 * Default Implementation: storage done by draggableviews
 */
class draggableviews_handler_native extends draggableviews_handler {
  function init($field_name, &$view) {
    parent::init($field_name, $view);
    $this->delta = $this->view->field[$field_name]->definition['draggableviews_delta'];
  }

  function save($nid, $value) {
    $args = NULL;

    if (!empty($this->view->style_plugin->options['draggableviews_arguments']['use_args'])) {
      $args = implode('/', $this->view->args);
    }

    db_query("DELETE FROM {draggableviews_structure} WHERE view_name='%s' AND nid=%d AND delta=%d AND args='%s'", $this->view->name, $nid, $this->delta, $args);
    db_query("INSERT INTO {draggableviews_structure}(view_name, nid, delta, value, args) VALUES('%s', %d, %d, %d, '%s')", $this->view->name, $nid, $this->delta, $value, $args);
  }
}
