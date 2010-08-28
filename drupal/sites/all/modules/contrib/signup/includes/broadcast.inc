<?php
// $Id: broadcast.inc,v 1.1.2.3 2009/08/14 23:26:10 dww Exp $


/**
 * @file
 * Code related to sending signup broadcast messages.
 */

/**
 * Form builder for the signup broadcast form.
 *
 * @param $node
 *   The node that the broadcast form is being attached to.
 */
function signup_broadcast_form($form_state, $node) {
  $signups = signup_get_signups($node->nid);
  if (empty($signups)) {
    $form['no_users'] = array(
      '#value' => t('No users have signup up for this %node_type.', array('%node_type' => node_get_types('name', $node->type))),
    );
    return $form;
  }

  $tokens = array('%node_title', '%node_url', '%user_mail', '%user_name', t('%cancel_signup_url (access to this link is denied to users without the "cancel own signups" permission)'));
  if (_signup_get_node_scheduler($node) != 'none') {
    $tokens = array_merge(array('%node_start_time'), $tokens);
  }
  if (module_exists('token')) {
    $token_text = t('Supported string substitutions: %tokens, and any tokens in the %replacement_tokens list.', array('%tokens' => implode(', ', $tokens), '%replacement_tokens' => t('Replacement tokens')));
  }
  else {
    $token_text = t('Supported string substitutions: %tokens.', array('%tokens' => implode(', ', $tokens)));
  }

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message body'),
    '#required' => TRUE,
    '#description' => t('Body of the email message you wish to send to all users who have signed up for this %node_type. !token_description', array('%node_type' => node_get_types('name', $node->type), '!token_description' => $token_text)),
    '#rows' => 10,
  );

  if (module_exists('token')) {
    module_load_include('inc', 'signup', 'includes/token_help');
    _signup_token_help($form, 'message_tokens_fieldset');
  }

  $form['copy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send yourself a copy.'),
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  global $user;
  if (user_access('administer site configuration')) {
    $form['from'] = array(
      '#type' => 'textfield',
      '#title' => t('From'),
      '#required' => TRUE,
      '#default_value' => $user->mail,
      '#weight' => '-10',
    );
  }
  else {
    $form['from'] = array(
      '#value' => t('This message will be sent from: %from', array('%from' => $user->mail)),
      '#pre' => '<strong>',
      '#post' => '</strong>',
    );
  }
  return $form;
}

/**
 * Submit callback for the signup broadcast form.
 *
 * @param $form
 *   The broadcast form being submitted.
 * @param $form_state
 *   The state of the submitted form (including values).
 *
 * @see signup_send_broadcast()
 */
function signup_broadcast_form_submit($form, &$form_state) {
  global $user;
  if (user_access('administer site configuration')) {
    $from = $form_state['values']['from'];
  }
  else {
    $from = $user->mail;
  }
  signup_send_broadcast($form_state['values']['nid'], $form_state['values']['subject'], $form_state['values']['message'], $from, !empty($form_state['values']['copy']));
}

/**
 * Send an email message to users who have signed up to a node.
 *
 * @param $nid
 *   Node ID of the signup node to send a broadcast about.
 * @param $subject
 *   The subject of the broadcast message to send.
 * @param $body
 *   The body of the broadcast message to send.
 * @param $from
 *   The from address to use when sending the broadcast message.
 * @param $copy
 *   Optional boolean indicating if the current user should get a copy.
 * @param $signups
 *   Optional array of signup objects to send the broadcast to.  Each object
 *   in the array should at least contain the following fields:
 *   - "sid": The unique signup ID
 *   - "mail" or "anon_mail": E-mail address to send to depending on if it's
 *     an authenticated or anonymous signup.
 *   - "name": Username of an authenticated signup.
 *   - "form_data": The optional custom signup form data for this signup.
 *   - "language": The user's prefered language (if known).
 *
 * @see signup_get_signup()
 * @see _signup_get_email()
 * @see _signup_get_email_tokens()
 */
function signup_send_broadcast($nid, $subject, $body, $from, $copy = FALSE, $signups = array()) {
  global $user;
  $send_all = FALSE;
  // If the parameter isn't defined, grab all users who signed up to the node.
  if (empty($signups)) {
    $signups = signup_get_signups($nid);
    $send_all = TRUE;
  }
  // Make sure that signup_get_signups() returned something for this node.
  if (!empty($signups)) {
    $node = node_load($nid);
    foreach ($signups as $signup) {
      $user_mail = _signup_get_email($signup);
      $params = array(
        'subject' => $subject,
        'body' => $body,
        'node' => $node,
        'signup' => $signup,
      );
      if (module_exists('token')) {
        $params['body'] = token_replace_multiple($params['body'], array('node' => $node, 'signup' => $signup, 'global' => NULL));
      }
      $language = user_preferred_language($signup);
      drupal_mail('signup', 'signup_broadcast_mail', $user_mail, $language, $params, $from);
      watchdog('signup', 'Broadcast email for %title sent to %email.', array('%title' => $node->title, '%email' => $user_mail), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
    }
    if ($copy) {
      $sender_email = _signup_get_email($user);
      $signup_tokens = _signup_get_email_tokens($node, $user);
      $message = strtr($body, $signup_tokens);
      if (module_exists('token')) {
        // If the token.module is enabled, also handle any tokens it provides.
        $message = token_replace_multiple($message, array('node' => $node, 'signup' => $signup, 'global' => NULL));
      }
      $final_text = theme('signup_broadcast_sender_copy', $body, $message);
      $params = array(
        'subject' => $subject,
        'body' => $final_text,
        'ignore_tokens' => TRUE,
      );
      $language = user_preferred_language($user);
      drupal_mail('signup', 'signup_broadcast_mail', $sender_email, $language, $params, $from);
      watchdog('signup', 'Broadcast email copy for %title sent to %email.', array('%title' => $node->title, '%email' => $sender_email), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
      drupal_set_message(t('Sent a copy of this message to %email', array('%email' => $sender_email)));
    }
  }
  if ($send_all) {
    drupal_set_message(t('Message sent to all users who have signed up'));
  }
  else {
    drupal_set_message(t('Message sent to selected users who have signed up'));
  }
}

