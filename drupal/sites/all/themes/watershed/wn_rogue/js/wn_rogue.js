$(function(){
  // make sidebar and content area same height so border spans all content
  window.setInterval(function(){
    $('#content-inner, #sidebar-second').height('auto').vjustify();
  },1000);
  
})