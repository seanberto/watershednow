// $Id: openlayers_behavior_zoombox.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Zoom Box Behavior
 */
Drupal.behaviors.openlayers_behavior_zoombox = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_zoombox']) {
    // Add control
    var control = new OpenLayers.Control.ZoomBox();
    data.openlayers.addControl(control);
    control.activate();
  }
}