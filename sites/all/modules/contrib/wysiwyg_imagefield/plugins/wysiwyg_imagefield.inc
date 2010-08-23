<?php
// $Id: wysiwyg_imagefield.inc,v 1.4 2010/08/10 04:51:03 deciphered Exp $
/**
 * @file
 * Define WYSIWYG ImageField WYSIWYG plugin.
 */

/**
 * Implements hook_INCLUDE_plugin().
 */
function wysiwyg_imagefield_wysiwyg_imagefield_plugin() {
  $plugins = array();

  // TODO: Come up with alternate solution, as this potentially causes more
  // issues than it solves.
  if (_wysiwyg_imagefield_attach_plugin()) {
    $plugins['wysiwyg_imagefield'] = array(
      'title' => t('WYSIWYG ImageField'),
      'vendor url' => 'http://drupal.org/project/wysiwyg_imagefield',
      'icon file' => 'wysiwyg_imagefield.gif',
      'icon title' => t('WYSIWYG ImageField'),
      'settings' => array(),
    );
  }

  return $plugins;
}

/**
 * Helper function for WYSIWYG ImageField WYSIWYG plugin.
 */
function _wysiwyg_imagefield_attach_plugin() {
  if (arg(0) == 'node') {
    if (arg(1) == 'add') {
      $content_type = str_replace('-', '_', arg(2));
      if (variable_get('wysiwyg_imagefield_' . $content_type, NULL) != NULL) {
        return TRUE;
      }
      return FALSE;
    }

    elseif (arg(2) == 'edit') {
      $node = node_load(arg(1));
      if (variable_get('wysiwyg_imagefield_' . $node->type, NULL) != NULL) {
        return TRUE;
      }
      return FALSE;
    }
  }
  return TRUE;
}
