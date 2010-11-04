/* makes all elements the same height */
jQuery.fn.vjustify = function( min_h ) {
  var h = min_h || 0;
  return this.each(function(){
    var nh = jQuery(this).height();
    if( nh > h ) h = nh;
  }).height(h);
}

function shadow_css() {
  var $main = $('#main');
  var nav_h = $('#nav').height() || 0;
  var callout_h = $('#callout').height() || 0;
  return {
    'top': $main.offset().top - callout_h - nav_h + 20,
    'left': $main.offset().left + 5,
    'height': $main.height() + $('#footer').height() + callout_h + nav_h - 11,
    'width': $main.width() - 5
  }
}

$(function(){
  $('#block-boxes-showcase_1, #block-boxes-showcase_2, #block-boxes-showcase_3').vjustify();
  var $page = $('#page');
  var $main = $('#main');
  
  var $page_shadow = $page.addClass('clear-block').clone().attr('id','page-shadow');
  $page_shadow.children().remove();
  $('#page, #footer').css({
    'position':'relative',
    'zIndex':1
  });

  $page_shadow.css({
    'position':'absolute',
    'zIndex':0
  });
  $page_shadow.css(shadow_css());
  $page_shadow.append('<div id="page-shadow-tr-corner"></div><div id="page-shadow-right"></div><div id="page-shadow-br-corner"></div>')
  
  $(window).resize(function(){
    $page_shadow.css(shadow_css());
  });
    
  $('body').append($page_shadow);
});