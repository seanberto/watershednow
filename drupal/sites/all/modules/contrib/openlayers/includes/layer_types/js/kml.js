// $Id: kml.js,v 1.1.2.5 2010/08/04 17:22:25 tmcw Exp $

/**
 * @file
 * Layer handler for KML layers
 */

/**
 * Openlayer layer handler for KML layer
 */
Drupal.openlayers.layer.kml = function(title, map, options) {
  var styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);

  options.projection = 'EPSG:' + options.projection;

  var layer = new OpenLayers.Layer.Vector(
    title, 
    $.extend(options, {
    strategies: [new OpenLayers.Strategy.Fixed()],
    protocol: new OpenLayers.Protocol.HTTP({
        url: options.url, 
        format: new OpenLayers.Format.KML(
          options.formatOptions
        )
      })
    })
  );
  layer.drupalID = options.drupalID;
  layer.styleMap = styleMap;
  return layer;
};
