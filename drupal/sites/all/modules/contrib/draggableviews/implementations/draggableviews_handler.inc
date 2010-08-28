<?php
// $Id: draggableviews_handler.inc,v 1.1.2.1 2009/09/07 19:14:47 sevi Exp $

/**
 * @file
 * The parent class of all draggableviews handlers.
 */

/*
 * Parent class
 */
class draggableviews_handler {
  var $view;

  function init($field_name, &$view) {
    $this->field_name = $field_name;
    $this->view       = $view;
    $this->pager      = $view->pager;

    if ($field_name == $view->style_plugin->options['tabledrag_hierarchy']['field']) {
      $this->type = 'hierarchy';
    }
    else {
      $this->type = 'order';
    }
  }

  function get($value) {
    return $value;
  }

  function save($nid, $value) {
    return FALSE;
  }

  function get_form_element($value, $attributes = array()) {
    switch ($this->type) {
      default:
        $options = array();
        for ($i = $this->range_start; $i <= $this->range_end; $i++) $options[$i] = $i;

        return array(
          '#type'  => 'select',
          '#name'  => $attributes['field_name'],
          '#value' => $value,
          '#options' => $options,
          '#attributes' => array('class' => $attributes['class']),
        );
        break;

      case 'hierarchy':
        return array(
          '#type' => 'hidden',
          '#name' => $attributes['field_name'],
          '#value' => $value,
          '#attributes' => array('class' => $attributes['class']),
        );
    }
  }

  function set_range($start, $end) {
    $this->range_start = $start;
    $this->range_end   = $end;
  }
}
