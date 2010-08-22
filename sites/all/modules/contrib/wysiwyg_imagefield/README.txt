// $Id: README.txt,v 1.1 2010/08/06 11:05:26 deciphered Exp $

The WYSIWYG ImageField module is an inline image management tool for the WYSIWYG
module based on the ImageField and Insert modules with an IMCE-esque image
library provided by the FileField Sources and Views modules.

WYSIWYG ImageField was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark


Features
-------------------

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
-------------------

* WYSIWYG module.
* ImageField module.
* Insert module.
* jQuery UI module.


Recommended modules
-------------------

* FileField Sources module.
* Views module.


Configuration
-------------------

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


  * Requires FileField Sources module and Views module to provide Image Library
    functionality.
