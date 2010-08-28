<?php
// $Id: signup.views_default.inc,v 1.1.2.7 2009/07/22 21:47:47 dww Exp $


/**
 * @file
 * Provides default views on behalf of the signup module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function signup_views_default_views() {
  // Signup schedule at user/N/signups and the "Current signups" block.
  $view = new view;
  $view->name = 'signup_current_signups';
  $view->description = 'A list of all signups for the current user';
  $view->tag = 'Signup';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'uid' => array(
      'label' => 'Signup user',
      'required' => 1,
      'id' => 'uid',
      'table' => 'signup_log',
      'field' => 'uid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Current signups for %1',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'user',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'validate_user_restrict_roles' => 0,
      'validate_argument_signup_status' => 'any',
      'validate_argument_signup_node_access' => 0,
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
    ),
    'status' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'status',
        'label' => 'Signup status',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'status',
      'table' => 'signup',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'user_signup_list',
    'signup_user_argument' => 'uid',
  ));
  $handler->override_option('title', 'Current Signups');
  $handler->override_option('empty', 'This user has not signed up for any matching content.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'list');
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('path', 'user/%/signups/current');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'Current',
    'description' => '',
    'weight' => '-2',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'tab',
    'title' => 'Signups',
    'description' => '',
    'weight' => '10',
  ));
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Current signups for %1',
      'default_argument_type' => 'current_user',
      'default_argument' => '',
      'validate_type' => 'user',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'validate_user_restrict_roles' => 0,
      'validate_argument_signup_status' => 'any',
      'validate_argument_signup_node_access' => 0,
    ),
  ));
  $handler->override_option('items_per_page', 10);
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  // Subtab of user/N/signups for all signup-enabled nodes that the
  // user hasn't replied to and the "Available signups" block.
  $view = new view;
  $view->name = 'signup_available_signups';
  $view->description = 'A view of all available signups (signup-enabled content the user has not replied to).';
  $view->tag = 'Signup';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid_no_signups' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Available signups for %1',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'user',
      'validate_fail' => 'not found',
      'break_phrase' => FALSE,
      'not' => FALSE,
      'id' => 'uid_no_signups',
      'table' => 'signup_log',
      'field' => 'uid_no_signups',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'validate_user_restrict_roles' => 0,
      'validate_argument_signup_status' => 'any',
      'validate_argument_signup_node_access' => 0,
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
    ),
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'signup',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'user_signup_list',
    'signup_user_argument' => 'uid_no_signups',
  ));
  $handler->override_option('title', 'Available signups');
  $handler->override_option('empty', 'Congratulations, you have replied to all of the content on the site that is accepting signups.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'type' => 'ul',
  ));
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('path', 'user/%/signups/available');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Available',
    'description' => '',
    'weight' => '10',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->override_option('arguments', array(
    'uid_no_signups' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Available signups for %1',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'user',
      'validate_fail' => 'not found',
      'break_phrase' => FALSE,
      'not' => FALSE,
      'id' => 'uid_no_signups',
      'table' => 'signup_log',
      'field' => 'uid_no_signups',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'validate_user_restrict_roles' => 0,
      'validate_argument_signup_status' => 'any',
      'validate_argument_signup_node_access' => 0,
    ),
  ));
  $handler->override_option('items_per_page', 10);
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  // All users who have signed up for a given node.
  $view = new view;
  $view->name = 'signup_user_list';
  $view->description = 'List of all users signed up for a given node';
  $view->tag = 'Signup';
  $view->view_php = '';
  $view->base_table = 'signup_log';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'uid' => array(
      'label' => 'Signup user',
      'required' => 1,
      'id' => 'uid',
      'table' => 'signup_log',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'Signup node',
      'required' => 1,
      'id' => 'nid',
      'table' => 'signup_log',
      'field' => 'nid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Name',
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'uid',
    ),
    'signup_time' => array(
      'label' => 'Signup time',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'signup_time',
      'table' => 'signup_log',
      'field' => 'signup_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'signup_status',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'nid',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_signup_status' => 'any',
      'validate_argument_signup_node_access' => 1,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view all signups',
  ));
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'signup_time' => 'signup_time',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'signup_time' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'signup_time',
  ));
  $views[$view->name] = $view;

  if (module_exists('views_bulk_operations')) {
    $view = new view;
    $view->name = 'signup_user_vbo_admin_list';
    $view->description = t('Per-node signup administration interface with checkboxes for bulk operations.');
    $view->tag = 'Signup';
    $view->view_php = '';
    $view->base_table = 'signup_log';
    $view->is_cacheable = FALSE;
    $view->api_version = 2;
    $view->disabled = FALSE;
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->override_option('relationships', array(
      'nid' => array(
        'label' => 'Signup node',
        'required' => 1,
        'id' => 'nid',
        'table' => 'signup_log',
        'field' => 'nid',
        'relationship' => 'none',
      ),
      'uid' => array(
        'label' => 'Signup user',
        'required' => 1,
        'id' => 'uid',
        'table' => 'signup_log',
        'field' => 'uid',
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('fields', array(
      'name' => array(
        'label' => 'Name',
        'link_to_user' => 1,
        'exclude' => 0,
        'id' => 'name',
        'table' => 'users',
        'field' => 'name',
        'relationship' => 'uid',
      ),
      'signup_time' => array(
        'label' => 'Signup time',
        'date_format' => 'small',
        'custom_date_format' => '',
        'exclude' => 0,
        'id' => 'signup_time',
        'table' => 'signup_log',
        'field' => 'signup_time',
        'relationship' => 'none',
      ),
      'form_data' => array(
        'label' => 'Additional info',
        'type' => 'ul',
        'separator' => ', ',
        'empty' => '',
        'exclude' => 0,
        'id' => 'form_data',
        'table' => 'signup_log',
        'field' => 'form_data',
        'relationship' => 'none',
        'form_data_fieldname' => '',
      ),
      'attended' => array(
        'label' => 'Attendance',
        'exclude' => 0,
        'id' => 'attended',
        'table' => 'signup_log',
        'field' => 'attended',
        'relationship' => 'none',
      ),
      'edit_link' => array(
        'label' => 'Operations',
        'text' => '',
        'exclude' => 0,
        'id' => 'edit_link',
        'table' => 'signup_log',
        'field' => 'edit_link',
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('arguments', array(
      'nid' => array(
        'default_action' => 'ignore',
        'style_plugin' => 'default_summary',
        'style_options' => array(),
        'wildcard' => 'all',
        'wildcard_substitution' => 'All',
        'title' => '',
        'default_argument_type' => 'fixed',
        'default_argument' => '',
        'validate_type' => 'signup_status',
        'validate_fail' => 'not found',
        'break_phrase' => 0,
        'not' => 0,
        'id' => 'nid',
        'table' => 'node',
        'field' => 'nid',
        'relationship' => 'nid',
        'default_options_div_prefix' => '',
        'default_argument_user' => 0,
        'default_argument_fixed' => '',
        'default_argument_php' => '',
        'validate_argument_node_access' => 0,
        'validate_argument_nid_type' => 'nid',
        'validate_argument_vocabulary' => array(),
        'validate_argument_type' => 'tid',
        'validate_argument_signup_status' => 'any',
        'validate_argument_signup_node_access' => 1,
        'validate_argument_php' => '',
      ),
    ));
    $handler->override_option('access', array(
      'type' => 'none',
    ));
    $handler->override_option('items_per_page', 100);
    $handler->override_option('style_plugin', 'bulk');
    $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'name' => 'name',
        'signup_time' => 'signup_time',
        'form_data' => 'form_data',
        'attended' => 'attended',
        'edit_link' => 'edit_link',
      ),
      'info' => array(
        'name' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'signup_time' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'form_data' => array(
          'separator' => '',
        ),
        'attended' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'edit_link' => array(
          'separator' => '',
        ),
      ),
      'default' => '-1',
      'execution_type' => '1',
      'display_type' => '0',
      'skip_confirmation' => 0,
      'display_result' => 1,
      'selected_operations' => array(
        '5c7373a6aa7a8bf5549412c9f887c2d3' => '5c7373a6aa7a8bf5549412c9f887c2d3',
        'd3a1a31a80ae6f3a3445aa18d827c62e' => 'd3a1a31a80ae6f3a3445aa18d827c62e',
        '486e69f274fc0c7fb96d00a55ce752ae' => '486e69f274fc0c7fb96d00a55ce752ae',
      ),
    ));
    $views[$view->name] = $view;
  }

  $view = new view;
  $view->name = 'signup_user_admin_list';
  $view->description = t('Administrative list of all users signed up for a given node.');
  $view->tag = 'Signup';
  $view->view_php = '';
  $view->base_table = 'signup_log';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'nid' => array(
      'label' => 'Signup node',
      'required' => 1,
      'id' => 'nid',
      'table' => 'signup_log',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'uid' => array(
      'label' => 'Signup user',
      'required' => 1,
      'id' => 'uid',
      'table' => 'signup_log',
      'field' => 'uid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Name',
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'uid',
    ),
    'signup_time' => array(
      'label' => 'Signup time',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'signup_time',
      'table' => 'signup_log',
      'field' => 'signup_time',
      'relationship' => 'none',
    ),
    'form_data' => array(
      'label' => 'Additional info',
      'type' => 'ul',
      'separator' => ', ',
      'empty' => '',
      'exclude' => 0,
      'id' => 'form_data',
      'table' => 'signup_log',
      'field' => 'form_data',
      'relationship' => 'none',
      'form_data_fieldname' => '',
    ),
    'attended' => array(
      'label' => 'Attendance',
      'exclude' => 0,
      'id' => 'attended',
      'table' => 'signup_log',
      'field' => 'attended',
      'relationship' => 'none',
    ),
    'edit_link' => array(
      'label' => 'Operations',
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_link',
      'table' => 'signup_log',
      'field' => 'edit_link',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'signup_status',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'nid',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_signup_status' => 'any',
      'validate_argument_signup_node_access' => 1,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 100);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'name' => 'name',
      'signup_time' => 'signup_time',
      'form_data' => 'form_data',
      'attended' => 'attended',
      'edit_link' => 'edit_link',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'signup_time' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'form_data' => array(
        'separator' => '',
      ),
      'attended' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'edit_link' => array(
        'separator' => '',
      ),
    ),
    'default' => 'signup_time',
  ));
  $views[$view->name] = $view;

  return $views;
}
