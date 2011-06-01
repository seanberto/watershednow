
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