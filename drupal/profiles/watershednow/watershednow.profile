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
// function watershednow_form_alter(&$form, $form_state, $form_id) {
//   if ($form_id == 'install_configure') {
//     $form['site_information']['site_name']['#default_value'] = $_SERVER['HTTP_HOST'];
//     $form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
//     $form['admin_account']['account']['name']['#default_value'] = 'admin';
//     $form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
//   }
// }

function _watershednow_profile_task_profile(&$task, $url) {
  require_once(dirname(__FILE__).'/libraries/profiler/profiler_module.inc');
  $config = profiler_v2_load_config('watershednow');
  $modules = array_merge($config['modules']['core'], $config['modules']['contrib'], $config['features']);
  
  $files = profiler_module_rebuild_cache();
  $operations = array();

  foreach ($modules as $module) {
    $operations[] = array('_install_module_batch', array($module, $files[$module]->info['name']));
  }
  $batch = array(
    'operations' => $operations,
    'finished' => '_watershed_module_install_finished',
    'title' => st('Installing @drupal', array('@drupal' => drupal_install_profile_name())),
    'error_message' => st('The installation has encountered an error.'),
  );
  // Start a batch, switch to 'profile-install-batch' task. We need to
  // set the variable here, because batch_process() redirects.
  variable_set('install_task', 'profile-install-batch');
  batch_set($batch);
  batch_process($url,$url);
}

function watershednow_profile_tasks(&$task, $url) {

  if ( $task == 'profile' ) {
    _watershednow_profile_task_profile($task, $url);
  }

  if( $task == 'watershednow-configure' ) {
    $task = 'profile-finished'; //hand control back to the installer
  }
  return '';
}

function _watershed_module_install_finished($success, $results) {
  variable_set('install_task', 'watershednow-configure');
}