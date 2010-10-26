<?php

// Grab the active theme. Adjust theme variable calls. That way, we don't have to repeat these calls in
// each child theme.
$active_theme = variable_get('theme_default', 'watershed');

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting($active_theme . '_rebuild_registry')) {
  drupal_rebuild_theme_registry();
}

// If applicable, add a color palette stylesheet based upon theme settings.
if ($color = theme_get_setting($active_theme . '_color_palette')) {
  drupal_add_css( drupal_get_path('theme', $active_theme) .'/css/colors/' . $color . '.css', 'theme');
}

// Add Zen Tabs styles
if (theme_get_setting($active_theme . '_zen_tabs')) {
  drupal_add_css( drupal_get_path('theme', 'watershed') .'/css/tabs.css', 'theme', 'screen');
}

// Add "wireframing CSS" to sketch out layout
if (theme_get_setting($active_theme . '_wireframe')) {
  drupal_add_css(drupal_get_path('theme', 'watershed') .'/css/wireframing.css', 'theme');
}

/*
 *	 This function creates the body classes that are relative to each page
 *
 *	@param $vars
 *	  A sequential array of variables to pass to the theme template.
 *	@param $hook
 *	  The name of the theme function being called ("page" in this case.)
 */
function watershed_preprocess_page(&$vars, $hook) {

  // Grab the active theme. Adjust theme variable calls. That way, we don't have to repeat these calls in
  // each child theme.
  $active_theme = variable_get('theme_default', 'watershed');
  
  // Don't display empty help from node_help().
  if ($vars['help'] == '<div class="help"><p></p>\n</div>') {
    $vars['help'] = '';
  }

  $vars['mission'] = variable_get('site_mission', '');  //add mission to all pages
  $vars['newsletter'] = variable_get('site_newsletter', '');

  // Theme the mission statement and other theme variables similar to a block.
  // This cuts down on hardcoding in page.tpl.php and also lets us add hover edit links in hook_preprocess_block().
  if( !empty($vars['mission']) ) {
    $vars['mission'] = theme('block',(object)array(
      'subject' => 'about us',
      'delta' => 'mission',
      'module' => 'watershed',
      'content' => $vars['mission']
    ));
  }

  if( !empty($vars['search_box']) ) {
    $vars['search_box'] = theme('block',(object)array(
      'subject' => 'search',
      'delta' => 'search',
      'content' => $vars['search_box']
    ));
  }

  if( !empty($vars['newsletter']) ) {
    $vars['newsletter'] = theme('block',(object)array(
      'subject' => 'newsletter signup',
      'delta' => 'newsletter',
      'module' => 'watershed',
      'content' => $vars['newsletter']
    ));
  }

  if( function_exists('_follow_block_content') ) {
    $follow_content = _follow_block_content();
    if( !empty($follow_content) ) {
      $vars['follow_links'] = theme('block',(object)array(
        'subject' => 'stay in the loop',
        'delta' => 'site',
        'module' => 'follow',
        'content' => $follow_content
      ));
    }
  }

  if( !empty($vars['secondary_links']) ) {
    $vars['secondary_links'] = theme('block',(object)array(
      'subject' => 'navigate',
      'delta' => 'secondary_links',
      'module' => 'menu',
      'content' => theme('links',$vars['secondary_links'])
    ));
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  $body_classes = array($vars['body_classes']);
  if (user_access('administer blocks')) {
	  $body_classes[] = 'admin';
	}
	if (theme_get_setting($active_theme . '_wireframe')) {
    $body_classes[] = 'with-wireframes'; // Optionally add the wireframes style.
  }
  if (!empty($vars['primary_links']) or !empty($vars['secondary_links'])) {
    $body_classes[] = 'with-navigation';
  }
  if (!empty($vars['secondary_links'])) {
    $body_classes[] = 'with-secondary';
  }
  if (module_exists('taxonomy') && $vars['node']->nid) {
    foreach (taxonomy_node_get_terms($vars['node']) as $term) {
    $body_classes[] = 'tax-' . eregi_replace('[^a-z0-9]', '-', $term->name);
    }
  }
  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    list($section, ) = explode('/', $path, 2);
    $body_classes[] = watershed_id_safe('page-'. $path);
    $body_classes[] = watershed_id_safe('section-'. $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-add'; // Add 'section-node-add'
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-'. arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

  if ($vars['node']->type != '') {
      $vars['template_files'][] = 'page-type-' . $vars['node']->type;
    }
  if ($vars['node']->nid != '') {
      $vars['template_files'][] = 'page-node-' . $vars['node']->nid;
    }
  $vars['body_classes'] = implode(' ', $body_classes); // Concatenate with spaces
}

/*
 *	 This function creates the NODES classes, like 'node-unpublished' for nodes
 *	 that are not published, or 'node-mine' for node posted by the connected user...
 *
 *	@param $vars
 *	  A sequential array of variables to pass to the theme template.
 *	@param $hook
 *	  The name of the theme function being called ("node" in this case.)
 */

function watershed_preprocess_node(&$vars, $hook) {
  // Special classes for nodes
  $classes = array('node');
  if ($vars['sticky']) {
    $classes[] = 'sticky';
  }
  // support for Skinr Module
  if (module_exists('skinr')) {
    $classes[] = $vars['skinr'];
  }
  if (!$vars['status']) {
    $classes[] = 'node-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  if ($vars['teaser']) {
    $classes[] = 'node-teaser'; // Node is displayed as teaser.
  }
  $classes[] = 'clearfix';

  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $classes[] = watershed_id_safe('node-type-' . $vars['type']);
  $vars['classes'] = implode(' ', $classes); // Concatenate with spaces
}

/*
 *	This function create the EDIT LINKS for blocks and menus blocks.
 *	When overing a block (except in IE6), some links appear to edit
 *	or configure the block. You can then edit the block, and once you are
 *	done, brought back to the first page.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("block" in this case.)
 */

function watershed_preprocess_block(&$vars, $hook) {
  
  // Grab the active theme. Adjust theme variable calls. That way, we don't have to repeat these calls in
  // each child theme.
  $active_theme = variable_get('theme_default', 'watershed');
        
  $block = $vars['block'];
  // special block classes
  $classes = array('block');
  $classes[] = watershed_id_safe('block-' . $vars['block']->module);
  $classes[] = watershed_id_safe('block-' . $vars['block']->region);
  $classes[] = watershed_id_safe('block-id-' . $vars['block']->bid);
  $classes[] = 'clearfix';

  // support for Skinr Module
  if (module_exists('skinr')) {
    $classes[] = $vars['skinr'];
  }

  $vars['block_classes'] = implode(' ', $classes); // Concatenate with spaces

  if (theme_get_setting($active_theme . '_block_editing') && user_access('administer blocks') && ($block->module != 'boxes')) {
      // Display 'edit block' for custom blocks.
      if ($block->module == 'block') {
        $edit_links[] = l('<span>' . t('edit block') . '</span>', 'admin/build/block/configure/' . $block->module . '/' . $block->delta,
          array(
            'attributes' => array(
              'title' => t('edit the content of this block'),
              'class' => 'block-edit',
            ),
            'query' => drupal_get_destination(),
            'html' => TRUE,
          )
        );
      }
      // Display 'configure link' for site settings pseudo-blocks.
      elseif ($block->module == 'watershed' && user_access('administer site configuration')) {
        $edit_links[] = l('<span>' . t('configure') . '</span>', 'admin/settings/site-information',
          array(
            'attributes' => array(
              'title' => t('Configure site settings'),
              'class' => 'block-config',
            ),
            'query' => drupal_get_destination(),
            'html' => TRUE,
          )
        );  
      }
      // Display 'configure' for other blocks.
      else {
        $edit_links[] = l('<span>' . t('configure') . '</span>', 'admin/build/block/configure/' . $block->module . '/' . $block->delta,
          array(
            'attributes' => array(
              'title' => t('configure this block'),
              'class' => 'block-config',
            ),
            'query' => drupal_get_destination(),
            'html' => TRUE,
          )
        );
      }
      // Display 'edit menu' for Menu blocks.
      if (($block->module == 'menu' || ($block->module == 'user' && $block->delta == 1)) && user_access('administer menu')) {
        $menu_name = ($block->module == 'user') ? 'navigation' : $block->delta;
        $edit_links[] = l('<span>' . t('edit menu') . '</span>', 'admin/build/menu-customize/' . $menu_name,
          array(
            'attributes' => array(
              'title' => t('edit the menu that defines this block'),
              'class' => 'block-edit-menu',
            ),
            'query' => drupal_get_destination(),
            'html' => TRUE,
          )
        );
      }
      // Display 'edit menu' for Menu block blocks.
      elseif ($block->module == 'menu_block' && user_access('administer menu')) {
        list($menu_name, ) = split(':', variable_get("menu_block_{$block->delta}_parent", 'navigation:0'));
        $edit_links[] = l('<span>' . t('edit menu') . '</span>', 'admin/build/menu-customize/' . $menu_name,
          array(
            'attributes' => array(
              'title' => t('edit the menu that defines this block'),
              'class' => 'block-edit-menu',
            ),
            'query' => drupal_get_destination(),
            'html' => TRUE,
          )
        );
      }
      // Display 'edit links' for Follow blocks.
      elseif ($block->module == 'follow' && user_access('edit site follow links')) {
        $edit_links[] = l('<span>' . t('edit links') . '</span>', 'admin/build/follow',
          array(
            'attributes' => array(
              'title' => t('edit the follow links'),
              'class' => 'block-edit-menu',
            ),
            'query' => drupal_get_destination(),
            'html' => TRUE,
          )
        );
      }
      $vars['edit_links_array'] = $edit_links;
      $vars['edit_links'] = '<div class="edit">' . implode(' ', $edit_links) . '</div>';
    }
}

/*
 * Override or insert PHPTemplate variables into the block templates.
 *
 *  @param $vars
 *    An array of variables to pass to the theme template.
 *  @param $hook
 *    The name of the template being rendered ("comment" in this case.)
 */

function watershed_preprocess_comment(&$vars, $hook) {
  // Add an "unpublished" flag.
  $vars['unpublished'] = ($vars['comment']->status == COMMENT_NOT_PUBLISHED);

  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $vars['node']->type, 1) == 0) {
    $vars['title'] = '';
  }

  // Special classes for comments.
  $classes = array('comment');
  if ($vars['comment']->new) {
    $classes[] = 'comment-new';
  }
  $classes[] = $vars['status'];
  $classes[] = $vars['zebra'];
  if ($vars['id'] == 1) {
    $classes[] = 'first';
  }
  if ($vars['id'] == $vars['node']->comment_count) {
    $classes[] = 'last';
  }
  if ($vars['comment']->uid == 0) {
    // Comment is by an anonymous user.
    $classes[] = 'comment-by-anon';
  }
  else {
    if ($vars['comment']->uid == $vars['node']->uid) {
      // Comment is by the node author.
      $classes[] = 'comment-by-author';
    }
    if ($vars['comment']->uid == $GLOBALS['user']->uid) {
      // Comment was posted by current user.
      $classes[] = 'comment-mine';
    }
  }
  $vars['classes'] = implode(' ', $classes);
}

/*
 * 	Customize the PRIMARY and SECONDARY LINKS, to allow the admin tabs to work on all browsers
 * 	An implementation of theme_menu_item_link()
 *
 * 	@param $link
 * 	  array The menu item to render.
 * 	@return
 * 	  string The rendered menu item.
 */

function watershed_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }

  // If an item is a LOCAL TASK, render it as a tab
  if ($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab">' . check_plain($link['title']) . '</span>';
    $link['localized_options']['html'] = TRUE;
  }

  return l($link['title'], $link['href'], $link['localized_options']);
}


/*
 *  Duplicate of theme_menu_local_tasks() but adds clear-block to tabs.
 */

function watershed_menu_local_tasks() {
  $output = '';
  if ($primary = menu_primary_local_tasks()) {
    if(menu_secondary_local_tasks()) {
      $output .= '<ul class="tabs primary with-secondary clearfix">' . $primary . '</ul>';
    }
    else {
      $output .= '<ul class="tabs primary clearfix">' . $primary . '</ul>';
    }
  }
  if ($secondary = menu_secondary_local_tasks()) {
    $output .= '<ul class="tabs secondary clearfix">' . $secondary . '</ul>';
  }
  return $output;
}

/*
 * 	Add custom classes to menu item
 */

function watershed_menu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
  $class = ($menu ? 'expanded' : ($has_children ? 'collapsed' : 'leaf'));
  if (!empty($extra_class)) {
    $class .= ' '. $extra_class;
  }
  if ($in_active_trail) {
    $class .= ' active-trail';
  }
#New line added to get unique classes for each menu item
  $css_class = watershed_id_safe(str_replace(' ', '_', strip_tags($link)));
  return '<li class="'. $class . ' ' . $css_class . '">' . $link . $menu ."</li>\n";
}

/*
 *	Converts a string to a suitable html ID attribute.
 *
 *	 http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 *	 valid ID attribute in HTML. This function:
 *
 *	- Ensure an ID starts with an alpha character by optionally adding an 'n'.
 *	- Replaces any character except A-Z, numbers, and underscores with dashes.
 *	- Converts entire string to lowercase.
 *
 *	@param $string
 *	  The string
 *	@return
 *	  The converted string
 */

function watershed_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'n' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id'. $string;
  }
  return $string;
}

/*
 *  Return a themed breadcrumb trail.
 *	Alow you to customize the breadcrumb markup
 */

function watershed_breadcrumb($breadcrumb) {
  // Grab the active theme. Adjust theme variable calls. That way, we don't have to repeat these calls in
  // each child theme.
  $active_theme = variable_get('theme_default', 'watershed');
  
  if (theme_get_setting($active_theme . '_breadcrumb') && !empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' » ', $breadcrumb) .'</div>';
  }
}

