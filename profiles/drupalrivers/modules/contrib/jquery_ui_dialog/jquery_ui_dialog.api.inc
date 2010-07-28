<?php
// $Id: jquery_ui_dialog.api.inc,v 1.1.2.8 2010/07/01 14:31:20 eugenmayer Exp $
# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer

/*
 * Part of the jquery_ui_dialog API.
 * Call this method if you are going to open a jquer ui dialog. It includes needed
 * css and javascript tools to ease up the process
 */
function jquery_ui_dialog_parent_js() {
  static $processed;
  // Make sure external resources are not included more than once.
  if ($processed === true) {
    return;
  }
  $processed = TRUE;

  jquery_ui_add(array('ui.dialog', 'ui.draggable'));

  $m = drupal_get_path('module', 'jquery_ui_dialog');
  // Add the css files for the jquery ui theme
  drupal_add_css("$m/css/jquery_ui_dialog-dialog/jquery_ui_dialog.css");
  // Add the css files we need for the dialog in general
  drupal_add_css("$m/css/jquery_ui_dialog.main.css");  
  // Thats what handles the js tunnel to the client for communitcation
  drupal_add_js("$m/js/jquery_ui_dialog_parent.js");
}

/*
 * Part of the jquery_ui_dialog API.
 * This method must be called in the clien (child) window. E.g. when you load
 * the URL dialog/login you must call jquery_ui_dialog_child_js in the menu handler callback
 * In general this adds the needed js tool to connect to the parent communitcation tunnel
 * And handle all the auto-resize events.
 */
function jquery_ui_dialog_child_js($options = array()) {
  if(count($options) > 0) {    
    drupal_add_js(array('jqui_dialogChild' => $options), 'setting');
  }
  $m = drupal_get_path('module', 'jquery_ui_dialog');
  drupal_add_js($m .'/js/jquery_ui_dialog_child.js');
  drupal_add_css("$m/css/jquery_ui_dialog.default.css");
  module_invoke_all('jquery_ui_dialog_child_loading');
  $GLOBALS['jquery_ui_dialog_page_template'] = TRUE;
}

function jquery_ui_dialog_jquery_ui_dialog_child_loading() {
  module_invoke('admin', 'suppress');
  module_invoke('admin_menu', 'suppress');
}