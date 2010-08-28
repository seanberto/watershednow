// $Id: openlayers_behavior_dragpan.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * DragPan Behavior
 */
Drupal.behaviors.openlayers_behavior_dragpan = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_dragpan']) {
    // Add control
    var control = new OpenLayers.Control.DragPan();
    data.openlayers.addControl(control);
    control.activate();
  }
}
