// $Id: galleryformatter.js,v 1.1.2.8 2010/09/09 19:40:23 manuelgarcia Exp $

Drupal.behaviors.galleryformatter = function (context) {
  $('.galleryformatter:not(.gallery-processed)', context).each(function(){
    Drupal.galleryformatter.prepare(this);

  }).addClass('gallery-processed');
};

Drupal.galleryformatter = Drupal.galleryformatter || {};

// setting up the main behaviour
Drupal.galleryformatter.prepare = function(el) {
  // var $settings = Drupal.settings.galleryformatter;
  var $el = $(el);
  var $slides = $('li.gallery-slide', $el);

  var $thumbs = $('.gallery-thumbs', $el);
  var $thumbsLi = $('li', $thumbs);
  var thumbWidth = $thumbsLi.filter(':first').width() + 'px';
  var liWidth = $thumbsLi.outerWidth(); // includes padding

  /*
   * Only start the thumbs carrousel if needed
   */
  if (($thumbsLi.size() * liWidth) > $thumbs.width()) {
    $('ul', $thumbs).width('99999px');
    $thumbs.infiniteCarousel();
    $thumbsLi = $('li', $thumbs); // we need to reselect because infiniteCarousel inserts new empty li elements if necessary
  }

  $thumbsLi.each(function(){
    $(this).css({
        width: thumbWidth
      });
  });
  var $thumbslinks = $('a', $thumbsLi);

  /*
   * @TODO:
   * figure out how to get this into proper functions reusing selections
   */
  $thumbslinks.click(function(e){
    var $hash = $(this.hash);
    if(!$hash.is(':visible')){
      $thumbsLi.removeClass('active');
      $(this).parent().addClass('active');
      $slides.filter(':visible').fadeOut('slow');
      $hash.fadeIn('slow');
      /*
       * @FIXME
       * Need to figure out a way to update the location bar of the browser, for bookmarking etc, without making the scroll jump
       * window.location.hash = this.hash; solution below does update the location, but makes the scroll jump.
       */
      // window.location.hash = this.hash;  // not sure if this is the best way to do it.
    }
    e.preventDefault();
  });

  /*
   *  Startup behaviour (when the page first loads)
   */
  $slides.hide(); // hide all slides
  var $locationHash = window.location.hash; // if we are being deeplinked to a specific slide, capture that

  function showFirstSlide(){
    $slides.filter(':first').show(); // show the first one
    $thumbsLi.filter('.slide-0:not("cloned")').addClass('active'); // activate the first thumbnail
  }

  // if we have a hash in the url
  if ($locationHash) {
    var $slideToShow = $slides.filter($locationHash);
    // if the hash corresponds to one of our slides
    if ($slideToShow.length > 0) {
      $slideToShow.show(); //  show that slide
      $thumbsLi.not($(".cloned")).find("a[href="+$locationHash+"]").parent().addClass('active'); // activate that thumbnail
    }
    // otherwise the default
    else {
      showFirstSlide();
    }
  }
  // otherwise the default
  else {
    showFirstSlide();
  }
};
