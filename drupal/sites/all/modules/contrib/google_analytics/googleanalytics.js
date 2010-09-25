// $Id: googleanalytics.js,v 1.3.2.11 2010/09/19 11:38:41 hass Exp $

$(document).ready(function() {

  // Attach onclick event to document only and catch clicks on all elements.
  $(document.body).click(function(event) {
    // Catch only the first parent link of a clicked element.
    $(event.target).parents("a:first,area:first").andSelf().filter("a,area").each(function() {

      var ga = Drupal.settings.googleanalytics;
      // Expression to check for absolute internal links.
      var isInternal = new RegExp("^(https?):\/\/" + window.location.host, "i");
      // Expression to check for special links like gotwo.module /go/* links.
      var isInternalSpecial = new RegExp("(\/go\/.*)$", "i");
      // Expression to check for download links.
      var isDownload = new RegExp("\\.(" + ga.trackDownloadExtensions + ")$", "i");

      try {
        // Is the clicked URL internal?
        if (isInternal.test(this.href)) {
          // Is download tracking activated and the file extension configured for download tracking?
          if (ga.trackDownload && isDownload.test(this.href)) {
            // Download link clicked.
            var extension = isDownload.exec(this.href);
            pageTracker._trackEvent("Downloads", extension[1].toUpperCase(), this.href.replace(isInternal, ''));
          }
          else if (isInternalSpecial.test(this.href)) {
            // Keep the internal URL for Google Analytics website overlay intact.
            pageTracker._trackPageview(this.href.replace(isInternal, ''));
          }
        }
        else {
          if (ga.trackMailto && $(this).is("a[href^=mailto:],area[href^=mailto:]")) {
            // Mailto link clicked.
            pageTracker._trackEvent("Mails", "Click", this.href.substring(7));
          }
          else if (ga.trackOutgoing && this.href) {
            // External link clicked.
            pageTracker._trackEvent("Outgoing links", "Click", this.href);
          }
        }
      } catch(err) {}

    });
  });
});
