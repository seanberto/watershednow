<?php
// $Id: web_widgets_style_inline.inc,v 1.1.2.1 2010/06/29 14:19:33 developmentseed Exp $
/**
 * @file
 * Inline-embedding specific functions, preprocessing
 */

/**
 * Define the theme functions for the web_widgets module
 *
 * @return unknown
 */
function web_widgets_style_inline() {
  $definition = array(
    'theme' => array(
      // The HTML-Javascript code lines what should be pasted into the external site
      'web_widgets_inline' => array(
        'template' => 'web_widgets_inline',
        'arguments' => array('path' => NULL, 'width' => NULL, 'height' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/inline',
      ),
      // Possible wrapper code around the widget
      'web_widgets_inline_wrapper' => array(
        'template' => 'web_widgets_inline_wrapper',
        'arguments' => array('content' => NULL, 'title' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/inline',
      ),
    ),
    'human_readable' => t('Inline'),
  );
  return $definition;
}

/**
 * Process variables for web_widgets_inline.tpl.php
 */
function template_preprocess_web_widgets_inline(&$variables) {
  $variables['wid'] = 'web_widgets_inline_'. md5(microtime());

  // Create Javascript variables
  $widgetcontext = new stdClass();
  $widgetcontext->widgetid = $variables['wid'];

  $variables['js_variables'] = drupal_to_js($widgetcontext);
  $variables['script_url'] = $variables['path'];
}

function template_preprocess_web_widgets_inline_wrapper(&$variables) {
  $variables['js_string'] = drupal_to_js($variables['content']);
}
