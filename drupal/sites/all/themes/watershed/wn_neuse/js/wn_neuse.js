function shadow_css() {
  var $main = $('#main');
  var nav_h = $('#nav').outerHeight() || 0;
  var callout_h = $('#callout').outerHeight() || 0;
  return {
    'top': $main.offset().top - callout_h - nav_h + 20,
    'left': $main.offset().left + 5,
    'height': $main.outerHeight() + $('#footer').height() + callout_h + nav_h - 13,
    'width': $main.width() - 5
  }
}

$(function(){
  // make sidebar and content area same height so border spans all content
  $('#content-inner, #sidebar-second').vjustify();
  
  $('#block-boxes-showcase_1, #block-boxes-showcase_2, #block-boxes-showcase_3').vjustify();
  var $page = $('#page');
  var $main = $('#main');
  
  var $page_shadow = $page.addClass('clear-block').clone().attr('id','page-shadow');
  $page_shadow.children().remove();
  $('#page,#footer').css({
    'position':'relative',
    'zIndex':1
  });
  $('#page').css('zIndex',2);

  $page_shadow.css({
    'position':'absolute',
    'zIndex':0
  });
  $page_shadow.css(shadow_css());
  $page_shadow.append('<div id="page-shadow-tr-corner"></div><div id="page-shadow-right"></div><div id="page-shadow-br-corner"></div>')
  
  $(window).load(function(){
    $page_shadow.css(shadow_css());
  });
  
  $(window).resize(function(){
    $page_shadow.css(shadow_css());
  });
    
  $('body').append($page_shadow);
});