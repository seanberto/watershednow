
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