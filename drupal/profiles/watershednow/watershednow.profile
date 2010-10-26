<?php
/**
 * ## Install Workflow
 * 1. User is asked what profile they would like, user picks Watershed Now
 * 2. Dependencies are loaded, user puts in database credentials
 * 3. Dependencies are installed
 * 4. profiler_profile_tasks (in profiler_api.inc) is called and the following is done:
 *     1. all db blocks are disabled
 *     2. profiler components are run
 *     3. hook_install() is run
 *     4. menu_rebuild();
 *     5. module_rebuild_cache(); to detect the newly added bootstrap modules
 *     6. node_access_rebuild();
 *     7. drupal_get_schema('system', TRUE); // Clear schema DB cache
 *     8. drupal_flush_all_caches();
 *     9. profiler_install_configure($config);
 * 5. All modules specified in the watershednow.info are installed.
 * 6. _watershednow_configure() is called
 */

require_once('watershednow.inc');
$path = dirname(__FILE__).'/';
require_once( $path . 'libraries/profiler/profiler.inc');
require_once( $path . 'libraries/profiler/profiler_api.inc');
require_once( $path . 'libraries/profiler/profiler_module.inc');

// Bootstrap Profiler module. See: http:drupal.org/project/profiler for more info.
profiler_v2('watershednow');

function watershednow_profile_tasks(&$task, $url) {
  $config = profiler_v2_load_config('watershednow');

  if ( $task == 'profile' ) {
    profiler_profile_tasks($config, $task, $url);
    _watershednow_profile_task_profile($task, $url);
  }

  if( $task == 'watershednow-configure' ) {
    _watershednow_configure($config);
    drupal_flush_all_caches();

    /*
     * Enable all watershed themes
     * this must happen after clearing caches due to static variable
     * that can not be reset. Extra steps included to add theme variables to variables table.
     */
    _watershednow_install_themes();
    $task = 'profile-finished'; //hand control back to the installer
  }
  return '';
}

function _watershednow_profile_task_profile(&$task, $url) {
  $config = profiler_v2_load_config('watershednow');

  $modules = array_merge($config['modules']['core'], $config['modules']['contrib'], $config['features']);

  $files = module_rebuild_cache();
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

function _watershed_module_install_finished($success, $results) {
  variable_set('install_task', 'watershednow-configure');
}
