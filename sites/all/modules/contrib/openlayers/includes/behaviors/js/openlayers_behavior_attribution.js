// $Id: openlayers_behavior_attribution.js,v 1.1.2.3 2009/09/28 01:11:41 zzolo Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Attribution Behavior
 */
Drupal.behaviors.openlayers_behavior_attribution = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_attribution']) {
    // Add control
    var control = new OpenLayers.Control.Attribution();
    data.openlayers.addControl(control);
    control.activate();
  }
}