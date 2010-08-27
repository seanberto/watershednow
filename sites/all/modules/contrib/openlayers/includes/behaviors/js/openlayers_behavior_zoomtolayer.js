// $Id: openlayers_behavior_zoomtolayer.js,v 1.1.2.7 2010/05/22 22:36:45 zzolo Exp $

/**
 * OpenLayers Zoom to Layer Behavior
 */
Drupal.behaviors.openlayers_zoomtolayer = function(context) {
  var layerextent, layers, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_zoomtolayer']) {
    map = data.openlayers;
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
