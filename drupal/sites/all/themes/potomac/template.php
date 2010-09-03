<?php
// $Id: template.php,v 1.1 2009/06/26 13:55:36 kiterminal Exp $

/**
 * @file
 */

/**
 * Initialize theme settings
 */
if (is_null(theme_get_setting('potomac'))) {
  global $theme_key;

  // Save default theme settings
  $defaults = array(
    'breadcrumb_display'                    => 1,
    'iepngfix_setting'                      => 1,
    'taxonomy_display_default'              => 'all',
    'taxonomy_enable_content_type'          => 0,
  );

  // Get default theme settings.
  $settings = theme_get_settings($theme_key);

  // Save default theme settings
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );

  // Force refresh of Drupal internals
  theme_get_setting('', TRUE);
}

function potomac_preprocess_page(&$vars) {
  if (theme_get_setting('potomac_iepngfix_setting')) {
    drupal_add_js(drupal_get_path('theme', 'potomac') .'/js/jquery.pngFix.js', 'theme');
  }

  // Set front page title
  if (drupal_is_front_page()) {
    $title = t(variable_get('site_name', ''));
    $vars['head_title'] = $title;
  }
  
  // Hide breadcrumb on all pages
  if (theme_get_setting('potomac_breadcrumb_display') == 0) {
    $vars['breadcrumb'] = '';  
  }

}

function potomac_preprocess_node(&$vars) {
  // Submitted
  if (isset($vars['submitted']) && $vars['submitted'] != '') {
    $date = format_date($vars['node']->created, 'custom', 'M jS, Y'); 
    $author = theme('username', $vars['node']);
    $vars['submitted'] = t('!date by !author', array('!date' => $date, '!author' => $author));
  }

  // Taxonomy, adapted from "Acquia Marina" theme
  $taxonomy_content_type = (theme_get_setting('potomac_taxonomy_enable_content_type') == 1) ? $vars['node']->type : 'default';
  $taxonomy_display = theme_get_setting('potomac_taxonomy_display_'. $taxonomy_content_type);
  if ((module_exists('taxonomy')) && ($taxonomy_display == 'all' || ($taxonomy_display == 'only' && $vars['page']))) {
    $vocabularies = taxonomy_get_vocabularies($vars['node']->type);
    $output = '';

    foreach ($vocabularies as $vocabulary) {
      $terms = taxonomy_node_get_terms_by_vocabulary($vars['node'], $vocabulary->vid);
      if ($terms) {
        $links = array();
        foreach ($terms as $term) {
          $links[] = l($term->name, taxonomy_term_path($term), array('attributes' => array('rel' => 'tag', 'title' => strip_tags($term->description))));
        }
        $output .= implode(", ", $links);
      }
    }
    if ($output != '') {
      $vars['terms'] = $output;
    }
  }
  else {
    $vars['terms'] = '';
  }
}

function potomac_preprocess_comment_wrapper(&$vars) {
  // get total number of comments
  $comments_count = comment_num_all($vars['node']->nid);

  if ($comments_count == 0) {
    $vars['comments_count'] = t('No response');
  }
  else {
    $vars['comments_count'] = t('@count @comment to “@node-title”', array('@count' => $comments_count, '@comment' => format_plural($comments_count, 'Response', 'Responses'), '@node-title' => $vars['node']->title));
  }
}

function potomac_preprocess_comment(&$vars) {
  $date = format_date($vars['comment']->timestamp, 'custom', 'j M Y');
  $time = format_date($vars['comment']->timestamp, 'custom', 'g:i a');
  $author = theme('username', $vars['comment']);
  $vars['submitted'] = t('!author on !date at !time', array('!author' => $author, '!date' => $date, '!time' => $time));
}
