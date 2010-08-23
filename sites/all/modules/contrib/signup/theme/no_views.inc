<?php
// $Id: no_views.inc,v 1.2 2008/11/19 18:30:07 dww Exp $


/**
 * @file
 * Deprecated code if you don't have Views enabled.
 */

/**
 * Formats the list of users signed up for a particular node.
 *
 * @param $node
 *   The node object that users have signed up for.
 * @param $registered_signups
 *   Array of objects with data for each registered user signed up.
 * @param $anon_signups
 *   Array of objects with data for each anonymous user signed up.
 */
function theme_signup_user_list($node, $registered_signups, $anon_signups) {
  $header = array(array('data' => t('!users signed up', array('!users' => format_plural((count($registered_signups) + count($anon_signups)), '1 individual', '@count individuals')))));
  $rows = array();
  foreach ($registered_signups as $signup) {
    $rows[] = array(theme('username', $signup));
  }
  if (!empty($anon_signups)) {
    $rows[] = array(t('!count anonymous', array('!count' => count($anon_signups))));
  }
  return theme('table', $header, $rows);
}

/**
 * Format a user signup for display in a schedule list.
 *
 * @param node
 *   The node which needs theming
 */
function theme_signup_user_schedule($node) {
  $output = '';
  $output .= '<div class="signup-user-schedule"><div class="'. $node->type .'signup-title"><h4>'. l($node->title, "node/$node->nid") .'</h4></div></div>';
  return $output;
}

