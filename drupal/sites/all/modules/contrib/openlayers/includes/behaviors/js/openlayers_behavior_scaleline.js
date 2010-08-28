// $Id: openlayers_behavior_scaleline.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Scale Line Behavior
 */
Drupal.behaviors.openlayers_behavior_scaleline = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_scaleline']) {
    // Add control
    var control = new OpenLayers.Control.ScaleLine();
    data.openlayers.addControl(control);
    control.activate();
  }
}