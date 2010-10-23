<?php
// $Id: taxonomy_image.admin.inc,v 1.1.2.9 2009/02/17 02:45:04 nancyw Exp $

/**
 * @file
 *  taxonomy_image.admin.inc.
 */

/**
 * taxonomy_image.module:
 *  Simple module for providing an association between taxonomy terms and images.
 * Written by Jeremy Andrews <jeremy@kerneltrap.org>, May 2004.
 */

/**
 * Administration Page
 */
function taxonomy_image_admin_settings() {
  return drupal_get_form('taxonomy_image_admin_form');
}

function taxonomy_image_admin_form() {
  $error = NULL;
  if (!file_check_directory(file_create_path(variable_get('taxonomy_image_path', 'category_pictures')), FILE_CREATE_DIRECTORY)) {
    $error = theme('error', t('The picture directory does not exist, or is not writable.'));
  }

  drupal_add_css(drupal_get_path('module', 'taxonomy_image') .'/taxonomy_image.css');

  $files_path = file_directory_path() .'/';

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -6,
    );

  $form['general']['taxonomy_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Picture image path'),
    '#default_value' => variable_get('taxonomy_image_path', 'category_pictures'),
    '#size' => 60,
    '#maxlength' => 255,
    '#field_prefix' => '<strong>'. $files_path .'</strong>',
    '#description' => t('Subdirectory in the directory "%dir" where category pictures will be stored. Lower case is recommended. ', array('%dir' => $files_path)) . $error,
    );

  $form['general']['taxonomy_image_disable'] = array(
    '#type' => 'textarea',
    '#rows' => 1,
    '#title' => t('User "Disable" box description'),
    '#default_value' => variable_get('taxonomy_image_disable', t('Check this box to disable the display of content images.')),
    '#description' => t('This allows you to describe the ability to disable Taxonomy Image display in a manner that your users will understand.'),
    );

  $form['general']['taxonomy_image_link_title'] = array(
    '#type' => 'radios',
    '#options' => array(t('Term description'), t('Term name')),
    '#title' => t('Link title'),
    '#default_value' => variable_get('taxonomy_image_link_title', 0),
    '#description' => t('This indicates which information to use for the link title (tool tip). If you choose "description" and one is not available, the title will be used.'),
    '#prefix' => '<div class="taxonomy_image_radios">',
    '#suffix' => '</div>',
    );

  $form['general']['taxonomy_image_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image'),
    '#default_value' => variable_get('taxonomy_image_default', NULL),
    '#description' => t('If you specify a default image and the requested term has no image, the default will be used. You must enter the entire image path.'),
    '#size' => 80,
    '#maxlength' => 255,
    );

  $form['resizing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Resizing Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -4,
    );

  $ic_exists = module_exists('imagecache');
  if ($ic_exists) {
    $form['resizing']['ic'] = array(
      '#type' => 'fieldset',
      '#title' => t('Resizing Options - Imagecache'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => -3,
      );

    $form['resizing']['ic']['taxonomy_image_imagecache_preset'] = array(
      '#type' => 'radios',
      '#title' => t('Imagecache Preset'),
      '#description' => t('The Imagecache module is available. If you select one of these presets, or provide one in your call, then Taxonomy Image will use Imagecache to process the image and ignore the following settings. "ORIGINAL" effectively disables Imagecache and uses the following settings.'),
      '#options' => drupal_map_assoc(_taxonomy_image_presets()),
      '#default_value' => variable_get('taxonomy_image_imagecache_preset', NULL),
      '#prefix' => '<div class="taxonomy_image_radios">',
      '#suffix' => '</div>',
      );
  }

  $form['resizing']['ti'] = array(
    '#type' => 'fieldset',
    '#title' => t('Resizing Options - Taxonomy Image'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -2,
    );

  $form['resizing']['ti']['taxonomy_image_resize'] = array(
    '#type' => 'radios',
    '#title' => t('Constraint'),
    '#default_value' => variable_get('taxonomy_image_resize', 0),
    '#options' => array(3 => 'Exact', 2 => 'Not less than', 1 => 'Not greater than', 0 => 'Disabled'),
    '#description' => t('This option allows you to control the size of pictures displayed by this module.  If set to "disabled," pictures will not be resized, i.e. displayed exactly as they are uploaded.  If set to "not greater than," pictures larger than the specified size will be scaled down.  If set to "not less than," pictures smaller than the specified size will be scaled up.  If set to "exact," pictures will be resized to exactly the specified dimension(s).'),
    '#prefix' => '<div class="taxonomy_image_radios">',
    '#suffix' => '</div>',
    );

  $form['resizing']['ti']['taxonomy_image_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Picture height'),
    '#default_value' => variable_get('taxonomy_image_height', ''),
    '#size' => 5,
    '#maxlength' => 6,
    '#field_suffix' => '<em>'. t('pixels') .'</em>',
    '#description' => t('Specify a height in pixels.  If left blank or set to 0 this field is ignored.'),
    );

  $form['resizing']['ti']['taxonomy_image_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Picture width'),
    '#default_value' => variable_get('taxonomy_image_width', ''),
    '#size' => 5,
    '#maxlength' => 6,
    '#field_suffix' => '<em>'. t('pixels') .'</em>',
    '#description' => t('Specify a width in pixels.  If left blank or set to 0 this field is ignored.'),
    );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -3,
    );

  if ($ic_exists) {
    $form['advanced']['taxonomy_image_admin_preset'] = array(
      '#type' => 'radios',
      '#title' => t('Admin Preset'),
      '#description' => t('This setting is for the images shown in the Admin page. "ORIGINAL" causes the standard non-Imagcache resizing option to be used.'),
      '#options' => drupal_map_assoc(_taxonomy_image_presets()),
      '#default_value' => variable_get('taxonomy_image_admin_preset', 'ORIGINAL'),
      '#prefix' => '<div class="taxonomy_image_radios">',
      '#suffix' => '</div>',
      );
  }

  $form['advanced']['taxonomy_image_wrapper'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add wrapper div tag'),
    '#default_value' => variable_get('taxonomy_image_wrapper', FALSE),
    '#description' => t('This option indicates whether to add a "&lt;div&gt;" tag to the image. This may be useful in lists where the images are not of the same width.'),
    );

  $form['advanced']['taxonomy_image_recursive'] = array(
    '#type' => 'radios',
    '#title' => t('Recursive image display'),
    '#default_value' => variable_get('taxonomy_image_recursive', 0),
    '#options' => array(0 => 'Disabled', 1 => 'Enabled'),
    '#description' => t("When enabled, taxonomy_image_display() will recursively search for an image to display, starting with the passed in term, then trying the term's parents.  This functionality is only useful if you have defined hierarchical taxonomies, and multiple terms within a tree will share the same image.  If this doesn't mean anything to you, leave this option disabled."),
    '#prefix' => '<div class="taxonomy_image_radios">',
    '#suffix' => '</div>',
    );

  return system_settings_form($form);
}

function taxonomy_image_overview() {
  $dest = drupal_get_destination();
  $output = '';
  if (variable_get('taxonomy_image_recursive', 0)) {
    $output .= '<h4>'. t('Recursively displaying images.') .'</h4>';
  }
  $admin_preset = variable_get('taxonomy_image_admin_preset', NULL);

  // Since this is just an overview, I have removed the "op" column.
  $header = array(
    t('Name'),
    t('Node Types'),
    t('Image'),
    t('Op')
    );
  $attrs = array('class' => 'taxonomy_image_overview');

  $vocabularies = taxonomy_get_vocabularies();

  foreach ($vocabularies as $vocabulary) {
    $types = array();
    $rows = array();
    $types = $vocabulary->nodes;
    $rows[] = array(
      array('data' => '<strong>'. check_plain($vocabulary->name) .'</strong>', 'valign' => 'top', 'class' => 'title'),
      array('data' => implode(', ', $types), 'align' => 'center', 'valign' => 'middle'),
      '&nbsp;',      
      );

    $tree = taxonomy_get_tree($vocabulary->vid);
    if ($tree) {
      foreach ($tree as $term) {
        $data =  str_repeat('--', $term->depth) . ($term->depth > 0 ? ' ' : NULL) . check_plain($term->name);
        $exists = taxonomy_image_display($term->tid, array(), $admin_preset);
        $op = l(t('edit term'), 'admin/content/taxonomy/edit/term/'. $term->tid, array('query' => $dest));
        // Use taxonomy_image_display() instead of _taxonomy_image_exists() in
        // case image display recursion is enabled...
        $image = $exists ? $exists : ('<em>'. t('none') .'</em>');
        $rows[] = array(
          array('data' => $data, 'colspan' => '2'),
          array('data' => $image, 'align' => 'center'),
          $op,
          );
      }
    }

    $output .= '<br/>'. theme('table', $header, $rows, $attrs);
  }

  return $output;
}
