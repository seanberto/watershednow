<?php
// $Id: web_widgets.views.inc,v 1.1 2009/06/24 07:38:58 aronnovak Exp $
/**
 * @file
 * Web Widget display defition.
 */

/**
 * Implementation of hook_views_plugins().
 */
function web_widgets_views_plugins() {
  $data = array(
    'display' => array(
      'web_widgets' => array(
        'title' => t('Web widget'),
        'help' => t('Display the view as a widget. Offer a snippet of code that users can copy and paste to another web site.'),
        'handler' => 'web_widgets_plugin_display_web_widgets',
        'parent' => 'page',
        'path' => drupal_get_path('module', 'web_widgets') .'/views',
        'uses hook menu' => TRUE,
        'theme' => 'views_view',
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
      ),
    ),
  );
  return $data;
}
