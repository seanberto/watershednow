<?php
// $Id: signup_current_signups.view.php,v 1.1 2009/10/03 01:21:58 dww Exp $

/**
 * @file
 * Signup schedule at user/N/signups and the "Current signups" block.
 */

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
