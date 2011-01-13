<?php
// $Id: date.inc,v 1.16.2.9 2011/01/08 23:42:28 ezrag Exp $


/**
 * @file
 * Code to support using CCK date fields for time-based signup functionality.
 */

/**
 *
 * @return Array of SQL clauses for admin overview page query builder.
 */
function _signup_date_admin_sql($content_type) {
  // Get the date field information for this content type.
  $field = signup_date_field($content_type);

  // In the case where the same CCK date field is being reused on multiple
  // content types, we'll potentially be JOINing on the same tables and
  // columns for different content types.  To defend against duplicate table
  // names or ambiguous columns in the query, use the content type to alias.
  // Also, prefix it to make sure it's never a reserved word.
  $alias = 'signup_alias_' . db_escape_table($content_type);

  // See what fields to SELECT.
  $fields[] = $alias .'.'. $field['database']['columns']['value']['column'];
  if (isset($field['database']['columns']['timezone']['column'])) {
    $fields[] = $alias .'.'. $field['database']['columns']['timezone']['column'];
  }
  $table = '{'. $field['database']['table'] ."} ". $alias;
  return array(
    'fields' => $fields,
    'joins' => array("LEFT JOIN $table ON $alias.vid = n.vid"),
    'group_by' => $fields,
  );
}

function signup_field_names($content_type = NULL) {
  $fields = array();
  $content_type_info = _content_type_info();
  if ($content_type_info['content types'][$content_type]) {
    foreach ($content_type_info['content types'][$content_type]['fields'] as $field) {
      if (in_array($field['type'], array('date', 'datestamp', 'datetime'))) {
        $fields[$field['field_name']] = $field['widget']['label'];
      }
    }
  }
  return $fields;
}

function signup_date_field($content_type) {
  $field_name = variable_get('signup_date_field_'. $content_type, 0);
  if ($field_name === 0 || $field_name == '0') {
    // PHP is completely evil and 'none' == 0 is TRUE, hence the extra checks.
    return FALSE;
  }
  if ($field_name == 'none') {
    return 'none';
  }
  $field = content_fields($field_name, $content_type);
  if (empty($field)) {
    return array();
  }
  $field['database'] = content_database_info($field);
  return $field;
}

/**
 * Returns a list of all cck fields that have been set for use in signups
 */
function signup_content_type_fields() {
  $fields = array();
  foreach (signup_content_types() as $content_type) {
    $field = signup_date_field($content_type);
    if (!empty($field) && $field != 'none') {
      $fields[] = $field;
    }
  }
  return $fields;
}

/**
 * Determine if the specific node is date-enabled.
 */
function _signup_date_get_node_scheduler($node) {
  $field = signup_date_field($node->type);
  if (!empty($node->{$field['field_name']}[0]['value'])) {
    return 'date';
  }
  if (isset($node->{$field['database']['columns']['value']['column']})) {
    return 'date';
  }
  return 'none';
}

/**
 * Alter the form for configuring CCK date fields on node types.
 *
 * Hooks into CCK Date fields to provide an option to use the current
 * field as the Signup date field (for autoclose and reminder emails).
 *
 */
function signup_form_content_field_edit_form_alter(&$form, &$form_state) {
  $type = $form['type_name']['#value'];
  if (in_array($form['#field']['type'], array('date', 'datestamp', 'datetime')) && variable_get('signup_node_default_state_'. $type, 'disabled') != 'disabled') {
    $form['signup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Signup settings'),
      '#collapsible' => TRUE,
      '#weight' => 1,
    );
    $form['signup']['signup_date_field'] = _signup_date_field_element($type);
    $form['#submit'][] = '_signup_date_field_form_submit';
    // Make sure the submit button comes after the signup settings fieldset.
    $form['submit']['#weight'] = 50;
  }
}

/**
 * Custom submit handler for the CCK date field editing form.
 *
 * @see _signup_date_field_form_alter()
 */
function _signup_date_field_form_submit($form, &$form_state) {
  $type = $form_state['values']['type_name'];
  if (empty($form_state['values']['signup_date_field'])) {
    variable_del('signup_date_field_'. $type);
  }
  else {
    variable_set('signup_date_field_'. $type, $form_state['values']['signup_date_field']);
  }
}

/**
 * Alter the node type form to add a setting to select the signup date field.
 *
 * @see signup_alter_node_type_form()
 */
function _signup_date_alter_node_type_form(&$form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'signup') .'/js/admin.content_types.js');

  $type = $form['#node_type']->type;
  $default_signup_state = variable_get('signup_node_default_state_'. $type, 'disabled');

  // Add a div to the 'Signup options' radios for signup.date.js.
  $form['signup']['signup_node_default_state']['#prefix'] = '<div class="signup-node-default-state-radios">';
  $form['signup']['signup_node_default_state']['#suffix'] = '</div>';

  // If event.module is enabled, add a div for those settings, too.
  if (!empty($form['workflow']['event_nodeapi'])) {
    $form['workflow']['event_nodeapi']['#prefix'] = '<div class="event-nodeapi-radios">';
    $form['workflow']['event_nodeapi']['#suffix'] = '</div>';
    $event_enabled = $form['workflow']['event_nodeapi']['#default_value'] != 'never';
  }
  else {
    $event_enabled = FALSE;
  }

  // Figure out if we should hide the date field selector by default.
  $class = 'signup-date-field-setting';
  if ($default_signup_state == 'disabled' || $event_enabled) {
    $class .= ' js-hide';
  }

  $form['signup']['signup_date_field'] = _signup_date_field_element($type);
  $form['signup']['signup_date_field']['#prefix'] = '<div class="'. $class .'">';
  $form['signup']['signup_date_field']['#suffix'] = '</div>';
}

/**
 * Create the FAPI form element for the signup date field.
 *
 * @param $type
 *   The node type to generate the form element for.
 *
 * @return
 *   FAPI form array for the signup date field element.
 *
 * @see _signup_date_field_form_alter()
 * @see _signup_date_alter_node_type_form()
 */
function _signup_date_field_element($type) {
  return array(
    '#type' => 'select',
    '#title' => t('Date field to use with signup'),
    '#options' => _signup_get_date_field_options($type),
    '#default_value' => variable_get('signup_date_field_'. $type, 0),
    '#description' => t('Select the date field of this content type to use for signup time-based functionality, such as automatically closing signups when the start time has passed and sending reminder emails. Select "%none" to not use a date field for signup functionality at all.', array('%none' => t('None'))),
  );
}

/**
 * Check the signup and date configuration on node types depending on the URL.
 *
 * This function is invoked from signup_help() so that we can check the
 * configuration of any signup-enabled node types to ensure that the CCK date
 * field and signup settings make sense.
 *
 * @param $type
 *   The 4th element in the URL which specifies which node type is currently
 *   being configured.  If this is empty, it means we're at the node type
 *   overview listing and we should test all node types.
 *
 * @see signup_help()
 * @see signup_date_field_check_config()
 */
function signup_date_check_node_types($type = NULL) {
  $names = node_get_types('names');
  if (!empty($type)) {
    signup_date_field_check_config($type, $names[$type]);
  }
  else {
    foreach ($names as $type => $name) {
      signup_date_field_check_config($type, $name);
    }
  }
}

/**
 * Check that the date and signup configuration for a node type makes sense.
 *
 * This validates that if a node type is signup enabled, that it either has a
 * signup date field selected (for autoclose and reminder emails), or that the
 * signup date field has been explicitly set to 'None'.  It warns the site
 * administrator if they have signup-enabled a node type and not defined any
 * date fields at all, or if they have date fields but haven't selected the
 * one to use for signup functionality.
 *
 * @param $type
 *   The node type to check signup and CCK date field configuration on.
 * @param $name
 *   Human readable name of the node type to check.
 *
 * @return
 *   Nothing -- configuration errors are reported via drupal_set_message().
 *
 * @see signup_help()
 */
function signup_date_field_check_config($type, $name) {
  $signup_default_state = variable_get('signup_node_default_state_'. $type, 'disabled');
  $signup_scheduler = _signup_get_node_type_scheduler($type);
  if ($signup_scheduler != 'event' && $signup_default_state != 'disabled') {
    // Signups aren't disabled on this node type, see if there's a date field.
    $signup_date_field = signup_date_field($type);
    if ($signup_date_field != 'none') {
      $type_url = str_replace('_', '-', $type);
      $placeholders = array(
        '%node_type' => $name,
        '%signup_date_field' => t('Date field to use with signup'),
        '@type_admin_url' => url('admin/content/node-type/'. $type_url),
        '@type_add_field_url' => url('admin/content/node-type/'. $type_url .'/fields'),
        '%none' => '<' . t('none') . '>',
      );
      // Administrator hasn't specifically turned off date support...
      if (signup_field_names($type)) {
        // Node type has some date fields...
        if ($signup_date_field == 0) {
          drupal_set_message(t('You have enabled the %node_type content type for signups, and have added one or more date fields, but have not selected a date field for use with signup. You can modify the %signup_date_field setting at the <a href="@type_admin_url">%node_type configuration page</a> to select a date field to use, or disable this warning by selecting %none.', $placeholders), 'warning');
        }
      }
      else {
        // No date fields at all.
        drupal_set_message(t('You have enabled the %node_type content type for signups but have not added a date field. You can either <a href="@type_add_field_url">add a date field</a>, or disable this warning by selecting %none for the %signup_date_field setting at the <a href="@type_admin_url">%node_type configuration page</a>.', $placeholders), 'warning');
      }
    }
  }
}

/**
 * Helper function for the date field select to build its options.
 *
 * @param $type
 *   Content type whose date fields should be listed.
 *
 * @return
 *   Associative array with all date fields of the given content type plus
 *   'None' and an optional 'Not specified' if the user never selected a
 *   value.
 */
function _signup_get_date_field_options($type) {
  $options = array();
  // Add "Not specified" if the user never selected a field.
  if (variable_get('signup_date_field_'. $type, 0) == 0) {
    $options = array(0 => '<' . t('Not specified') . '>');
  }
  // Add any date fields from this node type.
  $options += signup_field_names($type);
  // Always add 'None' as the final choice.
  $options += array('none' => '<' . t('None') . '>');
  return $options;
}

/**
 *
 * @return Array of SQL clauses for cron reminder email query builder.
 */
function _signup_date_reminder_sql($content_type) {
  // Get the date field information for this content type.
  $field = signup_date_field($content_type);
  $start_field = $field['database']['columns']['value']['column'];

  // Figure out what TZ we want to do the date comparisons in.
  $compare_tz = $field['tz_handling'] == 'none' ? date_default_timezone_name() : 'UTC';
  // Get a DateAPI SQL handler class for this field.
  $handler = date_field_get_sql_handler($field, $compare_tz);

  // Find the current time in the appropriate TZ for this field.
  $now_date = date_now($compare_tz);
  // Need to enclose this in ' marks to use directly in the SQL.
  $now = "'". date_format($now_date, DATE_FORMAT_DATETIME) ."'";

  // Extract the correct SQL to represent the start time.
  $start_time = $handler->sql_field($start_field);

  // Create SQL to represent the time we should start sending reminders, based
  // on the SQL for the start time and the reminder_days_before field.
  $reminder_start = $handler->sql_date_math($start_time, 'SUB', 's.reminder_days_before', 'DAY');
  $reminder_stop = $handler->sql_date_math($start_time, 'ADD', 1, 'HOUR');

  // The WHERE clauses are now trivial: We want to make sure a) the current
  // time is after the time we should start sending reminders, but before the
  // actual start time itself.
  $where = array(
    "$now >= $reminder_start",
    "$now <= $reminder_stop",
  );

  // See what fields to SELECT.
  $fields[] = $start_field;
  if (isset($field['database']['columns']['timezone']['column'])) {
    $fields[] = $field['database']['columns']['timezone']['column'];
  }
  $table = '{'. $field['database']['table'] .'}';
  return array(
    'fields' => $fields,
    'joins' => array("INNER JOIN $table ON $table.vid = n.vid"),
    'where' => $where,
  );
}

/**
 *
 * @return Array of SQL clauses for cron auto-close query builder.
 */
function _signup_date_autoclose_sql($content_type) {
  // Get the date field information for this content type.
  $field = signup_date_field($content_type);
  $start_field = $field['database']['columns']['value']['column'];

  // Figure out what TZ we want to do the date comparisons in.
  $compare_tz = $field['tz_handling'] == 'none' ? date_default_timezone_name() : 'UTC';
  // Get a DateAPI SQL handler class for this field.
  $handler = date_field_get_sql_handler($field, $compare_tz);

  // Compute a string representing the moment when signups should start
  // auto-closing.  If the field has no TZ handling, we just want to grab the
  // current local time.  If the field has any TZ handling, the date will be
  // stored in the DB in UTC time, so start from current UTC time.  Once we
  // have the right current time, we need to add our close-in-advance offset.
  $close_early_hours = variable_get('signup_close_early', 1);
  $close_date = date_now($compare_tz);
  date_modify($close_date, "+$close_early_hours hours");
  $close_date_str = date_format($close_date, DATE_FORMAT_DATETIME);

  // Use the DateAPI SQL handler to construct an appropriate WHERE clause.
  // Make sure that the start time is <= NOW plus the auto-close window.
  $where = $handler->sql_where_date('DATE', $start_field, '<=', $close_date_str);

  // See what fields to SELECT.
  $fields[] = $start_field;
  if (isset($field['database']['columns']['timezone']['column'])) {
    $fields[] = $field['database']['columns']['timezone']['column'];
  }
  $table = '{'. $field['database']['table'] .'}';
  return array(
    'fields' => $fields,
    'joins' => array("INNER JOIN $table ON $table.vid = n.vid"),
    'where' => $where,
  );
}

/**
 * Returns TRUE if the given node is event-enabled, and the start time
 * has already passed the "Close x hours before" setting.
 */
function _signup_date_node_completed($node) {
  $field = signup_date_field($node->type);
  if ($field && $field != 'none' && isset($node->{$field['field_name']})) {
    // Grab whatever date value we actually have, regardless of format.
    $date_value = $node->{$field['field_name']}[0]['value'];
    // Figure out the timezone handling for this date.
    if ($field['tz_handling'] == 'date') {
      $tz = $node->{$field['field_name']}[0]['timezone'];
    }
    else {
      $tz = date_default_timezone_name();
    }
    $db_tz = date_get_timezone_db($field['tz_handling'], $tz);
    // Create a date object
    $date = date_make_date($date_value, $db_tz, $field['type']);
    // Make sure the date object is going to print UTC values.
    date_timezone_set($date, timezone_open('UTC'));
    // Find out how early signups should be automatically closed.
    $close_early_hours = variable_get('signup_close_early', 1);
    date_modify($date, "-$close_early_hours hours");
    $close_time = date_format($date, 'U');
    // Find the current UTC time.
    $now = date_now('UTC');
    if (date_format($now, 'U') >= $close_time) {
      // It's now later than when this node would automatically close signups.
      return TRUE;
    }
  }
  return FALSE;
}

function _signup_date_format_date($node, $include_to_date = FALSE) {
  $field = signup_date_field($node->type);
  if (!$field || $field == 'none') {
    return '';
  }
  if ($field['tz_handling'] == 'date') {
    if (isset($node->{$field['field_name']})) {
      $tz = $node->{$field['field_name']}[0]['timezone'];
    }
    else {
      $tz = $node->{$field['database']['columns']['timezone']['column']};
    }
  }
  else {
    $tz = date_default_timezone_name();
  }
  $display_tz = date_get_timezone($field['tz_handling'], $tz);
  $db_tz = date_get_timezone_db($field['tz_handling'], $tz);

  if (isset($node->{$field['field_name']})) {
    $date_value = $node->{$field['field_name']}[0]['value'];
  }
  else {
    $date_value = $node->{$field['database']['columns']['value']['column']};
  }
  $date = date_make_date($date_value, $db_tz, $field['type']);
  if ($db_tz != $display_tz) {
    date_timezone_set($date, timezone_open($display_tz));
  }
  $format = date_formatter_format('default', $field['field_name']);
  $date_out = date_format_date($date, 'custom', $format);

  if ($include_to_date) {
    if (isset($node->{$field['field_name']})) {
      $date_value = $node->{$field['field_name']}[0]['value2'];
    }
    else {
      $date_value = $node->{$field['database']['columns']['value2']['column']};
    }
    $date = date_make_date($date_value, $db_tz, $field['type']);
    if ($db_tz != $display_tz) {
      date_timezone_set($date, timezone_open($display_tz));
    }
    $date = date_format_date($date, 'custom', $format);
    if ($date_value) {
      $date_out .= t(' to ') . date_format_date(date_make_date($date_value), 'custom', $format);
    }
  }

  return $date_out;
}

