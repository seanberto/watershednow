<?php
// $Id: jquery_ui_dialog.admin.inc,v 1.1.2.8 2010/07/01 14:31:20 eugenmayer Exp $

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer
/*
 * Providing a administration interface for tagging.
 */
function jquery_ui_dialog_admin_settings() {
  $form['jquery_ui_dialog_default_css'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include the default dialog styles'),
      '#description' => t('Include default dialogs styles and override your theme ones (White background). If you disable this, you have to care about all this things yourself.'),
      '#default_value' => variable_get('jquery_ui_dialog_default_css', true),
  );

  return system_settings_form($form);
}