/*
 * jQuery resize event - v1.1 - 3/14/2010
 * http://benalman.com/projects/jquery-resize-plugin/
 * 
 * Copyright (c) 2010 "Cowboy" Ben Alman
 * Dual licensed under the MIT and GPL licenses.
 * http://benalman.com/about/license/
 */
(function($,h,c){var a=$([]),e=$.resize=$.extend($.resize,{}),i,k="setTimeout",j="resize",d=j+"-special-event",b="delay",f="throttleWindow";e[b]=250;e[f]=true;$.event.special[j]={setup:function(){if(!e[f]&&this[k]){return false}var l=$(this);a=a.add(l);$.data(this,d,{w:l.width(),h:l.height()});if(a.length===1){g()}},teardown:function(){if(!e[f]&&this[k]){return false}var l=$(this);a=a.not(l);l.removeData(d);if(!a.length){clearTimeout(i)}},add:function(l){if(!e[f]&&this[k]){return false}var n;function m(s,o,p){var q=$(this),r=$.data(this,d);r.w=o!==c?o:q.width();r.h=p!==c?p:q.height();n.apply(this,arguments)}if($.isFunction(l)){n=l;return m}else{n=l.handler;l.handler=m}}};function g(){i=h[k](function(){a.each(function(){var n=$(this),m=n.width(),l=n.height(),o=$.data(this,d);if(m!==o.w||l!==o.h){n.trigger(j,[o.w=m,o.h=l])}});g()},e[b])}})(jQuery,this);

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
  if( $('#sidebar-second').height() > $('#content-inner').height() ) {
    $('#content-inner').css({
      'minHeight': $('#sidebar-second').height()
      
    })
  }
  
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
  
  $('#page').resize(function(){
    $page_shadow.height(1); //used to get rid of scrollbars, as they alter the offsets
    $page_shadow.css(shadow_css());
  });
  
  //recalculate shadow position if the window is resized
  $(window).resize(function(){
    $page_shadow.height(1); //used to get rid of scrollbars, as they alter the offsets
    $page_shadow.css(shadow_css());
  });

  $('body').append($page_shadow);
});

/*
 * Disabling Drupal's fieldset animation.
 * The animation will cause the resize function to be called multiple times
 * causing browser to jitter.
 */
Drupal.toggleFieldset = function(fieldset) {
  fieldset.animating = false;
  if ($(fieldset).is('.collapsed')) {
    $(fieldset).removeClass('collapsed');
  }
  else {
    $(fieldset).addClass('collapsed');
  }
};