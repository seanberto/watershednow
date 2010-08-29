<?php
// $Id: draggableviews_book.views.inc,v 1.1.2.1 2009/09/06 16:27:24 sevi Exp $
/**
 * @file
 * Provide special views data and handlers for book.module
 */

/**
 * Implementation of hook_views_data()
 */
function draggableviews_book_views_data() {
  // Book hierarchy and weight data are now in {menu_links}.
  $data['draggableviews_book_book_parent']['table']['group'] = t('Book');
  $data['draggableviews_book_book_parent']['table']['join'] = array(
    'node' => array(
      'table' => 'menu_links',
      'left_table' => 'book',
      'left_field' => 'mlid',
      'field' => 'mlid',
    ),
  );

  $data['draggableviews_book_book_parent']['p'] = array(
    'title' => t('All sub nodes of this book page.'),
    'help' => t('All sub nodes of this book page.'),
    'argument' => array(
      'handler' => 'views_handler_argument_draggableviews_book',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function draggableviews_book_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'draggableviews_book'),
    ),
    'handlers' => array(
      // argument handlers
      'views_handler_argument_draggableviews_book' => array(
        'parent' => 'views_handler_argument',
      ),
    ),
  );
}
