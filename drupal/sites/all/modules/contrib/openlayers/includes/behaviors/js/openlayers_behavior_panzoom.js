
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Pan Zoom Bar Behavior
 */
Drupal.behaviors.openlayers_behavior_panzoom = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_panzoom']) {
    // Add control
    var control = new OpenLayers.Control.PanZoom();
    data.openlayers.addControl(control);
    control.activate();
  }
}
