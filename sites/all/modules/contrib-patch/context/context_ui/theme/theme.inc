<?php

/**
 * Preprocessor for theme('context_ui_editor').
 */
function template_preprocess_context_ui_editor(&$vars) {
  drupal_add_css(drupal_get_path('module', 'context_ui') . '/context_ui.css');
  drupal_add_js(drupal_get_path('module', 'context_ui') . '/context_ui.js');
  drupal_add_js(drupal_get_path('module', 'context_ui') . '/jquery.pageEditor.js');

  $vars['contexts'] = $vars['form']['contexts'];
  unset($vars['form']['contexts']);

  $vars['buttons'] = $vars['form']['buttons'];
  unset($vars['form']['buttons']);
}

/**
 * Preprocessor for theme('context_ui_plugins').
 */
function template_preprocess_context_ui_plugins(&$vars) {
  drupal_add_css(drupal_get_path("module", "context_ui") ."/context_ui.css");
  drupal_add_js(drupal_get_path("module", "context_ui") ."/context_ui.js");
  drupal_add_js(drupal_get_path('module', 'context_ui') . '/jquery.pageEditor.js');

  // Provide title & desc.
  $vars['title'] = check_plain($vars['form']['#title']);
  $vars['description'] = check_plain($vars['form']['#description']);

  // Build list of plugins.
  $plugins = array();
  $vars['plugins'] = array();
  foreach (element_children($vars['form']['plugins']) as $plugin) {
    $link = array(
      'title' => $vars['form']['plugins'][$plugin]['#plugin']->title . "<span class='remove'>". t('Remove') ."</span>",
      'href' => $_GET['q'],
      'html' => TRUE,
      'fragment' => "context-plugin-form-{$plugin}",
    );
    $class = $vars['form']['plugins'][$plugin]['#context_enabled'] ? "context-plugin-{$plugin}" : "context-plugin-{$plugin} disabled";
    $vars['plugins'][$class] = $link;
  }
}

/**
 * Preprocessor for theme('context_ui_form').
 */
function template_preprocess_context_ui_form(&$vars) {
  drupal_add_css(drupal_get_path("module", "context_ui") ."/context_ui.css");
  drupal_add_js(drupal_get_path("module", "context_ui") ."/context_ui.js");
  drupal_add_js(drupal_get_path('module', 'context_ui') . '/jquery.pageEditor.js');
  $vars['buttons'] = $vars['form']['buttons'];
  unset($vars['form']['buttons']);
}

/**
 * Generates the main context_ui admin page with a tiered context listing.
 */
function theme_context_ui_admin($form) {
  drupal_add_css(drupal_get_path("module", "context_ui") ."/context_ui.css");

  // Iterate once to group by tag.
  $by_tag = array();
  foreach ($form['#contexts'] as $context) {
    if (!empty($context->tag)) {
      $by_tag[$context->tag][$context->name] = $context;
    }
    else {
      $by_tag[''][$context->name] = $context;
    }
  }

  // Generate listing of existing contexts
  ksort($by_tag);
  $rows = array();
  foreach ($by_tag as $tag => $contexts) {
    if (!empty($tag)) {
      $rows[] = array(array('data' => check_plain($tag), 'colspan' => 3, 'class' => 'tag'));
    }
    ksort($contexts);
    foreach ($contexts as $context) {
      $row = array('data' => array());

      // Context info
      $storage = t('Default');
      if ($context->export_type & EXPORT_IN_DATABASE) {
        $storage = $context->export_type & EXPORT_IN_CODE ? t('Overridden') : t('Normal');
      }
      $data = "<strong>{$context->name}</strong> <em class='storage'>({$storage})</em>";
      $data .= !empty($context->description) ? '<div class="description">'. filter_xss_admin($context->description) .'</div>' : '';
      $row['data'][] = array('data' => $data, 'colspan' => 2, 'class' => 'context-name');

      // Context actions
      $links = array(
        'edit'    => l(t('Edit'), "admin/build/context/list/{$context->name}"),
        'delete'  => l(t('Delete'), "admin/build/context/list/{$context->name}/delete"),
        'revert'  => l(t('Revert'), "admin/build/context/list/{$context->name}/revert"),
        'export'  => l(t('Export'), "admin/build/context/list/{$context->name}/export"),
        'clone'   => l(t('Clone'), "admin/build/context/list/{$context->name}/clone"),
        'disable' => l(t('Disable'), "admin/build/context/list/{$context->name}/disable"),
        'enable'  => l(t('Enable'), "admin/build/context/list/{$context->name}/enable"),
      );
      foreach (array_keys($links) as $key) {
        if (!context_ui_task_access($context, $key)) {
          unset($links[$key]);
        }
      }
      $row['data'][] = array('data' => implode(' | ', $links), 'class' => 'context-links');
      $row['class'] = empty($context->disabled) ? 'enabled' : 'disabled';
      $rows[] = $row;
    }
  }
  $rows[] = array(
    drupal_render($form['tag']),
    drupal_render($form['name']),
    drupal_render($form['submit']),
  );
  $output = theme('table', array(t('Context'), '', t('Operations')), $rows, array('class' => 'context-admin'));
  $output .= drupal_render($form);
  return $output;
}
