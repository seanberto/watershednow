<?php
// $Id: node_admin.inc,v 1.1.2.5 2009/01/14 18:11:21 dww Exp $


/**
 * @file
 * Code related to the signup administration tab on each node.
 */

/**
 * Print the signup administration tab for a single node.
 */
function signup_node_admin_page($node) {
  drupal_set_title(check_plain($node->title));

  // Administrative table to control signups for this node.
  // We only want this if we're not in the middle of confirming a bulk
  // operation on the signup details table. Even though directly inspecting
  // POST is almost always a bad idea, in this case, we're not using or
  // displaying any of the data at all.  We're just making a cosmetic decision
  // based on if it's a certain kind of POST...
  if (empty($_POST['operation'])) {
    module_load_include('inc', 'signup', 'includes/node_admin_summary');
    $signup_node_admin_summary_form = drupal_get_form('signup_node_admin_summary_form', $node);
  }
  else {
    // We're either dealing with a validation error or on a confirm form, so
    // don't render the summary form at all.
    $signup_node_admin_summary_form = '';
  }

  // Signup details table, including cancel checkboxes.
  $display_list = variable_get('signup_display_signup_admin_list', 'signup');
  if ($display_list == 'signup') {
    $signup_node_admin_details_form = drupal_get_form('signup_node_admin_details_form', $node);
  }
  elseif ($display_list == 'embed-view' && module_exists('views')) {
    $signup_view = _signup_get_admin_list_view();
    $signup_view_parts = explode(':', $signup_view);
    $view_name = $signup_view_parts[0];
    $view_display = $signup_view_parts[1];
    $view = views_get_view($view_name);
    $view->override_path = 'node/%/signups/admin';
    $view_args = array($node->nid);
    $signup_node_admin_details_form = $view->preview($view_display, $view_args);
  }
  else {
    // They don't want the administrative user list at all.
    $signup_node_admin_details_form = '';
  }

  return theme('signup_node_admin_page', $node, $signup_node_admin_summary_form, $signup_node_admin_details_form);
}

function signup_node_admin_details_form(&$form_state, $node) {
  unset($_SESSION['signup_cancel_multiple_users']);
  $form = array();

  // Prepare a table header that allows sorting on name and signup time.
  $header = array(
    theme('table_select_header_cell'),
    array('data' => t('Name'), 'field' => 'u.name', 'sort' => 'asc'),
    array('data' => t('Signup time'), 'field' => 's.signup_time'),
    array('data' => t('Extra information')),
    array('data' => t('Attendance'), 'field' => 's.attended'),
  );

  $sql = "SELECT u.uid, u.name, s.* FROM {signup_log} s INNER JOIN {users} u ON u.uid = s.uid WHERE s.nid = %d";
  $sql .= tablesort_sql($header);
  $result = db_query($sql, $node->nid);

  // Loop through the users, unserializing their user data.
  while ($signed_up_user = db_fetch_object($result)) {
    // The "username" to print is different for anon signups and registered
    // user signups.  For registered users, provide a link to the user profile
    // For anon, use the user's email address as the name.
    if ($signed_up_user->uid == 0) {
      $username = check_plain($signed_up_user->anon_mail);
    }
    else {
      $username = theme('username', $signed_up_user);
    }
    $key = $signed_up_user->sid;
    $users[$key] = '';
    $form['username'][$key] = array('#value' => $username);
    $form['signup_date'][$key] = array('#value' => format_date($signed_up_user->signup_time, variable_get('signup_date_format', 'small')));
    $form['signup_form_data'][$key] = array('#value' => theme('signup_custom_data', unserialize($signed_up_user->form_data)));
    $form['attended'][$key] = array('#value' => theme('signup_attended_text', $signed_up_user->attended));
  }
  if (empty($users)) {
    $form['no_users'] = array('#value' => t('No users have signed up for this %node_type.', array('%node_type' => node_get_types('name', $node->type))));
  }
  else {
    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid,
    );
    $form['users'] = array('#type' => 'checkboxes', '#options' => $users);
    $form['operation'] = array(
      '#type' => 'select',
      '#options' => array(
        'attend_yes' => t('Mark as attended'),
        'attend_no' => t('Mark as did not attend'),
      ),
    );
    if (user_access('cancel signups')) {
      $form['operation']['#options']['cancel'] = t('Cancel signup');
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#submit' => array('signup_node_admin_multiple_submit'),
      '#validate' => array('signup_node_admin_multiple_validate'),
    );
    $form['#header'] = $header;
  }
  return $form;
}

/**
 * Validate handler for updating multiple signups via node/N/signups.
 */
function signup_node_admin_multiple_validate($form, &$form_state) {
  $users = array_filter($form_state['values']['users']);
  if (empty($users)) {
    form_set_error('', t('No users selected.'));
  }
}

/**
 * Submit handler for updating multiple signups via node/N/signups.
 *
 * If the operation is non-destructive (recording attendance), do the deed.
 * For cancelling multiple users, this just saves the selected users into
 * SESSION and redirects to a confirm form which is registered at
 * node/N/signups/confirm.
 */
function signup_node_admin_multiple_submit($form, &$form_state) {
  $users = array_filter($form_state['values']['users']);
  switch ($form_state['values']['operation']) {
    case 'cancel':
      if (user_access('cancel signups')) {
        $_SESSION['signup_cancel_multiple_users'] = $users;
        $form_state['redirect'] = 'node/'. $form_state['values']['nid'] .'/signups/confirm';
        return;
      }
      else {
        drupal_set_message(t('You do not have permission to cancel signups.'), 'error');
        return;
      }

    case 'attend_yes':
      $attend = 1;
      break;

    case 'attend_no':
      $attend = 0;
      break;

  }
  $placeholders = db_placeholders($users);
  db_query("UPDATE {signup_log} SET attended = %d WHERE sid IN ($placeholders)", array($attend) + $users);
}

/**
 * Builds the confirm form when canceling multiple signups from node/N/signups.
 */
function signup_cancel_multiple_confirm(&$form_state, $node) {
  $form = array();
  $form['nid'] =  array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['users'] =  array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );
  $placeholders = db_placeholders($_SESSION['signup_cancel_multiple_users']);
  $query = db_query("SELECT u.name, u.uid, s.* FROM {signup_log} s INNER JOIN {users} u ON s.uid = u.uid WHERE s.sid IN (". $placeholders .")", $_SESSION['signup_cancel_multiple_users']);
  while ($signup = db_fetch_object($query)) {
    $key = $signup->sid;
    if ($signup->uid) {
      $label = theme('username', $signup);
    }
    else {
      $label = t('Anonymous signup: %anon_mail', array('%anon_mail' => $signup->anon_mail));
    }
    $form['users'][$key] = array(
      '#type' => 'hidden',
      '#value' => $key,
      '#prefix' => '<li>',
      '#suffix' => $label ."</li>\n",
    );
  }
  $form['#submit'][] = 'signup_cancel_multiple_confirm_submit';
  return confirm_form(
    $form,
    t('Are you sure you want to cancel signups for these users?'),
    'node/'. $node->nid .'/signups',
    t('This action cannot be undone.'),
    t('Cancel signups'), t('Keep signups')
  );
}

/**
 * Submit handler for the confirm form to cancel multiple signups.
 */
function signup_cancel_multiple_confirm_submit($form, &$form_state) {
  $nid = $form_state['values']['nid'];
  if (user_access('cancel signups')) {
    foreach ($form_state['values']['users'] as $key) {
      signup_cancel_signup($key);
    }
  }
  else {
    drupal_set_message(t('You do not have permission to cancel signups.'), 'error');
  }
  $form_state['redirect'] = 'node/'. $nid .'/signups';
  unset($_SESSION['signup_cancel_multiple_users']);
}

