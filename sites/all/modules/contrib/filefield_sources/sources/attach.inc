<?php
// $Id: attach.inc,v 1.2 2010/08/09 15:06:50 quicksketch Exp $

/**
 * @file
 * A FileField extension to allow use of files within a server directory.
 *
 * The "hooks" in this file are not true hooks, they're called individually
 * from the main filefield_sources.module in the corresponding hook by the
 * same name. Any of these hooks could be broken out into a separate module.
 */

/**
 * Implementation of hook_filefield_source_info().
 */
function filefield_source_attach_info() {
  $source = array();
  $source['attach'] = array(
    'name' => t('File attach from server directory'),
    'label' => t('File attach'),
    'description' => t('Select a file from a directory on the server.'),
    'process' => 'filefield_source_attach_process',
    'value' => 'filefield_source_attach_value',
    'weight' => 3,
  );
  return $source;
}

/**
 * Implementation of hook_theme().
 */
function filefield_source_attach_theme() {
  return array(
    'filefield_source_attach_element' => array(
      'arguments' => array('element' => NULL),
      'file' => 'sources/attach.inc',
    ),
  );
}

/**
 * Implementation of hook_filefield_source_settings().
 */
function filefield_source_attach_settings($op, $field) {
  $return = array();

  if ($op == 'form') {
    $return['source_attach'] = array(
      '#title' => t('File attach settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('File attach allows for selecting a file from a directory on the server, commonly used in combination with FTP.') . ' <strong>' . t('This file source will ignore file size checking when used.') . '</strong>',
      '#element_validate' => array('_filefield_source_attach_file_path_validate'),
      '#weight' => 3,
    );
    $return['source_attach']['filefield_source_attach_path'] = array(
      '#type' => 'textfield',
      '#title' => t('File attach path'),
      '#default_value' => empty($field['filefield_source_attach_path']) ? 'file_attach' : $field['filefield_source_attach_path'],
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('The directory within the <em>File attach location</em> that will contain attachable files.'),
    );
    $return['source_attach']['filefield_source_attach_absolute'] = array(
      '#type' => 'radios', 
      '#title' => t('File attach location'),
      '#options' => array(
        0 => t('Within the files directory'),
        1 => t('Absolute server path'),
      ),
      '#default_value' => isset($field['filefield_source_attach_absolute']) ? $field['filefield_source_attach_absolute'] : 0,
      '#description' => t('The <em>File attach path</em> may be with the files directory (%file_directory) or from the root of your server. If an absolute path is used and it does not start with a "/" your path will be relative to your site directory: %realpath.', array('%file_directory' => file_directory_path(), '%realpath' => realpath('./'))),
    );
    $return['source_attach']['filefield_source_attach_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Attach method'),
      '#options' => array(
        'move' => t('Move the file directly to the final location'),
        'copy' => t('Leave a copy of the file in the attach directory'),
      ),
      '#default_value' => isset($field['filefield_source_attach_mode']) ? $field['filefield_source_attach_mode'] : 'move',
    );
    $return['source_attach']['tokens'] = array(
      '#type' => 'markup',
      '#value' => theme('token_help', 'user'),
    );
  }
  elseif ($op == 'save') {
    $return[] = 'filefield_source_attach_path';
    $return[] = 'filefield_source_attach_absolute';
    $return[] = 'filefield_source_attach_mode';
  }

  return $return;
}

function _filefield_source_attach_file_path_validate($element, &$form_state) {
  // Strip slashes from the end of the file path.
  $filepath = rtrim($element['filefield_source_attach_path']['#value'], '\\/');
  form_set_value($element['filefield_source_attach_path'], $filepath, $form_state);

  $filepath = _filefield_source_attach_directory($form_state['values']);

  // Check that the directory exists and is writable.
  if (!field_file_check_directory($filepath, FILE_CREATE_DIRECTORY)) {
    form_error($element['filefield_source_attach_path'], t('Specified file attach path must exist or be writable.'));
  }
}

/**
 * A #process callback to extend the filefield_widget element type.
 */
function filefield_source_attach_process($element, $edit, &$form_state, $form) {
  $field = content_fields($element['#field_name'], $element['#type_name']);

  $element['filefield_attach'] = array(
    '#theme' => 'filefield_source_attach_element',
    '#weight' => 100.5,
    '#access' => empty($element['fid']['#value']),
  );

  $path = _filefield_source_attach_directory($field['widget']);
  $options = _filefield_source_attach_options($path);

  $description = t('This method may be used to attach files that exceed the file size limit. Files may be attached from the %directory directory on the server, usually uploaded through FTP.', array('%directory' => realpath($path)));

  // Error messages.
  if ($options === FALSE || empty($field['widget']['filefield_source_attach_path'])) {
    $attach_message = t('A file attach directory could not be located.');
    $attach_description = t('Please check your settings for the %field field.',  array('%field' => $field['widget']['label']));
  }
  elseif (!count($options)) {
    $attach_message = t('There currently no files to attach.');
    $attach_description = $description;
  }

  if (isset($attach_message)) {
    $element['filefield_attach']['attach_message'] = array(
      '#value' => $attach_message,
    );
    $element['filefield_attach']['#description'] = $attach_description;
  }
  else {
    $validators = $element['#upload_validators'];
    if (isset($validators['filefield_validate_size'])) {
      unset($validators['filefield_validate_size']);
    }
    $description .= '<br />' . filefield_sources_element_validation_help($validators);
    $element['filefield_attach']['filename'] = array(
      '#type' => 'select',
      '#options' => $options,
    );
    $element['filefield_attach']['#description'] = $description;
  }

  $element['filefield_attach']['attach'] = array(
    '#type' => 'submit',
    '#value' => $attach_message ? t('Refresh') : t('Attach'),
    '#submit' => array('node_form_submit_build_node'),
    '#ahah' => array(
       'path' => 'filefield/ahah/'. $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'],
       'wrapper' => $element['#id'] .'-ahah-wrapper',
       'method' => 'replace',
       'effect' => 'fade',
    ),
  );

  return $element;
}

function _filefield_source_attach_options($path) {
  if (!field_file_check_directory($path, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Specified file attach path must exist or be writable.'), 'error');
    return FALSE;
  }

  $options = array();
  $file_attach = file_scan_directory($path, '.*', array('.', '..', 'CVS', '.svn'), 0, TRUE, 'filename', 0, 0);

  if (count($file_attach)) {
    $options = array('' => t('-- Select file --'));
    foreach ($file_attach as $filename => $fileinfo) {
      $filename = basename($filename);
      $options[$filename] = $filename;
    }
  }

  natcasesort($options);
  return $options;
}

/**
 * A #filefield_value_callback function.
 */
function filefield_source_attach_value($element, &$item) {
  if (!empty($item['filefield_attach']['filename'])) {
    $field = content_fields($element['#field_name'], $element['#type_name']);
    $attach_path = _filefield_source_attach_directory($field['widget']);
    $filepath = $attach_path . '/' . $item['filefield_attach']['filename'];

    // Clean up the file name extensions and transliterate.
    $original_filepath = $filepath;
    $new_filepath = filefield_sources_clean_filename($filepath);
    rename($filepath, $new_filepath);
    $filepath = $new_filepath;

    // Run all the normal validations, minus file size restrictions.
    $validators = $element['#upload_validators'];
    if (isset($validators['filefield_validate_size'])) {
      unset($validators['filefield_validate_size']);
    }

    // Save the file to the new location.
    if ($file = field_file_save_file($filepath, $validators, filefield_widget_file_path($field))) {
      $item = array_merge($item, $file);

      // Delete the original file if "moving" the file instead of copying.
      if (empty($field['widget']['filefield_source_attach_mode']) || $field['widget']['filefield_source_attach_mode'] !== 'copy') {
        @unlink($filepath);
      }
    }

    // Restore the original file name if the file still exists.
    if (file_exists($filepath) && $filepath != $original_filepath) {
      rename($filepath, $original_filepath);
    }
  }

  $item['filefield_attach']['filename'] = '';
}

/**
 * Theme the output of the autocomplete field.
 */
function theme_filefield_source_attach_element($element) {
  if (isset($element['attach_message'])) {
    $output = $element['attach_message']['#value'];
  }
  else {
    $select = '';
    $size = $element['#size'] ? ' size="'. $element['filename']['#size'] .'"' : '';
    _form_set_class($element['filename'], array('form-select'));
    $multiple = $element['#multiple'];
    $output = '<select name="'. $element['filename']['#name'] .''. ($multiple ? '[]' : '') .'"'. ($multiple ? ' multiple="multiple" ' : '') . drupal_attributes($element['filename']['#attributes']) .' id="'. $element['filename']['#id'] .'" '. $size .'>'. form_select_options($element['filename']) .'</select>';
  }
  $output .= theme('submit', $element['attach']);
  $element['#type'] = 'item';
  return '<div class="filefield-source filefield-source-attach clear-block">' . theme('form_element', $element, $output) . '</div>';
}

function _filefield_source_attach_directory($field, $account = NULL) {
  $account = isset($account) ? $account : $GLOBALS['user'];  
  $path = $field['filefield_source_attach_path'];
  $absolute = !empty($field['filefield_source_attach_absolute']);

  // Replace user level tokens.
  // Node level tokens require a lot of complexity like temporary storage
  // locations when values don't exist. See the filefield_paths module.
  if (module_exists('token')) {
    $path = token_replace($path, 'user', $account);
  }

  return $absolute ? $path : file_directory_path() . '/' . $path;
}
