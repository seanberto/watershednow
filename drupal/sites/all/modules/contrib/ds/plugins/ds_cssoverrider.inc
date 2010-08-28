<?php
// $Id: ds_cssoverrider.inc,v 1.1.2.8 2010/05/28 23:44:56 swentel Exp $

/**
 * @file
 * DS_CSSOverrider: plugin to override css per region.
 */

class ds_cssoverrider {

  /**
   * Constructor().
   */
  function __construct() {

  }

  /**
   * plugin_form().
   */
  public function plugin_form(&$form, $display_settings) {

    $form['cssoverrider'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Either add extra classes or override CSS inline per region.'),
    );

    $build_mode = $form['#build_mode'];
    $regions = ds_regions();
    foreach ($regions as $region => $title) {
      if ($region != 'disabled') {
        $form['cssoverrider']['region-class-'. $region] = array(
          '#title' => $title .' class',
          '#type' => 'textfield',
          '#description' => t('Enter extra classes, seperated by comma.'),
          '#default_value' => ds_default_value($display_settings, $build_mode, 'cssoverrider', 'class', $region, ''),
        );
        $form['cssoverrider']['region-inline-'. $region] = array(
          '#title' => $title .' inline css',
          '#type' => 'textfield',
          '#description' => t('Enter inline css. Note, you only need to enter the css rules, the plugin will add style="" for you.'),
          '#default_value' => ds_default_value($display_settings, $build_mode, 'cssoverrider', 'inline', $region, ''),
        );
      }
    }
  }

  /**
   * plugin_form_submit().
   */
  public function plugin_form_submit($form, $form_state, &$display_settings) {

    $regions = ds_regions();
    $css_save = array();
    $build_mode = $form['#build_mode'];

    foreach ($regions as $region => $title) {
      if ($region != 'disabled') {
        $css_save['class'][$region] = $form_state['values']['cssoverrider']['region-class-'. $region];
        $css_save['inline'][$region] = $form_state['values']['cssoverrider']['region-inline-'. $region];
      }
    }

    $display_settings[$build_mode]['cssoverrider'] = $css_save;
  }

  /**
   * process().
   */
  public function execute(&$vars, &$display, $display_settings, $object_type, $module) {

    foreach ($display->themed_regions as $region_name => $region_data) {

      $classes = ds_default_value($display_settings, $display->build_mode, 'cssoverrider', 'class', $region_name, '');
      $inline = ds_default_value($display_settings, $display->build_mode, 'cssoverrider', 'inline', $region_name, '');

      if (!empty($classes)) {
        $display->themed_regions[$region_name]['extra_class'] .= ' '. $classes;
      }

      if (!empty($inline)) {
        $display->themed_regions[$region_name]['inline_css'] = 'style="'. $inline .'"';
      }
    }
  }
}