<?php
// $Id: ds.groups.inc,v 1.1.2.8 2010/06/08 00:22:20 swentel Exp $

/**
 * @file
 * Manage fieldgroups.
 */

/**
 * Fieldgroups overview.
 */
function ds_fieldgroups($module) {
  $output = '';
  $action = arg(5);
  $fieldgroup = arg(6);

  drupal_add_js(drupal_get_path('module', 'ds') .'/js/fields.js');

  // Get API information.
  $api_info = ds_api_info($module);

  // Extra info.
  $extra = array();

  // Get fields.
  $db_fields = variable_get($module .'_fields', array());

  // Delete form.
  if (in_array($fieldgroup, array_keys($db_fields)) && $action == 'delete') {
    $output .= drupal_get_form('ds_fieldgroup_delete_form', $module, $fieldgroup);
  }
  // Edit form.
  elseif (in_array($fieldgroup, array_keys($db_fields)) && $action == 'edit') {
    $output .= drupal_get_form('ds_fieldgroup_form', $module, $fieldgroup, $db_fields[$fieldgroup]);
  }
  // Custom fieldgroups overview form.
  else {
    $rows = array();
    foreach ($db_fields as $key => $value) {
      if ($value['type'] == DS_FIELD_TYPE_GROUP) {
        $row = array();
        $row[] = l($value['title'], 'admin/build/ds/'. $module .'/fieldgroups/edit/'. $key);
        $row[] = $key;
        $operations = l(t('Edit'), 'admin/build/ds/'. $module .'/fieldgroups/edit/'. $key);
        $operations .= ' - '. l(t('Delete'), 'admin/build/ds/'. $module .'/fieldgroups/delete/'. $key);
        $row[] = $operations;
        $rows[] = $row;
      }
    }

    if (!empty($rows)) {
      $header = array(
        t('Title'), t('Key'), t('Operations'),
      );

      $output .= '<p>'. t('This is a list of fieldgroups defined via the UI.') .'</p>';
      $output .= theme('table', $header, $rows);
    }
    else {
      $output .= '<p>'. t('You have not defined any custom fieldgroups.') .'</p>';
    }

    // Additional modules support.
    if ($module == 'nd')
    $output .= '<p>'. t('<strong>Additional modules</strong><br />These fieldgroups are independant from the fieldgroups module that comes with the CCK package.<br />You can also output fieldgroups as tabs if you have the <a href="!url_tabs">tabs</a> module enabled.<br />It\'s not necessary to have the fieldgroup and/or <a href="!url_cck_tabs">CCK fieldgroup tabs</a> modules enabled.', array('!url_tabs' => 'http://drupal.org/project/tabs', '!url_cck_tabs' => 'http://drupal.org/project/cck_fieldgroup_tabs')) .'</p>';
    else
    $output .= '<p>'. t('<strong>Additional modules</strong><br />You can also output fieldgroups as tabs if you have the <a href="!url_tabs">tabs</a> module enabled.', array('!url_tabs' => 'http://drupal.org/project/tabs', '!url_cck_tabs' => 'http://drupal.org/project/cck_fieldgroup_tabs')) .'</p>';

    // New fieldgroup form.
    $output .= drupal_get_form('ds_fieldgroup_form', $module, $fieldgroup);
  }

  return $output;
}

/**
 * Fieldgroup form.
 */
function ds_fieldgroup_form($form_state, $module, $key = '', $fieldgroup = array()) {
  $form = array();

  if (empty($fieldgroup)) {
    $fieldgroup = array(
      'title' => '',
      'exclude' => array(),
    );
  }

  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($key)) ? t('Add new fieldgroup'): t('Update fieldgroup'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['identity']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'Key name',
    '#description' => t('The machine-readable name of this fieldgroup. Saving will automatically prepend <em>ds_group</em> to the key. <br /><strong>Warning: there is no validation against fields from other modules, so make sure you create a unique key.</strong>'),
    '#required' => TRUE,
  );
  if (!empty($key)) {
    $form['identity']['key']['#disabled'] = TRUE;
    $form['identity']['key']['#value'] = $key;
    $form['identity']['key']['#description'] = t('The machine-readable name of this fieldgroup. Note: you can not edit this key.');
  }
  $form['identity']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Fieldgroup name',
    '#description' => t('The name of this fieldgroup which will be used for every type on the display fields page.'),
    '#required' => TRUE,
    '#default_value' => $fieldgroup['title'],
  );

  $api_info = ds_api_info($module);
  if (isset($api_info['types']) && count($api_info['types']()) > 1) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {
      $types[$tkey] = $type->name;
    }
    $form['identity']['exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field exclude'),
      '#options' => $types,
      '#default_value' => $fieldgroup['exclude'],
      '#attributes' => array('class' => 'exclude-types')
    );
    $form['identity']['exclude_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select all'),
      '#description' => t('Toggle types which you don\'t want the fieldgroups to appear in.'),
      '#attributes' => array('class' => 'select-all')
    );
  }
  else {
    $form['identity']['exclude'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }

  $form['identity']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save fieldgroup'),
    '#submit' => array('ds_fieldgroup_form_submit'),
  );

  $form['#module'] = $module;
  $form['#form_type'] = (empty($key)) ? 'insert' : 'update';
  return $form;
}

/**
 * Validate new custom fieldgroup submission.
 */
function ds_fieldgroup_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $module = $form['#module'];
    $existing = array_keys(variable_get($module .'_fields', array()));

    if (in_array('ds_group_'. $form_state['values']['key'], $existing)) {
      form_set_error('key', t('This fieldgroup already exists.'));
    }

    if (!preg_match('!^[a-z_0-9]+$!', $form_state['values']['key'])) {
      form_set_error('key', t('The machine-readable name must contain only lowercase letters, numbers and underscores.'));
    }

    // Prepend with 'ds_group'.
    $form_state['values']['key'] = 'ds_group_'. $form_state['values']['key'];
  }
}

/**
 * Save new fieldgroup.
 */
function ds_fieldgroup_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());

  $key = $form_state['values']['key'];
  $fields[$key] = array(
    'title' => $form_state['values']['name'],
    'exclude' => $form_state['values']['exclude'],
    'type' => DS_FIELD_TYPE_GROUP,
    'status' => DS_FIELD_STATUS_STATIC,
    'properties' => array(
      'formatters' => array(
        'ds_field' => 'A simple div',
        'ds_group_fieldset_open' => 'Fieldset open',
        'ds_group_fieldset_collapsible' => 'Fieldset collapsible',
        'ds_group_fieldset_collapsed' => 'Fieldset collapsed',
      ),
    ),
  );

  // Tabs support.
  if (module_exists('tabs')) {
    $fields[$key]['properties']['formatters']['ds_tabs'] = 'Tabs';
  }

  // Safe field and invalidate cached fields.
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  // Redirect and confirmation message.
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/fieldgroups';
  drupal_set_message(t('Fieldgroup %field has been saved.', array('%field' => $form_state['values']['name'])));
}

/**
 * Fieldgroup delete form.
 */
function ds_fieldgroup_delete_form($form_state, $module, $fieldgroup) {
  $form = array();

  $fields = variable_get($module .'_fields', array());
  $field_title = $fields[$fieldgroup]['title'];

  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to delete the fieldgroup %fieldgroup ?', array('%fieldgroup' => $field_title)) .'</p>',
  );
  $form['fieldgroup'] = array(
    '#type' => 'value',
    '#value' => $fieldgroup,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/build/ds/'. $module .'/fieldgroups'),
  );

  $form['#module'] = $module;
  return $form;
}

/**
 * Delete build mode.
 */
function ds_fieldgroup_delete_form_submit($form, &$form_state) {

  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());
  $fieldgroup = $form_state['values']['fieldgroup'];
  $field_title = $fields[$fieldgroup]['title'];

  // Save and invalidate fields_cached.
  unset($fields[$fieldgroup]);
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  drupal_set_message(t('Fieldgroup %fieldgroup has been deleted.', array('%fieldgroup' => $field_title)));
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/fieldgroups';
}
