<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wn_takeaction_ctools_plugin_api() {
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
function wn_takeaction_node_info() {
  $items = array(
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
function wn_takeaction_views_api() {
  return array(
    'api' => '2',
  );
}
