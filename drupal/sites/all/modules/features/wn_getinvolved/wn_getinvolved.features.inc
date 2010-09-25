<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wn_getinvolved_ctools_plugin_api() {
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
function wn_getinvolved_node_info() {
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
      'help' => '"Donation pages" are created for specific fundraising initiatives. They contain a simple donation link that goes to PayPal. When donations are made payment information is sent back to this site for tracking purposes.',
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
      'help' => '"Events" have a start date/time and optionally an end date/time. They are listed on the events calendar. They can also be enabled to accept sign-ups with registration limits.',
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
      'help' => '"Take action pages" are used to create survey forms and e-petitions. Create an "take action" page to gather site visitor information and/or to send petition letters to representatives. Form results can be downloaded as spreadsheet data.',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function wn_getinvolved_views_api() {
  return array(
    'api' => '2',
  );
}
