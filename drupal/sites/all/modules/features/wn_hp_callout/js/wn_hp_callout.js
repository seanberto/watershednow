jQuery(function(){
  var $ = jQuery;
  $('.wn-hp-callout').each(function(){
    var $t = $(this);
    var pager_id = 'wn-hp-callout-pager-'+ Math.floor(Math.random()*99999);
    $('<div id="' + pager_id + '" class="wn-hp-callout-pager" />').appendTo($t);
    $t.find('ul').cycle({
      'pager': '#'+pager_id, //can't pass a object, so generated random id
      'timeout':8000
    })
  })
});