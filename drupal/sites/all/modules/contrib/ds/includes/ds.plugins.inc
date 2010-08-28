<?php
// $Id: ds.plugins.inc,v 1.1.2.6 2010/04/24 19:04:04 swentel Exp $

/**
 * @file
 * Administration functions for DS plugins.
 */

/**
 * Menu callback. Registers plugins for a module.
 */
function ds_plugins($form_state, $module) {
  $form = array();
  $form['#module'] = $module;

  $form['info'] = array(
    '#type' => 'item',
    '#value' => t('Enable or disable plugins for this module.'),
  );

  $plugins = ds_plugins_retrieve($module);
  $plugin_settings = variable_get($module .'_plugin_settings', array());
  if (is_array($plugins)) {
    $form['#plugins'] = $plugins;
    foreach ($plugins as $key => $plugin) {
      $form['plugin_'. $plugin['key']] = array(
        '#type' => 'checkbox',
        '#default_value' => isset($plugin_settings[$plugin['key']]['status']) ? $plugin_settings[$plugin['key']]['status'] : 0,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Save the plugin settings.
 */
function ds_plugins_submit($form, &$form_state) {
  $plugin_settings = array();
  foreach ($form['#plugins'] as $key => $plugin) {
    if ($form_state['values']['plugin_'. $plugin['key']] == '1') {
      $plugin_settings[$plugin['key']] = $plugin;
      $plugin_settings[$plugin['key']]['status'] = '1';
    }
  }
  variable_set($form['#module'] .'_plugin_settings', $plugin_settings);
}

/**
 * Retrieve all available plugins.
 */
function ds_plugins_retrieve($module) {

  $plugins = module_invoke_all('ds_plugins');

  foreach ($plugins as $plugin => $data) {
    if ($data['target'] == 'all' || (is_array($data['target']) && isset($data['target'][$module]))) {

      $data['key'] = $plugin;
      $data['class'] = $data['class'];
      if (isset($data['file']) && isset($data['path']))
      $data['filename'] = $data['path'] .'/'. $data['file'];

      $plugins[$plugin] = $data;
    }
  }

  // Give modules a change to alter plugins.
  drupal_alter('ds_plugins', $plugins);

  return $plugins;
}

