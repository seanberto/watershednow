<?php
// $Id: web_widgets_style_uwa.inc,v 1.1.2.1 2010/06/29 14:19:33 developmentseed Exp $
/**
 * @file
 * UWA-embedding specific functions, preprocessing
 * http://dev.netvibes.com/
 */

/**
 * Define the theme functions for the web_widgets module
 *
 * @return unknown
 */
function web_widgets_style_uwa() {
  $definition = array(
    'theme' => array(
      'web_widgets_uwa' => array(
        'template' => 'web_widgets_uwa',
        'arguments' => array('path' => NULL, 'width' => NULL, 'height' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/uwa',
      ),
      // Possible wrapper code around the widget
      'web_widgets_uwa_wrapper' => array(
        'template' => 'web_widgets_uwa_wrapper',
        'arguments' => array('content' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/uwa',
      ),
      // Widget file with the views URL
      'web_widgets_uwa_widget' => array(
        'template' => 'web_widgets_uwa_widget',
        'arguments' => array('url' => NULL, 'title' => NULL),
        'path' => drupal_get_path('module', 'web_widgets') .'/uwa',
      ),
    ),
    'human_readable' => t('NetVibes'),
  );
  return $definition;
}

/**
 * Overload the render() of view object.
 * @see render() at web_widgets_plugin_display_web_widgets.inc
 */
function web_widgets_uwa_render($view) {
  $current_style = empty($current_style) ? 'iframe' : $current_style;
  // Decide if it's the widget only (almost static) or the content itself
  $views_path = $view->view->display[$view->view->current_display]->display_options['path'];
  if (strstr($_GET['q'], $views_path .'/uwa')) {
    return theme('web_widgets_uwa_widget', $views_path, $view->view->get_title());
  }
  else {
    $content = theme($view->theme_functions(), $view->view);
    return theme('web_widgets_uwa_wrapper', $content);
  }
}

/**
 * Process variables for web_widgets_uwa.tpl.php
 */
function template_preprocess_web_widgets_uwa(&$variables) {
  $variables['wid'] = 'web_widgets_uwa_'. md5(microtime());

  $views_path_html = $variables['path'] .'/uwa';
  $variables['url'] = $views_path_html;
}

/**
 * Process variables for web_widgets_uwa_widget.tpl.php
 */
function template_preprocess_web_widgets_uwa_widget(&$variables) {
  $variables['url'] = url($variables['url'], array('absolute' => TRUE));
}
