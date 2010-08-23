<?php
// $Id: remote.inc,v 1.9 2010/08/09 04:11:50 quicksketch Exp $

/**
 * @file
 * A FileField extension to allow referencing of existing files.
 *
 * The "hooks" in this file are not true hooks, they're called individually
 * from the main filefield_sources.module in the corresponding hook by the
 * same name. Any of these hooks could be broken out into a separate module.
 */

define('FILEFIELD_SOURCE_REMOTE_HINT_TEXT', 'http://example.com/files/file.png');

/**
 * Implementation of hook_filefield_source_info().
 */
function filefield_source_remote_info() {
  $source = array();
  $source['remote'] = array(
    'name' => t('Remote URL textfield'),
    'label' => t('Remote URL'),
    'description' => t('Download a file from a remote server.'),
    'process' => 'filefield_source_remote_process',
    'value' => 'filefield_source_remote_value',
  );
  return $source;
}

/**
 * Implementation of hook_menu().
 */
function filefield_source_remote_menu() {
  $items = array();

  $items['filefield/remote/progress/%/%/%'] = array(
    'page callback' => 'filefield_source_remote_progress',
    'page arguments' => array(3, 4, 5),
    'access arguments' => array('access content'),
    'file' => 'sources/remote.inc',
    'type' => MENU_CALLBACK,
  );
  return $items;
}

/**
 * Implementation of hook_theme().
 */
function filefield_source_remote_theme() {
  return array(
    'filefield_source_remote_element' => array(
      'arguments' => array('element' => NULL),
      'file' => 'sources/remote.inc',
    ),
 );
}

/**
 * Implementation of hook_filefield_source_settings().
 */
function filefield_source_remote_settings($op, $field) {
  $return = array();

  // Add settings to the FileField widget form.

  return $return;

}

/**
 * A #process callback to extend the filefield_widget element type.
 */
function filefield_source_remote_process($element, $edit, &$form_state, $form) {

  $element['filefield_remote'] = array(
    '#theme' => 'filefield_source_remote_element',
    '#weight' => 100.5,
    '#access' => empty($element['fid']['#value']),
    '#filefield_sources_hint_text' => FILEFIELD_SOURCE_REMOTE_HINT_TEXT,
  );

  $element['filefield_remote']['url'] = array(
    '#type' => 'textfield',
    '#description' => filefield_sources_element_validation_help($element),
    '#maxlength' => NULL,
  );

  $element['filefield_remote']['transfer'] = array(
    '#type' => 'submit',
    '#value' => t('Transfer'),
    '#submit' => array('node_form_submit_build_node'),
    '#ahah' => array(
       'path' => 'filefield/ahah/'. $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'],
       'wrapper' => $element['#id'] .'-ahah-wrapper',
       'method' => 'replace',
       'effect' => 'fade',
       'progress' => array(
         'type' => 'bar',
         'path' => 'filefield/remote/progress/' . $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'],
         'message' => t('Starting transfer...'),
       )
    ),
  );

  return $element;
}

/**
 * A #filefield_value_callback function.
 */
function filefield_source_remote_value($element, &$item) {
  if (isset($item['filefield_remote']['url']) && strlen($item['filefield_remote']['url']) > 0 && valid_url($item['filefield_remote']['url']) && $item['filefield_remote']['url'] != FILEFIELD_SOURCE_REMOTE_HINT_TEXT) {
    $field = content_fields($element['#field_name'], $element['#type_name']);
    $url = $item['filefield_remote']['url'];

    // Check the headers to make sure it exists and is within the allowed size.
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, TRUE);
    curl_setopt($ch, CURLOPT_NOBODY, TRUE);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    // Causes a warning if PHP safe mode is on.
    @curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_exec($ch);
    $info = curl_getinfo($ch);
    curl_close($ch);

    if ($info['http_code'] != 200) {
      switch ($info['http_code']) {
        case 403:
          form_error($element, t('The remote file could not be transfered because access to the file was denied.'));
          break;
        case 404:
          form_error($element, t('The remote file could not be transfered because it was not found.'));
          break;
        default:
          form_error($element, t('The remote file could not be transfered due to an HTTP error (@code).', array('@code' => $info['http_code'])));
      }
      return;
    }

    // Update the $url variable to reflect any redirects.
    $url = $info['url'];
    $url_info = parse_url($url);
    $pathinfo = pathinfo($url_info['path']);
    $filename = rawurldecode(basename($url_info['path']));
    $filename = filefield_sources_clean_filename($filename);
    $filepath = file_create_filename($filename, file_directory_temp());

    if (empty($pathinfo['extension'])) {
      form_error($element, t('The remote URL must be a file and have an extension.'));
      return;
    }

    // Perform basic extension check on the file before trying to transfer.
    $extensions = $field['widget']['file_extensions'];
    $regex = '/\.('. ereg_replace(' +', '|', preg_quote($extensions)) .')$/i';
    if (!empty($extensions) && !preg_match($regex, $filename)) {
      form_error($element, t('Only files with the following extensions are allowed: %files-allowed.', array('%files-allowed' => $extensions)));
      return;
    }

    // Check file size based off of header information.
    if (!empty($element['#upload_validators']['filefield_validate_size'][0])) {
      $max_size = $element['#upload_validators']['filefield_validate_size'][0];
      $file_size = $info['download_content_length'];
      if ($file_size > $max_size) {
        form_error($element, t('The remote file is %filesize exceeding the maximum file size of %maxsize.', array('%filesize' => format_size($file_size), '%maxsize' => format_size($max_size))));
        return;
      }
    }

    // Set progress bar information.
    $options = array(
      'key' => $element['#type_name'] . '_' . $element['#field_name'] . '_' . $element['#delta'],
      'filepath' => $filepath,
    );
    filefield_source_remote_set_transfer_options($options);

    // Then make the actual request to download the file.
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, FALSE);
    curl_setopt($ch, CURLOPT_WRITEFUNCTION, 'filefield_source_remote_curl_write');
    // Causes a warning if PHP safe mode is on.
    @curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    if (curl_exec($ch) && $file = field_file_save_file($filepath, $element['#upload_validators'], filefield_widget_file_path($field))) {
      $item = array_merge($item, $file);
    }
    curl_close($ch);

    // Delete the temporary file.
    @unlink($filepath);
  }
}

/**
 * Menu callback; progress.js callback to return upload progress.
 */
function filefield_source_remote_progress($type_name, $field_name, $delta) {
  $key = $type_name . '_' . $field_name . '_' . $delta;
  $progress = array(
    'message' => t('Starting transfer...'),
    'percentage' => -1,
  );

  if ($cache = cache_get('filefield_transfer:'. session_id() . ':' . $key)) {
    $current = $cache->data['current'];
    $total = $cache->data['total'];
    $progress['message'] = t('Transfering... (@current of @total)', array('@current' => format_size($current), '@total' => format_size($total)));
    $progress['percentage'] = round(100 * $current / $total);
  }

  drupal_json($progress);
}

/**
 * cURL write function to save the file to disk. Also updates progress bar.
 */
function filefield_source_remote_curl_write(&$ch, $data) {
  $progress_update = 0;
  $options = filefield_source_remote_get_transfer_options();

  // Get the current progress and update the progress value.
  // Only update every 64KB to reduce cache_set calls. cURL usually writes
  // in 16KB chunks.
  if (curl_getinfo($ch, CURLINFO_SIZE_DOWNLOAD) / 65536 > $progress_update) {
    $progress_update++;
    $progress = array(
      'current' => curl_getinfo($ch, CURLINFO_SIZE_DOWNLOAD),
      'total' => curl_getinfo($ch, CURLINFO_CONTENT_LENGTH_DOWNLOAD),
    );
    // Set a cache so that we can retrieve this value from the progress bar.
    $cid = 'filefield_transfer:'. session_id() . ':' . $options['key'];
    if ($progress['current'] != $progress['total']) {
      cache_set($cid, $progress, 'cache', time() + 300);
    }
    else {
      cache_clear_all($cid, 'cache');
    }
  }

  $data_length = 0;
  if ($fp = @fopen($options['filepath'], 'a')) {
    fwrite($fp, $data);
    fclose($fp);
    $data_length = strlen($data);
  }

  return $data_length;
}

/**
 * Set a transfer key that can be retreived by the progress function.
 */
function filefield_source_remote_set_transfer_options($options = NULL) {
  static $current = FALSE;
  if (isset($options)) {
    $current = $options;
  }
  return $current;
}

/**
 * Get a transfer key that can be retrieved by the progress function.
 */
function filefield_source_remote_get_transfer_options() {
  return filefield_source_remote_set_transfer_options();
}

/**
 * Theme the output of the autocomplete field.
 */
function theme_filefield_source_remote_element($element) {
  $element['url']['#field_suffix'] = theme('submit', $element['transfer']);
  return '<div class="filefield-source filefield-source-remote clear-block">' . theme('textfield', $element['url']) . '</div>';
}

