<?php
// $Id: content.node_form.inc,v 1.7.2.19.2.6 2009/12/29 09:03:15 markuspetrux Exp $

/**
 * @file
 * Create fields' form for a content type.
 *
 * Each field defines its own component of the content entry form, via its
 * chosen widget.
 */
function content_form(&$form, &$form_state) {
  $type = content_types($form['type']['#value']);
  foreach ($type['fields'] as $field_name => $field) {
    $form['#field_info'][$field['field_name']] = $field;
    $form += (array) content_field_form($form, $form_state, $field);
  }
  return $form;
}

/**
 * Create a separate form element for each field.
 *
 * // TODO: $count param ? not used anymore ?
 * Hook_widget() picks up two new values, $count and $delta, to help
 * widgets know what information to return since multiple values are
 * sometimes controlled by the content module.
 *
 * @param $form
 *   the form to add this field element to
 * @param $form_state
 *   the form_state for the above form
 * @param $field
 *   the field array to use to create the form element
 * @param $get_delta
 *   use to get only a specific delta value of a multiple value field, otherwise
 *   function will return the entire $field form element.
 */
function content_field_form(&$form, &$form_state, $field, $get_delta = NULL) {
  $form['#cache'] = FALSE;
  $node = $form['#node'];
  $addition = array();
  $form_element = array();
  $field_name = $field['field_name'];

  $items = array();

  // TODO: is the "if (function_exists($function)) {" needed ?
  // defining the $function here makes it unclear where it is actually called
  $function = $field['widget']['module'] .'_widget';
  if (function_exists($function)) {
    // Prepare the values to be filled in the widget.
    // We look in the following places:
    // - Form submitted values
    // - Node values (when editing an existing node), or pre-filled values (when
    //   creating a new node translation)
    // - Default values set for the field (when creating a new node).
    if (!empty($form_state['values'][$field['field_name']])) {
      $items = $form_state['values'][$field['field_name']];
      // If there was an AHAH add more button in this field, don't save it.
      unset($items[$field['field_name'] .'_add_more']);
    }
    elseif (!empty($node->$field['field_name'])) {
      $items = $node->$field['field_name'];
    }
    elseif (empty($node->nid)) {
      if (content_callback('widget', 'default value', $field) != CONTENT_CALLBACK_NONE) {
        // If a module wants to insert custom default values here,
        // it should provide a hook_default_value() function to call,
        // otherwise the content module's content_default_value() function
        // will be used.
        $callback = content_callback('widget', 'default value', $field) == CONTENT_CALLBACK_CUSTOM ? $field['widget']['module'] .'_default_value' : 'content_default_value';
        if (function_exists($callback)) {
          $items = $callback($form, $form_state, $field, 0);
        }
      }
    }

    // See if access to this form element is restricted,
    // if so, skip widget processing and just set the value.
    $access = content_access('edit', $field, NULL, $node);
    if (!$access) {
      $addition[$field_name] = array(
        '#access' => $access,
        '#type' => 'value',
        '#value' => $items,
      );
      return $addition;
    }

    // If content module handles multiple values for this form element,
    // and not selecting an individual $delta, process the multiple value form.
    if (!isset($get_delta) && content_handle('widget', 'multiple values', $field) == CONTENT_HANDLE_CORE) {
      $form_element = content_multiple_value_form($form, $form_state, $field, $items);
    }
    // If the widget is handling multiple values (e.g optionwidgets),
    // or selecting an individual element, just get a single form
    // element and make it the $delta value.
    else {
      $delta = isset($get_delta) ? $get_delta : 0;
      if ($element = $function($form, $form_state, $field, $items, $delta)) {
        $title = check_plain(t($field['widget']['label']));
        $description = content_filter_xss(t($field['widget']['description']));
        $defaults = array(
          '#required' => $get_delta > 0 ? FALSE : $field['required'],
          '#columns'  => array_keys($field['columns']),
          '#title' => $title,
          '#description' => $description,
          '#delta' => $delta,
          '#field_name' => $field['field_name'],
          '#type_name' => $field['type_name'],
        );
        // If we're processing a specific delta value for a field where the
        // content module handles multiples, set the delta in the result.
        // For fields that handle their own processing, we can't make assumptions
        // about how the field is structured, just merge in the returned value.
        if (content_handle('widget', 'multiple values', $field) == CONTENT_HANDLE_CORE) {
          $form_element[$delta] = array_merge($element, $defaults);
        }
        else {
          $form_element = array_merge($element, $defaults);
        }
      }
    }

    // Field name is needed at top level as well as the individual elements
    // so the multiple values or other field level theme or processing can find it.
    if ($form_element) {
      $defaults = array(
        '#field_name' => $field['field_name'],
        '#tree' => TRUE,
        '#weight' => $field['widget']['weight'],
        '#access' => $access,
        // TODO: what's the need for #count ? does not seem to be used anywhere ?
        '#count' => count($form_element),
      );
      $addition[$field['field_name']] = array_merge($form_element, $defaults);
    }
  }
  return $addition;
}

/**
 * Special handling to create form elements for multiple values.
 *
 * Handles generic features for multiple fields:
 * - number of widgets
 * - AHAH-'add more' button
 * - drag-n-drop value reordering
 */
function content_multiple_value_form(&$form, &$form_state, $field, $items) {
  $field_name = $field['field_name'];

  switch ($field['multiple']) {
    case 0:
      $deltas = array(0);
      $max = 0;
      break;

    case 1:
      $deltas = array_keys($items);
      $current_item_count = max(1, (isset($form_state['item_count'][$field_name]) ? $form_state['item_count'][$field_name] : count($deltas)));
      $max = (!empty($deltas) ? max($deltas) : -1);
      while (count($deltas) < $current_item_count) {
        $max++;
        $deltas[] = $max;
      }
      break;

    default:
      $max = $field['multiple'] - 1;
      $deltas = range(0, $max);
      break;
  }

  $title = check_plain(t($field['widget']['label']));
  $description = content_filter_xss(t($field['widget']['description']));

  $form_element = array(
    '#theme' => 'content_multiple_values',
    '#title' => $title,
    '#required' => $field['required'],
    '#description' => $description,
  );
  $function = $field['widget']['module'] .'_widget';

  foreach ($deltas as $delta) {
    if ($element = $function($form, $form_state, $field, $items, $delta)) {
      $defaults = array(
        '#title' => ($field['multiple'] >= 1) ? '' : $title,
        '#description' => ($field['multiple'] >= 1) ? '' : $description,
        '#required' => ($field['multiple'] == 0 ? $field['required'] : FALSE),
        '#weight' => $delta,
        '#delta' => $delta,
        '#columns' => array_keys($field['columns']),
        '#field_name' => $field_name,
        '#type_name' => $field['type_name'],
      );

      // Add an input field for the delta (drag-n-drop reordering), which will
      // be hidden by tabledrag js behavior.
      if ($field['multiple'] >= 1) {
        // We name the element '_weight' to avoid clashing with column names
        // defined by field modules.
        $element['_weight'] = array(
          '#type' => 'weight',
          '#delta' => $max, // this 'delta' is the 'weight' element's property
          '#default_value' => isset($items[$delta]['_weight']) ? $items[$delta]['_weight'] : $delta,
          '#weight' => 100,
        );
      }

      // Add a checkbox to allow users remove a single delta item.
      // See content_set_empty() and theme_content_multiple_values().
      if ($field['multiple'] == 1) {
        // We name the element '_remove' to avoid clashing with column names
        // defined by field modules.
        $element['_remove'] = array(
          '#type' => 'checkbox',
          '#attributes' => array('class' => 'content-multiple-remove-checkbox'),
          '#default_value' => isset($items[$delta]['_remove']) ? $items[$delta]['_remove'] : 0,
        );
      }

      $form_element[$delta] = array_merge($element, $defaults);
    }
  }

  // Add an #after_build callback to prevent validation of fields that are
  // flagged for removal and enforce field requirement settings.
  if ($field['multiple'] >= 1) {
    $form_element['#after_build'] = array('content_multiple_value_after_build_proxy');
  }

  // Add AHAH add more button, if not working with a programmed form.
  if ($field['multiple'] == 1 && empty($form['#programmed'])) {
    // Make sure the form is cached so ahah can work.
    $form['#cache'] = TRUE;
    $content_type = content_types($field['type_name']);
    $field_name_css = str_replace('_', '-', $field_name);

    $form_element[$field_name .'_add_more'] = array(
      '#type' => 'submit',
      '#name' => $field_name .'_add_more',
      '#value' => t('Add another item'),
      '#weight' => $field['widget']['weight'] + $max + 1,
      // Submit callback for disabled JavaScript. drupal_get_form() might get
      // the form from the cache, so we can't rely on content_form_alter()
      // including this file. Therefore, call a proxy function to do this.
      '#submit' => array('content_add_more_submit_proxy'),
      '#ahah' => array(
        'path' => 'content/js_add_more/'. $content_type['url_str'] .'/'. $field_name,
        'wrapper' => $field_name_css .'-items',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      // When JS is disabled, the content_add_more_submit handler will find
      // the relevant field using these entries.
      '#field_name' => $field_name,
      '#type_name' => $field['type_name'],
    );

    // Add wrappers for the fields and 'more' button.
    $form_element['#prefix'] = '<div id="'. $field_name_css .'-items">';
    $form_element['#suffix'] = '</div>';
    $form_element[$field_name .'_add_more']['#prefix'] = '<div class="content-add-more clear-block">';
    $form_element[$field_name .'_add_more']['#suffix'] = '</div>';
  }
  return $form_element;
}

/**
 * After build callback for multiple value fields.
 */
function content_multiple_value_after_build($elements, &$form_state) {
  $items_map = array();

  foreach (element_children($elements) as $delta) {
    // Find delta items for this field when the form if being processed for validation.
    if (isset($elements[$delta]) && $elements[$delta] && is_numeric($delta) && !empty($elements[$delta]['#needs_validation'])) {

      // Find items that have been flagged for removal.
      if (isset($elements[$delta]['_remove']) && !empty($elements[$delta]['_remove']['#value'])) {

        // Update the value in the #post attribute of the elements.
        $post = &$elements[$delta]['#post'];
        foreach ($elements[$delta]['#parents'] as $name) {
          $post = &$post[$name];
        }
        $post = array('_weight' => $elements[$delta]['_weight']['#value'], '_remove' => 1);

        // Alter the value of this element and children recursively.
        content_multiple_value_after_build_recursive($elements[$delta], $elements[$delta]['#post']);

        $items_map[$delta] = TRUE;
      }
      else {
        $items_map[$delta] = FALSE;
      }
    }
  }

  // If the multiple values field is required, then make sure there's at
  // least one item not flagged for removal. This is necessary to point
  // the user to the correct form element when the validation error is
  // issued from content_multiple_value_nodeapi_validate().
  $items_count = count($items_map);
  if (!empty($elements['#required']) && $items_count > 0) {
    // If the number of removed items is equal to the total number of
    // items, then we'll reset the '_remove' flag of the first item, and
    // that will be used to point the user when the required field error
    // is issued by content_multiple_value_nodeapi_validate().
    if ($items_count == count(array_filter($items_map))) {
      $delta = key($items_map);
      if (isset($elements[$delta]['_remove'])) {
        $elements[$delta]['_remove']['#value'] = 0;
      }
    }
  }

  return $elements;
}

/**
 * Helper function to deal with items flagged for removal recursively.
 */
function content_multiple_value_after_build_recursive(&$elements, $post) {
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key] && !in_array($key, array('_weight', '_remove', '_error_element'))) {
      // Recurse through all children elements.
      content_multiple_value_after_build_recursive($elements[$key], $post);
    }
  }

  // Remove values for items flagged for removal.
  if (isset($elements['#value'])) {
    $elements['#value'] = NULL;
    form_set_value($elements, NULL, $form_state);
    $elements['#post'] = $post;
  }
}

/**
 * Implementation of nodeapi('validate') for multiple value fields
 * managed by content module itself.
 */
function content_multiple_value_nodeapi_validate(&$node, $field, &$items, $form) {
  $field_name = $field['field_name'];

  // Getting the field structure from the form allows other modules alter
  // field properties such as the required attribute.
  $field = $form['#field_info'][$field_name];

  // Get rid of the add more items element.
  unset($items[$field_name .'_add_more']);

  // Reorder items to account for drag-n-drop reordering.
  $items = _content_sort_items($field, $items);

  // Create a copy of the items before filtering those that are flagged
  // for removal. We need this copy later to obtain the error element.
  $items_copy = $items;

  // Filter out items flagged for removal.
  $items = content_set_empty($field, $items);

  // Enforce field requirement settings.
  if ($field['required'] && empty($node->_content_ignore_required_fields[$field_name]) && content_access('edit', $field, NULL, $node)) {
    // Count non-empty items.
    $count = 0;
    $function = $field['module'] .'_content_is_empty';
    foreach ($items as $item) {
      if (!$function($item, $field)) {
        $count++;
      }
    }
    // The field is required so we expect at least one non-empty item.
    if ($count == 0) {
      // Try to guess the element path in the form from the first item that
      // is not flagged for removal. Defaults to first item.
      $error_element_index = 0;
      foreach ($items_copy as $index => $item) {
        if (empty($item['_remove'])) {
          $error_element_index = $index;
          break;
        }
      }
      $error_element = isset($items_copy[$error_element_index]) && is_array($items_copy[$error_element_index]) && isset($items_copy[$error_element_index]['_error_element']) ? $items_copy[$error_element_index]['_error_element'] : '';
      form_set_error($error_element, t('%name field is required.', array('%name' => t($field['widget']['label']))));
    }
  }
}

/**
 * Submit handler to add more choices to a content form. This handler is used when
 * JavaScript is not available. It makes changes to the form state and the
 * entire form is rebuilt during the page reload.
 */
function content_add_more_submit($form, &$form_state) {
  // Set the form to rebuild and run submit handlers.
  node_form_submit_build_node($form, $form_state);
  $field_name = $form_state['clicked_button']['#field_name'];
  $type_name = $form_state['clicked_button']['#type_name'];

  // Make the changes we want to the form state.
  if ($form_state['values'][$field_name][$field_name .'_add_more']) {
    $form_state['item_count'][$field_name] = count($form_state['values'][$field_name]);
  }
}

/**
 * Menu callback for AHAH addition of new empty widgets.
 */
function content_add_more_js($type_name_url, $field_name) {
  $type = content_types($type_name_url);
  $field = content_fields($field_name, $type['type']);

  if (($field['multiple'] != 1) || empty($_POST['form_build_id'])) {
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
  unset($form_state['values'][$field_name][$field['field_name'] .'_add_more']);
  foreach ($_POST[$field_name] as $delta => $item) {
    $form_state['values'][$field_name][$delta]['_weight'] = $item['_weight'];
    $form_state['values'][$field_name][$delta]['_remove'] = isset($item['_remove']) ? $item['_remove'] : 0;
  }
  $form_state['values'][$field_name] = _content_sort_items($field, $form_state['values'][$field_name]);
  $_POST[$field_name] = _content_sort_items($field, $_POST[$field_name]);

  // Build our new form element for the whole field, asking for one more element.
  $delta = max(array_keys($_POST[$field_name])) + 1;
  $form_state['item_count'] = array($field_name => count($_POST[$field_name]) + 1);
  $form_element = content_field_form($form, $form_state, $field);
  // Let other modules alter it.
  drupal_alter('form', $form_element, array(), 'content_add_more_js');

  // Add the new element at the right place in the (original, unbuilt) form.
  if (module_exists('fieldgroup') && ($group_name = _fieldgroup_field_get_group($type['type'], $field_name))) {
    $form[$group_name][$field_name] = $form_element[$field_name];
  }
  else {
    $form[$field_name] = $form_element[$field_name];
  }

  // Save the new definition of the form.
  $form_state['values'] = array();
  form_set_cache($form_build_id, $form, $form_state);

  // Build the new form against the incoming $_POST values so that we can
  // render the new element.
  $_POST[$field_name][$delta]['_weight'] = $delta;
  $form_state = array('submitted' => FALSE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $field_form = (!empty($group_name)) ? $form[$group_name][$field_name] : $form[$field_name];
  // We add a div around the new content to receive the ahah effect.
  $field_form[$delta]['#prefix'] = '<div class="ahah-new-content">'. (isset($field_form[$delta]['#prefix']) ? $field_form[$delta]['#prefix'] : '');
  $field_form[$delta]['#suffix'] = (isset($field_form[$delta]['#suffix']) ? $field_form[$delta]['#suffix'] : '') .'</div>';
  // Prevent duplicate wrapper.
  unset($field_form['#prefix'], $field_form['#suffix']);

  // If a newly inserted widget contains AHAH behaviors, they normally won't
  // work because AHAH doesn't know about those - it just attaches to the exact
  // form elements that were initially specified in the Drupal.settings object.
  // The new ones didn't exist then, so we need to update Drupal.settings
  // by ourselves in order to let AHAH know about those new form elements.
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';

  $output = theme('status_messages') . drupal_render($field_form) . $output_js;

  // Using drupal_json() breaks filefield's file upload, because the jQuery
  // Form plugin handles file uploads in a way that is not compatible with
  // 'text/javascript' response type.
  $GLOBALS['devel_shutdown'] =  FALSE;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}
