<?php
// $Id: nd_location.admin.inc,v 1.1.2.3 2010/06/08 00:21:18 swentel Exp $

/**
 * @file
 * Manage location settings.
 */

/**
 * Location settings.
 */
function nd_location_settings() {
  $output = '';
  $type = arg(5);
  $action = arg(6);
  $value = arg(7);
  $location_fields = variable_get('nd_location_fields', array());
  $location_formatters = variable_get('nd_location_formatters', array());

  // Delete form field.
  if (in_array($value, array_keys($location_fields)) && $action == 'delete' && $type == 'field') {
    $output .= drupal_get_form('nd_location_field_delete_form', $value);
  }
  // Delete form formatter.
  elseif (in_array($value, array_keys($location_formatters)) && $action == 'delete' && $type == 'formatter') {
    $output .= drupal_get_form('nd_location_formatter_delete_form', $value);
  }
  // Edit form field.
  elseif (in_array($value, array_keys($location_fields)) && $action == 'edit' && $type == 'field') {
    $output .= drupal_get_form('nd_location_field_form', $value);
  }
  // Edit form formatter.
  elseif (in_array($value, array_keys($location_formatters)) && $action == 'edit' && $type == 'formatter') {
    $output .= drupal_get_form('nd_location_formatter_form', $value);
  }
  // Overview form.
  else {

    // Location fields.
    if (empty($location_fields)) {
      $output .= '<p>'. t('You have not defined any custom location fields.') .'</p>';
    }
    else {
      $output .= '<p>'. t('This is a list of custom location fields defined via the UI.') .'</p>';

      $header = array(
        t('Name'), t('Type'), t('Key'), t('Operations'),
      );

      $rows = array();
      foreach ($location_fields as $key => $field) {
        $row = array();
        $row[] = check_plain($field['name']);
        $row[] = check_plain($field['type']);
        $row[] = $key;
        $row[] = l(t('Edit'), 'admin/build/ds/nd/nd_location/field/edit/'. $key) .' - '. l(t('Delete'), 'admin/build/ds/nd/nd_location/field/delete/'. $key);
        $rows[] = $row;
      }

      $output .= theme('table', $header, $rows);
    }

    // New location field form.
    $output .= drupal_get_form('nd_location_field_form');

    // Location formatters.
    if (empty($location_formatters)) {
      $output .= '<p>'. t('You have not defined any location formatters.') .'</p>';
    }
    else {
      $output .= '<p>'. t('This is a list of location formatters defined via the UI.') .'</p>';

      $header = array(
        t('Name'), t('Width'), t('Height'), t('Zoom'), t('Key'), t('Operations'),
      );

      $rows = array();
      foreach ($location_formatters as $key => $field) {
        $row = array();
        $row[] = check_plain($field['name']);
        $row[] = check_plain($field['width']);
        $row[] = check_plain($field['height']);
        $row[] = check_plain($field['zoom']);
        $row[] = $key;
        $row[] = l(t('Edit'), 'admin/build/ds/nd/nd_location/formatter/edit/'. $key) .' - '. l(t('Delete'), 'admin/build/ds/nd/nd_location/formatter/delete/'. $key);
        $rows[] = $row;
      }

      $output .= theme('table', $header, $rows);
    }

    // New width & height settings form.
    $output .= drupal_get_form('nd_location_formatter_form');
  }

  return $output;
}

/**
 * Location field form.
 */
function nd_location_field_form($form_state, $location_field = '') {
  $form = array();

  if (!empty($location_field)) {
    $location_fields = variable_get('nd_location_fields', array());
    $type = $location_fields[$location_field]['type'];
  }
  else {
    $type = '';
  }

  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($location_field)) ? t('Add new location field'): t('Update location field'),
    '#description' => t('This will make new location fields available on the content type edit screen.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['identity']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'Key name',
    '#description' => t('The machine-readable name of this location field. A display name will be automatically generated from this key name. <strong>Important, there is no check on the existing location fields, so make sure your key is unique.</strong>'),
    '#required' => TRUE,
  );
  if (!empty($location_field)) {
    $form['identity']['key']['#disabled'] = TRUE;
    $form['identity']['key']['#value'] = $location_field;
    $form['identity']['key']['#description'] = t('The machine-readable name of this location field. Note: you can not edit this field.');
  }
  $form['identity']['type'] = array(
    '#type' => 'select',
    '#options' => array(
      'textfield' => t('Text field'),
      'textarea' => t('Textarea'),
    ),
    '#title' => 'Field type',
    '#description' => t('The type of this location field.'),
    '#required' => TRUE,
    '#default_value' => $type,
  );
  $form['identity']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save location field'),
    '#submit' => array('nd_location_field_form_submit'),
  );

  $form['#form_type'] = (empty($location_field)) ? 'insert' : 'update';

  return $form;
}

/**
 * Validate new custom location field submission.
 */
function nd_location_field_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $existing = array_keys(variable_get('nd_location_fields', array()));
    if (in_array($form_state['values']['key'], $existing)) {
      form_set_error('key', t('This location field already exists.'));
    }

    if (!preg_match('!^[a-z_]+$!', $form_state['values']['key'])) {
      form_set_error('key', t('The machine-readable name must contain only lowercase letters and underscores.'));
    }
  }
}

/**
 * Save new location field.
 */
function nd_location_field_form_submit($form, &$form_state) {
  $location_fields = variable_get('nd_location_fields', array());
  $location_fields[$form_state['values']['key']] = array(
    'name' => ucfirst(str_replace('_', ' ', $form_state['values']['key'])),
    'type' => $form_state['values']['type'],
  );
  variable_set('nd_location_fields', $location_fields);

  drupal_set_message(t('Location field %loc_field has been saved.', array('%loc_field' => $form_state['values']['key'])));
  $form_state['redirect'] = 'admin/build/ds/nd/nd_location';
}

/**
 * Location field delete form.
 */
function nd_location_field_delete_form($form_state, $location_field) {
  $form = array();
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to location field %location_field ?', array('%location_field' => $location_field)) .'</p>',
  );
  $form['display'] = array(
    '#type' => 'value',
    '#value' => $location_field,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/build/ds/nd/nd_location'),
  );

  return $form;
}

/**
 * Delete location field.
 */
function nd_location_field_delete_form_submit($form, &$form_state) {
  $location_fields = variable_get('nd_location_fields', array());
  unset($location_fields[$form_state['values']['display']]);
  variable_set('nd_location_fields', $location_fields);

  drupal_set_message(t('Custom location field %location_field has been removed.', array('%location_field' => $form_state['values']['display'])));
  $form_state['redirect'] = 'admin/build/ds/nd/nd_location';
}

/**
 * Location formatter form.
 */
function nd_location_formatter_form($form_state, $formatter = '') {
  $form = array();

  if (!empty($formatter)) {
    $location_formatters = variable_get('nd_location_formatters', array());
    $width = $location_formatters[$formatter]['width'];
    $height = $location_formatters[$formatter]['height'];
    $zoom = $location_formatters[$formatter]['zoom'];
  }
  else {
    $width = '';
    $height = '';
    $zoom = '3';
  }

  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($formatter)) ? t('Add new location formatter'): t('Update location formatter'),
    '#description' => t('These formatters will be used as in the formatters fieldset on the display settings screen.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['identity']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'Key name',
    '#description' => t('The machine-readable name of this location formatter. A display name will be automatically generated from this key name.'),
    '#required' => TRUE,
  );
  if (!empty($formatter)) {
    $form['identity']['key']['#disabled'] = TRUE;
    $form['identity']['key']['#value'] = $formatter;
    $form['identity']['key']['#description'] = t('The machine-readable name of this location field. Note: you can not edit this field.');
  }
  $form['identity']['width'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#description' => t('The with of this location formatter. Only enter the number.'),
    '#required' => TRUE,
    '#default_value' => $width,
  );
  $form['identity']['height'] = array(
    '#type' => 'textfield',
    '#title' => 'height',
    '#description' => t('The with of this location formatter. Only enter the number.'),
    '#required' => TRUE,
    '#default_value' => $height,
  );
  $form['identity']['zoom'] = array(
    '#type' => 'textfield',
    '#title' => 'zoom level',
    '#description' => t('The zoom level of this location formatter. Only enter the number.'),
    '#required' => TRUE,
    '#default_value' => $zoom,
  );
  $form['identity']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save location formatter'),
    '#submit' => array('nd_location_formatter_form_submit'),
  );

  $form['#form_type'] = (empty($formatter)) ? 'insert' : 'update';

  return $form;
}

/**
 * Validate new custom location formatter submission.
 */
function nd_location_formatter_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $existing = array_keys(variable_get('nd_location_formatters', array()));
    if (in_array($form_state['values']['key'], $existing)) {
      form_set_error('key', t('This location formatter already exists.'));
    }

    if ($form_state['values']['key'] == 'default') {
      form_set_error('key', t('The default key is reserved.'));
    }

    if (!preg_match('!^[a-z_]+$!', $form_state['values']['key'])) {
      form_set_error('key', t('The machine-readable name must contain only lowercase letters and underscores.'));
    }
  }
}

/**
 * Save new location field.
 */
function nd_location_formatter_form_submit($form, &$form_state) {
  $location_formatters = variable_get('nd_location_formatters', array());
  $location_formatters[$form_state['values']['key']] = array(
    'name' => ucfirst(str_replace('_', ' ', $form_state['values']['key'])),
    'width' => $form_state['values']['width'],
    'height' => $form_state['values']['height'],
    'zoom' => $form_state['values']['zoom'],
  );
  variable_set('nd_location_formatters', $location_formatters);
  drupal_rebuild_theme_registry();

  drupal_set_message(t('Location field %loc_field has been saved.', array('%loc_field' => $form_state['values']['key'])));
  $form_state['redirect'] = 'admin/build/ds/nd/nd_location';
}

/**
 * Location field delete form.
 */
function nd_location_formatter_delete_form($form_state, $location_formatter) {
  $form = array();
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to location formatter %location_formatter ?', array('%location_formatter' => $location_formatter)) .'</p>',
  );
  $form['display'] = array(
    '#type' => 'value',
    '#value' => $location_formatter,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/build/ds/nd/nd_location'),
  );

  return $form;
}

/**
 * Delete location formatter.
 */
function nd_location_formatter_delete_form_submit($form, &$form_state) {
  $location_formatters = variable_get('nd_location_formatters', array());
  unset($location_formatters[$form_state['values']['display']]);
  variable_set('nd_location_formatters', $location_formatters);
  drupal_rebuild_theme_registry();

  drupal_set_message(t('Custom location formatter %$location_formatter has been removed.', array('%$location_formatter' => $form_state['values']['display'])));
  $form_state['redirect'] = 'admin/build/ds/nd/nd_location';
}
