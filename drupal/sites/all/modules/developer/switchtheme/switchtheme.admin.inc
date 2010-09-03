<?php
// $Id: switchtheme.admin.inc,v 1.1.2.3 2008/11/02 12:44:36 sun Exp $

/**
 * @file
 * Switchtheme administration functions.
 */

/**
 * Form builder function for theme settings; menu callback.
 */
function switchtheme_admin_settings() {
  $options = switchtheme_options();
  foreach ($options as $option) {
    $form['switchtheme']['switchtheme_'. $option] = array(
      '#type' => 'textfield',
      '#title' => $option,
      '#default_value' => variable_get('switchtheme_'. $option, drupal_ucfirst($option)),
    );
  }
  return system_settings_form($form);
}

/**
 * Form builder function for browser settings; menu callback.
 */
function switchtheme_admin_browser_settings() {
  $form = array();
  $form['switchtheme_browser_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Browser-based theme switching'),
    '#description' => t('If enabled, the theme will be switched based on the browser of a visitor.'),
    '#default_value' => variable_get('switchtheme_browser_enabled', FALSE),
  );
  
  $themes = switchtheme_select();
  $themes['default'] = 'Default';
  $useragents = array();
  $result = db_query('SELECT data FROM {browscap}');
  while ($row = db_fetch_object($result)) {
    $data = $row->data;
    $data = unserialize($data);
    if (isset($data['parent'])) {
      $useragents[trim($data['parent'])][] = isset($data['platform']) ? $data['platform'] : '';
    }
  }
  $form['switchtheme_browser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browsers'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  foreach ($useragents as $parent => $platforms) {
    $form['switchtheme_browser']['switchtheme_browser_'. md5($parent)] = array(
      '#type' => 'select',
      '#title' => $parent,
      '#options' => $themes,
      '#default_value' => variable_get('switchtheme_browser_'. md5($parent), 'default'),
    );
  }
  return system_settings_form($form);
}

