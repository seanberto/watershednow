<?php
// $Id: jquery_ui_dialog-page.tpl.php,v 1.1.2.17 2010/07/01 14:31:20 eugenmayer Exp $

# Copyright (c) 2010 KontextWork
# Author: Eugen Mayer

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html id="jquery_ui_dialog" xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
<?php print $head; ?>
<title><?php print (!empty($title) ? strip_tags($title) : $head_title); ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>
<link href="<?php print base_path().drupal_get_path('module','jquery_ui_dialog')?>/css/jquery_ui_dialog.child.css" media="all" rel="stylesheet" type="text/css"/>
</head>
<body id="jquery_ui_dialog" class="clear-block">
<div class="dialog-page-wrapper">
  <div class="dialog-page-container">
    <div class="dialog-page-content">
      <?php if ($show_messages && $messages): print $messages; endif; ?>         
      <?php print $content; ?>      
    </div>
  </div>
</div>
<?php print $closure; ?>
</body>
</html>
