<?php
/**
 * Implementation of hook_form_alter() for spaces_features_form,
 * system_theme_settings.
 */
function _designkit_form_alter(&$form, &$form_state) {
  $info = designkit_get_info();

  // Allow uploads
  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  // Images
  if (!empty($info['designkit']['image'])) {
    $image = variable_get('designkit_image', array());
    $form['designkit_image'] = array(
      '#title' => t('Images'),
      '#description' => t('Upload an image. It will be resized to better fit the design of this site.'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => empty($image),
    );
    foreach ($info['designkit']['image'] as $name => $image_info) {
      $form['designkit_image'][$name] = array('#tree' => TRUE);
      if (!empty($image[$name]) && $file = db_fetch_object(db_query('SELECT * FROM {files} f WHERE f.fid = %d', $image[$name]))) {
        $form['designkit_image'][$name][$name] = array(
          '#type' => 'value',
          '#value' => $file,
          '#element_validate' => array('designkit_upload_delete'),
        );
        $form['designkit_image'][$name]['display'] = array(
          '#title' => isset($image_info['title']) ? t($image_info['title']) : check_plain($name),
          '#description' => isset($image_info['description']) ? t($image_info['description']) : '',
          '#type' => 'item',
          '#value' => theme('imagecache', "designkit-image-{$name}", $file->filepath),
        );
        $form['designkit_image'][$name]['delete'] = array(
          '#type' => 'checkbox',
          '#title' => t('Delete current image'),
        );
      }
      else {
        $form['designkit_image'][$name][$name] = array(
          '#title' => isset($image_info['title']) ? t($image_info['title']) : check_plain($name),
          '#description' => isset($image_info['description']) ? t($image_info['description']) : t('Upload a new image.'),
          '#type' => 'file',
          '#tree' => FALSE, // Uploads may not be nested.
          '#size' => 30,
          '#element_validate' => array('designkit_upload_validate'),
        );
        $form['designkit_image'][$name]['fid'] = array(
          '#type' => 'value',
          '#value' => !empty($image[$name]) ? $image[$name] : 0,
        );
      }
    }
  }

  // Color
  if (!empty($info['designkit']['color'])) {
    $color = variable_get('designkit_color', array());
    $form['designkit_color'] = array(
      '#title' => t('Colors'),
      '#description' => t('Enter an RGB hexidecimal value like <strong>#ffffff</strong>. Leave blank to use default colors.'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($color) || $color == $info['designkit']['color'],
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );
    foreach ($info['designkit']['color'] as $name => $color_info) {
      if (isset($color[$name]) && designkit_valid_color($color[$name])) {
        $default = $color[$name];
      }
      else {
        $default = isset($color_info['default']) ? $color_info['default'] : '#888888';
      }
      $form['designkit_color'][$name] = array(
        '#title' => isset($color_info['title']) ? t($color_info['title']) : check_plain($name),
        '#description' => isset($color_info['description']) ? t($color_info['description']) : '',
        '#attributes' => array('class' => 'designkit-colorpicker'),
        '#element_validate' => array('designkit_validate_color'),
        '#theme' => 'designkit_colorpicker',
        '#type' => 'textfield',
        '#size' => '7',
        '#maxlength' => '7',
        '#default_value' => $default,
      );
    }
  }
}

/**
 * Submit handler for system_theme_settings.
 */
function _designkit_system_theme_settings_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    variable_del('designkit_image');
    variable_del('designkit_color');
  }
  else {
    if (isset($form_state['values']['designkit_image'])) {
      variable_set('designkit_image', $form_state['values']['designkit_image']);
    }
    if (isset($form_state['values']['designkit_color'])) {
      variable_set('designkit_color', $form_state['values']['designkit_color']);
    }
  }
}

/**
 * Element validate callback for color setting.
 */
function designkit_validate_color($element, &$form_state) {
  $color = trim($element['#value']);
  if (empty($color) || $color === '#' || designkit_valid_color($color)) {
    return TRUE;
  }
  form_set_error($element['#name'], t('Please enter a valid color or leave blank.'));
}

/**
 * Element validator for image delete. Handles file delete and value setting
 * tasks all at once.
 */
function designkit_upload_delete($element, &$form_state) {
  $name = end($element['#parents']);

  if (isset($form_state['values']['designkit_image'][$name][$name])) {
    // Delete the old image.
    if (!empty($form_state['values']['designkit_image'][$name]['delete'])) {
      $old_file = $form_state['values']['designkit_image'][$name][$name];
      if (file_exists($old_file->filepath)) {
        file_delete($old_file->filepath);
        imagecache_image_flush($old_file->filepath);
      }
      if ($old_file->fid) {
        db_query('DELETE FROM {files} WHERE fid = %d', $old_file->fid);
      }
      unset($form_state['values']['designkit_image'][$name]);
    }
    // Retain the image fid.
    else {
      $form_state['values']['designkit_image'][$name] = $form_state['values']['designkit_image'][$name][$name]->fid;
    }
  }
}

/**
 * Element validator for image upload. Handles file creation and value setting
 * tasks all at once.
 */
function designkit_upload_validate($element, &$form_state) {
  $name = end($element['#parents']);

  // Clean out upload values.
  unset($form_state['values'][$name]);

  // Save uploaded file.
  $validators = array(
    'file_validate_is_image' => array(),
    'file_validate_image_resolution' => array('3000x3000'),
    'file_validate_size' => array(1000 * 1024),
  );
  $file = file_save_upload($name, $validators, file_directory_path());
  $error = $file ? file_validate_is_image($file) : FALSE;

  // Save the new file & settings.
  if ($file && !$error) {
    file_set_status($file, 1);
    imagecache_image_flush($file->filepath);
    $form_state['values']['designkit_image'][$name] = $file->fid;
    // @TODO: Implement an autocolor key in the designkitinfo file spec
    // so that images can be related to colors.
    // $form_state['values']['designkit_color'] =
    //   designkit_image_autocolor($file->filepath) ?
    //   designkit_image_autocolor($file->filepath) :
    //   $form_state['values']['designkit_color'];
  }
  // There was an error.
  else {
    if ($error) {
      form_set_error($name, $error);
    }
    if ($file) {
      file_delete($file->filepath);
      db_query('DELETE FROM {files} WHERE fid = %d', $file->fid);
    }
    $form_state['values']['designkit_image'][$name] = 0;
  }
}

/**
 * Attempt to retrieve a suitable background color value from an image.
 */
/*
function designkit_image_autocolor($filepath) {
  // Do additional handling post-save
  $image = imageapi_image_open($filepath);
  $toolkit = variable_get('imageapi_image_toolkit', 'imageapi_gd');

  // Currently we only handle background color selection through the GD library.
  $autocolor = '';
  if ($toolkit == 'imageapi_gd' && !empty($image->resource)) {
    $raw = array();
    $raw['nw'] = imagecolorat($image->resource, 0, 0);
    $raw['ne'] = imagecolorat($image->resource, $image->info['width'] - 1, 0);
    $raw['se'] = imagecolorat($image->resource, $image->info['width'] - 1, $image->info['height'] - 1);
    $raw['sw'] = imagecolorat($image->resource, 0, $image->info['height'] - 1);

    $colors = array();
    foreach ($raw as $k => $index) {
      $rgb = imagecolorsforindex($image->resource, $index);

      $color = array();
      $color[] = str_pad(dechex($rgb['red']), 2, '0', STR_PAD_LEFT);
      $color[] = str_pad(dechex($rgb['green']), 2, '0', STR_PAD_LEFT);
      $color[] = str_pad(dechex($rgb['blue']), 2, '0', STR_PAD_LEFT);
      $color = "#". implode('', $color);

      $colors[$color] = $colors[$color] + 1;
    }
    $max = 1;
    $excluded = array('#ffffff', '#000000');
    foreach ($colors as $color => $count) {
      $unpacked = _color_unpack($color, TRUE);
      $hsl = _color_rgb2hsl($unpacked);

      if ($count > $max && !in_array($color, $excluded) && $hsl[2] < .95 &&  $hsl[2] > .05) {
        $autocolor = $color;
      }
    }
  }
  return $autocolor;
}
*/
