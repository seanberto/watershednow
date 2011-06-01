
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
