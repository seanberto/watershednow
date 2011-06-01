
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