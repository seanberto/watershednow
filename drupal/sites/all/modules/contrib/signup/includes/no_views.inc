<?php
// $Id: no_views.inc,v 1.15.2.2 2009/08/10 17:45:53 dww Exp $


/**
 * @file
 * Provides all the code for required UI elements for sites that do
 * not have views.module enabled. If views is enabled, there are
 * default views for all of these things (which are therefore
 * customizable and more powerful) in signup/views/views.inc.
 *
 */

/**
 * Implementation of hook_block().
 *
 * @ingroup signup_core
 *
 * @param $op
 *   The operation that is being requested. This defaults to 'list', which
 *   indicates that the method should return which blocks are available.
 * @param $delta
 *   The specific block to display (the offset into an array).
 *
 * @return
 *   One of two possibilities. The first is an array of available blocks.
 *   The other is an array containing a block.
 */
function signup_block($op = 'list', $delta = 0) {
  global $user;
  switch ($op) {
    case 'list':
      $blocks[0]['info'] = t('Current signups');
      return $blocks;
      break;
    case 'view':
      if (user_access('access content')) {
        switch ($delta) {
          case 0:
            $titles = signup_list_user_signups($user->uid);
            if (count($titles)) {
              $block['subject'] = t('Current signups');
              $block['content'] = theme_item_list($titles) . l(t('View signup schedule'), "user/$user->uid/signups");
            }
            return $block;
        }
      }
  }
}

/**
 * Private helper as a partial implementation of hook_user().
 *
 * @see signup_user()
 */
function _signup_user_no_views($op, &$edit, &$user, $category = NULL) {
  switch ($op) {
    case 'view':
      // grab list of nodes the user signed up for.
      $signups = signup_list_user_signups($user->uid);
      if (count($signups) && variable_get('signup_no_views_user_info', TRUE)) {
        $user->content['signup'] = array(
          '#type' => 'user_profile_category',
          '#attributes' => array('class' => 'signup'),
          '#weight' => 5,
          '#title' => t('Signup information'),
        );
        $user->content['signup']['current'] = array(
          '#type' => 'user_profile_item',
          '#attributes' => array('class' => 'signup-current'),
          '#title' => t('Current signups'),
          '#value' => theme_item_list($signups),
          '#weight' => 0,
        );
        $user->content['signup']['schedule'] = array(
          '#type' => 'user_profile_item',
          '#attributes' => array('class' => 'signup-schedule'),
          '#title' => t('Signup schedule'),
          '#value' => l(t('View full signup schedule'), 'user/'. $user->uid .'/signups'),
          '#weight' => 2,
        );
      }
      break;
  }
}

/**
 * Add menu items we only need to define if views is not enabled.
 */
function signup_no_views_menu(&$items) {
  // User signup schedule callback
  $items['user/%user/signups'] = array(
    'title' => 'Signups',
    'page callback' => 'signup_user_schedule',
    'page arguments' => array(1),
    'type' => MENU_CALLBACK,
    'access callback' => '_signup_no_views_user_menu_access',
    'access arguments' => array(1),
    'file' => 'includes/no_views.inc',
  );
}

function _signup_no_views_user_menu_access($account) {
  return variable_get('signup_no_views_user_info', TRUE) && _signup_user_menu_access($account);
}

/**
 * Print a schedule of the given user's signups.
 *
 * @ingroup signup_callback
 */
function signup_user_schedule($account) {
  $output = '';
  drupal_set_title(t('Signups for @user', array('@user' => $account->name)));
  $titles = signup_list_user_signups($account->uid);
  foreach ($titles as $nid => $title) {
    $node = node_load($nid);
    $output .= theme('signup_user_schedule', $node);
  }
  return $output;
}

/**
 * Return an array of all nodes the specified user has signed up for.
 *
 * @param $uid
 *   The ID of the user to generate a list of signups for.
 *
 * @return
 *   Array of all nodes the given user has signed up for. The array is indexed
 *   by node ID, and contains titles as links to each node.
 */
function signup_list_user_signups($uid) {
  $titles = array();

  // We don't want to return anything for anon users.
  if ($uid != 0) {
    $sql = "SELECT n.nid, n.title FROM {node} n INNER JOIN {signup_log} s_l ON n.nid = s_l.nid WHERE s_l.uid = %d ORDER BY n.nid";
    $result = db_query(db_rewrite_sql($sql), $uid);
    while ($node = db_fetch_array($result)) {
      $titles[$node['nid']] = l($node['title'], 'node/'. $node['nid']);
    }
  }
  return $titles;
}
