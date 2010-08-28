$Id: README.txt,v 1.1.2.3 2010/01/04 17:18:06 yhahn Exp $

DesignKit
---------

DesignKit is a small API module that assists themes by providing them with a
way to register settings that would otherwise be repetitive, complex, or
impossible by using the built-in theme-settings.php API.

Currently, DesignKit supports customizable settings for multiple CSS colors and
multiple image uploads that can be processed by ImageCache presets.


Dependencies
------------

- Color (included in Drupal core)
- ImageCache (http://drupal.org/project/imagecache)
- ImageAPI (http://drupal.org/project/imageapi)


Installation
------------

DesignKit can be installed like any other module. Visit `admin/build/modules` as
an administrative user and enable the "DesignKit" module. DesignKit will not
"do anything" after it's been installed -- you must use a theme which takes
advantage of DesignKit's API to make use of its features.


API
---

You can leverage DesignKit's API in your theme by adding directives to your
theme's info file. The two currently supported directives are `color` and
`image`. Once you have added the directives, make sure you clear your cache
(`drush cache clear` or the "Clear all caches" button on
`admin/settings/performance).


1. Colors

You can specify as many colors as you'd like your theme to use by adding a line
for each color. Each entry should use as its key the name of the variable to be
used in the `designkit.tpl.php` (see below) and the following keys for
specifying further metadata:

    ; Background color
    designkit[color][background][title] = "Background"
    designkit[color][background][description] = "Background color."
    designkit[color][background][default] = "#ffffff"

    ; Foo bar color
    designkit[color][foo][title] = "Foo"
    designkit[color][foo][description] = "Foo bar baz."
    designkit[color][foo][default] = "#cc0099"

And so on. Once you have registered the color values, you can customize them on
Drupal's theme settings page (`admin/build/themes/settings`). To make use of
these color variables, your theme will need to have a copy of the
`designkit.tpl.php` which will included the specified CSS on each page. You can
make use of the following designkit color API functions in your template:

- `designkit_colorshift($source, $shift, $opacity)`
  Lets you blend two colors together, applying the $shift color to the $source
  color at the $opacity specified.
- `designkit_colorhsl($source, $key)`
  Returns useful HSL information about a color.

Here is an example override of designkit.tpl.php:

    <style type='text/css'>
    body.designkit {
      background: <?php print $background ?>;
    }

    #page-title {
      background: <?php print designkit_colorshift($background, '#000000', .1) ?>;
      color: <?php print (designkit_colorhsl($background, 'l') > .5) ? '#fff' : '#000' ?>;
    }
    </style>

In this case, `designkit_colorshift` is used to darken the background color
(e.g. 10% black fill) when used for `#page-title` and the text color is set to
either white or black based on the lightness of `$background`.


2. Images

You can specify multiple images to use with your theme by adding entries to
`designkit[image]` key. Each image should be keyed on the variable that will be
used for it in the `page.tpl.php` vars and a set of keys representing metadata
about that image. The 'imagecache' key should have a corresponding string
representing parameters for generating a default imagecache preset that will
be used to process the image.

Here is an example:

    ; Site logo
    designkit[image][logo][title] = "Site logo"
    designkit[image][logo][description] = "Header logo."
    designkit[image][logo][imagecache] = "imagecache_scale:200x50"

    ; Print logo
    designkit[image][logo_print][title] = "Print logo"
    designkit[image][logo_print][description] = "Print logo."
    designkit[image][logo_print][imagecache] = "imagecache_scale:600x300"

If the variable name used is one that already exists in the `page.tpl.php` vars
the previous value *will be overridden*. In other words, don't name your image
variables `mission`, `help`, `content` or some other critical variable that you
don't want to inadvertently override.

Each image variable is also provided to designkit.tpl.php as a URL suitable for
use with the CSS background-image property. Each image will have corresponding
variables $image and $image_raw (representing URLs to the imagecache processed
and unprocessed versions) in designkit.tpl.php.

The imagecache params should follow this format:

    [imagecache action]:[width]x[height]

Note that any additional imagecache action params are not currently supported.


Maintainers
-----------

- Young Hahn (young@developmentseed.org)
