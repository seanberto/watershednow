<?php
// $Id: theme.inc,v 1.1.2.55 2010/07/06 22:38:40 swentel Exp $

/**
 * @file
 * Theming functions for ds.
 */

/**
 * Theme function to render the field content.
 *
 * @param string $content The content to render.
 * @param array $field Collection of field properties.
 */
function theme_ds_field($content, $field) {
  $output = '';

  if (!empty($content)) {
    if ($field['type'] == 'ds') {

      $output .= '<div class="field '. $field['class'] .'">';
      // Above label.
      if ($field['labelformat'] == 'above') {
        $output .= '<div class="field-label">'. $field['title'] .': </div>';
      }
      // Inline label
      if ($field['labelformat'] == 'inline') {
        $output .= '<div class="field-label-inline-first">'. $field['title'] .': </div>';
      }
      $output .= $content;
      $output .= '</div>';
    }
    else {
      $output = $content;
    }
  }

  return $output;
}

/**
 * Theme function to render all regions with their content.
 *
 * @param stdClass $object_display The complete object and display properties.
 * @param string $module The module name.
 *
 * Other variables which you can use:
 *  - $region_data['count']: holds the number of fields per region.
 */
function theme_ds_regions($object_display, $module) {
  $output = '';

  foreach ($object_display->themed_regions as $region_name => $region_data) {
    $output .= '<div class="'. $module .'-region-'. $region_name;

    // The middle region has a wrapper region and some sidebar related classes.
    if ($region_name == 'middle') {
      $output .= '-wrapper ';
    }
    if ($region_name == 'header' || $region_name == 'footer') {
      $output .= ' clear-block';
    }

    // Extra classes or inline styles.
    if (!empty($region_data['extra_class'])) {
      $output .= $region_data['extra_class'];
    }
    $output .= '" ';
    if (isset($region_data['inline_css'])) {
      $output .= $region_data['inline_css'];
    }

    // Close the wrapper for middle if necessary.
    if ($region_name == 'middle') {
      $output .= '><div class="'. $module .'-region-'. $region_name .'">'. $region_data['content'] .'</div></div>';
    }
    else {
      $output .= '>'. $region_data['content'] .'</div>';
    }
  }

  return $output;
}

/**
 * Evaluate custom code.
 *
 * @param array $field The field with all its properties.
 */
function theme_ds_eval_code($field) {
  if (isset($field['properties']['code'])) {
    $value = ds_eval($field['properties']['code'], $field['object']);
    // Token support - check on token property so we don't run every single field through token.
    if (module_exists('token') && isset($field['properties']['token']) && $field['properties']['token'] == TRUE) {
      $value = token_replace($value, $field['object_type'], $field['object']);
    }
    return $value;
  }
}

/**
 * Evaluate block field.
 *
 * @param array $field The field array.
 */
function theme_ds_eval_block($field) {
  if (isset($field['properties'])) {
    list($module, $delta) = explode('|', $field['properties']['block']);
    $block = module_invoke($module, 'block', 'view', $delta);
    if (!empty($block)) {
      switch ($field['properties']['render']) {
        case DS_BLOCK_TEMPLATE:
          global $theme_key;
          $block = (object) $block;
          $block->region = NULL;
          $block->module = $module;
          $block->delta = $delta;
          $block_title = db_result(db_query("SELECT title FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme_key));
          if (!empty($block_title)) {
            $block->subject = $block_title == '<none>' ? '' : check_plain($block_title);
          }
          $content = theme('block', $block);
          break;
        case DS_BLOCK_TITLE_CONTENT:
          $content = '<h2 class="block-title">'. $block['subject'] .'</h2>';
          $content .= $block['content'];
          break;
        case DS_BLOCK_CONTENT:
          $content = $block['content'];
          break;
      }
      return $content;
    }
  }
}

/**
 * Theme a collection of ds objects.
 *
 * @param array $data A collection of data to print.
 * @param array $extra A collection data, ie context etc ..
 */
function theme_ds_objects($data, $extra) {
  $output = '';
  foreach ($data as $key => $value) {
    $output .= $value->content;
  }
  return $output;
}

/**
 * Theme a group (fieldset).
 *
 * @param string $content The content to render.
 * @param string $field The field to theme.
 */
function ds_group_fieldset($content, $field) {
  $add_js = FALSE;
  $class = $field['class'];
  $variation = array_pop(explode('_', $field['theme']));

  switch ($variation) {
    case 'collapsible':
      $add_js = TRUE;
      $class .= ' collapsible';
      break;
    case 'collapsed':
      $add_js = TRUE;
      $class .= ' collapsible collapsed';
      break;
    default:
      break;
  }

  // Add js.
  if ($add_js == TRUE) {
    drupal_add_js('misc/collapse.js');
  }

  return '<fieldset class="group '. $class .'"><legend>'. $field['title'] .'</legend>'. $content .'</fieldset>';
}

/**
 * Theme tabs. This is based on the theme_tabset() function
 * from the tabs module. We use our own theming function because
 * otherwhise we need to loop through our fields to build the
 * right array before we an send it to theme('tabset');
 */
function theme_ds_tabs($field) {
  tabs_load();

  static $i = 1;
  $fields = $field['fields'];
  ksort($fields);

  $tab_navigation = (variable_get('tabs_navigation', 0)) ? ' tabs-navigation' : '';

  $tabs_content = '';
  $output = '<div id="tabs-'. $field['name'] .'" class="drupal-tabs js-hide'. $tab_navigation .'" style="display:block;">';
  $tabs = '<ul class="clear-block ui-tabs-nav tabs">';
  foreach ($fields as $weight => $data) {
    $tabs .= '<li class="tabset-tab-'. $i .'">
    <a href="#tabset-tab-'. $i .'"><span class="tab">'. $data['title'] .'</span></a></li>';
    $tabs_content .= '<div id="tabset-tab-'. $i .'" class="tabs-'. $field['name'] .' ui-tabs-panel '. $data['class'] .'">'. $data['content'] .'</div>';
    $i++;
  }
  $tabs .= '</ul>';
  $output .= $tabs;
  $output .= $tabs_content;
  $output .= '</div>';
  return $output;
}

/**
 * Theme author name.
 */
function theme_ds_author_nolink($field) {
  return $field['object']->name;
}

/**
 * Theme author name linked.
 */
function theme_ds_author_link($field) {
  return theme('username', $field['object']);
}

/**
 * Theme the user picture.
 */
function theme_ds_picture($field) {
  return theme('user_picture', $field['object']);
}

/**
 * Template preprocess function for theme_views_view_row_object().
 */
function template_preprocess_ds_row_object(&$vars) {
  $options = $vars['options'];
  $vars['object'] = ''; // make sure var is defined.

  // We delegate the manipulation of the object to a function
  // which every module implementing the ds_api should define.
  // This can either be the simple selector or the advanced selector.
  // The advanced selector searches for a function build like this:
  // ds_views_row_adv_VIEWSNAME.
  if ($options['advanced']) {
    $build_mode = $options['build_mode'];
    $row_function = 'ds_views_row_adv_'. $vars['view']->name;
    $row_function($vars, $build_mode);
  }
  // Simple selector.
  else {
    $row_function = 'ds_views_row_'. $vars['view']->base_table;
    $build_mode = $options['build_mode'];
    $row_function($vars, $build_mode);
  }
}

/**
 * Preprocess theme function to print a single record from a row, with fields.
 */
function template_preprocess_ds_row_fields(&$vars) {
  template_preprocess_views_view_fields($vars);
}
