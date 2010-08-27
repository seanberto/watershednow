// $Id: openlayers_cck_vector_layer.js,v 1.1.2.4 2010/06/06 19:54:28 zzolo Exp $

/**
 * @file
 * Main JS file for openlayers_cck
 *
 * @ingroup openlayers_cck
 */
Drupal.behaviors.openlayers_cck_vector_layer = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_cck_vector_layer']) {
    var features = data.map.behaviors['openlayers_cck_vector_layer'].features;

    // Create options and layer
    var options = {
      drupalID: 'openlayers_cck_vector_layer'
    };
    var styleMap = Drupal.openlayers.getStyleMap(data.map, options.drupalID);
    var dataLayer = new OpenLayers.Layer.Vector(Drupal.t("Data Layer"), options);
    
    // Add features, styles, and layers
    dataLayer.styleMap = styleMap;
    Drupal.openlayers.addFeatures(data.map, dataLayer, features);
    data.openlayers.addLayer(dataLayer);
  }
};
