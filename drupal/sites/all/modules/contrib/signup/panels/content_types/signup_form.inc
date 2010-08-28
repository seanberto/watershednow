<?php
// $Id: signup_form.inc,v 1.4 2008/11/15 02:03:50 dww Exp $


/**
 * Supply a list of panels content types.
 */
function signup_signup_form_panels_content_types() {
  $items['signup_form'] = array(
    'title' => t('Signup form'),
    'content_types' => 'signup_panels_content_types_signup_form',
    'single' => TRUE,
    'render callback' => 'signup_panels_content_signup_form',
    'add callback' => 'signup_panels_edit_signup_form',
    'edit callback' => 'signup_panels_edit_signup_form',
    'title callback' => 'signup_panels_title_signup_form',
  );
  return $items;
}

/**
 * Return all content types available.
 */
function signup_panels_content_types_signup_form() {
  return array(
    'signup_form' => array(
      'title' => t('Signup form'),
      'icon' => 'icon_node.png',  // TODO: choose different icon
      'path' => panels_get_path('content_types/node'), // TODO: adjust path
      'description' => t('Signup form.'),
      'required context' => new panels_required_context(t('Node'), 'node'),
      'category' => array(t('Contributed modules'), 0),
    ),
  );
}

/**
 * Render a pane of the 'signup form' content type.
 *
 * Outputs the signup form based on a given nid, as provided by the $context.
 */
function signup_panels_content_signup_form($subtype, $conf, $panel_args, &$context) {
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  $block->module = 'signup';
  $block->subject = '';
  if (empty($node)) {
    $block->delta = 0;
    $block->content = '';
  }
  else {
    $block->delta = $node->nid;
    module_load_include('inc', 'signup', 'includes/node_output');
    $block->content = _signup_node_output($node);
  }
  return $block;
}

/**
 * Return an edit form for the signup form content type.
 */
function signup_panels_edit_signup_form($id, $parents, $conf = array()) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  return $form;
}

/**
 * Return the administrative title for a signup form pane.
 */
function signup_panels_title_signup_form($subtype, $conf, $context) {
  $choices = signup_panels_content_types_signup_form();
  return t('"@s" @type', array('@s' => $context->identifier, '@type' => drupal_strtolower($choices[$conf['id']]['title'])));
}

