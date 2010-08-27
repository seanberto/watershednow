<?php
// $Id: ds-page-fieldexamples.tpl.php,v 1.1.2.1 2010/04/26 14:32:26 swentel Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head ?>
    <title><?php print $head_title ?></title>
    <?php print $styles ?>
  <?php print $scripts ?>

  </head>
  <body>

  <div style="padding: 10px; font-size: 16px;">
    <?php if ($messages) print $messages; ?>
    <?php print $content; ?>
  </div>

  </body>
</html>
