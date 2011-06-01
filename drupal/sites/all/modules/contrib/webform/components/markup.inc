<?php

/**
 * @file
 * Webform module markup component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_markup() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'format' => FILTER_FORMAT_DEFAULT,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_markup($component) {
  $form = array();
  $form['markup']['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Value'),
    '#default_value' => $component['value'],
    '#description' => t('Markup allows you to enter custom HTML or PHP logic into your form.') . theme('webform_token_help'),
    '#weight' => -1,
    '#parents' => array('value'),
  );
  // Add the filter form.
  $form['markup']['format'] = filter_form($component['extra']['format'], 0, array('extra', 'format'));

  $form['extra']['description'] = array(); // No description for markup.
  $form['display'] = array('#type' => 'markup'); // Hide the display options.

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_markup($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? NULL : $component['name'],
    '#weight' => $component['weight'],
    '#markup' => $filter ? _webform_filter_values(check_markup($component['value'], $component['extra']['format'], FALSE), NULL, NULL, NULL, FALSE) : $component['value'],
    '#input_format' => $component['extra']['format'],
    '#theme_wrappers' => array('webform_element_wrapper'),
    '#post_render' => array('webform_element_wrapper'),
    '#webform_component' => $component,
  );

  // TODO: Remove when #markup becomes available in D7.
  $element['#value'] = $element['#markup'];

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_markup($component, $value, $format = 'html') {
  return array();
}
