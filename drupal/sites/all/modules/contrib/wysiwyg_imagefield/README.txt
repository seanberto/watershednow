// $Id: README.txt,v 1.4 2010/08/18 06:08:35 deciphered Exp $

The WYSIWYG ImageField module is an inline image management tool for the WYSIWYG
module based on the ImageField and Insert modules with an IMCE-esque image
library provided by the FileField Sources and Views modules.

WYSIWYG ImageField was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark


Features
--------------------------

* Support for WYSIWYG libraries:
  * CKEditor/FCKEditor.
  * TinyMCE.
* Support for FileField/ImageField based modules:
  * FileField Paths module.
  * ImageField Extended.
* Support for CCK Formatters:
  * Custom Formatters module.
  * ImageCache module.
  * Lightbox2 module.
  * etc.
* Views based FileField Sources browser:
  * Modify output fields.
  * Add exposed filters for search.


Required modules
--------------------------

* WYSIWYG module.
* ImageField module.
* Insert module.
* jQuery UI module.


Recommended modules
--------------------------

* FileField Sources module.
* Views module.


Configuration
--------------------------

WYSIWYG ImageField must be enabled and configured on each desire Content type,
follow these simple steps to do so:

1. Create a new ImageField or edit a pre-defined ImageField on the desire
   Content types 'Manage fields' page.

2. Check the 'Use with WYSIWYG ImageField?' checkbox.
   Note: Only one ImageField per Content type can be used.

3. Under the 'Insert' section, check the 'Enable insert button' checkbox and
   configure the 'Enabled insert styles' and 'Default insert styles' sections.

4. (optional*) Under the 'File sources' section, check the 'Autocomplete
   reference textfield' checkbox.

5. Set 'Number of values' to 'Unlimited'.

Once WYSIWYG ImageField is configured in your desired Content types, enable the
WYSIWYG plugin on your desired WYSIWYG profile and you are ready to go.

  * Requires FileField Sources module and Views module to provide Image Library
    functionality.


Frequently asked questions
--------------------------

Q. Where did my ImageField go?

A. To make the process of adding inline images as streamlined as possible,
   WYSIWYG ImageField removes the ImageField widget from the user interface,
   making in visible only when needed.

   It is recommended that if you require a ImageField accessible outside of
   the WYSIWYG ImageField experience that you create an ImageField for the sole
   purpose of using it with ImageField instead of using your existing
   ImageField.


Q. How can I add a search to the 'Reference existing files' tab?

A. If you have both the FileField Sources and Views modules installed, the
   'Reference existing files' section will be displayed as a Views table. This
   View can be modified by the administrator (or approved roles) to add or
   remove columns, filters and a number of options.

   By adding an Exposed filter, you are essentially adding a search engine to
   the existing file list.

   The View can be modified at:
   http://[www.yoursite.com/path/to/drupal]/admin/build/views/edit/wysiwyg_imagefield


Q. How can I see all the files uploaded to the WYSIWYG ImageField widget?

A. Currently this is not available, for the sole purpose of keeping the system
   as simple as possible.

   Ideas have been put forward and there will be a solution for this in a
   future release.


Known issues
--------------------------

- Pager on FileField Sources View occasionally themed incorrectly.
- FileField Sources View style issue with IE6.
- Dialog doesn't close on Insert in IE6.
