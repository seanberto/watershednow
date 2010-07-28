<?php
// $Id: context.core.inc,v 1.1.2.3.2.20.2.18 2010/05/25 21:57:44 yhahn Exp $

/**
 * Implementation of hook_context_registry().
 */
function context_context_registry() {
  $registry = array();
  $registry['conditions'] = array(
    'node' => array(
      'title' => t('Node type'),
      'description' => t('Set this context when viewing a node page or using the add/edit form of one of these content types.'),
      'plugin' => 'context_condition_node',
    ),
    'sitewide' => array(
      'title' => t('Sitewide context'),
      'description' => t('Should this context always be set? If <strong>true</strong>, this context will be active across your entire site.'),
      'plugin' => 'context_condition_sitewide',
    ),
    'path' => array(
      'title' => t('Path'),
      'description' => t('Set this context when any of the paths above match the page path. Put each path on a separate line. You can use the "*" character as a wildcard and &lt;front&gt; for the front page.'),
      'plugin' => 'context_condition_path',
    ),
    'user' => array(
      'title' => t('User role'),
      'description' => t('Set this context when the current user has one of the selected role(s).'),
      'plugin' => 'context_condition_user',
    ),
    'menu' => array(
      'title' => t('Menu'),
      'description' => t('Set this context when any of the selected menu items belong to the current active menu trail.'),
      'plugin' => 'context_condition_menu',
    ),
  );
  if (module_exists('views')) {
    $registry['conditions']['views'] = array(
      'title' => t('Views'),
      'description' => t('Set this context when displaying the page of one of these views.'),
      'plugin' => 'context_condition_views',
    );
  }
  if (module_exists('book')) {
    $registry['conditions']['book'] = array(
      'title' => t('Book'),
      'description' => t('Set this context when a node in the selected book is viewed.'),
      'plugin' => 'context_condition_book',
    );
    $registry['conditions']['bookroot'] = array(
      'title' => t('Book root'),
      'description' => t('Set this context when viewing a node whose root book is of the selected type.'),
      'plugin' => 'context_condition_bookroot',
    );
  }
  if (module_exists('locale')) {
    $registry['conditions']['language'] = array(
      'title' => t('Language'),
      'description' => t('Set this context when viewing the site in the selected language.'),
      'plugin' => 'context_condition_language',
    );
  }
  if (module_exists('taxonomy')) {
    $registry['conditions']['node_taxonomy'] = array(
      'title' => t('Taxonomy'),
      'description' => t('Set this context when viewing a node with the selected taxonomy terms.'),
      'plugin' => 'context_condition_node_taxonomy',
    );
  }
  $registry['reactions'] = array(
    'block' => array(
      'title' => t('Blocks'),
      'description' => t('Control block visibility using context.'),
      'plugin' => 'context_reaction_block',
    ),
    'menu' => array(
      'title' => t('Menu'),
      'description' => t('Control menu active class using context.'),
      'plugin' => 'context_reaction_menu',
    ),
    'theme' => array(
      'title' => t('Theme'),
      'description' => t('Control theme variables using context.'),
      'plugin' => 'context_reaction_theme',
    ),
    'debug' => array(
      'title' => t('Debug'),
      'description' => t('Debug output reaction for SimpleTest.'),
      'plugin' => 'context_reaction_debug',
    ),
  );
  if (module_exists('css_injector')) {
    $registry['reactions']['css_injector'] = array(
      'title' => t('CSS Injector'),
      'description' => t('Inject the selected css when this context is set.'),
      'plugin' => 'context_reaction_css_injector',
    );
  }
  return $registry;
}

/**
 * Implementation of hook_theme().
 */
function context_theme() {
  $items = array();
  $items['context_block_form'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_regions_form'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_editor'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_browser'] = array(
    'arguments' => array('blocks' => array(), 'context' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'template' => 'context-block-browser',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_browser_item'] = array(
    'arguments' => array('block' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'template' => 'context-block-browser-item',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_editable_block'] = array(
    'arguments' => array('block' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'template' => 'context-block-editable-block',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_editable_region'] = array(
    'arguments' => array('region' => '', 'blocks' => array()),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'template' => 'context-block-editable-region',
    'file' => 'context_reaction_block.theme.inc',
  );
  $items['context_block_script_placeholder'] = array(
    'arguments' => array('text' => NULL),
    'path' => drupal_get_path('module', 'context') . '/theme',
    'file' => 'context_reaction_block.theme.inc',
  );
  return $items;
}

/**
 * Implementation of hook_theme_registry_alter().
 */
function context_theme_registry_alter(&$theme_registry) {
  // Push theme_page() through a context_preprocess to provide
  // context-sensitive menus and variables. Ensure that
  // context_preprocess_page() comes immediately after
  // template_preprocess_page().
  $position = array_search('context_preprocess_page', $theme_registry['page']['preprocess functions']);
  if ($position !== FALSE) {
    unset($theme_registry['page']['preprocess functions'][$position]);
  }
  $position = array_search('template_preprocess_page', $theme_registry['page']['preprocess functions']);
  $position = $position ? $position + 1 : 2;
  array_splice($theme_registry['page']['preprocess functions'], $position, 0, 'context_preprocess_page');

  // Add a page preprocess function to the very top of the theme_page()
  // stack so that we can actually set contexts *before* the page theming
  // is executed.
  if (!in_array('context_page_alter', $theme_registry['page']['preprocess functions'])) {
    array_unshift($theme_registry['page']['preprocess functions'], 'context_page_alter');
  }
  // Reroute theme_blocks() through context_blocks to determine block
  // visibility by context. Only override theme_blocks() if a theme
  // has not overridden it. It is the responsibility of any themes
  // implementing theme_blocks() to take advantage of context block
  // visibility on their own.
  if (!isset($theme_registry['blocks']['type']) || !in_array($theme_registry['blocks']['type'], array('base_theme_engine', 'theme_engine')) && !isset($theme_registry['blocks']['file'])) {
    unset($theme_registry['blocks']['preprocess functions']);
    $theme_registry['blocks']['function'] = 'context_blocks';
  }
}

/**
 * Implementation of hook_ctools_render_alter().
 * Used to detect the presence of a page manager node view or node form.
 */
function context_ctools_render_alter($info, $page, $args, $contexts, $task, $subtask) {
  if ($page && in_array($task['name'], array('node_view', 'node_edit'), TRUE)) {
    foreach ($contexts as $ctools_context) {
      if ($ctools_context->type === 'node' && !empty($ctools_context->data)) {
        context_node_condition($ctools_context->data, $task['name'] === 'node_view' ? 'view' : 'form');
        break;
      }
    }
  }
}

/**
 * Implementation of hook_nodeapi().
 */
function context_nodeapi(&$node, $op, $teaser, $page) {
  if ($op == 'view' && $page) {
    $object = menu_get_object();
    if (isset($object->nid) && $object->nid === $node->nid) {
      context_node_condition($node, 'view');
    }
  }
}

/**
 * Implementation of hook_form_alter().
 */
function context_form_alter(&$form, $form_state, $form_id) {
  // Prevent this from firing on admin pages... damn form driven apis...
  if ($form['#id'] === 'node-form' && arg(0) != 'admin') {
    context_node_condition($form['#node'], 'form');
  }
  else if ($form_id == 'system_modules') {
    context_invalidate_cache();
  }
}

/**
 * Centralized node condition call function for the ever increasing number of
 * ways to get at a node view / node form.
 */
function context_node_condition(&$node, $op) {
  if ($plugin = context_get_plugin('condition', 'node')) {
    $plugin->execute($node, $op);
  }
  if (module_exists('taxonomy')) {
    if ($plugin = context_get_plugin('condition', 'node_taxonomy')) {
      $plugin->execute($node, $op);
    }
  }
  if (module_exists('book')) {
    if ($plugin = context_get_plugin('condition', 'book')) {
      $plugin->execute($node, $op);
    }
    if ($plugin = context_get_plugin('condition', 'bookroot')) {
      $plugin->execute($node, $op);
    }
  }
}

/**
 * Implementation of hook_form_alter() for comment_form.
 */
function context_form_comment_form_alter(&$form, $form_state) {
  if ($nid = $form['nid']['#value']) {
    $plugin = context_get_plugin('condition', 'node');
    $plugin->execute($node = node_load($nid), 'comment');
  }
}

/**
 * Implementation of hook_views_pre_view().
 */
function context_views_pre_view($view, $args) {
  if ($plugin = context_get_plugin('condition', 'views')) {
    $plugin->execute($view);
  }
}

/**
 * BLOCK HANDLING =====================================================
 */

/**
 * This override of theme_blocks() is called because of an alter of the
 * theme registry. See context_theme_registry_alter().
 */
function context_blocks($region) {
  if ($plugin = context_get_plugin('reaction', 'block')) {
    return $plugin->execute($region);
  }
}

/**
 * THEME FUNCTIONS & RELATED ==========================================
 */

/**
 * Generates an array of links (suitable for use with theme_links)
 * to the node forms of types associated with current active contexts.
 */
function context_links($reset = FALSE) {
  static $links;
  if (!$links || $reset) {
    $contexts = context_active_contexts();
    $active_types = array();
    $conditions = array('node', 'bookroot');
    foreach ($conditions as $condition) {
      foreach ($contexts as $k => $v) {
        if (!empty($v->conditions[$condition]['values'])) {
          $active_types = array_merge($active_types, array_filter($v->conditions[$condition]['values']));
        }
      }
    }

    $links = array();
    if (!empty($active_types)) {
      // Iterate over active contexts
      foreach ($active_types as $type) {
        $add_url = 'node/add/'. str_replace('_', '-', $type);
        $item = menu_get_item($add_url);
        if ($item && $item['access'] && strpos($_GET['q'], $add_url) !== 0) {
          $links[$type] = array('title' => t('Add @type', array('@type' => node_get_types('name', $type))), 'href' => $add_url);
        }
      }
    }
    drupal_alter('context_links', $links);
    uasort($links, 'element_sort');
  }
  return $links;
}

/**
 * A preprocess_page() function that is called *before* all other
 * preprocessors (including template_preprocess_page()). This allows
 * any final context conditions to be set and any initial reactions
 * to be triggered.
 */
function context_page_alter(&$vars) {
  module_invoke_all('context_page_condition');
  module_invoke_all('context_page_reaction');
}

/**
 * Implementation of hook_context_page_condition().
 */
function context_context_page_condition() {
  if ($plugin = context_get_plugin('condition', 'menu')) {
    $plugin->execute();
  }
  if ($plugin = context_get_plugin('condition', 'sitewide')) {
    $plugin->execute(1);
  }
}

/**
 * Implementation of hook_context_page_reaction().
 */
function context_context_page_reaction() {
  if ($plugin = context_get_plugin('reaction', 'css_injector')) {
    $plugin->execute();
  }
  if ($plugin = context_get_plugin('reaction', 'debug')) {
    $plugin->execute();
  }
}

/**
 * Implementation of preprocess_page().
 */
function context_preprocess_page(&$vars) {
  $plugin = context_get_plugin('reaction', 'menu');
  $plugin->execute($vars);
  $plugin = context_get_plugin('reaction', 'theme');
  $plugin->execute($vars);

  if ($context_links = context_links()) {
    $vars['context_links'] = theme('links', $context_links);
  }
  else {
    $vars['context_links'] = '';
  }
}
