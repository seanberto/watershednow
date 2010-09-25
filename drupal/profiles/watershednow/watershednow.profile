<?php

// Bootstrap Profiler module. See: http:drupal.org/project/profiler for more info.
!function_exists('profiler_v2') ? require_once('libraries/profiler/profiler.inc') : FALSE;
profiler_v2('watershednow');

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function watershednow_form_alter(&$form, $form_state, $form_id) {
	if ($form_id == 'install_configure') {
		$form['site_information']['site_name']['#default_value'] = $_SERVER['HTTP_HOST'];
		$form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
		$form['admin_account']['account']['name']['#default_value'] = 'admin';
		$form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
	}
}