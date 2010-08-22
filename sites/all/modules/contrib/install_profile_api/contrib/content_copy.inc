<?php
// $Id: content_copy.inc,v 1.1.2.4.2.2 2009/05/29 23:25:12 jamesandres Exp $

/**
 * @file
 * Allow direct imports of content types by using content_copy module.
 */
 
/**
 * Import a content type from a file containing the exported code from 
 * admin/content/types/export.
 *
 * This method can be more easy to implement than manually creating node types
 * with the node.inc and then creating fields with content.inc. Content copy
 * can create the node type and all the fields in a single command.
 *
 * @param $file
 *   The path to a file containing the export.
 * @param $type_name
 *   If importing into an existing type, pass the content type name along
 *   and the type will be updated.
 */
function install_content_copy_import_from_file($file, $type_name = '') {
  // The import will be populate into the $content variable.
  $content = array();

  ob_start();
  include $file;
  ob_end_clean();

  $form_state = array();
  $form = content_copy_import_form($form_state, $type_name);

  $form_state['values']['type_name'] = $type_name ? $type_name : '<create>';
  $form_state['values']['macro'] = '$content = '. var_export($content, 1) .';';
  $form_state['values']['op'] = t('Import');

  // Call the submit function directly.
  // Using drupal_execute() leads to problems with the form_state when called
  // again within this submit function.
  content_copy_import_form_submit($form, $form_state);
}
