<?php
// $Id: imagecache.inc,v 1.1.2.1 2008/08/15 20:12:47 quicksketch Exp $

/**
 * Just a helpful wrapper around Imagecache's built in functions.
 * 
 * @param $name
 *   The name of the imagecache preset.
 * @param $actions
 *   A numerically-indexed array of actions. The index becomes the weight of
 *   the action. Each action is itself an array with indexes of action, weight
 *   and any other data needed for the action.
 */
function install_imagecache_add_preset($name, $actions) {
  $presets = imagecache_presets();
  if (!in_array($name, $presets)) {
    $preset = imagecache_preset_save(array('presetname' => $name));
  }
  else {
    // Do we want to update? Probably not.
    return;
  }

  foreach ($actions as $weight => $data) {
    $action = array();
    $action['action'] = $data['action'];
    unset($data['action']);
    $action['presetid'] = $preset['presetid'];
    $action['weight'] = $weight;
    $action['data'] = $data;

    imagecache_action_save($action);
  }
}