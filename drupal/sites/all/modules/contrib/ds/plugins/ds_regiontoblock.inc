<?php
// $Id: ds_regiontoblock.inc,v 1.1.2.4 2010/07/06 22:38:40 swentel Exp $

/**
 * @file
 * DS_RegionToBlock: Plugin to make a region available as a block.
 */

class ds_regiontoblock {

  /**
   * Constructor().
   */
  function __construct() {

  }

  /**
   * plugin_form().
   */
  public function plugin_form(&$form, $display_settings) {

    $form['regiontoblock'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Toggle regions you want to make available as a block.'),
    );

    $build_mode = $form['#build_mode'];
    $regions = ds_regions();
    foreach ($regions as $region => $title) {
      if ($region != 'disabled') {
        $default_value = ds_default_value($display_settings, $build_mode, 'regiontoblock', 'block', $region, '');
        $form['regiontoblock']['region-block-'. $region] = array(
          '#title' => $title,
          '#type' => 'checkbox',
          '#default_value' => $default_value,
        );
        if ($default_value == TRUE) {
          $form['#regions'][$region] .= ' - '. t('assigned to block');
        }
      }
    }
  }

  /**
   * plugin_form_submit().
   */
  public function plugin_form_submit($form, $form_state, &$display_settings) {

    $regions = ds_regions();
    $regions_save = array();
    $build_mode = $form['#build_mode'];

    foreach ($regions as $region => $title) {
      if (isset($form_state['values']['regiontoblock']['region-block-'. $region]))
      $regions_save['block'][$region] = $form_state['values']['regiontoblock']['region-block-'. $region];
    }

    $display_settings[$build_mode]['regiontoblock'] = $regions_save;
  }

  /**
   * plugin block listing.
   *
   * Make sure you create a unique name for your $block_key.
   * You should return info & plugin so ds_block knows
   * which block to load. You can add other info, which
   * you'll receive back in block_list.
   */
  public function block_list($module, &$blocks, $data) {
    $api_info = ds_api_info($module);
    $types = $api_info['types']();
    if (!empty($types)) {
      foreach ($types as $type_key => $object) {
        $display_settings = variable_get($module .'_display_settings_'. $type_key, array());
        foreach ($display_settings as $build_mode => $settings) {
          if (isset($settings['regiontoblock'])) {
            foreach ($settings['regiontoblock']['block'] as $region => $value) {
              if ($value) {
                $block_key = 'ds_regiontoblock_'. $module .'_'. $object->type .'_'. $build_mode .'_'. $region;
                $block_info = $api_info['title'] .': '. $object->name .', '. $build_mode .', '.$region;
                $blocks[] = array(
                  'info' => $block_info,
                  'key' => $block_key,
                  'data' => $data,
                );
              }
            }
          }
        }
      }
    }
  }

  /**
   * plugin block view
   */
  public function block_view($block) {
    $content = array();
    $data = ds_static_variables($block['key']);
    if (!empty($data)) {
      $content['subject'] = $block['info'];
      $content['content'] = theme('ds_objects', $data, array('context' => 'blocktoregion', 'block' => $block));
    }
    return $content;
  }

  /**
   * execute().
   */
  public function execute(&$vars, &$display, $display_settings, $object_type, $module) {
    $object = $vars[$object_type];

    foreach ($display->all_regions as $region_name => $region_title) {

      $region_to_block = ds_default_value($display_settings, $display->build_mode, 'regiontoblock', 'block', $region_name, FALSE);

      if ($region_to_block == TRUE && isset($display->themed_regions[$region_name])) {
        $region_content = $display->themed_regions[$region_name];
        $key = 'ds_regiontoblock_'. $module .'_'. $object->type .'_'. $object->build_mode .'_'. $region_name;
        $data = new stdClass;
        $data->content = $region_content['content'];
        $data->object = $vars[$object_type];
        ds_static_variables($key, $data);
        unset($display->themed_regions[$region_name]);
        if ($region_name == 'left' || $region_name == 'right') {
          unset($display->region_classes[$region_name]);
        }
      }
    }
  }
}
