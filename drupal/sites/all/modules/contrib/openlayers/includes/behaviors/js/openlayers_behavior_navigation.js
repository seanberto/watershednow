// $Id: openlayers_behavior_navigation.js,v 1.1.2.4 2010/04/06 15:34:38 tmcw Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Navigation Behavior
 */
Drupal.behaviors.openlayers_behavior_navigation = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_navigation']) {
    // Add control
    var options = {
      'zoomWheelEnabled': data.map.behaviors['openlayers_behavior_navigation'].zoomWheelEnabled
    };
    var control = new OpenLayers.Control.Navigation(options);
    data.openlayers.addControl(control);
    control.activate();
  }
}
