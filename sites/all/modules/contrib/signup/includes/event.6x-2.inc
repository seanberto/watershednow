<?php
// $Id: event.6x-2.inc,v 1.2.2.1 2009/01/09 03:49:54 dww Exp $


/**
 *
 * @return Array of SQL clauses for cron reminder email query builder.
 */
function _signup_event_reminder_sql($content_type) {
  global $db_type;
  event_include_files();
  switch ($db_type) {
    case 'mysql':
    case 'mysqli':
      $where = array(
        "('". gmdate('Y-m-d H:i:s') ."' > (". event_where_utc() ." - INTERVAL s.reminder_days_before DAY))",
        "('". gmdate('Y-m-d H:i:s') ."' <= (". event_where_utc() ."))",
      );
      break;

    case 'pgsql':
      $where = array(
        "('". gmdate('Y-m-d H:i:s') ."' > (". event_where_utc() ." - INTERVAL 's.reminder_days_before days'))",
        "('". gmdate('Y-m-d H:i:s') ."' <= (". event_where_utc() ."))",
      );
      break;
  }
  return array(
    'fields' => array(event_select(), 'e.timezone'),
    'joins' => array(event_join()),
    'where' => $where,
  );
}

/**
 *
 * @return Array of SQL clauses for cron auto-close query builder.
 */
function _signup_event_autoclose_sql($content_type) {
  event_include_files();
  return array(
    'fields' => array(event_select(), 'e.timezone'),
    'joins' => array(event_join('s')),
    'where' => array(event_where_utc() ." < '". gmdate('Y-m-d H:i:s', time() + (variable_get('signup_close_early', 1) * 3600)) ."'"),
  );
}

/**
 *
 * @return Array of SQL clauses for admin overview page query builder.
 */
function _signup_event_admin_sql($content_type = NULL) {
  // Since all event node types store their event data in the same table, we
  // need to make sure we only JOIN on this table once.  It doesn't matter
  // which node type we use to add this JOIN.
  static $did_event_join = FALSE;
  if ($did_event_join) {
    return array();
  }
  $did_event_join = TRUE;
  return array(
    'fields' => array(event_select(), 'e.timezone'),
    'group_by' => array('event_start', 'e.timezone'),
    'joins' => array(event_join('n', 'LEFT')),
  );
}

/**
 * Returns true if the given node is event-enabled, and the start time
 * has already passed the "Close x hours before" setting.
 */
function _signup_event_node_completed($node) {
  if (isset($node->event)) {
    $closing_time = gmdate('Y-m-d H:i:s', time() + (variable_get('signup_close_early', 1) * 3600));
    if (event_is_later($closing_time, $node->event['start_utc'], 'string')) {
      return TRUE;
    }
  }
  return FALSE;
}

function _signup_event_format_date($node) {
  $event_date = array();
  if (!empty($node->event['start'])) {
    $event_date = event_explode_date($node->event['start']);
  }
  elseif (!empty($node->event_start)) {
    $event_date = event_explode_date($node->event_start);
  }
  return !empty($event_date) ? event_format_date($event_date, variable_get('signup_date_format', 'medium')) : t('[Untimed]');
}

/**
 * Determine if the given node has a start time managed by the event.module.
 *
 * @param $node
 *   Fully loaded node object to test.
 *
 * @return
 *   'event' if this is an event node, otherwise 'none'.
 *
 * @see _signup_get_node_scheduler()
 */
function _signup_event_get_node_scheduler($node) {
  if (isset($node->event) && isset($node->event['start'])) {
    return 'event';
  }
  return isset($node->event_start) ? 'event' : 'none';
}

