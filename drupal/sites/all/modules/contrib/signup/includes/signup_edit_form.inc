<?php
// $Id: signup_edit_form.inc,v 1.1.2.6 2009/09/19 00:33:42 dww Exp $


/**
 * @file
 * Code for the form to edit existing signups.
 */

/**
 * Build the form for editing existing signups.
 *
 * @param $form_state
 *   The state of the form to build (not currently used).
 * @param $signup
 *   The fully-loaded signup object with data about the signup to edit.
 * @param $type
 *   The type of signup edit form to render, can be 'node', 'tab', or 'admin'.
 *
 * @return
 *   The FAPI form array for the signup edit form.
 */
function signup_edit_form($form_state, $signup, $type) {
  global $user;
  $form = array();

  $form['#signup'] = $signup;
  $node = node_load($signup->nid);

  // Check permissions.
  $admin = _signup_menu_access($node, 'admin');
  $own = !empty($user->uid) && $user->uid == $signup->uid;
  $can_cancel = $admin || (user_access('cancel own signups') && $own);
  $can_edit = $admin || (user_access('edit own signups') && $own);

  if ($type == 'admin') {
    $title = t("Information for !user's signup to !node", array('!user' => _signup_get_username($signup, TRUE), '!node' => l($node->title, 'node/'. $node->nid)));
  }
  else {
    $title = t('Your signup information');
  }

  if ($type == 'node') {
    $form['elements'] = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('signup_fieldset_collapsed', 1),
    );
  }
  else {
    $form['elements'] = array();
    $form['elements']['header'] = array(
      '#type' => 'markup',
      '#value' => $title,
      '#prefix' => '<h4>',
      '#suffix' => '</h4>',
    );
  }

  if (!empty($signup->anon_mail)) {
    $form['elements']['signup_anon_mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#default_value' => $signup->anon_mail,
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => TRUE,
    );
    $form['#validate'][] = 'signup_validate_anon_mail';
  }

  if ($admin) {
    $options = array();
    if (1|| !isset($signup->attended)) {
      $options[-1] = t('- Not recorded -');
    }
    $options[1] = theme('signup_attended_text', 1);
    $options[0] = theme('signup_attended_text', 0);
    $form['elements']['attended'] = array(
      '#type' => 'select',
      '#title' => t('Attendance'),
      '#default_value' => isset($signup->attended) ? $signup->attended : -1,
      '#options' => $options
    );
  }

  // Build the themed signup form for this site and include that.
  $site_form = theme('signup_user_form', $node);
  $form_data = unserialize($signup->form_data);

  // This is sort of a hack, but we don't support nested arrays for the custom
  // signup form anyway, so it works for now.  Obviously all this will change
  // with signup_fields and friends, but for now it works.
  foreach ($form_data as $key => $value) {
    if (!empty($site_form['signup_form_data'][$key])) {
      $site_form['signup_form_data'][$key]['#default_value'] = $value;
      if (!$can_edit) {
        // If they can't edit, mark all the fields as disabled.
        $site_form['signup_form_data'][$key]['#disabled'] = TRUE;
      }
    }
  }
  $form['elements'] += $site_form;

  // Add the appropriate buttons based on permissions.
  if ($can_edit) {
    $form['elements']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('signup_edit_form_save_submit'),
    );
  }
  if ($can_cancel) {
    if (isset($_REQUEST['destination'])) {
      $destination = drupal_get_destination();
    }
    else {
      // If there's no destination already set, redirect to the node.
      $destination = 'destination='. urlencode("node/$signup->nid");
    }
    $signup_token = signup_get_token($signup->sid, 'cancel');
    $form['elements']['cancel-signup'] = array(
      '#type' => 'markup',
      '#value' => l(t('Cancel signup'), "signup/cancel/$signup->sid/$signup_token", array('query' => $destination)),
    );
  }

  return $form;
}

/**
 * Validation callback when editing the anonymous email for an existing signup.
 */
function signup_validate_anon_mail($form, &$form_state) {
  $mail = $form_state['values']['signup_anon_mail'];
  if (!valid_email_address($mail)) {
    form_set_error('signup_anon_mail', t('The e-mail address %mail is not valid.', array('%mail' => $mail)));
  }
}

/**
 * Submit callback when saving changes to an existing signup.
 */
function signup_edit_form_save_submit($form, $form_state) {
  $signup = $form['#signup'];
  if (!empty($form_state['values']['signup_form_data'])) {
    $signup->form_data = $form_state['values']['signup_form_data'];
  }

  // If the form contains an e-mail address for an anonymous signup, save it.
  if (!empty($form_state['values']['signup_anon_mail'])) {
    $signup->anon_mail = $form_state['values']['signup_anon_mail'];
  }

  // If the form contains attendance info, save it.
  if (isset($form_state['values']['attended'])) {
    if ($form_state['values']['attended'] == -1) {
      unset($signup->attended);
    }
    else {
      $signup->attended = $form_state['values']['attended'];
    }
  }

  // Invoke hook_signup_data_alter() to let other modules change this.
  drupal_alter('signup_data', $signup, $form_state['values']);

  // Update the signup in the {signup_log} table.
  signup_save_signup($signup);

  // Because drupal_write_record() doesn't gracefully handle columns that can
  // be NULL, if the attendence was cleared out by this edit, we need to
  // manually set the DB record to NULL here.
  if (!isset($signup->attended)) {
    db_query("UPDATE {signup_log} SET attended = NULL WHERE sid = %d", $signup->sid);
  }

  drupal_set_message(t('Signup information updated.'));
}

/**
 * Page handler for the administrator page to edit an existing signup.
 *
 * @param $signup
 *   The fully-loaded signup object to edit.
 *
 * @return
 *   The HTML to use for the signup edit page.
 */
function signup_edit_page($signup) {
  drupal_set_title(t('Edit signup'));
  return drupal_get_form('signup_edit_form', $signup, 'admin');
}

