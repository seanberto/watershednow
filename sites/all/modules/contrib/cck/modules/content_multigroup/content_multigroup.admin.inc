<?php
// $Id: content_multigroup.admin.inc,v 1.1.2.5 2009/11/02 19:22:37 markuspetrux Exp $

/**
 * @file
 * Implementation of node type administration functions for content multigroup.
 */

/**
 * Helper function to build the multiple values options for multigroups.
 */
function content_multigroup_multiple_values() {
  return array(
    //'' => t('N/A'),
    1 => t('Unlimited'),
    0 => 1) + drupal_map_assoc(range(2, 10));
}

/**
 * Validation for creating/moving fields and groups on the
 * Manage Fields screen.
 */
function content_multigroup_field_overview_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];

  $type_name = $form['#type_name'];
  $fields = array();
  $groups = array();

  $group = $form_values['_add_new_group'];
  if (array_filter(array($group['label'], $group['group_name']))) {
    $group['settings'] = field_group_default_settings($group['group_type']);
    $validation = fieldgroup_validate_name($group, $form['#type_name']);

    // If there's something wrong with the new group,
    // don't bother doing any more validation, further
    // processing will be stopped by the fieldgroup module.
    if (!empty($validation['errors'])) {
      return;
    }
    $group['group_name'] = $validation['group_name'];
    $new_group_name = $group['group_name'];
    $groups['_add_new_group'] = $group;
  }

  // See if we have fields moving into or out of a Multigroup.
  // Set any fields to use the new name here so they will get processed
  // correctly by the fieldgroup module when saved.
  foreach ($form_values as $key => $values) {
    if ($values['parent'] == '_add_new_group') {
      $values['parent'] = $new_group_name;
      $form_values[$key] = $values;
    }

    if (!empty($form[$key]['#row_type']) && $form[$key]['#row_type'] == 'group') {
      // Gather up info about all groups.
      $group_name = $form_values[$key]['group']['group_name'];
      $groups[$group_name] = $form_values[$key]['group'];
    }
    if (!empty($form[$key]['#row_type']) && $form[$key]['#row_type'] == 'field') {
      if ($values['prev_parent'] != $values['parent']) {
        // Gather up fields that have moved in or out of a group.
        $fields[$key] = $form_values[$key]['field'];
      }
    }
  }

  $rebuild = FALSE;
  foreach ($fields as $field_name => $field) {
    $new_group = $form_values[$field_name]['parent'];
    $old_group = $form_values[$field_name]['prev_parent'];
    if (!empty($new_group) && isset($groups[$new_group]) && $groups[$new_group]['group_type'] == 'multigroup') {
      $allowed_in = content_multigroup_allowed_in($field, $groups[$new_group]);
      if (!$allowed_in['allowed']) {
        form_set_error($field_name, $allowed_in['message']);
      }
      else {
        if (!empty($allowed_in['message'])) {
          drupal_set_message($allowed_in['message']);
        }
        module_load_include('inc', 'content', 'includes/content.crud');
        $content_type = content_types($type_name);
        $group_multiple = $groups[$new_group]['settings']['multigroup']['multiple'];
        $multiple_values = content_multigroup_multiple_values();
        $field = $content_type['fields'][$field_name];
        $field['multiple'] = $group_multiple;
        $field = content_field_instance_collapse($field);
        content_field_instance_update($field, FALSE);
        $rebuild = TRUE;
        drupal_set_message(t('The field %field has been updated to use %multiple values, to match the multiple value setting of the Multigroup %group.', array(
          '%field' => $field['label'], '%multiple' => $multiple_values[$group_multiple], '%group' => $groups[$new_group]['label'])));
      }
    }
    elseif (!empty($old_group) && isset($groups[$old_group]) && $groups[$old_group]['group_type'] == 'multigroup') {
      $allowed_out = content_multigroup_allowed_out($field, $groups[$old_group]);
      if (!$allowed_out['allowed']) {
        form_set_error($field_name, $allowed_out['message']);
      }
      elseif (!empty($allowed_out['message'])) {
        drupal_set_message($allowed_out['message']);
      }
    }
  }

  // Clear caches and rebuild menu only if any field has been updated.
  if ($rebuild) {
    content_clear_type_cache(TRUE);
    menu_rebuild();
  }
}

/**
 * Helper function for deciding if a field is
 * allowed into a Multigroup.
 */
function content_multigroup_allowed_in($field, $group) {
  if ($group['group_type'] != 'multigroup') {
    return array('allowed' => TRUE, 'message' => '');
  }

  // We can't allow fields with more multiple values than the group has
  // to be moved into it.
  $max_existing = content_max_delta($field['field_name']);
  $group_multiple = $group['settings']['multigroup']['multiple'];
  $multiple_values = content_multigroup_multiple_values();
  if ($group_multiple != 1 && $max_existing > $group_multiple) {
    return array(
      'allowed' => FALSE,
      'message' => t('This change is not allowed. The field %field already has %multiple values in the database but the group %group only allows %group_max. Making this change would result in the loss of data.', array('%field' => $field['widget']['label'], '%multiple' => $max_existing, '%group' => $group['label'], '%group_max' => $multiple_values[$group_multiple]))
    );
  }

  // Fields that handle their own multiple values may not have the same values
  // in Multigroup fields and normal fields. We don't know if they will work or not.

  // Adding a hook here where widgets that handle their own multiple values
  // that will work correctly in Multigroups can allow their fields in.

  if (content_handle('widget', 'multiple values', $field) != CONTENT_HANDLE_CORE) {
    $allowed_widgets = array(
      'optionwidgets_select',
      'optionwidgets_buttons',
      'optionwidgets_onoff',
      'nodereference_buttons',
      'nodereference_select',
      'userreference_buttons',
      'userreference_select',
    );
    $allowed_widgets = array_merge($allowed_widgets, module_invoke_all('content_multigroup_allowed_widgets'));
    if (!in_array($field['widget']['type'], $allowed_widgets)) {
      return array(
        'allowed' => FALSE,
        'message' => t('This change is not allowed. The field %field handles multiple values differently than the Content module. Making this change could result in the loss of data.', array('%field' => $field['widget']['label']))
      );
    }
  }

  // Allow other modules to intervene.
  // Any failure will prevent this action.
  foreach (module_implements('content_multigroup_allowed_in') as $module) {
    $function = $module .'_content_multigroup_allowed_in';
    $result = $function($field, $group);
    if ($result['allowed'] === FALSE) {
      return array('allowed' => FALSE, 'message' => $result['message']);
    }
  }

  $message = t('You are moving the field %field into a Multigroup.', array('%field' => $field['widget']['label']));
  return array('allowed' => TRUE, 'message' => $message);
}

/**
 * Helper function for deciding if a field is
 * allowed out of a Multigroup.
 */
function content_multigroup_allowed_out($field, $group) {
  if ($group['group_type'] != 'multigroup') {
    return array('allowed' => TRUE, 'message' => '');
  }
  // Optionwidgets do not behave the same in a Multigroup field as out of it.
  // In a Multigroup the same option can be selected multiple times,
  // but that is not possible in a normal group.

  // Adding a hook here where widgets that handle their own multiple values
  // can indicate their fields should not be removed from Multigroups.

  $max_existing = content_max_delta($field['field_name']);
  $no_remove_widgets = array(
    'optionwidgets_select',
    'optionwidgets_buttons',
    'optionwidgets_onoff',
    'nodereference_buttons',
    'nodereference_select',
    'userreference_buttons',
    'userreference_select',
  );
  $no_remove_widgets = array_merge($no_remove_widgets, module_invoke_all('content_multigroup_no_remove_widgets'));
  if (in_array($field['widget']['type'], $no_remove_widgets) && $max_existing > 0) {
    return array(
      'allowed' => FALSE,
      'message' => t('This change is not allowed. The field %field already has data created and uses a widget that stores data differently in a Standard group than in a Multigroup. Making this change could result in the loss of data.', array('%field' => $field['widget']['label']))
    );
  }

  // Allow other modules to intervene.
  // Any failure will prevent this action.
  foreach (module_implements('content_multigroup_allowed_out') as $module) {
    $function = $module .'_content_multigroup_allowed_out';
    $result = $function($field, $group);
    if ($result['allowed'] === FALSE) {
      return array('allowed' => FALSE, 'message' => $result['message']);
    }
  }

  $message = t('You are moving the field %field out of a Multigroup.', array('%field' => $field['widget']['label']));
  return array('allowed' => TRUE, 'message' => $message);
}

/**
 * Alter the basic field settings form.
 *
 * It should not be possible to choose a widget type that is not compatible
 * with multigroups.
 */
function content_multigroup_field_basic_form(&$form, &$form_state) {
  $field_name = $form['basic']['field_name']['#value'];
  $type_name = $form['type_name']['#value'];

  // Ignore this field if it is not part of a field group.
  if (!($group_name = fieldgroup_get_group($type_name, $field_name))) {
    return;
  }

  // Retrieve information about the group the field is in.
  $groups = fieldgroup_groups($type_name);
  $group = $groups[$group_name];

  // Ignore this field if it is not part of a multigroup.
  if ($group['group_type'] != 'multigroup') {
    return;
  }

  // Retrieve information about the field itself.
  $field = content_fields($field_name, $type_name);

  // Check if the widget can be moved out of the multigroup.
  $allowed_out = content_multigroup_allowed_out($field, $group);
  if (!$allowed_out['allowed']) {
    $form['basic']['widget_type']['#disabled'] = TRUE;
    $form['basic']['widget_type']['#suffix'] = '<div class="warning">'. t('The widget type cannot be changed because the field %field already has data created and this widget stores data differently in a Standard group than in a Multigroup. Allowing this change could result in the loss of data.', array('%field' => $field['widget']['label'])) .'</div>';
    return;
  }

  // Remove from the list of available widgets those that are not
  // compatible with multigroups.
  $widget_types = _content_widget_types();
  foreach (array_keys($form['basic']['widget_type']['#options']) as $widget_type) {
    if ($field['widget']['type'] != $widget_type) {
      $field_copy = $field;
      $field_copy['widget']['type'] = $widget_type;
      $field_copy['widget']['module'] = $widget_types[$widget_type]['module'];
      $allowed_in = content_multigroup_allowed_in($field_copy, $group);
      if (!$allowed_in['allowed']) {
        unset($form['basic']['widget_type']['#options'][$widget_type]);
      }
    }
  }
}

/**
 * Alter the "Display fields" form.
 *
 * Add an additional selector for setting multigroup field display format.
 */
function content_multigroup_display_overview_form(&$form, &$form_state) {

  $type_name = $form['#type_name'];
  $contexts_selector = $form['#contexts'];

  // Gather type information.
  $content_type = content_types($type_name);

  // The content module stops building the form if the type has no fields.
  if (empty($content_type['fields'])) {
    return;
  }

  $contexts = content_build_modes($contexts_selector);

  // Multigroups, extra values.
  $label_options = array(
    'above' => t('Above'),
    'hidden' => t('<Hidden>'),
  );
  $options = array(
    'simple' => t('Simple'),
    'fieldset' => t('Fieldset'),
    'fieldset_collapsible' => t('Fieldset - collapsible'),
    'fieldset_collapsed' => t('Fieldset - collapsed'),
    'hr' => t('Horizontal line'),
    'table-single' => t('Table - Single column'),
    'table-multiple' => t('Table - Multiple columns'),
  );
  foreach (fieldgroup_groups($type_name) as $group_name => $group) {
    if ($group['group_type'] != 'multigroup') {
      continue;
    }
    $subgroup_settings = isset($group['settings']['multigroup']['subgroup']) ? $group['settings']['multigroup']['subgroup'] : array();

    $subgroup_name = $group_name .'_subgroup';
    $form['#fields'] = array_merge(array($subgroup_name), $form['#fields']);
    $form[$subgroup_name] = array(
      'human_name' => array('#value' => t('[Subgroup format]')),
      'weight' => array('#type' => 'value', '#value' => -20),
      'parent' => array('#type' => 'value', '#value' => $group_name),
      'subgroup' => array('#type' => 'value', '#value' => 1),
    );
    if ($contexts_selector == 'basic') {
      $form[$subgroup_name]['label'] = array(
        '#type' => 'select',
        '#options' => $label_options,
        '#default_value' => isset($subgroup_settings['label']) ? $subgroup_settings['label'] : 'above',
      );
    }
    foreach ($contexts as $key => $title) {
      $form[$subgroup_name][$key]['format'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($subgroup_settings[$key]['format']) ? $subgroup_settings[$key]['format'] : 'fieldset',
      );
      $form[$subgroup_name][$key]['exclude'] = array('#type' => 'value', '#value' => 0);
    }
  }

  $form['#submit'] = array_merge(array('content_multigroup_display_overview_form_submit'), $form['#submit']);
}

/**
 * Submit handler for the display overview form.
 *
 * Do this in pre_save so we catch it before the content module
 * tries to use our 'field'.
 */
function content_multigroup_display_overview_form_submit($form, &$form_state) {
  $groups = fieldgroup_groups($form['#type_name']);
  $reset_cache = FALSE;

  // Find any subgroups we inserted into the display fields form,
  // save our settings, and remove them from $form_state.
  foreach ($form_state['values'] as $key => $values) {
    if (in_array($key, $form['#fields']) && !empty($values['parent']) && !empty($values['subgroup'])) {
      $group_name = $values['parent'];
      $group = $groups[$group_name];
      unset($values['subgroup'], $values['parent']);

      // We have some numeric keys here, so we can't use array_merge.
      foreach ($values as $k => $v) {
        $group['settings']['multigroup']['subgroup'][$k] = $v;
      }

      // Update the group information in the database. Note that
      // 'fieldgroup_data' in cache tables are also cleared here,
      // but we need to reset static caches of fieldgroup_groups().
      fieldgroup_save_group($form['#type_name'], $group);
      $reset_cache = TRUE;

      // Remove the subgroup from $form_state.
      unset($form_state['values'][$key]);
    }
  }
  if ($reset_cache) {
    fieldgroup_groups('', FALSE, TRUE);
  }
}

/**
 * Alter the Fieldgroup edit form to add Multigroup settings.
 */
function content_multigroup_group_edit_form(&$form, &$form_state) {
  $type_name = $form['#content_type']['type'];
  $group_name = $form['group_name']['#default_value'];

  $content_type = content_types($type_name);
  $groups = fieldgroup_groups($type_name);
  $group = $groups[$group_name];

  if ($group['group_type'] != 'multigroup') {
    return;
  }

  module_load_include('inc', 'content', 'includes/content.admin');
  module_load_include('inc', 'content', 'includes/content.crud');
  $form['group_type'] = array(
    '#type' => 'hidden',
    '#value' => $group['group_type'],
  );
  $form['settings']['multigroup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Multigroup settings'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  if (isset($group['settings']['multigroup']['subgroup'])) {
    // Preserve subgroup display settings.
    $form['settings']['multigroup']['subgroup'] = array(
      '#type' => 'value',
      '#value' => $group['settings']['multigroup']['subgroup'],
    );
  }

  $form['settings']['multigroup']['multiple-columns'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple columns'),
    '#default_value' => isset($group['settings']['multigroup']['multiple-columns']) ? $group['settings']['multigroup']['multiple-columns'] : 0,
    '#description' => t('Enable this option to render each field on a separate column on the node edit form.'),
  );

  $form['settings']['multigroup']['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#default_value' => isset($group['settings']['multigroup']['required']) ? $group['settings']['multigroup']['required'] : 1,
    '#description' => t('Enable this option to require a minimum of one collection of fields in this Multigroup.'),
  );

  $description = t('Number of times to repeat the collection of Multigroup fields.') .' ';
  $description .= t("'Unlimited' will provide an 'Add more' button so the users can add items as many times as they like.") .' ';
  $description .= t('All fields in this group will automatically be set to allow this number of values.');

  $group_multiple = isset($group['settings']['multigroup']['multiple']) ? $group['settings']['multigroup']['multiple'] : 1;
  $form['settings']['multigroup']['multiple'] = array(
    '#type' => 'select',
    '#title' => t('Number of repeats'),
    '#options' => content_multigroup_multiple_values(),
    '#default_value' => $group_multiple,
    '#description' => $description,
  );

  $form['settings']['multigroup']['labels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Labels'),
    '#description' => t("Labels for each subgroup of fields. Labels can be hidden or shown in various contexts using the 'Display fields' screen."),
  );
  if ($group_multiple < 2) {
    $group_multiple = 0;
  }
  for ($i = 0; $i < 10; $i++) {
    $form['settings']['multigroup']['labels'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Subgroup %number label', array('%number' => $i + 1)),
      '#default_value' => isset($group['settings']['multigroup']['labels'][$i]) ? $group['settings']['multigroup']['labels'][$i] : '',
    );
  }

  $form['#validate'][] = 'content_multigroup_group_edit_form_validate';
  $form['#submit'][] = 'content_multigroup_group_edit_form_submit';
}

/**
 * Validate the Fieldgroup edit form.
 */
function content_multigroup_group_edit_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];
  $group_type = $form_values['group_type'];
  if ($group_type != 'multigroup') {
    return;
  }
  $content_type = $form['#content_type'];
  $groups = fieldgroup_groups($content_type['type']);
  $group = $groups[$form_values['group_name']];
  foreach ($group['fields'] as $field_name => $data) {
    // Make sure we don't set the multiple values to a number that
    // would result in lost data.
    $max_existing = content_max_delta($field_name);
    if ($form_values['settings']['multigroup']['multiple'] != 1
      && $max_existing > $form_values['settings']['multigroup']['multiple']) {
      form_set_error('settings][multigroup][multiple', t('The field %field in this group already has %multiple values in the database. To prevent the loss of data you cannot set the number of Multigroup values to less than this.', array('%field' => $data['label'], '%multiple' => $max_existing)));
    }
  }
}

/**
 * Submit the Fieldgroup edit form.
 *
 * Update multiple values of fields contained in Multigroups.
 */
function content_multigroup_group_edit_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $group_type = $form_values['group_type'];
  if ($group_type != 'multigroup') {
    return;
  }
  module_load_include('inc', 'content', 'includes/content.crud');
  $content_type = $form['#content_type'];
  $groups = fieldgroup_groups($content_type['type']);
  $group = $groups[$form_values['group_name']];
  $group_fields = array_intersect_key($content_type['fields'], $group['fields']);
  if (!empty($group_fields)) {
    foreach ($group_fields as $field_name => $field) {
      $field['multiple'] = $form_values['settings']['multigroup']['multiple'];
      $field = content_field_instance_collapse($field);
      content_field_instance_update($field, FALSE);
    }
    content_clear_type_cache(TRUE);
    menu_rebuild();
  }
}
