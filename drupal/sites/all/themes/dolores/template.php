<?php

// Dolor theme inherits template overrides from the Watershed parent theme.

/*
 * Override the default search form layout.
 */
function dolores_preprocess_search_theme_form(&$vars, $hook) {
  
  // Override the title field.
  $vars['form']['search_theme_form']['#title'] = t('');

  // Set a default value for the search box
  $search_help = theme_get_setting('dolores_search_help');
  $vars['form']['search_theme_form']['#value'] = $search_help;

  // Add a custom class to the search box
  $vars['form']['search_theme_form']['#attributes'] = array('class' => t('NormalTextBox txtSearch'),
  'onfocus' => "if (this.value == '" . $search_help . "') {this.value = '';}",
  'onblur' => "if (this.value == '') {this.value = '" . $search_help . "';}");

  // Change the text on the submit button
  $button_text = theme_get_setting('dolores_search_button');
  if ($button_text != '') {
    $vars['form']['submit']['#value'] = $button_text;
  } else {
    $vars['form']['submit']['#value'] = t('Go');
  }

  // Rebuild the rendered version (search form only, rest remains unchanged)
  unset($vars['form']['search_theme_form']['#printed']);
  $vars['search']['search_theme_form'] = drupal_render($vars['form']['search_theme_form']);

  // Rebuild the rendered version (submit button, rest remains unchanged)
  unset($vars['form']['submit']['#printed']);
  $vars['search']['submit'] = drupal_render($vars['form']['submit']);

  // Collect all form elements to make it easier to print the whole form.
  $vars['search_form'] = implode($vars['search']);
}