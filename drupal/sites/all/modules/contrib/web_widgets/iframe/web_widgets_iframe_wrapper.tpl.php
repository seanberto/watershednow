<?php
// $Id: web_widgets_iframe_wrapper.tpl.php,v 1.1.2.1 2010/02/19 19:56:12 diggersf Exp $
/**
 * @file
 * HTML wrapper code for the iframe'ed code at the external site.
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
    <title><?php print $title ?></title>
  </head>
  <body id="widget">
    <h1><?php print $title ?></h1>
  <?php print $content ?>
  </body>
</html>
