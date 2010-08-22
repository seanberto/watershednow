<?php
// $Id: boxes.admin.inc,v 1.5.2.2 2010/06/21 19:30:07 jmiccolis Exp $

/**
 * Generate form for creating new boxes.
 */
function boxes_add_form($form_state, $plugin_key) {
  $form = boxes_box_form(boxes_create($plugin_key));
  $form['delta'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('Lowercase letters, numbers and underscores only'),
    '#required' => true,
    '#element_validate' => array('boxes_validate_delta'),
    '#size' => 32,
    '#maxlength' => 32,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Validate handler for box delta.
 */
function boxes_validate_delta($element, &$form_state) {
  if (!preg_match('!^[a-z0-9_]+$!', $element['#value'])) {
    form_error($element, t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
  if ((strpos($element['#value'], 'add__') === 0) || boxes_load($element['#value'])) {
    form_error($element, t('The machine-readable name is already taken.'));
  }
}

/**
 * Submit handler for box_add_form.
 */
function boxes_add_form_submit($form, &$form_state) {
  $box = boxes_create($form_state['values']['plugin_key'], $form_state['values']);
  $box->save();
  drupal_set_message(t('%name has been created.', array('%name' => $box->description)));
  $form_state['redirect'] = 'admin/build/block';
}

/**
 * Box deletion form.
 */
function boxes_delete_form($form_state, $box) {
  $form['delta'] = array('#type' => 'hidden', '#value' => $box->delta);
  if (($box->export_type & EXPORT_IN_DATABASE) && ($box->export_type & EXPORT_IN_CODE)) {
    return confirm_form($form, t('Are you sure you want to revert the block %name?', array('%name' => $box->title)), 'admin/build/block', '', t('Revert'), t('Cancel'));
  }
  elseif (!($box->export_type & EXPORT_IN_CODE)) {
    return confirm_form($form, t('Are you sure you want to delete the block %name?', array('%name' => $box->title)), 'admin/build/block', '', t('Delete'), t('Cancel'));
  }
  drupal_not_found();
  die;
}

/**
 * Submit handler for boxes_delete_form
 */
function boxes_delete_form_submit($form, &$form_state) {
  boxes_load($form_state['values']['delta'])->delete();
  $form_state['redirect'] = 'admin/build/block';
}

/**
 * Form for in page editing.
 */
function boxes_box_inline_form(&$form_state, $box, $block) {
  $form = boxes_box_form($box);

  // Hide form labels, helptext to keep things tight.
  unset($form['description']['#description']);
  unset($form['title']['#description']);
  unset($form['body_field']['body']['#description']);

  $form['#attributes'] = array('class' => 'boxes-box-inline-form');
  $form['#new_box'] = $box->new;
  $form['delta'] = array('#type' => 'value', '#value' => $box->delta);
  $form['content']['#value'] = $block['content'];

  // Add in the current path to be used when AHAH rendering the block.
  $form['path'] = array('#type' => 'value', '#value' => $_GET['q']);

  // Ensure our path gets rewritten. We don't use url() here because we're
  // not interested in rewrites to parts of the request other than $_GET['q'].
  $path = 'boxes/ajax-submit';
  if (function_exists('custom_url_rewrite_outbound')) {
    $original_path = $path;
    $options = array();
    custom_url_rewrite_outbound($path, $options, $original_path);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('boxes_box_inline_form_submit'),
    '#attributes' => array('class' => 'boxes-box-submit'),

    // AHAH stack: We need to assign our submit button its own ID as auto
    // assignment will quickly lead to a situation where our AJAX form button
    // has a different ID from the original.
    '#id' => "edit-submit-boxes-box-{$box->delta}",
    '#ahah' => array(
      'path' => $path,
      'wrapper' => "block-boxes-{$box->delta}",
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  if (!$box->new) {
    $export_in_code = ($box->export_type == EXPORT_IN_CODE);
    $export_in_database = ($box->export_type == EXPORT_IN_DATABASE);
    if (!$export_in_code) {
      $path = 'boxes/ajax-delete';
      if (function_exists('custom_url_rewrite_outbound')) {
        $original_path = $path;
        $options = array();
        custom_url_rewrite_outbound($path, $options, $original_path);
      }
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => $export_in_database ? t('Delete') : t('Revert'),
        '#submit' => array('boxes_box_inline_form_delete'),
        '#attributes' => array('class' => $export_in_database ? 'boxes-box-delete' : 'boxes-box-revert'),
        '#disabled' => $export_in_code,

        // See comment above.
        '#id' => "edit-delete-boxes-box-{$box->delta}",
        '#ahah' => array(
          'path' => $path,
          'wrapper' => "block-boxes-{$box->delta}",
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
    }
    $form['controls'] = array('#value' =>
      theme('links', array(
        'edit' => array('title' => t('Edit'), 'href' => $_GET['q']),
        'cancel' => array('title' => t('Cancel'), 'href' => $_GET['q']),
      )),
    );
  }
  $form['#theme'] = 'boxes_box_inline_form';
  return $form;
}

/**
 * Submit handler for the inline form.
 */
function boxes_box_inline_form_submit($form, $form_state) {
  $box = boxes_create($form_state['values']['plugin_key'], $form_state['values']);

  if (module_exists('spaces') && $space = spaces_get_space()) {
    $space->controllers->boxes->set($box->delta, $box);
  }
  else {
    $box->save();
  }
}

/**
 * Submit handler for the inline deletion.
 */
function boxes_box_inline_form_delete($form, $form_state) {
  $delta = $form_state['values']['delta'];
  if (module_exists('spaces')) {
    if ($space = spaces_get_space()) {
      $space->controllers->boxes->del($delta);
    }
    else {
      boxes_load($delta)->delete();
      db_query("DELETE FROM {spaces_overrides} WHERE object_type = '%s' AND object_id = '%s'", 'boxes', $form_state['values']['delta']);
    }
  }
  else {
    boxes_load($delta)->delete();
  }
}

/**
 * AJAX submit handler callback.
 */
function boxes_box_ajax_submit() {
  // Set the header right away. This will inform any players in the stack
  // that we are in the middle of responding to an AJAX request.
  drupal_set_header('Content-Type: text/javascript; charset=utf-8');

  $cached_form_state = array();
  $files = array();

  $cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state);
  if ($cached_form['form_token']['#default_value'] == $_POST['form_token']) {
    $form_state = array('values' => $_POST);
    foreach (element_children($cached_form) as $elem) {
      if ($cached_form[$elem]['#type'] === 'value' && isset($cached_form[$elem]['#value'])) {
        $form_state['values'][$elem] = $cached_form[$elem]['#value'];
      }
    }
    boxes_box_inline_form_submit($cached_form, $form_state);

    // Spoof the $_GET request and go so far as executing the page handler in
    // case the block relies on some contextual information from the page build.
    if (isset($form_state['values']['path'])) {
      $_GET['q'] = $form_state['values']['path'];
      menu_execute_active_handler($_GET['q']);
    }

    $output = boxes_box_render_block($form_state['values']['delta']);
    echo drupal_to_js(array('status' => 1, 'data' => $output));
    exit;
  }
  echo drupal_to_js(array('status' => 0, 'data' => 'Error submitting form.'));
  exit;
}

/**
 * AJAX delete handler callback.
 */
function boxes_box_ajax_delete() {
  // Set the header right away. This will inform any players in the stack
  // that we are in the middle of responding to an AJAX request.
  drupal_set_header('Content-Type: text/javascript; charset=utf-8');

  $cached_form_state = array();
  $files = array();

  $cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state);
  if ($cached_form) {
    $form_state = array('values' => $_POST);
    foreach (element_children($cached_form) as $elem) {
      if ($cached_form[$elem]['#type'] === 'value' && isset($cached_form[$elem]['#value'])) {
        $form_state['values'][$elem] = $cached_form[$elem]['#value'];
      }
    }
    boxes_box_inline_form_delete($cached_form, $form_state);

    // Check whether box is exported to code and render it if necessary.
    $output = '';
    if ($box = boxes_load($form_state['values']['delta'])) {
      $output = boxes_box_render_block($form_state['values']['delta']);
    }
    drupal_json(array('status' => 1, 'data' => $output));
    exit;
  }
  drupal_json(array('status' => 0, 'data' => ''));
  exit;
}

/**
 * Helper function for rendering a boxes block identified by its $delta.
 */
function boxes_box_render_block($delta) {
  $block = new StdClass();
  $block->module = 'boxes';
  $block->delta = $delta;
  $block->bid = "boxes-$delta";
  $block_view = module_invoke('boxes', 'block', 'view', $delta);
  if (!empty($block_view)) {
    foreach ($block_view as $k => $v) {
      $block->{$k} = $v;
    }
  }
  return theme('block', $block);
}

/**
 * Theme function for the form.
 */
function theme_boxes_box_inline_form($form) {
  $output = '';
  $output .= '<div class="boxes-box-content">' . drupal_render($form['content']) . '</div>';
  $output .= isset($form['controls']) ? '<div class="boxes-box-controls">' . drupal_render($form['controls']) . '</div>' : '';
  $output .= '<div class="boxes-box-form">' . drupal_render($form) . '</div>';
  $class = !empty($form['#new_box']) ? 'boxes-box-editing' : '';
  return "<div id='boxes-box-inline-{$form['delta']['#value']}' class='boxes-box-inline {$class}'>{$output}</div>";
}

/**
 * Theme function for AJAX block script tag placeholders.
 */
function theme_boxes_script_placeholder($text = '') {
  $message = t('Save changes to view this block.');
  return "<div class='script-placeholder'><strong>{$text}</strong><div class='description'>{$message}</div></div>";
}
