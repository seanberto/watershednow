Introduction to the Watershed Theme

The Watershed theme is based upon the Basic theme by Rain City Studios.
See: http://drupal.org/project/basic
See: the diagram of the page elements and region layout that ships with this theme
(watershed_theme_diagram.png)
__________________________________________________________________________________________

Watershed "Child" Themes

The Watershed theme should not be used on it's own, but rather as the parent theme of one or more child themes that ship with it, or are created by other designers/developers. You cannot use any of these child themes w/o installing the parent Watershed theme, as child theme inherit code, layout, stylesheets, etc., from the parent theme.

To learn more about working with theme inheritance, see: http://drupal.org/node/225125.
__________________________________________________________________________________________

What are the files for ?
------------------------

- watershed.info => provide informations about the theme, like regions, css, settings, js ...
- block-system-main.tpl.php => template to edit the content
- block.tpl.php => template to edit the blocks
- comment.tpl.php => template to edit the comments
- node.tpl.php => template to edit the nodes (in content)
- page.tpl.php => template to edit the page
- template.php => used to modify drupal's default behavior before outputting HTML through 
  the theme
- theme-settings => used to create additional settings in the theme settings page

In /CSS
-------

- default.css => define default classes, browser resets and admin styles
- ie6 => used to debug IE6
- ie7 => used to debug IE7
- layout.css => define the layout of the theme
- print.css => define the way the theme look like when printed
- style.css => contains some default font styles. that's where you can add custom css
- tabs.css => styles for the admin tabs (from ZEN)

__________________________________________________________________________________________

Changing the Layout

The layout used in Basic (and thus used in the Watershed theme) is fairly similar to the Holy Grail method. It has been tested on all major browser including IE (5>8), Opera, Firefox, Safari, Chrome ...

The purpose of this method is to have a minimal markup for an ideal display. For accessibility and search engine optimization, the best order to display a page is ]
the following :

	1. header
	2. content
	3. sidebars
	4. footer

This is how the page template is buit in basic, and it works in fluid and fixed layout.
Refers to the notes in layout.css to see how to modify the layout.