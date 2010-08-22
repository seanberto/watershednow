<?php
// $Id: context_ui.admin.inc,v 1.1.2.7.2.12.2.6 2010/04/26 17:52:58 yhahn Exp $

/**
 * Context listing form. Allows for quick creation of new contexts.
 */
function context_ui_admin() {
  context_invalidate_cache();

  $form = array(
    '#theme' => 'context_ui_admin',
    '#contexts' => context_load(NULL, TRUE),
  );
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#size' => 15,
    '#element_validate' => array('context_ui_form_name_validate'),
  );
  $form['tag'] = array(
    '#title' => t('Tag'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
    '#size' => 15,
  );
  $form['submit'] = array(
    '#value' => t('Create new context'),
    '#type' => 'submit',
    '#submit' => array('context_ui_admin_submit'),
  );
  return $form;
}

/**
 * Context listing form submit handler.
 */
function context_ui_admin_submit(&$form, &$form_state) {
  ctools_include('export');
  $context = ctools_export_new_object('context');
  $context->name = $form_state['values']['name'];
  $context->tag = $form_state['values']['tag'];
  if (context_save($context)) {
    drupal_set_message(t('Saved context %title.', array('%title' =>  $context->name)));
  }
  else {
    drupal_set_message(t('Could not save context %title.', array('%title' =>  $context->name)), 'error');
  }
}

/**
 * Generates the omnibus context definition editing form.
 *
 * @param $op
 *   The type of form to build. Either "add", "view" or "edit"
 * @param $cid
 *   The db context identifier - required when $op == "edit"
 *
 * @return
 *   A Drupal form array.
 */
function context_ui_form(&$form_state, $op, $context = NULL) {
  // Set a context to prevent implementation of 
  // hook_context_reaction_fetch_alter() from running so we can edit the real
  // values of the context.
  context_set('context', 'no_alter', TRUE);
  ctools_include('export');

  $context = !$context ? ctools_export_new_object('context') : $context;
  switch ($op) {
    case 'add':
      drupal_set_title(t('Add a new context'));
      break;
    case 'edit':
      if ($context->export_type & EXPORT_IN_DATABASE) {
        drupal_set_title(t('Editing context %title', array('%title' => $context->name)));
      }
      else {
        drupal_set_title(t('Viewing context %title', array('%title' => $context->name)));
      }
      break;
    case 'clone':
      drupal_set_title(t('Cloning context %title', array('%title' => $context->name)));
      break;
  }

  $form = array(
    '#base' => 'context_ui_form',
    '#theme' => 'context_ui_form',
  );
  $form['context'] = array(
    '#type' => 'value',
    '#value' => $context,
  );

  // Core context definition
  $form['info'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
  );
  $form['info']['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => isset($context->name) ? $context->name : '',
    '#description' => t('Example: <code>blog_section</code>') .'<br/>'. t('A system name for this context. May only contain lowercase letters, dashes, underscores, and numbers.'),
    '#element_validate' => ($op === 'edit') ? array() : array('context_ui_form_name_validate'),
  );
  if ($op === 'edit') {
    $form['info']['name']['#disabled'] = TRUE;
    $form['info']['name']['#value'] = $context->name;
  }
  $form['info']['tag'] = array(
    '#title' => t('Tag'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
    '#default_value' => isset($context->tag) ? $context->tag : '',
    '#description' => t('Example: <code>theme</code>') .'<br/>'. t('A tag to group this context with others.'),
  );
  $form['info']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
    '#default_value' => isset($context->description) ? $context->description: '',
    '#description' => t('The description of this context definition.'),
  );

  // Condition mode
  $form['condition_mode'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($context->condition_mode) ? $context->condition_mode : FALSE,
    '#title' => t('Require all conditions'),
    '#description' => t('If checked, all conditions must be met for this context to be active. Otherwise, the first condition that is met will activate this context.')
  );

  // Condition plugin forms
  $form['conditions'] = array(
    '#theme' => 'context_ui_plugins',
    '#title' => t('Conditions'),
    '#description' => t('Trigger the activation of this context'),
    '#tree' => TRUE,
    'selector' => array(
      '#type' => 'select',
      '#options' => array(0 => '<'. t('Add a condition') .'>'),
      '#default_value' => 0,
    ),
    'state' => array(
      '#attributes' => array('class' => 'context-plugins-state'),
      '#type' => 'hidden',
    ),
    'plugins' => array('#tree' => TRUE),
  );
  $conditions = array_keys(context_conditions(TRUE));
  sort($conditions);
  foreach ($conditions as $condition) {
    if ($plugin = context_get_plugin('condition', $condition)) {
      $form['conditions']['plugins'][$condition] = array(
        '#tree' => TRUE,
        '#plugin' => $plugin,
        '#context_enabled' => isset($context->conditions[$condition]), // This flag is used at the theme layer.
        'values' => $plugin->condition_form($context),
        'options' => $plugin->options_form($context),
      );
      $form['conditions']['selector']['#options'][$condition] = $plugin->title;
    }
  }

  // Reaction plugin forms
  $form['reactions'] = array(
    '#theme' => 'context_ui_plugins',
    '#title' => t('Reactions'),
    '#description' => t('Actions to take when this context is active'),
    '#tree' => TRUE,
    'selector' => array(
      '#type' => 'select',
      '#options' => array(0 => '<'. t('Add a reaction') .'>'),
      '#default_value' => 0,
    ),
    'state' => array(
      '#attributes' => array('class' => 'context-plugins-state'),
      '#type' => 'hidden',
    ),
    'plugins' => array('#tree' => TRUE),
  );
  $reactions = array_keys(context_reactions(TRUE));
  sort($reactions);
  foreach ($reactions as $reaction) {
    if ($plugin = context_get_plugin('reaction', $reaction)) {
      $form['reactions']['plugins'][$reaction] = $plugin->options_form($context) + array(
        '#plugin' => $plugin,
        '#context_enabled' => isset($context->reactions[$reaction]), // This flag is used at the theme layer.
      );
      $form['reactions']['selector']['#options'][$reaction] = $plugin->title;
    }
  }

  // Buttons
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => $op === 'edit',
  );
  return $form;
}

/**
 * Provide a form to confirm one of the provided actions.
 */
function context_ui_confirm(&$form_state, $op = 'delete', $context) {
  $form = array();
  $form['context'] = array('#type' => 'value', '#value' => $context);
  $form['action'] = array('#type' => 'value', '#value' => $op);
  switch ($op) {
    case 'revert':
      $action = t('revert');
      $message = t('This action will permanently remove any customizations made to this context.');
      break;
    case 'delete':
      $action = t('delete');
      $message = t('This action will remove this context permanently from your site.');
      break;
    case 'disable':
      $action = t('disable');
      $message = '';
      break;
    case 'enable':
      $action = t('enable');
      $message = '';
      break;
  }
  $form = confirm_form($form,
    t('Are you sure you want to !action the context %title?', array('%title' => $context->name, '!action' => $action)),
    'admin/build/context',
    $message,
    drupal_ucfirst($action), t('Cancel')
  );
  return $form;
}

/**
 * Submit handler for the context_ui_confirm form.
 */
function context_ui_confirm_submit($form, &$form_state) {
  ctools_include('export');
  $context = $form_state['values']['context'];
  switch ($form_state['values']['action']) {
    case 'revert':
    case 'delete':
      context_delete($context);
      break;
    case 'disable':
      ctools_export_set_object_status($context);
      break;
    case 'enable':
      ctools_export_set_object_status($context, FALSE);
      break;
  }
  $form_state['redirect'] = 'admin/build/context';
}

/**
 * Page callback for import form. Switches form output to context form
 * if import submission has occurred.
 */
function context_ui_import_page() {
  if (!empty($_POST) && $_POST['form_id'] == 'context_ui_form') {
    return drupal_get_form('context_ui_form', 'add');
  }
  return drupal_get_form('context_ui_import');
}

/**
 * Import form. Provides simple helptext instructions and textarea for
 * pasting a context definition.
 */
function context_ui_import() {
  drupal_set_title(t('Import context'));
  $help = t('You can import a context definition by pasting the exported context object code into the field below.');
  $form = array();
  $form['help'] = array(
    '#type' => 'item',
    '#value' => $help,
  );
  $form['import'] = array(
    '#title' => t('Context Object'),
    '#type' => 'textarea',
    '#rows' => 10,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * Import form submit handler. Evaluates import code and transfers to
 * context definition form.
 */
function context_ui_import_submit($form, &$form_state) {
  $items = array();
  if ($import = $form_state['values']['import']) {
    ob_start();
    $context = eval($import);
    ob_end_clean();
  }
  if (is_object($context)) {
    if (isset($context->name) && $exists = context_load($context->name)) {
      drupal_set_message(t('A context with this name already exists. Please remove the existing context before importing this definition.'), 'error');
      $form_state['redirect'] = 'admin/build/context';
    }
    else {
      drupal_set_title(t('Add context'));
      $output = drupal_get_form('context_ui_form', 'add', (object) $context);
      print theme('page', $output);
      exit;
    }
  }
  else {
    drupal_set_message(t('An error occurred while importing. Please check your context definition.', 'error'));
    $form_state['redirect'] = 'admin/build/context';
  }
}

/**
 * Provides a form with an exported context definition for use in modules.
 *
 * @param $cid
 *   A context id.
 *
 * @return
 *   A FormAPI array.
 */
function context_ui_export(&$form_state, $context) {
  ctools_include('export');
  drupal_set_title(t('Export %title', array('%title' => $context->name)));
  return ctools_export_form($form_state, context_export_context($context), t('Export'));
}

/**
 * Validate context name values.
 */
function context_ui_form_name_validate($element, &$form_state) {
  // Check for string identifier sanity
  if (!preg_match('!^[a-z0-9_-]+$!', $element['#value'])) {
    form_set_error('name', t('The context name can only consist of lowercase letters, dashes, underscores, and numbers.'));
  }
  // Check for name collision
  else if ($exists = context_load($element['#value'])) {
    form_set_error('name', t('A context with this name already exists. Please choose another name or delete the existing context before creating a new one.'));
  }
}

/**
 * Produces a context object from submitted form values.
 *
 * @param $form
 *   A form array with submitted values
 *
 * @return
 *   A context object
 */
function context_ui_form_process($form) {
  ctools_include('export');
  $context = ctools_export_new_object('context');
  $context->name = isset($form['name']) ? $form['name'] : NULL;
  $context->description = isset($form['description']) ? $form['description'] : NULL;
  $context->tag = isset($form['tag']) ? $form['tag'] : NULL;
  $context->condition_mode = isset($form['condition_mode']) ? $form['condition_mode'] : NULL;
  $context->conditions = array();
  $context->reactions = array();
  if (!empty($form['conditions'])) {
    $enabled = explode(',', $form['conditions']['state']);
    foreach ($form['conditions']['plugins'] as $condition => $values) {
      if (in_array($condition, $enabled, TRUE) && ($plugin = context_get_plugin('condition', $condition))) {
        if (isset($values['values'])) {
          $context->conditions[$condition]['values'] = $plugin->condition_form_submit($values['values']);
        }
        if (isset($values['options'])) {
          $context->conditions[$condition]['options'] = $plugin->options_form_submit($values['options']);
        }
        if (context_empty($context->conditions[$condition]['values'])) {
          unset($context->conditions[$condition]);
        }
      }
    }
  }
  if (!empty($form['reactions'])) {
    $enabled = explode(',', $form['reactions']['state']);
    foreach ($form['reactions']['plugins'] as $reaction => $values) {
      if (in_array($reaction, $enabled, TRUE) && ($plugin = context_get_plugin('reaction', $reaction))) {
        if (isset($values)) {
          $context->reactions[$reaction] = $plugin->options_form_submit($values);
        }
        if (context_empty($context->reactions[$reaction])) {
          unset($context->reactions[$reaction]);
        }
      }
    }
  }
  return $context;
}

/**
 * Submit handler for main context_ui form.
 */
function context_ui_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#id']) {
    // Send user to delete confirmation page
    case 'edit-delete':
      if (isset($form_state['values']['context']->name)) {
        $form_state['redirect'] = "admin/build/context/list/{$form_state['values']['context']->name}/delete";
      }
      return;
    // Process form values and save and/or update the context in the db
    case 'edit-submit':
      $context = context_ui_form_process($form_state['values']);
      $result = context_save($context);
      if ($result) {
        drupal_set_message(t('Saved context %title.', array('%title' =>  $context->name)));
      }
      else {
        drupal_set_message(t('Could not save context %title.', array('%title' =>  $context->name)), 'error');
      }
      break;
  }
  $form_state['redirect'] = 'admin/build/context';
}
