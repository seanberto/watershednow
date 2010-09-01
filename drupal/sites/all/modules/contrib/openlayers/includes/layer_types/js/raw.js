// $Id: raw.js,v 1.1.2.1 2010/05/30 21:46:15 zzolo Exp $

/**
 * OpenLayers Raw Layer Handler
 */
Drupal.openlayers.layer.openlayers_raw = function(title, map, options) {
  // Note, so that we do not pass all the features along to the Layer
  // options, we use the options.options to give to Layer
  // options.options.drupalID = options.drupalID;
  var layer = new OpenLayers.Layer.Vector(title, options.options);
  
  // Add fetures if there are any
  if (options.features) {
    Drupal.openlayers.addFeatures(map, layer, options.features);
  }
  
  return layer;
};
