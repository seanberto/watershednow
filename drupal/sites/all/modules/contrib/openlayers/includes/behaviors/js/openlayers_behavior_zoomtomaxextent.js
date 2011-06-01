
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Zoom Max Extent Behavior
 */
Drupal.behaviors.openlayers_behavior_zoomtomaxextent = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_zoomtomaxextent']) {
    var panel = new OpenLayers.Control.Panel({
      allowSelection: true
    });  
        
    data.openlayers.addControl(panel);
    panel.activate();
        
    var button = new OpenLayers.Control.ZoomToMaxExtent();
    panel.addControls(button);
  }
}