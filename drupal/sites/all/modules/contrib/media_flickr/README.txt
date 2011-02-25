// $Id: README.txt,v 1.1.2.1 2010/11/16 21:12:23 aaron Exp $

Readme for Media: Flicker

The Media: Flickr project currently offers Flickr Photoset capabilities to the
Embedded Media Field module, available at http://drupal.org/project/emfield.
To use it, enable the Embedded Video Field module, and add a Third Party Video
field to a content type. For Thumbnails, you'll also need to apply for a Flickr
API key, from http://www.flickr.com/services/api/keys.

After that, editors will be able to paste a URL or the embed code for a Flickr
Photoset or Slideshow into the field, and it will be displayed automatically.
Additionally, that URL will be parsed automatically, so the module will know the
difference between a Flickr Photoset URL and another supported provider, such as
a YouTube video.


Configuration:
--------------------
(1) After you install the Embedded Media Field and Media: Flickr modules, go to
your admin configuration (http://[www.yoursite.com]/admin/content/emfield) and
set the "Flickr API" values under Images or Videos tab > "Flickr
configuration". Note: You need to create your Flickr API key and secret from
your account on Flickr.

(2) Now when you create your Content Type and add the "Embedded Image" and/or
"Embedded Video" field(s), you will need to fill out additional settings. In
the "Providers Supported" fieldset area, check the "Custom URL" and "Flickr".
Save the field settings.

Content Creation:
----------------
(3) If you want a single image from Flickr, then use the "Embedded Image" CCK
field.
Example Flickr URL: http://www.flickr.com/photos/thusthought/5156938698/in/set-72157625220814125/

(4) If you want to use a set from Flickr, then use the  "Embedded Video" CCK
field.
Example Flickr URL: http://www.flickr.com/photos/thusthought/sets/72157625220814125/show/
