
/**
 * OpenLayers Zoom to Layer Behavior
 */
Drupal.behaviors.openlayers_zoomtolayer = function(context) {
  var layerextent, layers, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_zoomtolayer']) {
    var map = data.openlayers,
        layers = map.getLayersBy('drupalID', 
      data.map.behaviors['openlayers_behavior_zoomtolayer'].zoomtolayer);
    
    // Go through selected layers to get full extent.
    for (var i in layers) {
      if (layers[i].features !== undefined) {
        layerextent = layers[i].getDataExtent();

        // Check for valid layer extent
        if (layerextent != null) {
          map.zoomToExtent(layerextent);
          
          // If unable to find width due to single point,
          // zoom in with point_zoom_level option.
          if (layerextent.getWidth() == 0.0) {
            map.zoomTo(data.map.behaviors['openlayers_behavior_zoomtolayer'].point_zoom_level);
          }
        }
      }
    }
  }
}
