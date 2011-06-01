
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Pan Zoom Bar Behavior
 */
Drupal.behaviors.openlayers_behavior_panzoombar = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_panzoombar']) {
    // Add control
    var control = new OpenLayers.Control.PanZoomBar();
    data.openlayers.addControl(control);
    control.activate();
  }
}