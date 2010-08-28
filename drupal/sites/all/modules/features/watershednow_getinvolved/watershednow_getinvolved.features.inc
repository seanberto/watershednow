<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function watershednow_getinvolved_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function watershednow_getinvolved_node_info() {
  $items = array(
    'donation' => array(
      'name' => t('Donation campaign'),
      'module' => 'features',
      'description' => t('A donation campaign provides a simple form for users to make a contribution via PayPal.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'event' => array(
      'name' => t('Event'),
      'module' => 'features',
      'description' => t('Events are posted to an event calendar and are signups enabled.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'take_action' => array(
      'name' => t('Take action page'),
      'module' => 'features',
      'description' => t('A take action page is a survey or online petition form.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function watershednow_getinvolved_views_api() {
  return array(
    'api' => '2',
  );
}
