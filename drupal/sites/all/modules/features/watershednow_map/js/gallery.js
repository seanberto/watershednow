$(document).ready(function() {
  $('#gallery-links a').click(function() {
    $('#gallery-links a').removeClass('gallery-selected');
    $(this).addClass('gallery-selected');
    showImage('#main-gallery-image', $(this).attr('href'), $('img', this).attr('title'), $('img', this).attr('alt'), $(this).attr('path'));
    return false;
  });
});

function showImage(container, src, title, alt, path)
{
  $(container).height($(container).height());
  var largeImage = $(container + " img");

  // ignore if we're clicking on the same image
  if ($(largeImage).attr("src") == src) {
    return false;
  }
  
  $(largeImage).hide();
  $('#gallery-title').html('');
  $(container).addClass("loading");
  $(largeImage).attr("src", src).attr('title', title).load(function()
  {
    $(container + ' a').attr('href', path);
    $(container).removeClass("loading");
    $(largeImage).fadeIn("slow");
    $('#gallery-title').html('<div class="help">(click to magnify)</div><h3 class="image-title">' + title + '</h3><div class"image-description">' + alt + '</div></div>');
  });
}