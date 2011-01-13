<?php
// $Id: node_form.inc,v 1.4.2.5 2011/01/03 20:45:19 ezrag Exp $


/**
 * @file
 * Signup-related code needed while editing a node.
 */

/**
 * Save signup-related information when a node is created or edited.
 *
 * This is a helper function invoked via signup_nodeapi(). If signups are
 * disabled, the record from {signup} (if any) is cleared. If the signup
 * administrator editing the node decided to remove all signup data, all the
 * records from the {signup_log} table for this node are also removed. This
 * function is also responsible for testing if the node has a start time and
 * if the autoclose period has already begun, in which case signups are
 * closed. If a new node is being saved, or an existing node is updated and is
 * newly-enabled for signups, the site-wide default signup settings are copied
 * into a record in the {signup} table for this node so that the node is
 * properly signup-enabled (these settings can be changed by visiting the
 * "Settings" subtab under the "Signups" tab at node/N/signups/settings).
 *
 * @param $node
 *   The node object given to signup_nodeapi().
 * @param $op
 *   The hook_nodeapi() operation, either 'insert' or 'update'.
 *
 * @return
 *   Nothing, this function is expected to update the database.
 *
 * @see signup_nodeapi()
 */
function signup_save_node($node, $op) {
  // See if a user is editing a node and disables signups for it.
  if ($op == 'update' && isset($node->signup_enabled)) {
    switch ($node->signup_enabled) {
      case 2: // Disabled, and delete {signup_log}, too
        db_query("DELETE FROM {signup_log} WHERE nid = %d", $node->nid);
        // No break, fall through and remove from {signup} too.
      case 0: // Disabled, but leave {signup_log} alone
        db_query("DELETE FROM {signup} WHERE nid = %d", $node->nid);
        // We're done.
        return;
    }
  }

  // If the form is configured to have signups enabled, or the form doesn't
  // include that information at all but the node type defaults to have
  // signups enabled (which would happen if a user without signup admin
  // permission created a node that defaulted to have signups enabled based on
  // the node type), see if we need to insert a new record into the {signup}
  // table for this node using the site-wide defaults.
  $needs_defaults = FALSE;
  if ((isset($node->signup_enabled) && $node->signup_enabled == 1) || (!isset($node->signup_enabled) && variable_get('signup_node_default_state_'. $node->type, 'disabled') == 'enabled_on')) {
    if ($op == 'insert') {
      $needs_defaults = TRUE;
    }
    else {
      // Updating -- see if we already have a record for this node.
      $has_record = db_result(db_query("SELECT nid FROM {signup} WHERE nid = %d", $node->nid));
      $needs_defaults = empty($has_record);
    }
  }

  if ($needs_defaults) {
    $values = db_fetch_array(db_query("SELECT forwarding_email, send_confirmation, confirmation_email, close_signup_limit, send_reminder, reminder_days_before, reminder_email FROM {signup} WHERE nid = 0"));
    $values[] = $node->nid;
    db_query("INSERT INTO {signup} (forwarding_email, send_confirmation, confirmation_email, close_signup_limit, send_reminder, reminder_days_before, reminder_email, nid) VALUES ('%s', %d, '%s', %d, %d, %d, '%s', %d)", $values);
  }
  db_query("UPDATE {signup} SET user_reg_form = %d WHERE nid = %d", $node->signup_user_reg, $node->nid);
  $node = node_load($node->nid);
  // We clear CCK's cache for this node due to an edge case bug:
  // http://drupal.org/node/605594.
  if (module_exists('content')) {
    cache_clear_all('content:'. $node->nid .':'. $node->vid, content_cache_tablename());
  }
  if (_signup_node_completed($node) && !empty($node->signup_status)) {
    // If this is an time-based node, and it's already past the close in
    // advance time (e.g. someone just changed the node start time), and
    // signups are still open, close them now.
    signup_close_signup($node->nid);
    drupal_set_message(t('%node_type start time is already past the signup close-in-advance time, signups now closed.', array('%node_type' => node_get_types('name', $node->type))));
  }
}

/**
 * Alters the node form to inject the appropriate per-node signup settings.
 */
function signup_alter_node_form(&$form, &$form_state, $form_id) {
  global $user;

  // Node is not saved but previewed (nid is empty).
  if (isset($form['#node']->build_mode) && $form['#node']->build_mode == NODE_BUILD_PREVIEW) {
    $node = $form['#node'];
  }
  // Load the node if it already exists.
  elseif (!empty($form['nid']['#value'])) {
    $node = node_load($form['nid']['#value']);
  }
  else {
    $node = NULL;
  }
  $node_type = $form['type']['#value'];

  $signup_type_default = variable_get('signup_node_default_state_'. $node_type, 'disabled');

  // If signups are possible, and the current user either has the global
  // 'administer all signups' permission or has the 'administer signups
  // for own content' permission and is creating new content or editing
  // their own content, add a fieldset for signup-related settings.

  // Signups are possible if they're not explicitly disallowed for this
  // node type, or if this node is already signup-enabled (in case an
  // admin erroneously marks a node-type to disallow signups when there
  // are already nodes of that type with signups enabled).
  $signups_possible = $signup_type_default != 'disabled' || (!empty($node) && !empty($node->nid) && !empty($node->signup));
  $admin_all = user_access('administer all signups');
  $admin_own = user_access('administer signups for own content') && (empty($node) || ($node->uid == $user->uid));
  if ($signups_possible && ($admin_all || $admin_own)) {
    $form['signup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Signup settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 30,
      '#group' => 'additional_settings',
    );

    // Figure out what the options should be.  If there are already
    // people signed-up for this node, we need a 3rd choice: disable
    // signups and remove all signup data.
    $has_signups = !empty($node) && db_result(db_query("SELECT COUNT(*) from {signup_log} WHERE nid = %d", $node->nid));
    $radio_options[1] = t('Enabled');
    if ($has_signups) {
      $radio_options[0] = t('Disabled, but save existing signup information');
      $radio_options[2] = t('Disabled, and remove all signup information') .' <strong>('. t('This can not be undone, use with extreme caution!') .')</strong>';
    }
    else {
      $radio_options[0] = t('Disabled');
    }

    // Figure out what the default selection for signups should be.
    if (isset($node->signup)) {
      $default_option = $node->signup;
    }
    else {
      $default_option = $signup_type_default == 'enabled_on' ? 1 : 0;
    }
    // Add the form element to toggle if signups are allowed.
    $form['signup']['signup_enabled'] = array(
      '#type' => 'radios',
      '#options' => $radio_options,
      '#default_value' => $default_option,
      '#description' => t('If enabled, you can control whether users may sign up by visiting the !signup_admin tab and toggling if signups are %open or %closed for this %node_type. Other signup-related settings can be defined at the !signup_settings tab.', array('!signup_admin' => !empty($node->signup) ? l(t('Signups: Administer'), 'node/'. $node->nid .'/signups/admin') : theme('placeholder', t('Signups: Administer')), '!signup_settings' => !empty($node->signup) ? l(t('Signups: Settings'), 'node/'. $node->nid .'/signups/settings') : theme('placeholder', t('Signups: Settings')), '%open' => t('open'), '%closed' => t('closed'), '%node_type' => node_get_types('name', $node_type))),
    );
    $form['signup']['signup_user_reg'] = array(
      '#type' => 'checkbox',
      '#title' => t('Users can sign up for this event from the user registration form.'),
      '#description' => t('If selected, new users will be able to sign up for this event when they register.'),
      '#default_value' => $node->signup_user_reg_form,
    );
  }
}
