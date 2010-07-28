<?php
// $Id: context_reaction_block.theme.inc,v 1.1.2.8 2010/05/25 18:56:21 yhahn Exp $

/**
 * Block form.
 */
function theme_context_block_form($form) {
  $rows = array(array(
    array('data' => drupal_render($form['blocks']), 'class' => 'blocks'),
    array('data' => drupal_render($form['selector']) . drupal_render($form['block']['help']), 'class' => 'selector'),
  ));
  $output = drupal_render($form);
  $output .= theme('table', array(), $rows, array('id' => 'context-blockform'));
  return $output;;
}

/**
 * Generates the AJAX enabled block administration portion of the context_ui admin form.
 */
function theme_context_block_regions_form($form) {
  // Add draggable weights
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'context') .'/plugins/context_reaction_block.js');
  drupal_add_css(drupal_get_path('module', 'context') .'/plugins/context_reaction_block.css');

  $output = '';

  foreach (element_children($form) as $region) {
    $attr = array(
      'id' => "context-blockform-region-{$region}",
      'class' => "context-blockform-region",
    );
    drupal_add_tabledrag($attr['id'], 'order', 'sibling', 'block-weight', NULL, NULL, FALSE);
    $rows = array();
    foreach (element_children($form[$region]) as $id) {
      $form[$region][$id]['weight']['#attributes'] = array('class' => 'block-weight');
      $label = $form[$region][$id]['#value'];
      $remove = l('X', $_GET['q'], array('fragment' => 'remove', 'attributes' => array('class' => 'remove')));
      $rows[] = array(
        'data' => array($label . drupal_render($form[$region][$id]['weight']), $remove),
        'class' => 'draggable',
        'id' => $id,
      );
    }
    $output .= "<div class='label context-blockform-regionlabel-{$region}'>";
    $output .= l('+ '. t('Add'), $_GET['q'], array('fragment' => $region, 'attributes' => array('class' => 'add-block')));
    $output .= $form[$region]['#title'];
    $output .= "</div>";
    $output .= theme('table', array(), $rows, $attr);
  }
  return $output;
}

/**
 * Use placeholder content for script tags that need to be replaced.
 */
function theme_context_block_script_placeholder($text = '') {
  $message = t('Please reload the page to view this block.');
  return "<div class='script-placeholder'><strong>{$text}</strong><div class='description'>{$message}</div></div>";
}

/**
 * Preprocessor for theme('context_block_browser').
 */
function template_preprocess_context_block_browser(&$vars) {
  $categories = array(
    '#type' => 'select',
    '#options' => array(0 => '<'. t('Choose a category') .'>'),
    '#attributes' => array('class' => 'context-block-browser-categories'),
    '#value' => 0,
    '#size' => 1,
    '#id' => '',
    '#name' => '',
    '#parents' => array(''),
    '#multiple' => FALSE,
    '#required' => FALSE,
  );
  $blocks = array();
  // Group blocks by module.
  foreach ($vars['blocks'] as $block) {
    if (!isset($categories[$block->module])) {
      $info = context_get_info('module', $block->module);
      $categories['#options'][$block->module] = !empty($info['name']) ? $info['name'] : $block->module;
    }
    $blocks[$block->module][$block->bid] = $block; // Don't call theme('context_block_browser_item') to allow others to alter.
  }
  $vars['categories'] = $categories; // Don't call theme('select') here to allow further preprocesses to alter the element.
  $vars['blocks'] = $blocks;
}

/**
 * Preprocessor for theme('context_block_browser_item').
 */
function template_preprocess_context_block_browser_item(&$vars) {
  $vars['bid'] = $vars['block']->bid;
  $vars['info'] = check_plain($vars['block']->info);
}

/**
 * Preprocessor for theme('context_block_editable_region').
 */
function template_preprocess_context_block_editable_region(&$vars) {
  $vars['class'] = empty($vars['blocks']) ? 'context-block-region-empty' : '';

  // Killswitch flag allowing other preprocessors to abort editable regions entirely.
  $vars['editable'] = isset($vars['editable']) ? $vars['editable'] : TRUE;

  $vars['show_always'] = variable_get('context_ui_show_empty_regions', false);

  // Provide the user-friendly name of the region
  global $theme_key;
  $regions = system_region_list($theme_key);
  $vars['region_description'] = isset($regions[$vars['region']]) ? $regions[$vars['region']] : $vars['region'];
}

/**
 * Preprocessor for theme('context_block_editable_block').
 */
function template_preprocess_context_block_editable_block(&$vars) {
  $plugin = context_get_plugin('reaction', 'block');
  $block_info = $plugin->get_blocks();
  $block = $vars['block'];
  $vars['label'] = isset($block_info[$block->bid]) ? check_plain($block_info[$block->bid]->info) : $block->bid;
  $vars['tools'] = l('X', $_GET['q'], array('fragment' => 'remove', 'attributes' => array('class' => 'remove')));
  $vars['class'] = !empty($block->context) ? "editable edit-{$block->context}" : '';
}
