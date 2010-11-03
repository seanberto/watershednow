/* makes all elements the same height */
jQuery.fn.vjustify = function( min_h ) {
  var h = min_h || 0;
  return this.each(function(){
    var nh = jQuery(this).height();
    if( nh > h ) h = nh;
  }).height(h);
}

$(function(){
  $('#block-boxes-showcase_1, #block-boxes-showcase_2, #block-boxes-showcase_3').vjustify();
})