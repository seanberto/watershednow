// $Id: openlayers_behavior_keyboarddefaults.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Keyboard Defaults Behavior
 */
Drupal.behaviors.openlayers_behavior_keyboarddefaults = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_keyboarddefaults']) {
    // Add control
    var control = new OpenLayers.Control.KeyboardDefaults();
    data.openlayers.addControl(control);
    control.activate();
  }
}