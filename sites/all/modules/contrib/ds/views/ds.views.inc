<?php
// $Id: ds.views.inc,v 1.1.2.6 2010/04/17 18:44:54 swentel Exp $

/**
 * @file
 * Views file for ds.
 */

/**
 * Implementation of hook_views_plugins().
 */
function ds_views_plugins() {
  $path = drupal_get_path('module', 'ds');
  return array(
    'module' => 'ds',
    'row' => array(
      'ds' => array(
        'title' => t('Display suite node'),
        'help' => t('Display the object through the Display suite module.'),
        'handler' => 'views_plugin_ds_object_view',
        'theme' => 'ds_row_object',
        'path' => $path .'/views',
        'theme path' => $path .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
      'ds_fields' => array(
        'uses fields' => TRUE,
        'title' => t('Display suite fields'),
        'help' => t('Display fields through the Display suite module.'),
        'handler' => 'views_plugin_ds_fields_view',
        'theme' => 'ds_row_fields',
        'path' => $path .'/views',
        'theme path' => $path .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function ds_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ds') .'/views',
    ),
    'handlers' => array(
      'ds_handler_field_fieldgroup' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function ds_views_data() {
  $data['displaysuite']['table']['group'] = t('Display suite');
  $data['displaysuite']['table']['join'] = array(
    '#global' => array(),
  );

  $data['displaysuite']['fieldgroup'] = array(
    'title' => t('Fieldgroup'),
    'help' => t('Create a fieldgroup.'),
    'field' => array(
      'handler' => 'ds_handler_field_fieldgroup',
      'click sortable' => FALSE,
    ),
  );

  return $data;
}
