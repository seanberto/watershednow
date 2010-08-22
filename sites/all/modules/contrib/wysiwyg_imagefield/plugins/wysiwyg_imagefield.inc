<?php
// $Id: wysiwyg_imagefield.inc,v 1.1 2010/08/06 11:05:26 deciphered Exp $
/**
 * @file
 * Define WYSIWYG ImageField WYSIWYG plugin.
 */

/**
 * Implements hook_INCLUDE_plugin().
 */
function wysiwyg_imagefield_wysiwyg_imagefield_plugin() {
  $plugins['wysiwyg_imagefield'] = array(
    'title' => t('WYSIWYG ImageField'),
    'vendor url' => 'http://drupal.org/project/wysiwyg_imagefield',
    'icon file' => 'wysiwyg_imagefield.gif',
    'icon title' => t('WYSIWYG ImageField'),
    'settings' => array(),
  );

  return $plugins;
}
