<?php

function wn_neuse_settings($saved_settings, $subtheme_defaults = array()) {

  // Get the default values from the .info file.
  $defaults = wn_neuse_theme_get_default_settings('wn_neuse');

  // Merge the saved variables and their default values.
  $settings = array_merge($defaults, $saved_settings);


  // Get color palette options by scanning optional css stylesheets.
  $colors_dir = drupal_get_path('theme', 'wn_neuse') . '/css/colors';
  $options = file_scan_directory($colors_dir, '\.css$', $nomask = array('.', '..', 'CVS'), $callback = 0, $recurse = TRUE, $key = 'name');
  foreach ($options as $color) {
    $colors[$color->name] = $color->name;
  }

  /*
   * Create the form using Forms API
   */

  $form['wn_neuse_color_palette'] = array(
    '#type' => 'radios',
    '#title' => t('Color Options'),
    '#options' => $colors,
    '#description' => t('Choose a color palette for this theme.'),
    '#default_value' => $settings['wn_neuse_color_palette'],
  );

  $form['wn_neuse_breadcrumb'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Breadcrumb trail'),
    '#default_value' => $settings['wn_neuse_breadcrumb'],
    '#description'   => t('Enable breadcrumb trail..'),
    '#prefix'        => '<strong>' . t('Enable Breadcrumb:') . '</strong>',
  );

  $form['wn_neuse_block_editing'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show block editing on hover'),
    '#description'   => t('When hovering over a block, privileged users will see block editing links.'),
    '#default_value' => $settings['wn_neuse_block_editing'],
    '#prefix'        => '<strong>' . t('Block Edit Links:') . '</strong>',
  );

  // Return the form
  return $form;
}


function _wn_neuse_theme(&$existing, $type, $theme, $path) {
  // Each theme has two possible preprocess functions that can act on a hook.
  // This function applies to every hook.
  $functions[0] = $theme . '_preprocess';
  // Inspect the preprocess functions for every hook in the theme registry.
  // @TODO: When PHP 5 becomes required (Zen 7.x), use the following faster
  // implementation: foreach ($existing AS $hook => &$value) {}
  foreach (array_keys($existing) AS $hook) {
    // Each theme has two possible preprocess functions that can act on a hook.
    // This function only applies to this hook.
    $functions[1] = $theme . '_preprocess_' . $hook;
    foreach ($functions AS $key => $function) {
      // Add any functions that are not already in the registry.
      if (function_exists($function) && !in_array($function, $existing[$hook]['preprocess functions'])) {
        // We add the preprocess function to the end of the existing list.
        $existing[$hook]['preprocess functions'][] = $function;
      }
    }
  }

  // Since we are rebuilding the theme registry and the theme settings' default
  // values may have changed, make sure they are saved in the database properly.
  wn_neuse_theme_get_default_settings($theme);

  // If we are auto-rebuilding the theme registry, warn about feature.
  if (theme_get_setting('wn_neuse_rebuild_registry')) {
    drupal_set_message(t('The theme registry has been rebuilt. <a href="!link">Turn off</a> this feature on production websites.', array('!link' => base_path() . 'admin/build/themes/settings/' . $GLOBALS['theme'])), 'warning');
  }

  // Since we modify the $existing cache directly, return nothing.
  return array();
}


function wn_neuse_theme_get_default_settings($theme) {
  $themes = list_themes();

  // Get the default values from the .info file.
  $defaults = !empty($themes[$theme]->info['settings']) ? $themes[$theme]->info['settings'] : array();

  if (!empty($defaults)) {
    // Get the theme settings saved in the database.
    $settings = theme_get_settings($theme);
    // Don't save the toggle_node_info_ variables.
    if (module_exists('node')) {
      foreach (node_get_types() as $type => $name) {
        unset($settings['toggle_node_info_' . $type]);
      }
    }
    // Save default theme settings.
    variable_set(
      str_replace('/', '_', 'theme_' . $theme . '_settings'),
      array_merge($defaults, $settings)
    );
    // If the active theme has been loaded, force refresh of Drupal internals.
    if (!empty($GLOBALS['theme_key'])) {
      theme_get_setting('', TRUE);
    }
  }

  // Return the default settings.
  return $defaults;
}
