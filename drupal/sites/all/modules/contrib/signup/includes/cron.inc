<?php
// $Id: cron.inc,v 1.1.2.2 2009/08/14 23:26:10 dww Exp $


/**
 * @file
 * Code required during regular cron runs.
 */

/**
 * Helper function that sends cron-based reminder e-mails.
 *
 * Invokes the method for the installed event/date backend module to get the
 * right query fragments, and builds a query to find all nodes that need a
 * reminder email. For each one, it loops over the users signed up for that
 * node and send off the emails.
 *
 * @see signup_cron()
 * @see signup_reminder_sql()
 * @see _signup_build_query()
 */
function _signup_cron_send_reminders() {
  $type_reminder_sql = array();
  foreach (signup_content_types() as $type) {
    $type_sql = signup_reminder_sql($type);
    if (!empty($type_sql)) {
      $type_reminder_sql[$type] = $type_sql;
    }
  }
  if (empty($type_reminder_sql)) {
    // No node types support reminder emails, so bail out now.
    return;
  }

  $reminder_common_sql = array(
    'primary' => '{node} n',
    'fields' => array('n.title', 'n.nid', 'n.type', 's.reminder_email', 's.forwarding_email'),
    'where' => array('s.send_reminder = 1', "n.type = '%s'"),
    'joins' => array('INNER JOIN {signup} s ON s.nid = n.nid'),
  );

  $from = variable_get('site_mail', ini_get('sendmail_from'));

  foreach ($type_reminder_sql as $type => $reminder_sql) {
    $sql = _signup_build_query($reminder_common_sql, $reminder_sql);
    $result = db_query($sql, $type);

    // Grab each node, construct the email header and subject, and query
    // the signup log to pull all users who are signed up for this node.
    while ($node = db_fetch_object($result)) {
      $subject = t('!node_type reminder: !title', array('!node_type' => node_get_types('name', $type), '!title' => $node->title));
      $signups = db_query("SELECT u.name, u.mail, s_l.sid, s_l.anon_mail, s_l.form_data FROM {signup_log} s_l INNER JOIN {users} u ON u.uid = s_l.uid WHERE s_l.nid = %d", $node->nid);

      // Loop through the users, composing their customized message
      // and sending the email.
      while ($signup = db_fetch_object($signups)) {
        $user_mail = _signup_get_email($signup);
        $params = array(
          'subject' => $subject,
          'body' => $node->reminder_email,
          'node' => $node,
          'signup' => $signup,
        );
        if (module_exists('token')) {
          $params['body'] = token_replace_multiple($params['body'], array('node' => node_load($node->nid), 'signup' => $signup, 'global' => NULL));
        }
        $language = user_preferred_language($signup);
        drupal_mail('signup', 'signup_reminder_mail', $user_mail, $language, $params, $from);
        watchdog('signup', 'Reminder for %title sent to %user_mail.', array('%title' => $node->title, '%user_mail' => $user_mail), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
      }

      // Reminders for this node are all sent, so mark it in the
      // database so they're not sent again.
      db_query("UPDATE {signup} SET send_reminder = 0 WHERE nid = %d", $node->nid);
    }
  }
}

/**
 * Helper function that handles auto-closing time-based nodes during cron.
 *
 * Loops over all the node types that are signup-enabled.  For each one, it
 * invokes the method for the installed event/date backend module to get the
 * right query fragments, and builds a query to find all nodes of that type
 * where signups should be closed (e.g. events that already started, etc).
 *
 * @see signup_cron()
 * @see signup_autoclose_sql()
 * @see _signup_build_query()
 */
function _signup_cron_autoclose() {
  $type_autoclose_sql = array();
  foreach (signup_content_types() as $type) {
    $type_sql = signup_autoclose_sql($type);
    if (!empty($type_sql)) {
      $type_autoclose_sql[$type] = $type_sql;
    }
  }
  if (empty($type_autoclose_sql)) {
    // No node types support auto-close, so bail out now.
    return;
  }

  $autoclose_common_sql = array(
    'primary' => '{node} n',
    'fields' => array('n.nid', 'n.type'),
    'where' => array('s.status = 1', "n.type = '%s'"),
    'joins' => array('INNER JOIN {signup} s ON s.nid = n.nid'),
  );

  foreach ($type_autoclose_sql as $type => $autoclose_sql) {
    $sql = _signup_build_query($autoclose_common_sql, $autoclose_sql);
    $result = db_query($sql, $type);

    // Loop through the results, calling the signup closing function.
    while ($signup = db_fetch_object($result)) {
      signup_close_signup($signup->nid, $cron = 'yes');
      $node = node_load($signup->nid);
      foreach (module_implements('signup_close') as $module) {
        $function = $module .'_signup_close';
        $function($node);
      }
      watchdog('signup', 'Signups closed for %title by cron.', array('%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
    }
  }
}

