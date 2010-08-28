// $Id: openlayers_behavior_zoompanel.js,v 1.1.2.1 2010/05/24 18:02:48 tmcw Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * ZoomPanel Behavior
 */
Drupal.behaviors.openlayers_behavior_zoompanel = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_zoompanel']) {
    // Add control
    var control = new OpenLayers.Control.ZoomPanel();
    data.openlayers.addControl(control);
    control.activate();
  }
}
