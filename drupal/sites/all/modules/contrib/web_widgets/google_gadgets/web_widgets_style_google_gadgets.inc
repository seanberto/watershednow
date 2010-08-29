<?php
// $Id: web_widgets_style_google_gadgets.inc,v 1.1 2009/06/24 07:38:58 aronnovak Exp $
/**
 * @file
 * google_gadgets specific functions, preprocessing
 */

/**
 * Define the theme functions for the web_widgets module
 */
function web_widgets_style_google_gadgets() {
  $definition = array(
    'theme' => array(
      // The HTML-Javascript code lines what should be pasted into the external site
      'web_widgets_google_gadgets' => array(
        'template' => 'web_widgets_google_gadgets',
        'arguments' => array('path' => NULL, 'width' => NULL, 'height' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/google_gadgets',
      ),
      // Possible wrapper code around the widget
      'web_widgets_google_gadgets_wrapper' => array(
        'template' => 'web_widgets_google_gadgets_wrapper',
        'arguments' => array('content' => NULL, 'title' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/google_gadgets',
      ),
    ),
    'human_readable' => t('Google Gadgets'),
  );
  return $definition;
}
