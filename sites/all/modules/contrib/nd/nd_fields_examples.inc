<?php
// $Id: nd_fields_examples.inc,v 1.1.2.4 2010/04/26 16:14:57 swentel Exp $

/**
 * @file
 * Loads a node object to show the rendered object and the variables.
 */

// Try to load a node, either from id or URL.
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
  $node = node_load($_GET['id']);
}
else {
  $node = node_load(array('type' => $type));
}

if (empty($node)) {
  $output .= t('No node found from type: !type', array('!type' => $type));
  return;
}

// Mimic devel render.
$node = node_build_content($node, FALSE, TRUE);
$output .= '<h3>'. t('Node build') .'</h3>';
$output .= kdevel_print_object($node, '$'. $type .'->');

// Run alter
node_invoke_nodeapi($node, 'alter', FALSE, TRUE);
$output .= '<h3>'. t('Node build (after alter hooks)') .'</h3>';
$output .= kdevel_print_object($node, '$'. $type .'->');

// Preprocess variables.
$variables['node'] = $node;
$variables['teaser'] = FALSE;
$variables['page'] = TRUE;
_ds_fields_run_preprocess($variables, 'node');
$output .= '<h3>'. t('Preprocess variables') .'</h3>';
$output .= kdevel_print_object($variables, '$'. $type .'->');
