<?php
// $Id: node_settings.inc,v 1.1.2.4 2009/09/21 05:23:09 dww Exp $


/**
 * @file
 * Code related to the per-node signup settings form.
 */

/**
 * Returns the form for the per-node signup settings.
 *
 * This is shared by the settings page and the node edit page.
 *
 * @param $node
 *   The fully loaded node object if we've got it.
 * @param $node_type
 *   The type of the node.  When creating new content, the caller can know the
 *   node type, even if $node is NULL.
 * @param $has_date
 *   Boolean flag indicating if this node (or site) has signup-aware
 *   date functionality, which is required for reminder emails to be in
 *   the form.
 * @param $include_buttons
 *   Boolean flag indicating if the form should include its own submit buttons.
 *
 * @return
 *   The form array for the per-node signup settings.
 *
 */
function signup_node_settings_form($form_state, $node = NULL, $node_type = NULL, $has_date = FALSE, $include_buttons = FALSE) {
  if (module_exists('token')) {
    $signup_token_description = t('Supported string substitutions: %node_title, %node_url, %node_start_time, %user_name, %user_mail, %user_signup_info (additional information from the signup form), %cancel_signup_url (access to this link is denied to users without the "%cancel_own_signups" permission), and any tokens in the %replacement_tokens list.', array('%replacement_tokens' => t('Replacement tokens'), '%cancel_own_signups' => t('cancel own signups')));
  }
  else {
    $signup_token_description = t('Supported string substitutions: %node_title, %node_url, %node_start_time, %user_name, %user_mail, %user_signup_info (additional information from the signup form), and %cancel_signup_url (access to this link is denied to users without the "%cancel_own_signups" permission).', array('%cancel_own_signups' => t('cancel own signups')));
  }

  // Load the default admin form data for new nodes.
  if (!$node || !$node->signup) {
    $result = db_fetch_object(db_query("SELECT * FROM {signup} WHERE nid = 0"));
    $node->signup_forwarding_email = $result->forwarding_email;
    $node->signup_send_confirmation = $result->send_confirmation;
    $node->signup_confirmation_email = $result->confirmation_email;
    $node->signup_send_reminder = $result->send_reminder;
    $node->signup_reminder_days_before = $result->reminder_days_before;
    $node->signup_reminder_email = $result->reminder_email;
    $node->signup_close_signup_limit = $result->close_signup_limit;
  }

  $form['signup_forwarding_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Send signups to'),
    '#default_value' => $node->signup_forwarding_email,
    '#size' => 40, '#maxlength' => 64,
    '#description' => t('Email address where notification of new signups will be sent. Leave blank for no notifications.'),
  );
  $form['signup_send_confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send confirmation'),
    '#default_value' => $node->signup_send_confirmation,
  );
  $form['signup_confirmation_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Confirmation email'),
    '#default_value' => $node->signup_confirmation_email,
    '#cols' => 40, '#rows' => 6,
    '#description' => t('Email sent to user upon signup. !token_description', array('!token_description' => $signup_token_description)),
  );
  if (module_exists('token')) {
    module_load_include('inc', 'signup', 'includes/token_help');
    _signup_token_help($form, 'signup_confirmation_token_fieldset');
  }

  if ($has_date) {
    // Define a sub-tree to wrap the next 2 form elements together in an
    // inline div for better display.
    $form['signup_reminder'] = array(
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
    );
    $form['signup_reminder']['signup_send_reminder'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send reminder'),
      '#default_value' => $node->signup_send_reminder,
    );
    $options = array();
    for ($i = 1; $i <= 60; $i++) {
      $options[$i] = $i;
    }
    $node_type_name = isset($node_type) ? node_get_types('name', $node_type) : '';
    $form['signup_reminder']['signup_reminder_days_before'] = array(
      '#type' => 'select',
      '#default_value' => $node->signup_reminder_days_before,
      '#options' => $options,
      '#suffix' => !empty($node_type_name) ? t('day(s) before this %node_type', array('%node_type' => $node_type_name)) : t('day(s) before start time'),
    );
    $form['signup_reminder_email'] = array(
      '#type' => 'textarea',
      '#title' => t('Reminder email'),
      '#default_value' => $node->signup_reminder_email,
      '#cols' => 40, '#rows' => 6,
      '#description' =>  !empty($node_type_name) ? t('Email sent to user as a reminder before the %node_type starts. !token_description', array('%node_type' => $node_type_name, '!token_description' => $signup_token_description)) : t('Email sent to user as a reminder before the start time. !token_description', array('!token_description' => $signup_token_description)),
    );
    if (module_exists('token')) {
      module_load_include('inc', 'signup', 'includes/token_help');
      _signup_token_help($form, 'signup_reminder_token_fieldset');
    }
  }

  $form['signup_close_signup_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Signup limit'),
    '#default_value' => $node->signup_close_signup_limit,
    '#size' => 4, '#maxlength' => 8,
    '#description' => t('Maximum number of users who can sign up before signups are automatically closed. If set to 0, there is no limit.'),
    '#prefix' => '<div id="signup-limit">',
    '#suffix' => '</div>',
  );
  $form['signup'] = array('#type' => 'hidden', '#value' => 1);

  if ($include_buttons) {
    $form['#node'] = $node;
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
    $form['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
    );
    $form['#submit'][] = 'signup_node_settings_form_submit';
  }

  return $form;
}

/**
 * Page callback for the node/N/signups/settings subtab.
 */
function signup_node_settings_page($node) {
  $node_scheduler = _signup_get_node_scheduler($node);
  $node_has_date = $node_scheduler != 'none';
  return drupal_get_form('signup_node_settings_form', $node, $node->type, $node_has_date, TRUE);
}

/**
 * Submit handler for the per-node signup settings form.
 *
 * @param $form_id
 *   The ID of the form being submitted.
 * @param $form_values
 *   The constructed form values array of the submitted form.
 */
function signup_node_settings_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    // If we're resetting, we just want to grab the site-wide defaults.
    $values = db_fetch_array(db_query("SELECT forwarding_email, send_confirmation, confirmation_email, close_signup_limit, send_reminder, reminder_days_before, reminder_email FROM {signup} WHERE nid = 0"));
  }
  else {
    // Populate $values from $form_state.
    foreach (array('forwarding_email', 'send_confirmation', 'confirmation_email', 'close_signup_limit') as $setting) {
      $values[$setting] = $form_state['values']["signup_$setting"];
    }
    // If we're dealing with a node that doesn't have a start time, these
    // fields are missing from the signup settings form, so we can't assume
    // they're defined.
    $values['send_reminder'] = isset($form_state['values']['signup_send_reminder']) ? $form_state['values']['signup_send_reminder'] : 0;
    $values['reminder_days_before'] = isset($form_state['values']['signup_reminder_days_before']) ? $form_state['values']['signup_reminder_days_before'] : 0;
    $values['reminder_email'] = isset($form_state['values']['signup_reminder_email']) ? $form_state['values']['signup_reminder_email'] : '';
  }

  // Either way, we want to make sure we're updating the values for the
  // current node, not nid 0...
  $node = $form['#node'];
  $values[] = $node->nid;
  db_query("UPDATE {signup} SET forwarding_email = '%s', send_confirmation = %d, confirmation_email = '%s', close_signup_limit = %d, send_reminder = %d, reminder_days_before = %d, reminder_email = '%s' WHERE nid = %d", $values);

  // See if the limit changed, and if so, take any necessary action.
  if ($node->signup_close_signup_limit != $form_state['values']['signup_close_signup_limit']) {
    $node->signup_close_signup_limit = $form_state['values']['signup_close_signup_limit'];
    $node->signup_effective_total = db_result(db_query("SELECT SUM(count_towards_limit) FROM {signup_log} WHERE nid = %d", $node->nid));
    _signup_check_limit($node, 'limit');
  }

  if ($op == t('Reset to defaults')) {
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    drupal_set_message(t('The configuration options have been saved.'));
  }
}

