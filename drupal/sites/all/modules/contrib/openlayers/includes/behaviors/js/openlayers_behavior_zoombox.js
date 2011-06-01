
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