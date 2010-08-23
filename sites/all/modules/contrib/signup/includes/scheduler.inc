<?php
// $Id: scheduler.inc,v 1.4 2008/11/06 10:20:48 dww Exp $


/**
 * @file
 * Code necessary to determine which scheduler backend(s) to use.
 */

/**
 * Load relevant scheduler backend include file(s).
 *
 * Decides which include files are needed based on which modules
 * exist and which versions are loaded.
 */
function _signup_load_scheduler_includes() {
  if (module_exists('event')) {
    module_load_include('inc', 'signup', 'includes/event.6x-2');
  }
  if (module_exists('date')) {
    module_load_include('inc', 'signup', 'includes/date');
  }
}

/**
 * Does this site have any date-enabled content?
 */
function signup_site_has_dates() {
  return (module_exists('event') || module_exists('date'));
}

/**
 * Determine which scheduler backend to use for the given node.
 *
 * @param $node
 *   Fully loaded node object to test.
 *
 * @return
 *   'event' to use the event.module, 'date' for the date.module, or 'none' if
 *   the node is untimed.
 *
 * @see _signup_get_node_scheduler()
 * @see _signup_event_get_node_scheduler()
 * @see _signup_date_get_node_scheduler()
 */
function _signup_get_node_scheduler($node) {
  switch (_signup_get_node_type_scheduler($node->type)) {
    case 'event':
      return _signup_event_get_node_scheduler($node);

    case 'date':
      return _signup_date_get_node_scheduler($node);

    default:
      return 'none';
  }
}

/**
 * Determine which scheduler backend to use for the given node type.
 *
 * @param $type
 *   Machine-readable name of the node type to test.
 *
 * @return
 *   'event' to use the event.module, 'date' for the date.module, or 'none' if
 *   the node type is untimed.
 */
function _signup_get_node_type_scheduler($type) {
  if (module_exists('event') && variable_get('event_nodeapi_'. $type, 'never') != 'never') {
    return 'event';
  }
  if (module_exists('date')) {
    $date_field = variable_get('signup_date_field_'. $type, 0);
    if (!empty($date_field) && $date_field != 'none') {
      return 'date';
    }
  }
  return 'none';
}

/**
 * Determine if the given node has date/time data and if it already started.
 */
function _signup_node_completed($node) {
  if (is_numeric($node)) {
    $node = node_load($node);
  }
  switch (_signup_get_node_scheduler($node)) {
    case 'event':
      return _signup_event_node_completed($node);

    case 'date':
      return _signup_date_node_completed($node);

    default:
      return FALSE;
  }
}

function signup_format_date($node) {
  switch (_signup_get_node_scheduler($node)) {
    case 'event':
      return _signup_event_format_date($node);

    case 'date':
      return _signup_date_format_date($node);

    default:
      return t('[Untimed]');
  }
}

function signup_reminder_sql($node_type) {
  switch (_signup_get_node_type_scheduler($node_type)) {
    case 'event':
      return _signup_event_reminder_sql($node_type);

    case 'date':
      return _signup_date_reminder_sql($node_type);

    default:
      return array();
  }
}

function signup_autoclose_sql($node_type) {
  switch (_signup_get_node_type_scheduler($node_type)) {
    case 'event':
      return _signup_event_autoclose_sql($node_type);

    case 'date':
      return _signup_date_autoclose_sql($node_type);

    default:
      return array();
  }
}

function signup_admin_sql($node_type) {
  switch (_signup_get_node_type_scheduler($node_type)) {
    case 'event':
      return _signup_event_admin_sql($node_type);

    case 'date':
      return _signup_date_admin_sql($node_type);

    default:
      return array();
  }
}

