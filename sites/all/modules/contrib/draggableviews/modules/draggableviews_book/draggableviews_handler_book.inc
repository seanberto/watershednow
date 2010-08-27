<?php
// $Id: draggableviews_handler_book.inc,v 1.1.2.3 2009/10/15 21:39:50 sevi Exp $

/**
 * @file
 * The default implementation for draggableviews.
 */

/*
 * Default Implementation with Books.
 */
class draggableviews_handler_book extends draggableviews_handler {
  function init($field_name, &$view) {
    parent::init($field_name, $view);

    $this->root = 0;
    // Check if filters or arguments define another root parent.
    $options = $view->style_plugin->options;
    if (isset($options['draggableviews_book_radios'])) {
      switch ($options['draggableviews_book_radios']) {
        case 1:
          $argument = $options['draggableviews_book_argument'];
          if (isset($this->view->argument[$argument]->position)) {
            $position = $this->view->argument[$argument]->position;
            $root_nid = $this->view->args[$position];
          }
          break;
        case 2:
          $filter = $options['draggableviews_book_filter'];
          if (isset($this->view->filter[$filter]->options['value']['value'])) {
            $root_nid = $this->view->filter[$filter]->options['value']['value'];
          }
          break;
      }
      if (!empty($root_nid)) {
        // Depending on the type of this field we now define the root nid or the root weight.
        switch ($this->type) {
          case 'hierarchy':
            // Define the mlid of the parent item as root.
            $this->root = $root_nid;
            break;
          case 'order':
            $mlid = db_result(db_query("SELECT mlid FROM {book} WHERE nid = %d", $root_nid));
            if (!empty($mlid)) {
              $weight = db_result(db_query("SELECT weight FROM {menu_links} WHERE mlid = %d", $mlid));
              // Define the weight of the parent item as root.
              $this->root = $weight;
            }
            break;
        }
      }
    }
  }

  function get($value) {
    switch ($this->type) {
      case 'hierarchy':
        if ($value == $this->root) {
          // We simulate that this node is the first root item of the structure.
          return 0;
        }
        break;
      case 'order':
          return $value - ($this->root + 1);
        break;
    }
    return $value;
  }

  function save($nid, $value) {
    // Load the noad and reset it if it's already cached.
    $node = node_load($nid, NULL, TRUE);
    switch ($this->type) {
      case 'hierarchy':
        if ($value == 0) {
          // This is the simulated "first root item". Restore the actual value now.
          $value = $this->root;
        }
        $node->book['plid'] = db_result(db_query("SELECT mlid FROM {book} WHERE nid = %d", $value));
        break;
      case 'order':
        // The order value of the simulated "first root item" is 0. So the order value of
        // the parent item + 1 (previous order value) must be added to each order value.
        $node->book['weight'] = $value + ($this->root + 1);
        break;
    }
    $node = node_submit($node);
    node_save($node);
  }
}
