// $Id: kml.js,v 1.1.2.2 2010/06/24 20:44:45 tmcw Exp $

/**
 * @file
 * Layer handler for KML layers
 */

/**
 * Openlayer layer handler for KML layer
 */
Drupal.openlayers.layer.kml = function(title, map, options) {
  // Get styles
  var styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);
  
  // Format options
  if (options.maxExtent !== undefined) {
    options.maxExtent = OpenLayers.Bounds.fromArray(options.maxExtent);
  }

  // Create layer
  var layer = new OpenLayers.Layer.Vector(
    title, 
    {
    strategies: [new OpenLayers.Strategy.Fixed()],
    protocol: new OpenLayers.Protocol.HTTP({
        url: options.url, 
        format: new OpenLayers.Format.KML({
          extractStyles: true,
          extractAttributes: true
        })
      })
    }
  );
  layer.styleMap = styleMap;
  return layer;
};
