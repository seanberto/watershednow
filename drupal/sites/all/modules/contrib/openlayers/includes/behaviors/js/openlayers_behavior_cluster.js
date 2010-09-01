// $Id: openlayers_behavior_cluster.js,v 1.1.2.1 2010/06/07 20:21:25 zzolo Exp $

/**
 * @file
 * OpenLayers Behavior implementation for clustering.

/**
 * OpenLayers Cluster Behavior
 */
Drupal.behaviors.openlayers_cluster = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_cluster']) {
    var options = data.map.behaviors['openlayers_behavior_cluster'];
    var map = data.openlayers;
    var distance = parseInt(options.distance);
    var threshold = parseInt(options.threshold);
    var layers = map.getLayersBy('drupalID', options.clusterlayer);
    
    // Go through chosen layers
    for (var i in layers) {
      var layer = layers[i];
      // Ensure vector layer
      if (layer.CLASS_NAME == 'OpenLayers.Layer.Vector') {
        var cluster = new OpenLayers.Strategy.Cluster(options);
        layer.addOptions({ 'strategies': [cluster] }); 
        cluster.setLayer(layer);
        cluster.features = layer.features.slice();
        cluster.activate();
        cluster.cluster();
      }
    }
  }
}