<?php

/**
 * Implementation of preprocess_page().
 *
 * @param $vars
 */
function savannah_preprocess_page(&$vars) {
  // Add a new template suggestion to allow page-[node type].tpl.php 
  if(isset($vars['node'])) {
    $vars['template_files'][] = 'page-' . $vars['node']->type;
  }
  
  // Add a new template for admin pages
  if ((arg(0) == 'admin')) {
         $vars['template_file'] = 'page-admin';
   }

  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }

  // Add more classes for body element.
  $classes = explode(' ', $vars['body_classes']);

    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    $classes[] = 'page-' . drupal_strtolower(str_replace(array('/', '?','(', ')'), '-',  check_plain($path)));
    
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $classes[] = 'section-' . check_plain($section);
  
  $vars['body_classes_array'] = $classes;
  $vars['body_classes'] = implode(' ', $classes); // Concatenate with spaces.
  
}

/**
 * Register user theme functions
 */
function savannah_theme(){ // No theme function defined for the user login block
  return array(
    // Register theming function, it's user login block and not user login form
    'user_login_block' => array(
      'arguments' => array('form' => NULL),
      // template file, ie: user-login-block.tpl.php
      'template' => 'user-login-block',
    ),
  );
}

function savannah_preprocess_user_login_block(&$vars) {
  // Let's modify the output
  $vars['form']['submit']['#value'] = "Login";
  $vars['form']['name']['#size'] = "12";
  $vars['form']['pass']['#size'] = "12";
  $vars['form_markup'] = drupal_render($vars['form']);
}

/**
 * Override Search
 */
function savannah_preprocess_search_theme_form(&$vars, $hook) {
	// Modify values of the search form
	unset($vars['form']['search_theme_form']['#title']);
	$vars['form']['submit']['#value'] = "Find";

	// Rebuild the rendered version
	unset($vars['form']['search_theme_form']['#printed']);
	unset($vars['form']['submit']['#printed']);
	$vars['search']['search_theme_form'] = drupal_render($vars['form']['search_theme_form']);
	$vars['search']['submit'] = drupal_render($vars['form']['submit']);
	// Group all variables
	$vars['search_form'] = implode($vars['search']);
}

function savannah_comment_submitted($comment) {
  return t('!datetime by !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

function savannah_node_submitted($node) {
  return t('!datetime by !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}

/**
 * Override or insert PHPTemplate variables into the node template.
 * Print all terms by vocabulary
 */
function phptemplate_preprocess_node(&$vars) {
  // If we have any terms...
  if ($vars['node']->taxonomy) {
    // Let's iterate through each term.
    foreach ($vars['node']->taxonomy as $term) {
      // We will build a new array where there will be as many
      // nested arrays as there are vocabularies
      // The key for each nested array is the vocabulary ID.     
      $vocabulary[$term->vid]['taxonomy_term_'. $term->tid]  = array(
        'title' => $term->name,
        'href' => taxonomy_term_path($term),
        'attributes' => array(
          'rel' => 'tag', 
          'title' => strip_tags($term->description),
        ),
      );       
    }
    // Making sure vocabularies appear in the same order.
    ksort($vocabulary, SORT_NUMERIC);
    // We will get rid of the old $terms variable.
    unset($vars['terms']);
    // And build a new $terms.
    foreach ($vocabulary as $vid => $terms) {
      // Getting the name of the vocabulary.
      $name = taxonomy_vocabulary_load($vid)->name;
      // Using the theme('links', ...) function to theme terms list.
      $terms = theme('links', $terms, array('class' => 'links inline'));
      // Wrapping the terms list.
      $vars['terms'] .= '<div class="vocabulary ';
      $vars['terms'] .= strtolower($name);
      $vars['terms'] .= '">';
      $vars['terms'] .= $name;
      $vars['terms'] .= ':&nbsp;';
      $vars['terms'] .= $terms;
      $vars['terms'] .= '</div>';
    }
  }    
}