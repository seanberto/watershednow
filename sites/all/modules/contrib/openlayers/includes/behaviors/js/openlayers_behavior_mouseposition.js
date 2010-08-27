// $Id: openlayers_behavior_mouseposition.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Mouse Position Behavior
 */
Drupal.behaviors.openlayers_behavior_mouseposition = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_mouseposition']) {
    // Add control
    var control = new OpenLayers.Control.MousePosition();
    data.openlayers.addControl(control);
    control.activate();
  }
}