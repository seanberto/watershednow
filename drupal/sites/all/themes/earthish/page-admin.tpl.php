<?php
// $Id: page-admin.tpl.php,v 1.1 2010/03/03 06:56:07 ishmaelsanchez Exp $

/**
 * Custom admin page template
 * For heavy admin work set an Administration theme at /admin/settings/admin
 * Better yet use an admin theme
 *
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <?php print $head ?>
  <title><?php print $head_title ?></title>
  <?php print $scripts ?>
  <link type="text/css" rel="stylesheet" media="all" href="<?php print $front_page; ?>/modules/block/block.css" />
  <link type="text/css" rel="stylesheet" media="all" href="<?php print $front_page; ?>/modules/node/node.css" />
  <link type="text/css" rel="stylesheet" media="all" href="<?php print $front_page; ?>/modules/system/admin.css" />
  <link type="text/css" rel="stylesheet" media="all" href="<?php print $front_page; ?>/modules/system/system.css" />
  <link type="text/css" rel="stylesheet" media="all" href="<?php print $front_page; ?>/modules/update/update.css" />
  <style>
  body {color:#252525;font-family:Helvetica,sans-serif;font-size:.825em;line-height:1.5;margin:5px 10px; }
  h2, h3, h4 {font-family:Times, serif;line-height:1;}
  #site-name, #search {width:48%;}
  a, a:active, a:visited {color:#000;}
  .sticky-header, .system-status-report, .sticky-enabled .sticky-table {border-collapse:collapse;margin:.75em 0;}
  div.tabs {clear:both;overflow:auto;}
  .module {background:#000; color:#fff;}
  .help {padding:.25em;background:#39be5f;border:1px solid #1e7537;color:#fff;}
  .messages { background-color:#fdfaa1;border:1px solid #f3f48a;clear:both;margin:1em 0;padding:.25em;}
  </style>
</head>

<body>
<table border="0" cellpadding="1" cellspacing="2" width="100%">
  <tr valign="top">
    <td id="site-name">
      <?php if ($site_name): ?>
          <h2><a href="<?php print $front_page ?>"><?php print $site_name ?></a></h2>
      <?php endif; ?>
    </td>
    <td id="search" valign="middle">
      <?php print $search_box ?>
    </td>
  </tr>
</table>

<table border="0" cellpadding="1" cellspacing="2" width="100%" id="content">
  <tr>
    <td valign="top">
      <?php print $breadcrumb ?>
      <div id="admin-main">
        <h2 class="title"><?php print $title ?></h2>
        <div class="tabs"><?php print $tabs ?></div>
        <?php if ($show_messages) { print $messages; } ?>
        <?php print $help ?>
        <?php print $content; ?>
      </div>
    </td>
  </tr>
</table>
<?php print $closure ?>
</body>
</html>
