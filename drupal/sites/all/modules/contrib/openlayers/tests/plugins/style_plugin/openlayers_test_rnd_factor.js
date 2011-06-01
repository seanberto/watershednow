
/**
 * @file
 * File to hold custom context styling
 */

/**
 * Style plugin context class
 */
Drupal.openlayers.style_plugin.openlayers_test_rnd_factor = function(params) {
  this.params = params;
};

/**
 * Style plugin context class methods
 */
Drupal.openlayers.style_plugin.openlayers_test_rnd_factor.prototype = {

  // Fill opacity context.  Sets random fill opacity.
  'getFactor' : function(feature) {
    // Random factor
    return Math.random();
  }

};
