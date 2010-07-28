<?php
// $Id:$

// Copyright (c) 2010 KontextWork
// Author: Eugen Mayer

/**
 * Providing a administration interface for tagging.
 */
function wysiwyg_imageupload_admin_settings() {
  $form['wysiwyg_imageupload_skip_dependencies_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do the requirements checks'),
    '#description' => t('If this is set, requirements checks will be done. If something is not correct, you get a anoying error message!'),
    '#default_value' => variable_get('wysiwyg_imageupload_skip_dependencies_check', true),
  );
  $form['styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles'),
    '#description' => t('Set the available styles (css) here'),
    '#collapsed' => true,
    '#collapsible' => true,
    '#weight' => -2,
  );
  $form['styles']['wysiwyg_imageupload_imgstyles'] = array(
    '#type' => 'textarea',
    '#title' => t('Image style'),
    '#description' => t('Define which CSS styles the user can chose when adding an image. Please do not define floating or size in that CSS classes, as this is handled seperately. You can give the style a descriptive name to make it easier to handle. The syntax is "Descriptive Name=imgupl_style_XYC". You _must_ prefix the style with imgupl_style_*! For each style use on separate line.'),
    '#default_value' => variable_get('wysiwyg_imageupload_imgstyles', '')
  );

  $presets = imagecache_presets();
  $styles = array();
  foreach ($presets as $preset) {
    // We can use the presetid here (http://drupal.org/node/694188).
    $styles[$preset['presetname']] = t("!preset", array('!preset' => $preset['presetname']));
  }

  $form['wysiwyg_imageupload_allow_presets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed presets'),
    '#description' => t('Only the selected presets are allowed to be chosen in the dialog'),
    '#collapsed' => true,
    '#collapsible' => true,
    '#weight' => -2,
  );

  $form['wysiwyg_imageupload_allow_presets']['wysiwyg_imageupload_presets_whitelist'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Presets'),
    '#default_value' => variable_get('wysiwyg_imageupload_presets_whitelist', $styles),
    '#options' => $styles
  );

  $form['wysiwyg_imageupload_destdir_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload destination'),
    '#description' => t('Upload path for the images'),
    '#collapsed' => true,
    '#collapsible' => true,
    '#weight' => -1,
  );
  $form['wysiwyg_imageupload_destdir_group']['wysiwyg_imageupload_dest_root'] = array(
    '#type' => 'textfield',
    '#title' => t('Root destination'),
    '#description' => t('This is the root destination relative to the current files directory ("'.file_directory_path().'"). Neither insert a preceding nor successive slash.'),
    '#default_value' => variable_get('wysiwyg_imageupload_dest_root', 'wysiwyg_imageupload')
  );

  $form['wysiwyg_imageupload_destdir_group']['wysiwyg_imageupload_relative_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Subdirectory for private uploads'),
    '#description' => t('This is the user-files destination relative to the current wysiwyg_imageupload root destination(see above) directory (e.g. "'.file_directory_path().'/rootdest/$uid"). $uid will be replaced by the current user-id. Keep blanc to save it in the root.'),
    '#default_value' => variable_get('wysiwyg_imageupload_relative_user', '$uid')
  );

  return system_settings_form($form);
}