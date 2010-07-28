<?php
// $Id: node_export.inc,v 1.1.2.2 2009/07/14 18:15:19 jamesandres Exp $

/**
 * @file
 * CRUD functions for node_export (aka "export") module.
 */

/**
 * Import a node from a file.
 *
 * @param $file
 *   The path to a file containing the export.
 * @param $properties
 *   An array of node properties to override for each node imported.
 * @param $author
 *   The user all imported nodes should be authored by.
 */
function install_node_export_import_from_file($file, $properties = array(), $author = NULL) {
  if (!file_exists($file)) {
    drupal_set_message(t('The file !file could not be found or opened.', array('!file' => $file)));
    return FALSE;
  }

  global $user;

  // If the author isn't specified, use the logged in user
  if (!$author) {
    $author = $user;
  }

  // TODO: Patch node_export module directly to support these features.
  // Default node properties
  $default_properties = array(
    'nid'     => NULL,
    'vid'     => NULL,
    'name'    => $author->name,
    'uid'     => $author->uid,
    'created' => NULL,
    'menu'    => NULL,
    'book'    => array('mlid' => NULL),
    'files'   => array(),
  );
  $properties = array_merge($default_properties, $properties);

  module_load_include('inc', 'node_export', 'node_export.pages');

  // This bit borrowed from node_export module, it wasn't possible to use
  // drupal_execute because the export_node_import() function deals with
  // $_POST directly.  When that issue is fixed, it should be feasible
  // to use reuse the node_export code.
  $import_code = file_get_contents($file);
  $import = node_export_node_decode($import_code);

  // Detect single import and create multiple
  if (is_object($import)) {
    $import = array($import);
  }

  $saved_nodes = array();

  foreach ($import as $node) {
    $node = (object) $node;

    foreach ($properties as $key => $value) {
      $node->$key = $value;
    }

    // Let other modules do special fixing up.
    // The function signature is: hook_export_node_alter(&$node, $original_node, $method)
    // Where $method is either 'prepopulate' or 'save-edit'.
    drupal_alter("node_export_node", $node, $original_node, "save-edit");

    node_save($node);

    $saved_nodes[] = $node;
  }

  return $saved_nodes;
}
