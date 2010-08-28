<?php
// $Id: reference.inc,v 1.6 2010/08/09 04:11:49 quicksketch Exp $

/**
 * @file
 * A FileField extension to allow referencing of existing files.
 *
 * The "hooks" in this file are not true hooks, they're called individually
 * from the main filefield_sources.module in the corresponding hook by the
 * same name. Any of these hooks could be broken out into a separate module.
 */

define('FILEFIELD_SOURCE_REFERENCE_HINT_TEXT', 'example.png [fid:123]');

/**
 * Implementation of hook_filefield_source_info().
 */
function filefield_source_reference_info() {
  $source = array();
  $source['reference'] = array(
    'name' => t('Autocomplete reference textfield'),
    'label' => t('Reference existing'),
    'description' => t('Reuse an existing file by entering its file name.'),
    'process' => 'filefield_source_reference_process',
    'value' => 'filefield_source_reference_value',
    'weight' => 1,
  );
  return $source;
}

/**
 * Implementation of hook_menu().
 */
function filefield_source_reference_menu() {
  $items = array();

  $items['filefield/reference/%/%'] = array(
    'page callback' => 'filefield_source_reference_autocomplete',
    'page arguments' => array(2, 3),
    'access callback' => 'filefield_edit_access',
    'access arguments' => array(2, 3),
    'file' => 'sources/reference.inc',
    'type' => MENU_CALLBACK,
  );
  return $items;
}

/**
 * Implementation of hook_theme().
 */
function filefield_source_reference_theme() {
  return array(
    'filefield_source_reference_element' => array(
      'arguments' => array('element' => NULL),
      'file' => 'sources/reference.inc',
    ),
    'filefield_source_reference_autocomplete_item' => array(
      'arguments' => array('file' => NULL),
      'file' => 'sources/reference.inc',
    ),
 );
}

/**
 * Implementation of hook_filefield_source_settings().
 */
function filefield_source_reference_settings($op, $field) {
  $return = array();

  if ($op == 'form') {
    $return['sources_reference'] = array(
      '#title' => t('Autocomplete reference options'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $return['sources_reference']['filefield_source_autocomplete'] = array(
      '#title' => t('Match file name'),
      '#options' => array(
        '0' => t('Starts with string'),
        '1' => t('Contains string'),
      ),
      '#type' => 'radios',
      '#default_value' => empty($field['filefield_source_autocomplete']) ? '0' : '1', 
    );
  }
  elseif ($op == 'save') {
    $return[] = 'filefield_source_autocomplete';
  }

  return $return;
}

/**
 * A #process callback to extend the filefield_widget element type.
 */
function filefield_source_reference_process($element, $edit, &$form_state, $form) {

  $element['filefield_reference'] = array(
    '#theme' => 'filefield_source_reference_element',
    '#weight' => 100.5,
    '#access' => empty($element['fid']['#value']),
    '#filefield_sources_hint_text' => FILEFIELD_SOURCE_REFERENCE_HINT_TEXT,
  );

  $element['filefield_reference']['autocomplete'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'filefield/reference/' . $element['#type_name'] . '/' . $element['#field_name'],
    '#description' => filefield_sources_element_validation_help($element['#upload_validators']),
  );

  $element['filefield_reference']['select'] = array(
    '#type' => 'submit',
    '#value' => t('Select'),
    '#submit' => array('node_form_submit_build_node'),
    '#name' => $element['#name'] . '[filefield_reference][button]',
    '#ahah' => array(
       'path' => 'filefield/ahah/'. $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'],
       'wrapper' => $element['#id'] .'-ahah-wrapper',
       'method' => 'replace',
       'effect' => 'fade',
    ),
  );

  return $element;
}

/**
 * A #filefield_value_callback function.
 */
function filefield_source_reference_value($element, &$item) {
  if (isset($item['filefield_reference']['autocomplete']) && strlen($item['filefield_reference']['autocomplete']) > 0 && $item['filefield_reference']['autocomplete'] != FILEFIELD_SOURCE_REFERENCE_HINT_TEXT) {
    $matches = array();
    if (preg_match('/\[fid:(\d+)\]/', $item['filefield_reference']['autocomplete'], $matches)) {
      $fid = $matches[1];
      if ($file = field_file_load($fid)) {
        if (filefield_sources_element_validate($element, (object) $file)) {
          $item = array_merge($item, $file);
        }
      }
      else {
        form_error($element, t('The referenced file could not be used because the file does not exist in the database.'));
      }
    }
    // No matter what happens, clear the value from the autocomplete.
    $item['filefield_reference']['autocomplete'] = '';
  }
}

/**
 * Menu callback; autocomplete.js callback to return a list of files.
 */
function filefield_source_reference_autocomplete($type_name, $field_name, $filename) {
  $field = content_fields($field_name, $type_name);

  $items = array();
  if (!empty($field)) {
    $files = filefield_source_reference_get_files($filename, $field);
    foreach ($files as $fid => $file) {
      $items[$file->filename ." [fid:$fid]"] = theme('filefield_source_reference_autocomplete_item', $file);
    }
  }

  drupal_json($items);
}

/**
 * Theme the output of a single item in the autocomplete list.
 */
function theme_filefield_source_reference_autocomplete_item($file) {
  $output = '';
  $output .= '<div class="filefield-source-reference-item">';
  $output .= '<span class="filename">' . $file->filename . '</span> <span class="filesize">(' . format_size($file->filesize) . ')</span>';
  $output .= '</div>';
  return $output;
}

/**
 * Theme the output of the autocomplete field.
 */
function theme_filefield_source_reference_element($element) {
  $element['autocomplete']['#field_suffix'] = theme('submit', $element['select']);
  return '<div class="filefield-source filefield-source-reference clear-block">' . theme('textfield', $element['autocomplete']) . '</div>';
}

/**
 * Get all the files used within a particular field (or all fields).
 *
 * @param $file_name
 *   The partial name of the file to retrieve.
 * @param $field
 *   Optional. A CCK field array for which to filter returned files.
 */
function filefield_source_reference_get_files($filename, $field = NULL) {
  if (!isset($field)) {
    foreach (content_fields() as $field) {
      if ($field['type'] == 'filefield') {
        $fields[] = $field;
      }
    }
  }
  else {
    $fields = array($field);
  }

  $files = array();
  foreach ($fields as $field) {
    $db_info = content_database_info($field);

    // 1 == contains, 0 == starts with.
    $like = empty($field['widget']['filefield_source_autocomplete']) ? '%s%%' : '%%%s%%';
    $result = db_query_range("SELECT f.* FROM {" . $db_info['table'] . "} c INNER JOIN {files} f ON c." . $db_info['columns']['fid']['column'] . " = f.fid WHERE f.filename LIKE '" . $like . "' AND f.status = 1 ORDER BY f.timestamp DESC", $filename, 0, 30);

    while ($file = db_fetch_object($result)) {
      $files[$file->fid] = $file;
    }
  }

  return $files;
}
