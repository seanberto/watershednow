<?php
// $Id: page.tpl.php,v 1.1 2009/06/26 13:55:36 kiterminal Exp $
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">

<head>
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
  <?php if (theme_get_setting('iepngfix_setting')): ?>
    <!--[if lte IE 6]>
      <script type="text/javascript">
        jQuery(function($) {
          $("img[@src$=png]").pngfix();
        });
      </script>
    <![endif]-->
  <?php endif; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <?php if ($primary_links || $secondary_links || $search_box): ?>
    <div id="navigation">
      <?php if ($secondary_links): ?>
        <?php print theme('links', $secondary_links, array('id' => 'secondary', 'class' => 'links sub-menu')); ?>
      <?php endif; ?>
      <?php if ($primary_links): ?>
        <?php print theme('links', $primary_links, array('id' => 'primary', 'class' => 'links sub-menu')); ?>
      <?php endif; ?>
      <!-- /primary_menu -->
      <?php if ($search_box): ?>
        <div class="search">
          <?php print $search_box; ?>
        </div> <!-- /search-box -->
      <?php endif; ?>
    </div> <!-- /navigation -->
  <?php endif; ?>

  <div id="container">
    <?php if ($site_name || $site_slogan): ?>
      <div id="header">
        <?php if ($site_name): ?>
          <h1 class='site-name'><a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
        <?php endif; ?>
      </div> <!-- /header -->
    <?php endif; ?>

    <div id="headerimage"> </div> <!-- /headerimage -->
    
    <div id="main">
      <div id="main-wrapper">
      <?php print $breadcrumb; ?>
      
      <?php if ($content_top): ?>
        <div id="content-top"><?php print $content_top; ?></div>
      <?php endif; ?>

      <?php if ($title): ?>
        <h1 class="title"><?php print $title; ?></h1>
      <?php endif; ?>

      <?php if ($mission): ?>
        <div id="mission"><?php print $mission; ?></div>
      <?php endif; ?>

      <?php print $messages; ?>

      <?php print $help; ?>
      
      <?php if ($notes): ?>
        <div id="notes"><?php print $notes; ?></div>
      <?php endif; ?>
      
      <?php if ($preface): ?>
        <div id="preface"><?php print $preface; ?></div>
      <?php endif; ?>

      <?php if ($tabs): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>

      <?php print $content ?>
      
      <?php if ($content_bottom): ?>
        <div id="content-bottom"><?php print $content_bottom; ?></div>
      <?php endif; ?>
      <?php print $feed_icons ?>
        
      </div> <!-- /main-wrapper -->

      <?php if ($left ||$right): ?>
        <div id="sidebar">
          <?php if ($left) : ?>
            <?php print $left; ?>
          <?php endif; ?> 
          <?php if ($right) : ?>
            <?php print $right; ?>
          <?php endif; ?>
        </div> <!-- /sidebar -->
      <?php endif; ?>

    </div>
    <!-- /main -->
  </div>
  <!-- /container -->

  <?php if ($footer_first || $footer_second || $footer || $footer_message): ?>
    <div id="footer">
      <?php if ($footer_first): ?>
        <?php print $footer_first; ?>
      <?php endif; ?>
      <?php if ($footer_second): ?>
        <?php print $footer_second; ?>
      <?php endif; ?>      
      <?php if ($footer): ?>
        <?php print $footer; ?>
      <?php endif; ?>
      <?php if ($footer_message): ?>
        <?php print $footer_message; ?>
      <?php endif; ?>
    </div>
    <!-- /footer -->
  <?php endif; ?>

  <?php print $closure ?>
</body>
</html>
