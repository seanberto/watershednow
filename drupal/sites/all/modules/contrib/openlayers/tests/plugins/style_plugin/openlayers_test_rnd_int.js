
/**
 * @file
 * File to hold custom context styling
 */

/**
 * Style plugin context class
 */
Drupal.openlayers.style_plugin.openlayers_test_rnd_int = function(params) {
  this.params = params;
  this.params.high = parseInt(this.params.high);
  this.params.low = parseInt(this.params.low);
};

/**
 * Style plugin context class methods
 */
Drupal.openlayers.style_plugin.openlayers_test_rnd_int.prototype = {

  // Private methods (not copied to final style context object)
  'prv' : {
    'random' : function(low, high) {
      return Math.floor(Math.random() * (high - low + 1)) + low;
    }
  },

  // Point radius context.  Given paramters, gets a random
  // pointRadius.
  'getInt' : function(feature) {
    var high = this.params.high;
    var low = this.params.low;
    var ret = this.prv.random(low, high);
    return ret;
  }
};
