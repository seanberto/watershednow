<?php

/**
 * @file
 * Sample hooks demonstrating usage in Webform.
 */

/**
 * @defgroup webform_hooks Webform Module Hooks
 * @{
 * Webform's hooks enable other modules to intercept events within Webform, such
 * as the completion of a submission or adding validation. Webform's hooks also
 * allow other modules to provide additional components for use within forms.
 */

/**
 * Define callbacks that can be used as select list options.
 *
 * @return
 *   An array of callbacks that can be used for select list options. This array
 *   should be keyed by the "name" of the pre-defined list. The values should
 *   be an array with the following additional keys:
 *     - title: The translated title for this list.
 *     - options callback: The name of the function that will return the list.
 *     - options arguments: Any additional arguments to send to the callback.
 *     - file: Optional. The file containing the options callback, relative to
 *       the module root.
 */
function hook_webform_select_options_info() {
  $items = array();

  $items['days'] = array(
    'title' => t('Days of the week'),
    'options callback' => 'webform_options_days',
    'file' => 'includes/webform.options.inc',
  );

  return $items;
}

/**
 * Respond to the loading of Webform submissions.
 *
 * @param $submissions
 *   An array of Webform submissions that are being loaded, keyed by the
 *   submission ID. Modifications to the submissions are done by reference.
 */
function hook_webform_submission_load(&$submissions) {
  foreach ($submissions as $sid => $submission) {
    $submissions[$sid]->new_property = 'foo';
  }
}

/**
 * Modify a Webform submission, prior to saving it in the database.
 *
 * @param $node
 *   The Webform node on which this submission was made.
 * @param $submission
 *   The Webform submission that is about to be saved to the database.
 */
function hook_webform_submission_presave($node, &$submission) {
  // Update some component's value before it is saved.
  $component_id = 4;
  $submission->data[$component_id]['value'][0] = 'foo';
}

/**
 * Respond to a Webform submission being inserted.
 *
 * Note that this hook is called after a submission has already been saved to
 * the database. If needing to modify the submission prior to insertion, use
 * hook_webform_submission_presave().
 *
 * @param $node
 *   The Webform node on which this submission was made.
 * @param $submission
 *   The Webform submission that was just inserted into the database.
 */
function hook_webform_submission_insert($node, $submission) {
  // Insert a record into a 3rd-party module table when a submission is added.
  db_query("INSERT INTO {mymodule_table} nid = %d, sid = %d, foo = '%s'", $node->nid, $submission->sid, 'foo_data');
}

/**
 * Respond to a Webform submission being updated.
 *
 * Note that this hook is called after a submission has already been saved to
 * the database. If needing to modify the submission prior to updating, use
 * hook_webform_submission_presave().
 *
 * @param $node
 *   The Webform node on which this submission was made.
 * @param $submission
 *   The Webform submission that was just updated in the database.
 */
function hook_webform_submission_update($node, $submission) {
  // Update a record in a 3rd-party module table when a submission is updated.
  db_query("UPDATE {mymodule_table} SET (foo) VALUES ('%s') WHERE nid = %d, sid = %d", 'foo_data', $node->nid, $submission->sid);
}

/**
 * Respond to a Webform submission being deleted.
 *
 * @param $node
 *   The Webform node on which this submission was made.
 * @param $submission
 *   The Webform submission that was just deleted from the database.
 */
function hook_webform_submission_delete($node, $submission) {
  // Delete a record from a 3rd-party module table when a submission is deleted.
  db_query("DELETE FROM {mymodule_table} WHERE nid = %d, sid = %d", $node->nid, $submission->sid);
}

/**
 * Provide a list of actions that can be executed on a submission.
 *
 * Some actions are displayed in the list of submissions such as edit, view, and
 * delete. All other actions are displayed only when viewing the submission.
 * These additional actions may be specified in this hook. Examples included
 * directly in the Webform module include PDF, print, and resend e-mails. Other
 * modules may extend this list by using this hook.
 *
 * @param $node
 *   The Webform node on which this submission was made.
 * @param $submission
 *   The Webform submission on which the actions may be performed.
 */
function hook_webform_submission_actions($node, $submission) {
  if (webform_results_access($node)) {
    $actions['myaction'] = array(
      'title' => t('Do my action'),
      'href' => 'node/' . $node->nid . '/submission/' . $submission->sid . '/myaction',
      'query' => drupal_get_destination(),
    );
  }

  return $actions;
}

/**
 * Alter the display of a Webform submission.
 *
 * This function applies to both e-mails sent by Webform and normal display of
 * submissions when viewing through the adminsitrative interface.
 *
 * @param $renderable
 *   The Webform submission in a renderable array, similar to FormAPI's
 *   structure. This variable must be passed in by-reference. Important
 *   properties of this array include #node, #submission, #email, and #format,
 *   which can be used to find the context of the submission that is being
 *   rendered.
 */
function hook_webform_submission_render_alter(&$renderable) {
  // Remove page breaks from sent e-mails.
  if (isset($renderable['#email'])) {
    foreach (element_children($renderable) as $key) {
      if ($renderable[$key]['#component']['type'] == 'pagebreak') {
        unset($renderable[$key]);
      }
    }
  }
}

/**
 * Modify a loaded Webform component.
 *
 * IMPORTANT: This hook does not actually exist because components are loaded
 * in bulk as part of webform_node_load(). Use hook_nodeapi() to modify loaded
 * components when the node is loaded. This example is provided merely to point
 * to hook_nodeapi().
 *
 * @see hook_nodeapi()
 * @see webform_node_load()
 */
function hook_webform_component_load() {
  // This hook does not exist. Instead use hook_nodeapi().
}

/**
 * Modify a Webform component before it is saved to the database.
 *
 * Note that most of the time this hook is not necessary, because Webform will
 * automatically add data to the component based on the component form. Using
 * hook_form_alter() will be sufficient in most cases.
 *
 * @see hook_form_alter()
 * @see webform_component_edit_form()
 *
 * @param $component
 *   The Webform component being saved.
 */
function hook_webform_component_presave(&$component) {
  $component['extra']['new_option'] = 'foo';
}

/**
 * Respond to a Webform component being inserted into the database.
 */
function hook_webform_component_insert($component) {
  // Insert a record into a 3rd-party module table when a component is inserted.
  db_query("INSERT INTO {mymodule_table} (nid, cid) VALUES (%d, %d)", $component['nid'], $component['cid']);
}

/**
 * Respond to a Webform component being updated in the database.
 */
function hook_webform_component_update($component) {
  // Update a record in a 3rd-party module table when a component is updated.
  db_query('UPDATE {mymodule_table} SET value "%s" WHERE nid = %d AND cid = %d)', 'foo', $component['nid'], $component['cid']);
}

/**
 * Respond to a Webform component being deleted.
 */
function hook_webform_component_delete($component) {
  // Delete a record in a 3rd-party module table when a component is deleted.
  db_query('DELETE FROM {mymodule_table} WHERE nid = %d AND cid = %d)', $component['nid'], $component['cid']);
}

/**
 * Define components to Webform.
 *
 * @return
 *   An array of components, keyed by machine name. Required properties are
 *   "label" and "description". The "features" array defines which capabilities
 *   the component has, such as being displayed in e-mails or csv downloads.
 *   A component like "markup" for example would not show in these locations.
 *   The possible features of a component include:
 *
 *     - csv
 *     - email
 *     - email_address
 *     - email_name
 *     - required
 *     - conditional
 *     - spam_analysis
 *     - group
 *
 *   Note that these features do not indicate the default state, but determine
 *   if the component can have this property at all. Setting "required" to TRUE
 *   does not mean that a field will always be required, but instead give the
 *   option to the administrator to choose the requiredness. See the example
 *   implementation for details on how these features may be set.
 *
 *   An optional "file" may be specified to be loaded when the component is
 *   needed. A set of callbacks will be established based on the name of the
 *   component. All components follow the pattern:
 *
 *   _webform_[callback]_[component]
 *
 *   Where [component] is the name of the key of the component and [callback] is
 *   any of the following:
 *
 *     - defaults
 *     - theme
 *     - edit
 *     - delete
 *     - render
 *     - display
 *     - analysis
 *     - table
 *     - csv_headers
 *     - csv_data
 *
 * See the sample component implementation for details on each one of these
 * callbacks.
 *
 * @see webform_components()
 */
function hook_webform_component_info() {
  $components = array();

  $components['textfield'] = array(
    'label' => t('Textfield'),
    'description' => t('Basic textfield type.'),
    'features' => array(
      // Add content to CSV downloads. Defaults to TRUE.
      'csv' => TRUE,

      // Show this field in e-mailed submissions. Defaults to TRUE.
      'email' => TRUE,

      // Allow this field to be used as an e-mail FROM or TO address. Defaults
      // to FALSE.
      'email_address' => FALSE,

      // Allow this field to be used as an e-mail SUBJECT or FROM name. Defaults
      // to FALSE.
      'email_name' => TRUE,

      // This field may be toggled as required or not. Defaults to TRUE.
      'required' => TRUE,

      // This field has a title that can be toggled as displayed or not.
      'title_display' => TRUE,

      // This field has a title that can be displayed inline.
      'title_inline' => TRUE,

      // If this field can be used as a conditional SOURCE. All fields may
      // always be displayed conditionally, regardless of this setting.
      // Defaults to TRUE.
      'conditional' => TRUE,

      // If this field allows other fields to be grouped within it (like a
      // fieldset or tabs). Defaults to FALSE.
      'group' => FALSE,

      // If this field can be used for SPAM analysis, usually with Mollom.
      'spam_analysis' => FALSE,

      // If this field saves a file that can be used as an e-mail attachment.
      // Defaults to FALSE.
      'attachment' => FALSE,
    ),
    'file' => 'components/textfield.inc',
  );

  return $components;
}

/**
 * Alter the list of available Webform components.
 *
 * @param $components
 *   A list of existing components as defined by hook_webform_component_info().
 *
 * @see hook_webform_component_info()
 */
function hook_webform_component_info_alter(&$components) {
  // Completely remove a component.
  unset($components['grid']);

  // Change the name of a component.
  $components['textarea']['label'] = t('Text box');
}

/**
 * @}
 */

/**
 * @defgroup webform_component Sample Webform Component
 * @{
 * In each of these examples, the word "component" should be replaced with the,
 * name of the component type (such as textfield or select). These are not
 * actual hooks, but instead samples of how Webform integrates with its own
 * built-in components.
 */

/**
 * Specify the default properties of a component.
 *
 * @return
 *   An array defining the default structure of a component.
 */
function _webform_defaults_component() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'description' => '',
    ),
  );
}

/**
 * Generate the form for editing a component.
 *
 * Create a set of form elements to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they
 * may be overridden if desired).
 *
 * @param $component
 *   A Webform component array.
 * @return
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_component($component) {
  $form = array();

  // Disabling the description if not wanted.
  $form['description'] = array();

  // Most options are stored in the "extra" array, which stores any settings
  // unique to a particular component type.
  $form['extra']['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $component['extra']['options'],
    '#description' => t('Key-value pairs may be entered separated by pipes. i.e. safe_key|Some readable option') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -3,
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Render a Webform component to be part of a form.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   If editing an existing submission or resuming a draft, this will contain
 *   an array of values to be shown instead of the default in the component
 *   configuration. This value will always be an array, keyed numerically for
 *   each value saved in this field.
 */
function _webform_render_component($component, $value = NULL) {
  $form_item = array(
    '#type' => 'textfield',
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#default_value' => $component['value'],
    '#prefix' => '<div class="webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  if (isset($value)) {
    $form_item['#default_value'] = $value[0];
  }

  return $form_item;
}

/**
 * Display the result of a submission for a component.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 * @return
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
function _webform_display_component($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => $component['extra']['field_prefix'],
    '#field_suffix' => $component['extra']['field_suffix'],
    '#component' => $component,
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
  );
}

/**
 * A hook for changing the input values before saving to the database.
 *
 * Note that Webform will save the result of this function directly into the
 * database.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   The POST data associated with the user input.
 * @return
 *   An array of values to be saved into the database. Note that this should be
 *   a numerically keyed array.
 */
function _webform_submit_component($component, $value) {
  // Clean up a phone number into 123-456-7890 format.
  if ($component['extra']['phone_number']) {
    $matches = array();
    $number = preg_replace('[^0-9]', $value[0]);
    if (strlen($number) == 7) {
      $number = substr($number, 0, 3) . '-' . substr($number, 3, 4);
    }
    else {
      $number = substr($number, 0, 3) . '-' . substr($number, 3, 3) . '-' . substr($number, 6, 4);
    }
  }

  $value[0] = $number;
  return $value;
}

/**
 * Delete operation for a component or submission.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 */
function _webform_delete_component($component, $value) {
  // Delete corresponding files when a submission is deleted.
  $filedata = unserialize($value['0']);
  if (isset($filedata['filepath']) && is_file($filedata['filepath'])) {
    unlink($filedata['filepath']);
    db_query("DELETE FROM {files} WHERE filepath = '%s'", $filedata['filepath']);
  }
}

/**
 * Module specific instance of hook_help().
 *
 * This allows each Webform component to add information into hook_help().
 */
function _webform_help_component($section) {
  switch ($section) {
    case 'admin/settings/webform#grid_description':
      return t('Allows creation of grid questions, denoted by radio buttons.');
  }
}

/**
 * Module specific instance of hook_theme().
 *
 * This allows each Webform component to add information into hook_theme().
 */
function _webform_theme_component() {
  return array(
    'webform_grid' => array(
      'arguments' => array('grid_element' => NULL),
    ),
    'webform_mail_grid' => array(
      'arguments' => array('component' => NULL, 'value' => NULL),
    ),
  );
}

/**
 * Calculate and returns statistics about results for this component.
 *
 * This takes into account all submissions to this webform. The output of this
 * function will be displayed under the "Results" tab then "Analysis".
 *
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @param $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will
 *   be limited to these sids.
 * @param $single
 *   Boolean flag determining if the details about a single component are being
 *   shown. May be used to provided detailed information about a single
 *   component's analysis, such as showing "Other" options within a select list.
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_component($component, $sids = array(), $single = FALSE) {
  // Generate the list of options and questions.
  $options = _webform_component_options($component['extra']['options']);
  $questions = array_values(_webform_component_options($component['extra']['questions']));

  // Generate a lookup table of results.
  $sidfilter = count($sids) ? " AND sid in (" . db_placeholders($sids, 'int') . ")" : "";
  $query = 'SELECT no, data, count(data) as datacount '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND cid = %d '.
    " AND data != '' ". $sidfilter .
    ' GROUP BY no, data';
  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  $counts = array();
  while ($data = db_fetch_object($result)) {
    $counts[$data->no][$data->data] = $data->datacount;
  }

  // Create an entire table to be put into the returned row.
  $rows = array();
  $header = array('');

  // Add options as a header row.
  foreach ($options as $option) {
    $header[] = $option;
  }

  // Add questions as each row.
  foreach ($questions as $qkey => $question) {
    $row = array($question);
    foreach ($options as $okey => $option) {
      $row[] = !empty($counts[$qkey][$okey]) ? $counts[$qkey][$okey] : 0;
    }
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows, array('class' => 'webform-grid'));

  return array(array(array('data' => $output, 'colspan' => 2)));
}

/**
 * Return the result of a component value for display in a table.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Table".
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_table_component($component, $value) {
  $questions = array_values(_webform_component_options($component['extra']['questions']));
  $output = '';
  // Set the value as a single string.
  if (is_array($value)) {
    foreach ($value as $item => $value) {
      if ($value !== '') {
        $output .= $questions[$item] . ': ' . check_plain($value) . '<br />';
      }
    }
  }
  else {
    $output = check_plain(!isset($value['0']) ? '' : $value['0']);
  }
  return $output;
}

/**
 * Return the header for this component to be displayed in a CSV file.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param $component
 *   A Webform component array.
 * @param $export_options
 *   An array of options that may configure export of this field.
 * @return
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_component($component, $export_options) {
  $header = array();
  $header[0] = array('');
  $header[1] = array($component['name']);
  $items = _webform_component_options($component['extra']['questions']);
  $count = 0;
  foreach ($items as $key => $item) {
    // Empty column per sub-field in main header.
    if ($count != 0) {
      $header[0][] = '';
      $header[1][] = '';
    }
    // The value for this option.
    $header[2][] = $item;
    $count++;
  }

  return $header;
}

/**
 * Format the submitted data of a component for CSV downloading.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param $component
 *   A Webform component array.
 * @param $export_options
 *   An array of options that may configure export of this field.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   An array of items to be added to the CSV file. Each value within the array
 *   will be another column within the file. This function is called once for
 *   every row of data.
 */
function _webform_csv_data_component($component, $export_options, $value) {
  $questions = array_keys(_webform_select_options($component['extra']['questions']));
  $return = array();
  foreach ($questions as $key => $question) {
    $return[] = isset($value[$key]) ? $value[$key] : '';
  }
  return $return;
}

/**
 * @}
 */
