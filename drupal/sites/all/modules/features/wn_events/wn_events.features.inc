<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wn_events_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function wn_events_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'module' => 'features',
      'description' => t('Events are posted to an event calendar and are signups enabled.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => t('"Events" have a start date/time and optionally an end date/time. They are listed on the events calendar. They can also be enabled to accept sign-ups with registration limits.'),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function wn_events_views_api() {
  return array(
    'api' => '2',
  );
}
