<?php
// $Id: signup_confirm_email.inc,v 1.1.2.3 2009/09/21 06:56:40 dww Exp $

/**
 * @file
 * Code to confirm the e-mail during signups. Since this code is only needed
 * when building or submitting the signup form (or editing an existing
 * signup), it all lives in this include file and is only loaded when needed.
 */


/**
 * Alter the signup form to add the e-mail confirmation functionality.
 */
function signup_confirm_email_alter_signup_form(&$form, &$form_state, $form_id) {
  global $user;

  if ($form_id == 'signup_edit_form') {
    $signup = $form['#signup'];

    // Only display the email confirmation fields if an authenticated user is
    // editing their own signup (anonymous signups already provide an email,
    // and we shouldn't confirm email when an admin is editing another signup).
    if (empty($user->uid) || $user->uid != $signup->uid) {
      return;
    } 
    
    // Ensure the user has permission to edit their own signups, or the rest
    // of this is wasted effort for a form that can't be submitted.
    $node = node_load($signup->nid);
    $admin = _signup_menu_access($node, 'admin');
    $own = !empty($user->uid) && $user->uid == $signup->uid;
    $can_edit = $admin || (user_access('edit own signups') && $own);
    if (!$can_edit) {
      return;
    }
  }

  // We only need to do this for authenticated users signing up
  // themselves. We already collect the (presumably current) e-mail for
  // "anonymous" signups. If an administrator is signing up another user,
  // there's no reason to include the e-mail confirmation field, either.
  if (!empty($user->uid) && empty($form['collapse']['signup_user_form']['signup_username'])) {
    drupal_add_js(drupal_get_path('module', 'signup_confirm_email') . '/signup_confirm_email.js');
    $email = array(
      '#weight' => -1,
    );
    $email['email_address'] = array(
      '#title' => t('E-mail address'),
      '#type' => 'textfield',
      '#default_value' => $user->mail,
      '#size' => 40,
    );
    $email['email_confirm'] = array(
      '#title' => t('Update e-mail address in user profile'),
      '#type' => 'checkbox',
      '#default_value' => 0,
      '#description' => t('You must confirm any changes to the e-mail address stored in your user profile by selecting this checkbox.'),
      // This div is used to hide the checkbox on page load when JS is
      // enabled, and to reveal it once the 'email_address' field is edited.
      '#prefix' => '<div class="js-hide" id="signup-confirm-email-checkbox">',
      '#suffix' => '</div>',
    );
    if ($form_id == 'signup_form') {
      $form['collapse']['email'] = $email;
      $form['#submit'][] = 'signup_email_confirm_submit';
    }
    elseif ($form_id == 'signup_edit_form') {
      $form['elements']['email'] = $email;
      $form['elements']['header']['#weight'] = -2;
      $form['elements']['save']['#submit'][] = 'signup_email_confirm_submit';
    }
    $form['#validate'][] = 'signup_email_confirm_validate';
  }
}

/**
 * Validation callback for the signup form.
 */
function signup_email_confirm_validate($form, $form_state) {
  global $user;
  $error = FALSE;
  if (!empty($form_state['values']['email_address'])) {
    if (!valid_email_address($form_state['values']['email_address'])) {
      form_set_error('email_address', t('Invalid e-mail address'));
    }
    if ($form_state['values']['email_address'] != $user->mail && empty($form_state['values']['email_confirm'])) {
      form_set_error('email_confirm', t('Please confirm that you wish to save this new e-mail address into your user profile.'));
      $error = TRUE;
    }
  }
  // Add a JS setting for if the checkbox has a validation error, in which
  // case we display it, even if the 'E-mail address' field isn't re-edited.
  drupal_add_js(array('signupConfirmEmailCheckboxError' => $error), 'setting');
}

/**
 * Submit callback for the signup form.
 *
 * This checks if the e-mail address is being changed. If not, it completes
 * the normal signup form workflow. However, if the e-mail address is changed
 * from the one stored in the profile, the user is redirected to a
 * confirmation form to verify they want to update the e-mail address in their
 * profile before the signup is completed.
 */
function signup_email_confirm_submit($form, &$form_state) {
  global $user;
  if (!empty($form_state['values']['email_confirm']) && 
      !empty($form_state['values']['email_address'])) {
    if ($form_state['values']['email_address'] != $user->mail) {
      // Update the user's e-mail address in their profile.
      $user->mail = $form_state['values']['email_address'];
      user_save($user, array('mail' => $user->mail));
      drupal_set_message(t('Updated the e-mail address in your profile to %new_address.', array('%new_address' => $user->mail)));
    }
  }
}

