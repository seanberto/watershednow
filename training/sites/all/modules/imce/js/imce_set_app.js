// $Id: imce_set_app.js,v 1.4.2.4 2010/08/29 03:41:32 ufku Exp $
/*
 * IMCE Integration by URL
 * Ex-1: http://example.com/imce?app=XEditor|url@urlFieldId|width@widthFieldId|height@heightFieldId
 * Creates "Insert file" operation tab, which fills the specified fields with url, width, height properties
 * of the selected file in the parent window
 * Ex-2: http://example.com/imce?app=XEditor|sendto@functionName
 * "Insert file" operation calls parent window's functionName(file, imceWindow)
 * Ex-3: http://example.com/imce?app=nomatter|imceload@functionName
 * Parent window's functionName(imceWindow) is called as soon as IMCE UI is ready. Send to operation
 * needs to be set manually. See imce.setSendTo() method in imce.js
 */

(function($) {

var appFields = {}, appWindow = (top.appiFrm||window).opener || parent;

//execute when imce loads.
imce.hooks.load.push(function(win) {
  var data = decodeURIComponent(location.href.substr(location.href.lastIndexOf('app=')+4)).split('|');
  var func, appName = data.shift();
  //extract fields
  for (var i in data) {
    var arr = data[i].split('@');
    arr.length > 1 && (appFields[arr[0]] = arr[1]);
  }
  //run custom onload function if available
  if (appFields['imceload'] && (func = isFunc(appFields['imceload']))) {
    func(win);
    delete appFields['imceload'];
  }
  //set custom sendto function. appFinish is the default.
  var sendtoFunc = appFields['url'] ? appFinish : false;
  //check sendto@funcName syntax in URL
  if (appFields['sendto'] && (func = isFunc(appFields['sendto']))) {
    sendtoFunc = func;
    delete appFields['sendto'];
  }
  //check windowname+ImceFinish. old method
  else if (win.name && (func = isFunc(win.name +'ImceFinish'))) {
    sendtoFunc = func;
  }
  //highlight file
  if (appFields['url']) {
    if (appFields['url'].indexOf(',') > -1) {//support multiple url fields url@field1,field2..
      var arr = appFields['url'].split(',');
      for (var i in arr) {
        if ($('#'+ arr[i], appWindow.document).size()) {
          appFields['url'] = arr[i];
          break;
        }
      }
    }
    var filename = $('#'+ appFields['url'], appWindow.document).val();
    imce.highlight(filename.substr(filename.lastIndexOf('/')+1));
  }
  //set send to
  sendtoFunc && imce.setSendTo(Drupal.t('Insert file'), sendtoFunc);
});

//sendTo function
var appFinish = function(file, win) {
  var $doc = $(appWindow.document);
  for (var i in appFields) {
    $doc.find('#'+ appFields[i]).val(file[i]);
  }
  if (appFields['url']) {
    try{
      $doc.find('#'+ appFields['url']).blur().change().focus();
    }catch(e){
      try{
        $doc.find('#'+ appFields['url']).trigger('onblur').trigger('onchange').trigger('onfocus');//inline events for IE
      }catch(e){}
    }
  }
  appWindow.focus();
  win.close();
};

//checks if a string is a function name in the given scope. returns function reference. supports x.y.z notation.
var isFunc = function(str, scope) {
  var obj = scope || appWindow;
  var parts = str.split('.'), len = parts.length;
  for (var i = 0; i < len && (obj = obj[parts[i]]); i++);
  return obj && i == len && (typeof obj == 'function' || typeof obj != 'string' && !obj.nodeName && obj.constructor != Array && /^[\s[]?function/.test(obj.toString())) ? obj : false;
}

})(jQuery);