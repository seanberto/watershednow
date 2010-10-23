$Id: README.txt,v 1.4.4.2 2008/03/03 02:00:32 nancyw Exp $
Overview:
--------
The taxonomy_image module allows site administrators to associate images
with taxonomy terms.  With the association created, an admin can then make
a call to 'taxonomy_image_display()' from their theme or other PHP code
to display the appropriate image.

The module allows the admin to create a one-to-one term-to-image relationship.
With image display recursion, it is also possible to create a many-to-one
relationship.  This is useful if creating taxonomy hierarchies in which an
entire tree of terms will use the same image.  With recursion enabled, you
only need to associate an image with the tree parent, and all children will
automatically inherit the same image.


Features:
--------
 - Image configuration happens within existing admin/categories menu structure.
 - Admins can add custom HTML attributes to <img> markup when displaying images.
 - Allows one-to-one term-to-image relationships.
 - Allows many-to-one term-to-image relationships.
 - Administrators can force images to be a standard size.
 - Admin-permitted users can disable the images.
 - Utilizes core file.inc api, supporting public and private files.
 - Small module with specific, focused functionality.
 - Provides the image to Views, if it is installed.
 - Will use Imagecache to resize the image, if it is installed and configured.


Installation and configuration:
------------------------------
Please refer to the INSTALL file for complete installation and 
configuration instructions.

There are add-on feature modules in the "contributed" sub-directory.
Refer to the documentation on how to use them.


Comments:
--------
Other modules do provide similar functionality.  However, I didn't find a
module that provided only what I needed and that was simple to configure,
hence the reason I wrote this module.  I didn't want anything more than I
needed.


Requires:
--------
 - Drupal 5.0
 - enabled taxonomy.module


Credits:
-------
 - Written and maintained by Jeremy Andrews <jeremy@kerneltrap.org>
 - co-maintained by Nancy Wichmann <nan_wich@bellsouth.net>
