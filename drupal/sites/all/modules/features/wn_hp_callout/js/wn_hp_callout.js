jQuery(function(){
  var $ = jQuery;
  $('.wn-hp-callout').each(function(){
    var $t = $(this);
    var cycle_opts = {
      'timeout':8000
    }

    //only display the pager if we are using half slide style 
    if( $t.hasClass('wn-hp-callout-half') ) {
      var pager_id = 'wn-hp-callout-pager-'+ Math.floor(Math.random()*99999);//generating random id
      $('<div id="' + pager_id + '" class="wn-hp-callout-pager" />').appendTo($t);
      cycle_opts['pager'] = '#'+pager_id; //can't pass a object, so generated random id
    }
    $t.find('ul').cycle(cycle_opts);
  })
});