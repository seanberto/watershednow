<?php
// $Id: theme_ui.inc,v 1.1.2.12 2010/06/30 10:28:00 swentel Exp $

/**
 * @file
 * Theming functions for ds ui.
 */

/**
 * Theme preprocess function for ds-display-overview-form.tpl.php.
 */
function template_preprocess_ds_display_overview_form(&$vars) {
  $form = &$vars['form'];
  $build_mode = $form['#build_mode'];

  // Synced or not?
  $vars['synced'] = '';
  if ($form['#synced']) {
    $vars['synced'] = $form['#synced'];
    $vars['rows'] = array();
    $vars['form'] = array();
    return;
  }

  // Sort fields.
  $order = array();
  $root_fields = array();
  $leaf_fields = array();
  foreach ($form['#fields'] as $key => $field) {
    if (!empty($form[$field][$build_mode]['parent_id']['#default_value'])) {
      $leaf_fields[$form[$field][$build_mode]['parent_id']['#default_value']][$field] = $form[$field]['ds_weight']['#default_value'];
    }
    else {
      $root_fields[$field] = $form[$field]['ds_weight']['#default_value'];
    }
  }

  asort($root_fields);
  foreach ($root_fields as $root_field => $root_weight) {
    $order[$root_field] = $root_weight;
    if (isset($leaf_fields[$root_field])) {
      $fields_in_leaf = $leaf_fields[$root_field];
      asort($fields_in_leaf);
      foreach ($fields_in_leaf as $leaf_field => $leaf_weight) {
        $order[$leaf_field] = $leaf_weight;
      }
    }
  }

  $rows = array();
  foreach ($order as $field => $field_weight) {
    $element = &$form[$field];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {

        $hide_label_position = FALSE;

        $row->{$child}->indentation = theme('indentation', $element[$child]['#depth']);
        if (empty($element[$child]['css-class']['#options'])) {
          $element[$child]['css-class']['#access'] = FALSE;
        }
        $row->{$child}->class = drupal_render($element[$child]['css-class']);

        // Parent & Field id.
        $row->{$child}->parent_id = drupal_render($element[$child]['parent_id']);
        $row->{$child}->field_id = drupal_render($element[$child]['field_id']);

        // Extra classes when fieldgroup.
        if ($element[$child]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
          $element[$child]['format']['#attributes']['class'] = 'fieldgroup-format';

          if (substr($element[$child]['format']['#default_value'], 0, 17) == 'ds_group_fieldset'
            || substr($element[$child]['format']['#default_value'], 0, 7) == 'ds_tabs'
            ) {
            $hide_label_position = TRUE;
          }
        }

        // Hide region.
        if (!empty($element[$child]['parent_id']['#default_value'])) {
          $element[$child]['region']['#attributes']['class'] .= ' ds-hidden';
        }

        // Other fields.
        $row->{$child}->region = drupal_render($element[$child]['region']);
        $row->{$child}->format = drupal_render($element[$child]['format']);
        $row->{$child}->label = drupal_render($element[$child]['label']);
        $row->{$child}->label_value = drupal_render($element[$child]['label_value']);

        if ($hide_label_position) {
          $row->{$child}->label = str_replace("form-item", "form-item ds-hidden", $row->{$child}->label);
        }

        // Summary.
        $row->{$child}->summary = drupal_render($element[$child]['summary']);
      }
      // Render the rest of the fields
      else {
        // Process weight.
        if ($child == 'ds_weight') {
          $element['ds_weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['ds_weight'] = process_weight($element['ds_weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }
    }

    // Add draggable.
    $row->class = 'draggable';
    if ($region == 'disabled') {
      $row->class .= ' region-css-'. $region;
    }

    $row->label_class = 'label-field';
    if ($element[$build_mode]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
      $row->label_class .= ' field-group';
      $row->class .= ' tabledrag-root';
    }
    else {
      $row->class .= ' tabledrag-leaf';
    }

    // Add to rows.
    $rows[$region][] = $row;
  }

  // CSS and JS.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds.js');
  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');

  // Parent & field order.
  drupal_add_tabledrag('fields', 'match', 'parent', 'ds-parent-id', 'ds-parent-id', 'ds-field-id', FALSE, 1);
  drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', NULL, NULL, TRUE);

  // Sync / copy tab.
  $vars['sync_copy_tab'] = '';
  if (isset($form['sync_copy'])) {
    $vars['sync_copy_tab'] = drupal_render($form['sync_copy']);
  }

  // Plugins available.
  $vars['plugins_tabs'] = array();
  $vars['plugins_content'] = '';
  if ($form['#plugins'] == TRUE) {
    foreach ($form['#plugin_keys'] as $key => $title) {
      $vars['plugins_tabs'][$key] = $title;
      $vars['plugins_content'][$key] = drupal_render($form[$key]);
    }
  }

  // Field classes.
  $vars['show_field_style'] = (isset($form['#all_styles']['fields'])) ? TRUE : FALSE;

  // Region classes.
  $vars['region_classes'] = array();
  $vars['region_classes_summary'] = array();
  foreach($form['#regions'] as $key => $region_name) {
    if (empty($form['region_styles_'. $key]['#options'])) {
      $form['region_styles_'. $key]['#access'] = FALSE;
    }
    $vars['region_classes'][$key] = drupal_render($form['region_styles_'. $key]);
    if (!empty($form['region_styles_'. $key .'_summary']['#value'])) {
      $summary = '<span class="style-info">Styles: '. drupal_render($form['region_styles_'. $key .'_summary']) .'</span>';
    }
    else {
      $summary = '<span class="style-info"></span>';
    }
    $vars['region_classes_summary'][$key] = $summary;
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $form['#regions'];
  $vars['build_mode'] = $build_mode;
}

/**
 * Theme preprocess function for ds-display-overview-views-form.tpl.php.
 */
function template_preprocess_ds_display_overview_views_form(&$vars) {

  $form = &$vars['form'];
  $build_mode = $form['#build_mode'];

  // Sort fields.
  $order = array();
  $root_fields = array();
  $leaf_fields = array();
  foreach ($form['#fields'] as $key => $field) {
    if (!empty($form[$field][$build_mode]['parent_id']['#default_value'])) {
      $leaf_fields[$form[$field][$build_mode]['parent_id']['#default_value']][$field] = $form[$field]['ds_weight']['#default_value'];
    }
    else {
      $root_fields[$field] = $form[$field]['ds_weight']['#default_value'];
    }
  }

  asort($root_fields);
  foreach ($root_fields as $root_field => $root_weight) {
    $order[$root_field] = $root_weight;
    if (isset($leaf_fields[$root_field])) {
      $fields_in_leaf = $leaf_fields[$root_field];
      asort($fields_in_leaf);
      foreach ($fields_in_leaf as $leaf_field => $leaf_weight) {
        $order[$leaf_field] = $leaf_weight;
      }
    }
  }

  $rows = array();
  foreach ($order as $field => $field_weight) {
    $element = &$form[$field];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {
        $row->{$child}->indentation = theme('indentation', $element[$child]['#depth']);
        $row->{$child}->class = drupal_render($element[$child]['css-class']);
        $row->{$child}->label = drupal_render($element[$child]['label']);

        // Parent & Field id.
        $row->{$child}->parent_id = drupal_render($element[$child]['parent_id']);
        $row->{$child}->field_id = drupal_render($element[$child]['field_id']);

        // Extra classes when fieldgroup.
        if ($element[$child]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
          $element[$child]['format']['#attributes']['class'] = 'fieldgroup-format';

          if (substr($element[$child]['format']['#default_value'], 0, 17) == 'ds_group_fieldset'
            || substr($element[$child]['format']['#default_value'], 0, 7) == 'ds_tabs'
            ) {
            $element[$child]['label']['format']['#attributes']['class'] = 'ds-hidden';
          }
        }

        // Hide region.
        if (!empty($element[$child]['parent_id']['#default_value'])) {
          $element[$child]['region']['#attributes']['class'] .= ' ds-hidden';
        }

        $row->{$child}->region = drupal_render($element[$child]['region']);
      }
      // Render the rest of the fields
      else {
        // Process weight.
        if ($child == 'ds_weight') {
          $element['ds_weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['ds_weight'] = process_weight($element['ds_weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }
    }

    // Add draggable.
    $row->class = 'draggable';
    if ($region == 'disabled') {
      $row->class .= ' region-css-'. $region;
    }

    $row->label_class = 'label-field';
    if ($element[$build_mode]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
      $row->label_class .= ' field-group';
      $row->class .= ' tabledrag-root';
    }
    else {
      $row->class .= ' tabledrag-leaf';
    }

    // Add to rows.
    $rows[$region][] = $row;
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $form['#regions'];
  $vars['build_mode'] = $build_mode;
}

/**
 * Theme build modes matrix form.
 */
function theme_ds_buildmodes_matrix_form($form) {
  $output = '';

  // Header.
  $header = array(0 => '');
  foreach ($form['#build_modes'] as $key => $mode) {
    $header[] = $mode['title'];
  }

  // Checkboxes.
  $rows = array();
  foreach ($form['#types'] as $key => $type) {
    $row = array();
    $row[] = array('data' => drupal_ucfirst($type->name));
    foreach ($form['#build_modes'] as $bkey => $mode) {
      $row[] = array('data' => drupal_render($form['exclude'][$key][$key .'-'. $bkey]));
    }
    $rows[] = $row;
  }

  // Theme as table.
  $table = theme('table', $header, $rows);
  $table .= drupal_render($form['exclude']['submit']);

  $fieldset = array(
    '#title' => t('Exclude matrix'),
    '#description' => t('Toggle per type which build modes you want to exclude from rendering. The "Toggle" checkbox will exclude/include all build modes at once.'),
    '#children' => $table,
    '#attributes' => array(),
  );

  $output = theme('fieldset', $fieldset);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Theme the form.
 */
function theme_ds_plugins($form) {

  $output = drupal_render($form['info']);

  $header = array(
    t('Status'),
    t('Info'),
  );

  $rows = array();
  foreach ($form['#plugins'] as $key => $plugin) {
    $row = array();
    $row[] = array('data' => drupal_render($form['plugin_'. $plugin['key']]));
    $row[] = array('data' => '<strong>'. $plugin['title'] .'</strong>: '. $plugin['description']);
    $rows[] = $row;
  }

  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}