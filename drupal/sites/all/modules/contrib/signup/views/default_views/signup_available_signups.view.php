<?php
// $Id: signup_available_signups.view.php,v 1.1 2009/10/03 01:21:58 dww Exp $

/**
 * @file
 * Subtab of user/N/signups for all signup-enabled nodes that the user hasn't
 * replied to and the "Available signups" block.
 */

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
