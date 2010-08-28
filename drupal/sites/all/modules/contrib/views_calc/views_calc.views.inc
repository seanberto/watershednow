<?php
// $Id: views_calc.views.inc,v 1.7 2009/05/17 11:11:17 karens Exp $
/**
 * Implementation of hook_views_handlers().
 */
function views_calc_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_calc'),
    ),
    'handlers' => array(
      'views_calc_field_handler' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 *  Implementation of hook_views_data().
 *
 *  Add calc fields to views field list.
 */
function views_calc_views_data() {
  $data = array();
  $results = _views_calc_fields();
  while($field = db_fetch_array($results)) {
    $data['node']['cid'. $field['cid']] = array(
      'group' => t('Views Calc'),
      'title' => t($field['label']),
      'help' =>  $field['calc'],
      'field' => array(
        'field' => 'cid'. $field['cid'],
        'table' => 'node',
        'handler' => 'views_calc_field_handler',
        'click sortable' => TRUE,
        'allow empty' => TRUE,
        'cid'     => $field['cid'],
        'format'  => $field['format'],
        'custom'  => $field['custom'],
        'calc'  => $field['calc'],
      ),
    );
  }
  return $data;
}

/**
 * Implementation of hook_views_plugins
 */
function views_calc_views_plugins() {
  $path = drupal_get_path('module', 'views_calc');
  $views_path = drupal_get_path('module', 'views');
  require_once "./$path/theme.inc";

  $data = array(
    'module' => 'views_calc', // This just tells our themes are elsewhere.
    'style' => array(
      // Style plugin for the navigation.
      'views_calc' => array(
        'title' => t('Views Calc Table'),
        'help' => t('Creates a table with column calculations.'),
        'handler' => 'views_calc_table',
        'path' => $path,
        'parent' => 'table',
        'theme' => 'views_calc_table',
        'theme file' => 'theme.inc',
        'theme path' => "$path",
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    )
  );
  return $data;
}