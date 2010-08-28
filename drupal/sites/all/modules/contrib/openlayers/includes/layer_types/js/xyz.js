// $Id: xyz.js,v 1.1.2.1 2010/05/30 21:46:15 zzolo Exp $

/**
 * @file
 * Layer handler for XYZ layers
 */

/**
 * Openlayer layer handler for XYZ layer
 */
Drupal.openlayers.layer.xyz = function (title, map, options) {
  var styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);
  if (options.maxExtent !== undefined) {
    options.maxExtent = new OpenLayers.Bounds.fromArray(options.maxExtent) || new OpenLayers.Bounds(-20037508.34, -20037508.34, 20037508.34, 20037508.34);
  }
  options.projection = 'EPSG:'+options.projection;
  options.sphericalMercator = true;
  
  var layer = new OpenLayers.Layer.XYZ(title, options.url, options);
  layer.styleMap = styleMap;
  return layer;
};
