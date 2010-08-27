<?php
// $Id: ds.buildmodes.inc,v 1.1.2.21 2010/06/08 00:22:20 swentel Exp $

/**
 * @file
 * Manage build modes.
 */

/**
 * Build modes overview.
 */
function ds_build_modes($module) {
  $output = '';
  $action = arg(5);
  $build_mode = arg(6);
  $build_modes = variable_get($module .'_build_modes', array());
  $api_info = ds_api_info($module);

  if (!isset($api_info['no_new_build_modes'])) {
    // Delete form.
    if (in_array($build_mode, array_keys($build_modes)) && $action == 'delete') {
      $output .= drupal_get_form('ds_build_mode_delete_form', $module, $build_mode);
    }
    // Edit form.
    elseif (in_array($build_mode, array_keys($build_modes)) && $action == 'edit') {
      $output .= drupal_get_form('ds_build_mode_form', $module, $build_mode);
    }
    // Custom build modes overview form.
    else {
      if (empty($build_modes)) {
        $output .= '<p>'. t('You have not defined any custom build modes.') .'</p>';
      }
      else {
        $output .= '<p>'. t('This is a list of custom build modes defined via the UI.') .'</p>';

        $header = array(
          t('Name'), t('Key'), t('Operations'),
        );

        $rows = array();
        foreach ($build_modes as $key => $name) {
          $row = array();
          $row[] = check_plain($name);
          $row[] = $key;
          $row[] = l(t('Edit'), 'admin/build/ds/'. $module .'/buildmodes/edit/'. $key) .' - '. l(t('Delete'), 'admin/build/ds/'. $module .'/buildmodes/delete/'. $key);
          $rows[] = $row;
        }

        $output .= theme('table', $header, $rows);
      }

      // New build mode form.
      $output .= drupal_get_form('ds_build_mode_form', $module);

    }
  }

  // Matrix form.
  if (isset($api_info['types'])) {
    $types = $api_info['types']();
    $output .= drupal_get_form('ds_buildmodes_matrix_form', $module, $types);
  }

  return $output;
}

/**
 * Build mode form.
 *
 * @param string $module The name of the module.
 * @param string $build_mode The name of the build mode.
 */
function ds_build_mode_form($form_state, $module, $build_mode = '') {
  $form = array();

  if (!empty($build_mode)) {
    $build_modes = variable_get($module .'_build_modes', array());
    $title = $build_modes[$build_mode];
  }
  else {
    $title = '';
  }

  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($build_mode)) ? t('Add new build mode'): t('Update build mode'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['identity']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'Key name',
    '#description' => t('The machine-readable name of this build mode.'),
    '#required' => TRUE,
  );
  if (!empty($build_mode)) {
    $form['identity']['key']['#disabled'] = TRUE;
    $form['identity']['key']['#value'] = $build_mode;
    $form['identity']['key']['#description'] = t('The machine-readable name of this build mode. Note: you can not edit this key.');
  }
  $form['identity']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Display name',
    '#description' => t('The name of this build mode which will be used for every type on the display fields page.'),
    '#required' => TRUE,
    '#default_value' => $title,
  );
  $form['identity']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save build mode'),
    '#submit' => array('ds_build_mode_form_submit'),
  );

  $form['#module'] = $module;
  $form['#form_type'] = (empty($build_mode)) ? 'insert' : 'update';

  return $form;
}

/**
 * Validate new custom build mode submission.
 */
function ds_build_mode_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $existing = array_keys(ds_get_build_modes($form['#module']));
    if (in_array($form_state['values']['key'], $existing)) {
      form_set_error('key', t('This build mode already exists.'));
    }

    if (!preg_match('!^[a-z_0-9]+$!', $form_state['values']['key'])) {
      form_set_error('key', t('The machine-readable name must contain only lowercase letters, numbers and underscores.'));
    }
  }
}

/**
 * Save new build mode.
 */
function ds_build_mode_form_submit($form, &$form_state) {
  $build_modes = variable_get($form['#module'] .'_build_modes', array());
  $build_modes[$form_state['values']['key']] = $form_state['values']['name'];
  variable_set($form['#module'] .'_build_modes', $build_modes);

  drupal_set_message(t('Build mode %build_mode has been saved.', array('%build_mode' => $build_modes[$form_state['values']['key']])));
  $form_state['redirect'] = 'admin/build/ds/'. $form['#module'] .'/buildmodes';
  menu_rebuild();
  drupal_rebuild_theme_registry();
}

/**
 * Build mode delete form.
 *
 * @param string $module The module name.
 * @param string $build_mode The build mode to delete.
 */
function ds_build_mode_delete_form($form_state, $module, $build_mode) {
  $form = array();
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to delete the build mode %build_mode ?', array('%build_mode' => $build_mode)) .'</p>',
  );
  $form['display'] = array(
    '#type' => 'value',
    '#value' => $build_mode,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/build/ds/'. $module .'/buildmodes'),
  );

  $form['#module'] = $module;
  return $form;
}

/**
 * Delete build mode.
 */
function ds_build_mode_delete_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $build_modes = variable_get($module .'_build_modes', array());
  unset($build_modes[$form_state['values']['display']]);
  variable_set($module .'_build_modes', $build_modes);

  drupal_set_message(t('Custom build mode %build_mode has been removed.', array('%build_mode' => $form_state['values']['display'])));
  $form_state['redirect'] = 'admin/build/ds/'. $module .'/buildmodes';
  menu_rebuild();
  drupal_rebuild_theme_registry();
}

/**
 * Build modes matrix form.
 *
 * @param string $module The name of the module.
 * @param array $types A list of types.
 */
function ds_buildmodes_matrix_form($form_state, $module, $types) {

  drupal_add_js(drupal_get_path('module', 'ds') .'/js/buildmodes.js');

  // Exclude build mode matrix.
  $exclude_build_modes = variable_get($module .'_buildmodes_exclude', array());
  $build_modes = array('ds_block_all' => array('title' => t('Toggle')));
  $all_build_modes = ds_get_build_modes($module);
  if (count($all_build_modes) > 1) {
    $build_modes += $all_build_modes;
  }

  $form['#build_modes'] = $build_modes;
  $form['#types'] = $types;
  $form['#module'] = $module;

  foreach ($types as $tkey => $type) {

    $row_exluded = variable_get($module .'_type_'. $tkey, FALSE);
    foreach ($build_modes as $bkey => $mode) {

      // We create 2 form keys here. The first one is the checkbox,
      // the second is a hidden value to store the default value in.
      $form['exclude'][$tkey][$tkey .'-'. $bkey] = array(
        '#type' => 'checkbox',
        '#attributes' => array('class' => 'checkbox-instance')
      );
      $form['exclude'][$tkey][$tkey .'-'. $bkey .'-disabled'] = array(
        '#type' => 'value',
        '#value' => isset($exclude_build_modes[$tkey][$bkey]) ? $exclude_build_modes[$tkey][$bkey] : NULL,
      );

      if ($bkey == 'ds_block_all') {
        $form['exclude'][$tkey][$tkey .'-'. $bkey]['#attributes']['class'] .= ' block-all';
        $form['exclude'][$tkey][$tkey .'-'. $bkey]['#default_value'] = $row_exluded;
      }
      else {
        $form['exclude'][$tkey][$tkey .'-'. $bkey]['#default_value'] = isset($exclude_build_modes[$tkey][$bkey]) ? $exclude_build_modes[$tkey][$bkey] : NULL;
        if ($row_exluded) {
          $form['exclude'][$tkey][$tkey .'-'. $bkey]['#disabled'] = 'disabled';
        }
      }
    }
  }

  $form['exclude']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('ds_build_modes_matrix_submit'),
  );

  return $form;
}

/**
 * Submit build mode matrix form.
 */
function ds_build_modes_matrix_submit($form, &$form_state) {
  $exclude_build_modes = array();
  foreach ($form['#types'] as $ckey => $type) {
    $global_exclude = FALSE;
    foreach ($form['#build_modes'] as $bkey => $mode) {
      // Global exclude.
      if ($bkey == 'ds_block_all') {
        $global_exclude = $form_state['values'][$ckey .'-'. $bkey];
        variable_set($form['#module'] .'_type_'. $ckey, $form_state['values'][$ckey .'-'. $bkey]);
        $exclude_build_modes[$ckey][$bkey] = $form_state['values'][$ckey .'-'. $bkey];
      }
      else {
        $value = ($global_exclude == TRUE) ? $form_state['values'][$ckey .'-'. $bkey .'-disabled'] : $form_state['values'][$ckey .'-'. $bkey];
        $exclude_build_modes[$ckey][$bkey] = $value;
      }
    }
  }
  variable_set($form['#module'] .'_buildmodes_exclude', $exclude_build_modes);

  drupal_set_message(t('Matrix settings have been saved.'));
  menu_rebuild();
  drupal_rebuild_theme_registry();
}
