<?php
// $Id: media_vimeo.admin.inc,v 1.1 2010/09/17 16:16:33 aaron Exp $

/**
 * @file media_vimeo/includes/media_vimeo.admin.inc
 * Administrative functions for Media: Vimeo.
 */

/**
 * The administrative settings form for Media: Vimeo.
 */
function media_vimeo_settings() {
  $form = media_vimeo_admin_form();
  $form['settings_info'] = array(
    '#type' => 'item',
    '#value' => t('These settings specifically affect videos displayed from !vimeo. You can also read more about its !api.', array('!vimeo' => l(t('Vimeo.com'), MEDIA_VIMEO_MAIN_URL, array('attributes' => array('target' => '_blank'))), '!api' => l(t("developer's API"), MEDIA_VIMEO_API_INFO, array('attributes' => array('target' => '_blank'))))),
    '#weight' => -10,
  );
  return system_settings_form($form);
}

/**
 * This form will be displayed both at /admin/settings/media_vimeo and
 * admin/content/emfield.
 */
function media_vimeo_admin_form() {
  $form['color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player color'),
    '#description' => t('If allowed, this color, in hexidecimal form (#RRGGBB), will be used to change the skin of the Vimeo player.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['color'][media_vimeo_variable_name('color_override')] = array(
    '#type' => 'checkbox',
    '#title' => t('Override player color'),
    '#default_value' => media_vimeo_variable_get('color_override'),
  );
  $form['color'][media_vimeo_variable_name('color')] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => media_vimeo_variable_get('color'),
  );
  $form['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['vimeo']['player_options'][media_vimeo_variable_name('universal')] = array(
    '#type' => 'checkbox',
    '#title' => t('Embed the Universal player'),
    '#description' => t('Checking this box will embed the player in an iFrame that uses HTML5 for display.'),
    '#default_value' => media_vimeo_variable_get('universal'),
  );
  $form['player_options'][media_vimeo_variable_name('on_screen_info')] = array(
    '#type' => 'checkboxes',
    '#title' => t('On-screen info'),
    '#default_value' => media_vimeo_variable_get('on_screen_info'),
  '#options' => array(
      'portrait' => t("Show video author's portrait"),
      'title' => t('Show video title'),
      'byline' => t('Show byline'),
    ),
    '#description' => t('Provide additional video information on the Vimeo player.'),
  );
  $form['player_options'][media_vimeo_variable_name('full_screen')] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
  '#default_value' => media_vimeo_variable_get('full_screen'),
  '#description' => t('Allow users to view video using the entire computer screen.'),
  );
  $form[media_vimeo_variable_name('api_key')] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo API Key'),
    '#default_value' => media_vimeo_variable_get('api_key'),
  );
  $form[media_vimeo_variable_name('api_secret')] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo API Shared Secret'),
    '#default_value' => media_vimeo_variable_get('api_secret'),
  );
  $form[media_vimeo_variable_name('thumb_size')] = array(
    '#type' => 'select',
    '#title' => t('Vimeo Thumbnail Size'),
    '#options' => array('96' => '96', '100' => '100', '160' => '160', '200' => '200', '460' => '460'),
    '#default_value' => media_vimeo_variable_get('thumb_size'),
  );
  return $form;
}
