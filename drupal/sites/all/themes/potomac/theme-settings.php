<?php
// $Id: theme-settings.php,v 1.1 2009/06/26 13:55:36 kiterminal Exp $

/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function potomac_settings($saved_settings) {
  // Get the node types
  $node_types = node_get_types('names');

  /**
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the template.php file.
   */
  $defaults = array(
    'potomac_breadcrumb_display'                    => 1,
    'potomac_iepngfix_setting'                      => 1,
    'potomac_taxonomy_display_default'              => 'all',
    'potomac_taxonomy_enable_content_type'          => 0,
  );

  // Make the default content-type settings the same as the default theme settings,
  // so we can tell if content-type-specific settings have been altered.
  $defaults = array_merge($defaults, theme_get_settings());

  // Set the default values for content-type-specific settings
  foreach ($node_types as $type => $name) {
    $defaults["potomac_taxonomy_{$type}"]         = $defaults['potomac_taxonomy_default'];
  }

  // Merge the saved variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  // Create theme settings form widgets using Forms API

  // Misty Look fieldset
  $form['potomac_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Potomac settings'),
    '#description' => t('Use these settings to change what and how information is displayed in your theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // General Settings
  $form['potomac_container']['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => 'general_settings'),
  );

  // Breadcrumb
  $form['potomac_container']['general_settings']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => $settings['potomac_breadcrumb_display'],
  );

  // IE PNG Fix Settings
  $form['potomac_container']['general_settings']['iepngfix_setting'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Use IE PNG Fix'), 
    '#default_value' => $settings['potomac_iepngfix_setting'], 
  );

  // Taxonomy Settings
  if (module_exists('taxonomy')) {
    $form['potomac_container']['potomac_taxonomy_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy settings'),
      '#description' => t('Here you can make adjustments to which information is shown with your content, and how it is displayed.  You can modify these settings so they apply to all content types, or check the "Use content-type specific settings" box to customize them for each content type.  For example, you may want to show the date on stories, but not pages.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    // Default & content-type specific settings
    foreach ((array('default' => 'Default') + node_get_types('names')) as $type => $name) {
      // Get taxonomy vocabularies by node type
      $vocabs_by_type = ($type == 'default') ? taxonomy_get_vocabularies() : taxonomy_get_vocabularies($type);

      if (!empty($vocabs_by_type)) {
        // taxonomy display per node
        $form['potomac_container']['taxonomy_settings']['display_taxonomy'][$type] = array(
          '#type' => 'fieldset',
          '#title' => t('!name', array('!name' => $name)),
          '#collapsible' => TRUE,
          '#collapsed' => ($settings['potomac_taxonomy_enable_content_type'] == 0) ? TRUE : FALSE,
        );
        // display
        $form['potomac_container']['taxonomy_settings']['display_taxonomy'][$type]["taxonomy_display_{$type}"] = array(
          '#type' => 'select',
          '#title' => t('When should taxonomy terms be displayed?'),
          '#default_value' => $settings["potomac_taxonomy_display_{$type}"],
          '#options' => array(
                          '' => '',
                          'never' => t('Never display taxonomy terms'),
                          'all' => t('Always display taxonomy terms'),
                          'only' => t('Only display taxonomy terms on full node pages'),
                        ),
        );
        // Options for default settings
        if ($type == 'default') {
          $form['potomac_container']['taxonomy_settings']['display_taxonomy']['default']['#title'] = t('Default');
          $form['potomac_container']['taxonomy_settings']['display_taxonomy']['default']['#collapsed'] = $settings['taxonomy_enable_content_type'] ? TRUE : FALSE;
          $form['potomac_container']['taxonomy_settings']['display_taxonomy']['taxonomy_enable_content_type'] = array(
            '#type'          => 'checkbox',
            '#title'         => t('Use custom settings for each content type instead of the default above'),
            '#default_value' => $settings['potomac_taxonomy_enable_content_type'],
          );
        }
        // Collapse content-type specific settings if default settings are being used
        else if ($settings['potomac_taxonomy_enable_content_type'] == 0) {
          $form['display_taxonomy'][$type]['#collapsed'] = TRUE;
        }
      }
    }
  }

  return $form;
}