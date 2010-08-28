<?php
// $Id: content_multigroup.node_form.inc,v 1.1.2.13 2009/12/29 16:14:20 markuspetrux Exp $

/**
 * @file
 * Implementation of node edit functions for content multigroup.
 */

/**
 * Implementation of hook_fieldgroup_form().
 *
 * Align the delta values of each field in the Multigroup.
 *
 * Swap the field name and delta for each Multigroup so we can
 * d-n-d each collection of fields as a single delta item.
 */
function _content_multigroup_fieldgroup_form(&$form, &$form_state, $form_id, $group) {
  $node = $form['#node'];
  $group_name = $group['group_name'];
  $group_multiple = (int)$group['settings']['multigroup']['multiple'];
  $content_type = content_types($group['type_name']);

  // Build list of accessible fields in this group.
  $group_fields = array();
  foreach ($content_type['fields'] as $field_name => $field) {
    if (isset($group['fields'][$field_name]) && isset($form[$group_name][$field_name])) {
      if (!isset($form[$group_name][$field_name]['#access']) || $form[$group_name][$field_name]['#access']) {
        $group_fields[$field_name] = $field;
      }
    }
  }

  // Quit if there are no field in the form for this group.
  if (empty($group_fields)) {
    return;
  }

  switch ($group_multiple) {
    case 0:
      $group_deltas = array(0);
      $max_delta = 0;
      break;

    case 1:
      // Compute unique deltas from all deltas used by fields in this multigroup.
      $group_deltas = array();
      $max_delta = -1;
      foreach (array_keys($group_fields) as $field_name) {
        if (!empty($node->$field_name) && is_array($node->$field_name)) {
          foreach (array_keys($node->$field_name) as $delta) {
            $group_deltas[$delta] = $delta;
          }
          sort($group_deltas);
          $max_delta = max($max_delta, max($group_deltas));
        }
      }
      $current_item_count = isset($form_state['item_count'][$group_name]) ? $form_state['item_count'][$group_name] : max(1, count($group_deltas));
      while (count($group_deltas) < $current_item_count) {
        $max_delta++;
        $group_deltas[] = $max_delta;
      }
      break;

    default:
      $max_delta = $group_multiple - 1;
      $group_deltas = range(0, $max_delta);
      break;
  }

  $form[$group_name]['#theme'] = 'content_multigroup_node_form';
  $form[$group_name]['#item_count'] = count($group_deltas);
  $form[$group_name]['#type_name'] = $group['type_name'];
  $form[$group_name]['#group_name'] = $group_name;
  $form[$group_name]['#group_label'] = $group['label'];
  $form[$group_name]['#group_fields'] = $group_fields;
  $form[$group_name]['#tree'] = TRUE;
  if (!isset($form['#multigroups'])) {
    $form['#multigroups'] = array();
  }
  $form['#multigroups'][$group_name] = $group_fields;

  // Add a visual indication to the fieldgroup title if the multigroup is required.
  if (!empty($group['settings']['multigroup']['required'])) {
    $form[$group_name]['#title'] .= '&nbsp;<span class="form-required" title="'. t('This group requires one collection of fields minimum.') .'">*</span>';
  }

  // Attach our own after build handler to the form, used to fix posting data
  // and the form structure, moving fields back to their original positions.
  // That is, move them from group->delta->field back to field->delta.
  if (!isset($form['#after_build'])) {
    $form['#after_build'] = array();
  }
  if (!in_array('content_multigroup_node_form_after_build', $form['#after_build'])) {
    array_unshift($form['#after_build'], 'content_multigroup_node_form_after_build');
  }

  // Attach our own validation handler to the form, used to check for empty fields.
  if (!isset($form['#validate'])) {
    $form['#validate'] = array();
  }
  if (!in_array('content_multigroup_node_form_validate', $form['#validate'])) {
    array_unshift($form['#validate'], 'content_multigroup_node_form_validate');
  }

  // Attach our own pre_render handler to the form, used to fix the required
  // attribute of all fields in multigroups.
  if (!isset($form['#pre_render'])) {
    $form['#pre_render'] = array();
  }
  if (!in_array('content_multigroup_node_form_pre_render', $form['#pre_render'])) {
    array_unshift($form['#pre_render'], 'content_multigroup_node_form_pre_render');
  }

  foreach ($group_deltas as $delta) {
    content_multigroup_group_form($form, $form_state, $group, $delta);
  }

  // Unset the original group field values now that we've moved them.
  foreach (array_keys($group_fields) as $field_name) {
    unset($form[$group_name][$field_name]);
  }

  if (($add_more = content_multigroup_add_more($form, $form_state, $group)) !== FALSE) {
    $form[$group_name] += $add_more;
  }
}

/**
 * Create a new delta value for the group.
 *
 * Called in form_alter and by AHAH add more.
 */
function content_multigroup_group_form(&$form, &$form_state, $group, $delta) {
  $group_name = $group['group_name'];
  if ($group['group_type'] != 'multigroup' || empty($form[$group_name]) || empty($form['#multigroups']) || empty($form['#multigroups'][$group_name])) {
    return;
  }
  module_load_include('inc', 'content', 'includes/content.node_form');

  $node = $form['#node'];
  $group_fields = $form['#multigroups'][$group_name];
  $group_multiple = $group['settings']['multigroup']['multiple'];

  foreach ($group_fields as $field_name => $field) {
    if (empty($form[$group_name][$delta])) {
      $form[$group_name] += array($delta => array($field_name => array()));
    }
    else {
      $form[$group_name][$delta][$field_name] = array();
    }

    $item_count = (isset($form_state['item_count'][$group_name]) ? $form_state['item_count'][$group_name] : $form[$group_name]['#item_count']);
    $form[$group_name][$delta]['_weight'] = array(
      '#type' => 'weight',
      '#delta' => $item_count, // this 'delta' is the 'weight' element's property
      '#default_value' => $delta,
      '#weight' => 100,
    );

    // Add a checkbox to allow users remove a single delta subgroup.
    // See content_set_empty() and theme_content_multigroup_node_form().
    if ($group_multiple == 1) {
      $form[$group_name][$delta]['_remove'] = array(
        '#type' => 'checkbox',
        '#attributes' => array('class' => 'content-multiple-remove-checkbox'),
        '#default_value' => isset($form_state['multigroup_removed'][$group_name][$delta]) ? $form_state['multigroup_removed'][$group_name][$delta] : 0,
      );
    }

    // Make each field into a pseudo single value field
    // with the right delta value.
    $field['multiple'] = 0;

    $form['#field_info'][$field_name] = $field;
    $node_copy = drupal_clone($node);

    // Set the form '#node' to the delta value we want so the Content
    // module will feed the right $items to the field module in
    // content_field_form().

    // There may be missing delta values for fields that were
    // never created, so check first.
    if (!empty($node->$field_name) && isset($node->{$field_name}[$delta])) {
      $node_copy->$field_name = array($delta => $node->{$field_name}[$delta]);
    }
    else {
      $value = NULL;
      // Try to obtain default values only if the node is being created.
      if (!isset($node->nid) && content_callback('widget', 'default value', $field) != CONTENT_CALLBACK_NONE) {
        // If a module wants to insert custom default values here,
        // it should provide a hook_default_value() function to call,
        // otherwise the content module's content_default_value() function
        // will be used.
        $callback = content_callback('widget', 'default value', $field) == CONTENT_CALLBACK_CUSTOM ? $field['widget']['module'] .'_default_value' : 'content_default_value';
        if (function_exists($callback)) {
          $items = $callback($form, $form_state, $field, 0);
          $value = $items[0];
        }
      }
      $node_copy->$field_name = array($delta => $value);
    }
    $form['#node'] = $node_copy;

    // Place the new element into the $delta position in the group form.
    if (content_handle('widget', 'multiple values', $field) == CONTENT_HANDLE_CORE) {
      $field_form = content_field_form($form, $form_state, $field, $delta);
      $value = array_key_exists($delta, $field_form[$field_name]) ? $delta : 0;
      $form[$group_name][$delta][$field_name] = $field_form[$field_name][$value];
    }
    else {
      // When the form is submitted, get the element data from the form values.
      if (isset($form_state['values'][$field_name])) {
        $form_state_copy = $form_state;
        if (isset($form_state_copy['values'][$field_name][$delta])) {
          $form_state_copy['values'][$field_name] = array($delta => $form_state_copy['values'][$field_name][$delta]);
        }
        else {
          $form_state_copy['values'][$field_name] = array($delta => NULL);
        }
        $field_form = content_field_form($form, $form_state_copy, $field, $delta);
      }
      else {
        $field_form = content_field_form($form, $form_state, $field, $delta);
      }

      // Multiple value fields have an additional level in the array form that
      // needs to get fixed in $form_state['values'].
      if (!isset($field_form[$field_name]['#element_validate'])) {
        $field_form[$field_name]['#element_validate'] = array();
      }
      $field_form[$field_name]['#element_validate'][] = 'content_multigroup_fix_multivalue_fields';

      $form[$group_name][$delta][$field_name] = $field_form[$field_name];
    }
    $form[$group_name][$delta][$field_name]['#weight'] = $field['widget']['weight'];
  }

  // Reset the form '#node' back to its original value.
  $form['#node'] = $node;
}

/**
 * Fix required flag during form rendering stage.
 *
 * Required fields should display the required star in the rendered form.
 */
function content_multigroup_node_form_pre_render(&$form) {
  foreach ($form['#multigroups'] as $group_name => $group_fields) {
    $required_fields = array();
    foreach ($group_fields as $field_name => $field) {
      if ($field['required']) {
        $required_fields[] = $field_name;
      }
    }
    if (!empty($required_fields)) {
      content_multigroup_node_form_fix_required($form[$group_name], $required_fields, TRUE);
    }
  }
  return $form;
}

/**
 * Fix form and posting data when the form is submitted.
 *
 * FormAPI uses form_builder() during form processing to map incoming $_POST
 * data to the proper elements in the form. It builds the '#parents' array,
 * copies the $_POST array to the '#post' member of all form elements, and it
 * also builds the $form_state['values'] array. Then the '#after_build' hook is
 * invoked to allow custom processing of the form structure, and that happens
 * just before validation and submit handlers are executed.
 *
 * During hook_form_alter(), the multigroup module altered the form structure
 * moving elements from field->delta to multigroup->delta->field position,
 * which is what has been processed by FormAPI to build the form structures,
 * but field validation (and submit) handlers expect their data to be located
 * in their original positions.
 *
 * We now need to move the fields back to their original positions in the form,
 * and we need to do so without altering the form rendering process, which is
 * now reflecting the structure the multigroup is interested in. We just need
 * to fix the parts of the form that affect validation and submit processing.
 */
function _content_multigroup_node_form_after_build($form, &$form_state) {
  // Disable required flag during FormAPI validation, except when building the
  // form for an 'Add more values' request.
  $required = !empty($form_state['multigroup_add_more']);
  foreach ($form['#multigroups'] as $group_name => $group_fields) {
    $required_fields = array();
    foreach ($group_fields as $field_name => $field) {
      if ($field['required']) {
        $required_fields[] = $field_name;
      }
    }
    if (!empty($required_fields)) {
      content_multigroup_node_form_fix_required($form[$group_name], $required_fields, $required);
    }
  }

  if ($form_state['submitted']) {
    // Fix value positions in $form_state for the fields in multigroups.
    foreach (array_keys($form['#multigroups']) as $group_name) {
      content_multigroup_node_form_transpose_elements($form, $form_state, $form['#node']->type, $group_name);
    }

    // Fix form element parents for all fields in multigroups.
    content_multigroup_node_form_fix_parents($form, $form['#multigroups']);

    // Update posting data to reflect delta changes in the form structure.
    if (!empty($_POST)) {
      content_multigroup_node_form_fix_post($form);
    }
  }

  return $form;
}

/**
 * Fix required flag for required fields.
 *
 * We need to let the user enter an empty set of fields for a delta subgroup,
 * even if it contains required fields, which is equivalent to say a subgroup
 * should be ignored, not to be stored into the database.
 * So, we need to check for required fields, but only for non-empty subgroups.
 *
 * When the form is processed for rendering, the required flag is enabled for
 * all required fields, so the user can see what's required and what's not.
 *
 * When the form is processed for validation, the required flag is disabled,
 * so that FormAPI does not report errors for empty fields.
 *
 * @see content_multigroup_node_form_validate().
 */
function content_multigroup_node_form_fix_required(&$elements, $required_fields, $required) {
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key]) {

      if (count($elements[$key]['#array_parents']) >= 3 && in_array($elements[$key]['#array_parents'][2], $required_fields) && isset($elements[$key]['#required'])) {
        $elements[$key]['#required'] = $required;
      }

      // Recurse through all children elements.
      content_multigroup_node_form_fix_required($elements[$key], $required_fields, $required);
    }
  }
}

/**
 * Node form validation handler.
 *
 * Perform validation for empty fields ignoring subgroups flagged for removal.
 * Note that FormAPI validation for required fields is disabled because we need
 * to accept empty fields that are flagged for removal.
 */
function content_multigroup_node_form_validate($form, &$form_state) {
  $type_name = $form['#node']->type;
  $groups = fieldgroup_groups($type_name);

  foreach ($form['#multigroups'] as $group_name => $group_fields) {
    $group = $groups[$group_name];
    $group_required = isset($group['settings']['multigroup']['required']) ? $group['settings']['multigroup']['required'] : 1;

    $non_empty_subgroups = $non_removed_subgroups = $required_field_errors = array();
    foreach ($group_fields as $field_name => $field) {
      // Tell the content module that it is not needed to enforce requirement
      // of fields in this multigroup because we are doing it here.
      // See content_multiple_value_nodeapi_validate().
      $form_state['values']['_content_ignore_required_fields'][$field_name] = TRUE;

      foreach ($form_state['values'][$field_name] as $delta => $item) {
        // Ignore subgroups flagged for removal.
        if ($form_state['multigroup_removed'][$group_name][$delta]) {
          continue;
        }
        // Keep track of non-removed subgroups.
        $non_removed_subgroups[$delta] = TRUE;

        $is_empty_function = $field['module'] .'_content_is_empty';
        if ($is_empty_function($form_state['values'][$field_name][$delta], $field)) {
          // Ignore fields that are not required.
          if (!$field['required']) {
            continue;
          }

          // Build an error message for this field in this subgroup, but do
          // not flag it, yet.
          if (!empty($item['_error_element'])) {
            // Here we don't know the number of elements and subelements a
            // widget could have added to the form, so we need to extract
            // components from the top, where we have group/delta/field, and
            // then push back field/delta on top of the list.
            $error_element = explode('][', $item['_error_element']);
            array_shift($error_element);
            array_shift($error_element);
            array_shift($error_element);
            array_unshift($error_element, $field_name, $delta);
            $error_element = implode('][', $error_element);
          }
          else {
            $error_element = '';
          }
          $required_field_errors[$delta][$field_name] = array(
            'element' => $error_element,
            'message' => t('!name field is required in group @group.', array(
              '!name' => $form[$group_name][$delta][$field_name]['#title'],
              '@group' => t($group['label']),
            )),
          );
        }
        else {
          $non_empty_subgroups[$delta] = TRUE;
        }
      }
    }

    // Required multigroups require at least one non-empty subgroup of fields.
    if ($group_required && empty($non_empty_subgroups)) {
      form_set_error('', t('Group @name requires one collection of fields minimum.', array('@name' => t($group['label']))));
      continue;
    }

    // Do not enforce field requirements if there is only one non-removed
    // subgroup of fields, and that subgroup is empty.
    if (count($non_removed_subgroups) == 1) {
      $delta = key($non_removed_subgroups);
      if (isset($required_field_errors[$delta]) && !isset($non_empty_subgroups[$delta])) {
        unset($required_field_errors[$delta]);
      }
    }

    // Ok, now we can flag errors for all required fields that have not been
    // filled in when they should.
    foreach ($required_field_errors as $delta => $error_list) {
      foreach ($error_list as $field_name => $error_info) {
        form_set_error($error_info['element'], $error_info['message']);
      }
    }
  }
}

/**
 * Transpose element positions in $form_state for the fields in a multigroup.
 */
function content_multigroup_node_form_transpose_elements(&$form, &$form_state, $type_name, $group_name) {
  $groups = fieldgroup_groups($type_name);
  $group = $groups[$group_name];
  $group_fields = $form['#multigroups'][$group_name];

  // Save the remove state of multigroup items in the $form_state array.
  if (!isset($form_state['multigroup_removed'])) {
    $form_state['multigroup_removed'] = array();
  }
  if (!isset($form_state['multigroup_removed'][$group_name])) {
    $form_state['multigroup_removed'][$group_name] = array();
  }

  // Move group data from group->delta->field to field->delta.
  $group_data = array();
  foreach ($form_state['values'][$group_name] as $delta => $items) {
    // Skip 'add more' button.
    if (!is_array($items) || !isset($items['_weight'])) {
      continue;
    }
    foreach ($group_fields as $field_name => $field) {
      if (!isset($group_data[$field_name])) {
        $group_data[$field_name] = array();
      }
      // Get field weight and remove state from the group and keep track of the
      // current delta for each field item.
      $item_defaults = array(
        '_weight' => $items['_weight'],
        '_remove' => $items['_remove'],
        '_old_delta' => $delta,
      );
      $group_data[$field_name][$delta] = (is_array($items[$field_name]) ? array_merge($items[$field_name], $item_defaults) : $item_defaults);
      // Store the remove state and the element weight in the form element as
      // well, so we can restore them later.
      // See content_multigroup_fix_multivalue_fields().
      // See content_multigroup_fix_element_values().
      $form[$group_name][$delta][$field_name]['#_weight'] = $items['_weight'];
      $form[$group_name][$delta][$field_name]['#removed'] = $items['_remove'];

      // Insert an element valitation callback of our own at the end of the
      // list to ensure the drag'n'drop weight of the element is not lost by
      // a form_set_value() operation made by the validation callback of the
      // widget element.
      if (!isset($form[$group_name][$delta][$field_name]['#element_validate'])) {
        $form[$group_name][$delta][$field_name]['#element_validate'] = array();
      }
      $form[$group_name][$delta][$field_name]['#element_validate'][] = 'content_multigroup_fix_element_values';
    }
    $form_state['multigroup_removed'][$group_name][$delta] = $items['_remove'];
  }

  $form_group_sorted = FALSE;
  foreach ($group_data as $field_name => $items) {

    // Sort field items according to drag-n-drop reordering. Deltas are also
    // rebuilt to start counting from 0 to n. Note that since all fields in the
    // group share the same weight, their deltas remain in sync.
    usort($items, '_content_sort_items_helper');

    // Now we need to apply the same ordering to the form elements. Also,
    // note that deltas have changed during the sort operation, so we need
    // to reflect this delta conversion in the form.
    if (!$form_group_sorted) {
      $form_group_items = array();
      $form_deltas = array();
      foreach ($items as $new_delta => $item) {
        $form_deltas[$item['_old_delta']] = $new_delta;
        $form_group_items[$new_delta] = $form[$group_name][$item['_old_delta']];
        unset($form[$group_name][$item['_old_delta']]);
      }
      foreach ($form_group_items as $new_delta => $form_group_item) {
        $form[$group_name][$new_delta] = $form_group_item;
      }
      content_multigroup_node_form_fix_deltas($form[$group_name], $form_deltas);
      $form_group_sorted = TRUE;
    }

    // Get rid of the old delta value.
    foreach (array_keys($items) as $delta) {
      unset($items[$delta]['_old_delta']);
    }

    // Fix field and delta positions in the $_POST array.
    if (!empty($_POST)) {
      $_POST[$field_name] = array();
      foreach ($items as $new_delta => $item) {
        $_POST[$field_name][$new_delta] = $item;
      }
      if (isset($_POST[$group_name])) {
        unset($_POST[$group_name]);
      }
    }

    // Move field items back to their original positions.
    $form_state['values'][$field_name] = $items;
  }

  // Finally, get rid of the group data in form values.
  unset($form_state['values'][$group_name]);
}

/**
 * Fix deltas for all affected form elements.
 */
function content_multigroup_node_form_fix_deltas(&$elements, $form_deltas) {
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key]) {

      // Fix the second item, the delta value, of the element's '#parents' array.
      $elements[$key]['#parents'][1] = $form_deltas[$elements[$key]['#parents'][1]];

      // If present, fix delta value in '#delta' attribute of the element.
      if (isset($elements[$key]['#delta']) && isset($form_deltas[$elements[$key]['#delta']])) {
        $elements[$key]['#delta'] = $form_deltas[$elements[$key]['#delta']];
      }

      // Recurse through all children elements.
      content_multigroup_node_form_fix_deltas($elements[$key], $form_deltas);
    }
  }
}

/**
 * Fix form element parents for all fields in multigroups.
 *
 * The $element['#parents'] array needs to reflect the position of the fields
 * in the $form_state['values'] array so that form_set_value() can be safely
 * used by field validation handlers.
 */
function content_multigroup_node_form_fix_parents(&$elements, $multigroups) {
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key]) {

      // Check if the current element is child of a multigroup. The #parents
      // array for field values has, at least, 3 parent elements, being the
      // first one the name of a multigroup.
      if (count($elements[$key]['#parents']) >= 3 && isset($multigroups[$elements[$key]['#parents'][0]])) {

        // Extract group name, delta and field name from the #parents array.
        array_shift($elements[$key]['#parents']);
        $delta = array_shift($elements[$key]['#parents']);
        $field_name = array_shift($elements[$key]['#parents']);

        // Now, insert field name and delta to the #parents array.
        array_unshift($elements[$key]['#parents'], $field_name, $delta);
      }

      // Recurse through all children elements.
      content_multigroup_node_form_fix_parents($elements[$key], $multigroups);
    }
  }
}

/**
 * Update posting data to reflect delta changes in the form structure.
 *
 * The $_POST array is fixed in content_multigroup_node_form_transpose_elements().
 */
function content_multigroup_node_form_fix_post(&$elements) {
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key]) {

      // Update the element copy of the $_POST array.
      $elements[$key]['#post'] = $_POST;

      // Recurse through all children elements.
      content_multigroup_node_form_fix_post($elements[$key]);
    }
  }

  // Update the form copy of the $_POST array.
  $elements['#post'] = $_POST;
}

/**
 * Make sure the '_weight' and '_remove' attributes of the element exist.
 *
 * @see content_multigroup_node_form_transpose_elements()
 */
function content_multigroup_fix_element_values($element, &$form_state) {
  $field_name = $element['#field_name'];
  $delta = $element['#delta'];
  if (!isset($form_state['values'][$field_name][$delta]['_weight']) || !isset($form_state['values'][$field_name][$delta]['_remove'])) {
    $value = array('_weight' => $element['#_weight'], '_remove' => $element['#removed']);
    if (isset($form_state['values'][$field_name][$delta]) && is_array($form_state['values'][$field_name][$delta])) {
      $value = array_merge($form_state['values'][$field_name][$delta], $value);
    }
    form_set_value($element, $value, $form_state);
  }
}

/**
 * Fix the value for fields that deal with multiple values themselves.
 */
function content_multigroup_fix_multivalue_fields($element, &$form_state) {
  $field_name = $element['#field_name'];
  $delta = $element['#delta'];
  if (isset($form_state['values'][$field_name][$delta][0]) && is_array($form_state['values'][$field_name][$delta][0])) {
    $value = array_merge($form_state['values'][$field_name][$delta][0], array('_remove' => $element['#removed']));
  }
  else {
    $value = array('_remove' => $element['#removed']);
  }
  form_set_value($element, $value, $form_state);
}

/**
 * Add AHAH add more button, if not working with a programmed form.
 */
function content_multigroup_add_more(&$form, &$form_state, $group) {
  $group_multiple = $group['settings']['multigroup']['multiple'];
  if ($group_multiple != 1 || !empty($form['#programmed'])) {
    return FALSE;
  }

  // Make sure the form is cached so ahah can work.
  $form['#cache'] = TRUE;
  $content_type = content_types($group['type_name']);
  $group_name = $group['group_name'];
  $group_name_css = str_replace('_', '-', $group_name);

  $form_element = array();
  $form_element[$group_name .'_add_more'] = array(
    '#type' => 'submit',
    '#name' => $group_name .'_add_more',
    '#value' => t('Add more values'),
    '#weight' => $group_multiple + 1,
    '#submit' => array('content_multigroup_add_more_submit'),
    '#ahah' => array(
      'path' => 'content_multigroup/js_add_more/'. $content_type['url_str'] .'/'. $group_name,
      'wrapper' => $group_name_css .'-items',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    // When JS is disabled, the content_multigroup_add_more_submit handler will
    // find the relevant group information using these entries.
    '#group_name' => $group_name,
    '#type_name' => $group['type_name'],
    '#item_count' => $form[$group_name]['#item_count'],
  );

  // Add wrappers for the group and 'more' button.
  $form_element['#prefix'] = '<div id="'. $group_name_css .'-items">';
  $form_element['#suffix'] = '</div>';
  $form_element[$group_name .'_add_more']['#prefix'] = '<div class="content-add-more clear-block">';
  $form_element[$group_name .'_add_more']['#suffix'] = '</div>';

  return $form_element;
}

/**
 * Submit handler to add more choices to a content form. This handler is used when
 * JavaScript is not available. It makes changes to the form state and the
 * entire form is rebuilt during the page reload.
 */
function content_multigroup_add_more_submit($form, &$form_state) {
  // Set the form to rebuild and run submit handlers.
  node_form_submit_build_node($form, $form_state);
  $group_name = $form_state['clicked_button']['#group_name'];
  $type_name = $form_state['clicked_button']['#type_name'];

  // Make the changes we want to the form state.
  if (isset($form_state['clicked_button']['#item_count'])) {
    $form_state['item_count'][$group_name] = $form_state['clicked_button']['#item_count'] + 1;
  }
}

/**
 * Menu callback for AHAH addition of new empty widgets.
 *
 * Adapted from content_add_more_js to work with groups instead of fields.
 */
function content_multigroup_add_more_js($type_name_url, $group_name) {
  $content_type = content_types($type_name_url);
  $groups = fieldgroup_groups($content_type['type']);
  $group = $groups[$group_name];

  if (($group['settings']['multigroup']['multiple'] != 1) || empty($_POST['form_build_id'])) {
    // Invalid request.
    drupal_json(array('data' => ''));
    exit;
  }

  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    exit;
  }

  // We don't simply return a new empty widget to append to existing ones, because
  // - ahah.js won't simply let us add a new row to a table
  // - attaching the 'draggable' behavior won't be easy
  // So we resort to rebuilding the whole table of widgets including the existing ones,
  // which makes us jump through a few hoops.

  // The form that we get from the cache is unbuilt. We need to build it so that
  // _value callbacks can be executed and $form_state['values'] populated.
  // We only want to affect $form_state['values'], not the $form itself
  // (built forms aren't supposed to enter the cache) nor the rest of $form_data,
  // so we use copies of $form and $form_data.
  $form_copy = $form;
  $form_state_copy = $form_state;
  $form_copy['#post'] = array();
  form_builder($_POST['form_id'], $form_copy, $form_state_copy);
  // Just grab the data we need.
  $form_state['values'] = $form_state_copy['values'];
  // Reset cached ids, so that they don't affect the actual form we output.
  form_clean_id(NULL, TRUE);

  // Sort the $form_state['values'] we just built *and* the incoming $_POST data
  // according to d-n-d reordering.
  unset($form_state['values'][$group_name][$group['group_name'] .'_add_more']);
  foreach ($_POST[$group_name] as $delta => $item) {
    $form_state['values'][$group_name][$delta]['_weight'] = $item['_weight'];
    $form_state['values'][$group_name][$delta]['_remove'] = isset($item['_remove']) ? $item['_remove'] : 0;
  }
  $group['multiple'] = $group['settings']['multigroup']['multiple'];
  $form_state['values'][$group_name] = _content_sort_items($group, $form_state['values'][$group_name]);
  $_POST[$group_name] = _content_sort_items($group, $_POST[$group_name]);

  // Build our new form element for the whole group, asking for one more element.
  $delta = max(array_keys($_POST[$group_name])) + 1;
  $form_state['item_count'] = array($group_name => count($_POST[$group_name]) + 1);
  content_multigroup_group_form($form, $form_state, $group, $delta);

  // Rebuild weight deltas to make sure they all are equally dimensioned.
  foreach ($form[$group_name] as $key => $item) {
    if (is_numeric($key) && isset($item['_weight']) && is_array($item['_weight'])) {
      $form[$group_name][$key]['_weight']['#delta'] = $delta;
    }
  }

  // Save the new definition of the form.
  $form_state['values'] = array();
  form_set_cache($form_build_id, $form, $form_state);

  // Build the new form against the incoming $_POST values so that we can
  // render the new element.
  $_POST[$group_name][$delta]['_weight'] = $delta;
  $form_state = array('submitted' => FALSE, 'multigroup_add_more' => TRUE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $group_form = $form[$group_name];
  // We add a div around the new content to receive the ahah effect.
  $group_form[$delta]['#prefix'] = '<div class="ahah-new-content">'. (isset($group_form[$delta]['#prefix']) ? $group_form[$delta]['#prefix'] : '');
  $group_form[$delta]['#suffix'] = (isset($group_form[$delta]['#suffix']) ? $group_form[$delta]['#suffix'] : '') .'</div>';
  // Prevent duplicate wrapper.
  unset($group_form['#prefix'], $group_form['#suffix']);
  // We're in the AHAH handler, so the fieldset was expanded.
  $group_form['#collapsed'] = FALSE;

  // If a newly inserted widget contains AHAH behaviors, they normally won't
  // work because AHAH doesn't know about those - it just attaches to the exact
  // form elements that were initially specified in the Drupal.settings object.
  // The new ones didn't exist then, so we need to update Drupal.settings
  // by ourselves in order to let AHAH know about those new form elements.
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';

  $output = theme('status_messages') . drupal_render($group_form) . $output_js;

  // Using drupal_json() breaks filefield's file upload, because the jQuery
  // Form plugin handles file uploads in a way that is not compatible with
  // 'text/javascript' response type.
  $GLOBALS['devel_shutdown'] =  FALSE;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}

/**
 * Theme an individual form element.
 *
 * Combine multiple values into a table with drag-n-drop reordering.
 */
function theme_content_multigroup_node_form($element) {
  $group_name = $element['#group_name'];
  $groups = fieldgroup_groups($element['#type_name']);
  $group = $groups[$group_name];
  $group_multiple = $group['settings']['multigroup']['multiple'];
  $group_fields = $element['#group_fields'];

  $table_id = $element['#group_name'] .'_values';
  $table_class = 'content-multiple-table';
  $order_class = $element['#group_name'] .'-delta-order';
  $subgroup_settings = isset($group['settings']['multigroup']['subgroup']) ? $group['settings']['multigroup']['subgroup'] : array();
  $show_label = isset($subgroup_settings['label']) ? $subgroup_settings['label'] : 'above';
  $subgroup_labels = isset($group['settings']['multigroup']['labels']) ? $group['settings']['multigroup']['labels'] : array();
  $multiple_columns = isset($group['settings']['multigroup']['multiple-columns']) ? $group['settings']['multigroup']['multiple-columns'] : 0;

  $headers = array();
  if ($group_multiple >= 1) {
    $headers[] = array('data' => '');
  }
  if ($multiple_columns) {
    foreach ($group_fields as $field_name => $field) {
      $required = !empty($field['required']) ? '&nbsp;<span class="form-required" title="'. t('This field is required.') .'">*</span>' : '';
      $headers[] = array(
        'data' => check_plain(t($field['widget']['label'])) . $required,
        'class' => 'content-multigroup-cell-'. str_replace('_', '-', $field_name),
      );
    }
    $table_class .= ' content-multigroup-edit-table-multiple-columns';
  }
  else {
    if ($group_multiple >= 1) {
      $headers[0]['colspan'] = 2;
    }
    $table_class .= ' content-multigroup-edit-table-single-column';
  }
  if ($group_multiple >= 1) {
    $headers[] = array('data' => t('Order'), 'class' => 'content-multiple-weight-header');
    if ($group_multiple == 1) {
      $headers[] = array('data' => '<span>'. t('Remove') .'</span>', 'class' => 'content-multiple-remove-header');
    }
  }
  $rows = array();

  $i = 0;
  foreach (element_children($element) as $delta => $key) {
    if (is_numeric($key)) {
      $cells = array();
      $label = ($show_label == 'above' && !empty($subgroup_labels[$i]) ? theme('content_multigroup_node_label', check_plain(t($subgroup_labels[$i]))) : '');
      $element[$key]['_weight']['#attributes']['class'] = $order_class;
      if ($group_multiple >= 1) {
        $cells[] = array('data' => '', 'class' => 'content-multiple-drag');
        $delta_element = drupal_render($element[$key]['_weight']);
        if ($group_multiple == 1) {
          $remove_element = drupal_render($element[$key]['_remove']);
        }
      }
      else {
        $element[$key]['_weight']['#type'] = 'hidden';
      }
      if ($multiple_columns) {
        foreach ($group_fields as $field_name => $field) {
          $cell = array(
            'data' => (isset($element[$key][$field_name]) ? drupal_render($element[$key][$field_name]) : ''),
            'class' => 'content-multigroup-cell-'. str_replace('_', '-', $field_name),
          );
          if (!empty($cell['data']) && !empty($element[$key][$field_name]['#description'])) {
            $cell['title'] = $element[$key][$field_name]['#description'];
          }
          $cells[] = $cell;
        }
      }
      else {
        $cells[] = $label . drupal_render($element[$key]);
      }
      if ($group_multiple >= 1) {
        $row_class = 'draggable';
        $cells[] = array('data' => $delta_element, 'class' => 'delta-order');
        if ($group_multiple == 1) {
          if (!empty($element[$key]['_remove']['#value'])) {
            $row_class .= ' content-multiple-removed-row';
          }
          $cells[] = array('data' => $remove_element, 'class' => 'content-multiple-remove-cell');
        }
        $rows[] = array('data' => $cells, 'class' => $row_class);
      }
      else {
        $rows[] = array('data' => $cells);
      }
    }
    $i++;
  }

  drupal_add_css(drupal_get_path('module', 'content_multigroup') .'/content_multigroup.css');
  $output = theme('table', $headers, $rows, array('id' => $table_id, 'class' => $table_class));
  $output .= drupal_render($element[$group_name .'_add_more']);

  // Enable drag-n-drop only if the group really allows multiple values.
  if ($group_multiple >= 1) {
    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
    drupal_add_js(drupal_get_path('module', 'content') .'/js/content.node_form.js');
  }

  return $output;
}

/**
 * Theme the sub group label in the node form.
 */
function theme_content_multigroup_node_label($text) {
  return !empty($text) ? '<h3>'. $text .'</h3>' : '';
}
