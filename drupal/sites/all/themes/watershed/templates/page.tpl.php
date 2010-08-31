<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <!--[if lte IE 6]><style type="text/css" media="all">@import "<?php print $base_path . path_to_theme() ?>/css/ie6.css";</style><![endif]-->
    <!--[if IE 7]><style type="text/css" media="all">@import "<?php print $base_path . path_to_theme() ?>/css/ie7.css";</style><![endif]-->
    <?php print $scripts; ?>
  </head>

  <body class="<?php print $body_classes; ?>">
    <div id="skip"><a href="#content"><?php print t('Skip to Content'); ?></a> <a href="#navigation"><?php print t('Skip to Navigation'); ?></a></div>  
    <div id="page">

    <!-- ______________________ HEADER _______________________ -->

    <div id="header">

      <div id="logo-title">
	
        <?php if (!empty($logo)): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
          </a>
        <?php endif; ?>

        <div id="name-and-slogan">
          <?php if (!empty($site_name)): ?>
            <h1 id="site-name">
              <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        </div> <!-- /name-and-slogan -->

      </div> <!-- /logo-title -->

      <?php if ($header): ?>
        <div id="header-region">
          <?php print $header; ?>
        </div>
      <?php endif; ?>

      <?php print $search_box; ?>

    </div> <!-- /header -->

    <!-- ______________________ MAIN _______________________ -->

    <div id="main" class="clearfix">
    
      <div id="content">
        <div id="content-inner" class="inner column center">

          <?php if ($callout): ?>
            <div id="callout">
              <?php print $callout; ?>
            </div> <!-- /#callout -->
          <?php endif; ?>

          <?php if ($breadcrumb || $title || $mission || $messages || $help || $tabs || $notes): ?>
            <div id="content-header">

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

            </div> <!-- /#content-header -->
          <?php endif; ?>

          <div id="content-area">
            <?php print $content; ?>
          </div> <!-- /#content-area -->

          <?php print $feed_icons; ?>

          <?php if ($content_bottom): ?>
            <div id="content-bottom">
              <?php print $content_bottom; ?>
            </div><!-- /#content-bottom -->
          <?php endif; ?>

          </div>
        </div> <!-- /content-inner /content -->

        <?php if (!empty($primary_links) || !empty($secondary_links)): ?>
          <div id="navigation" class="menu <?php if (!empty($primary_links)) { print "with-main-menu"; } if (!empty($secondary_links)) { print " with-sub-menu"; } ?>">
            <?php if (!empty($secondary_links)){ print theme('links', $secondary_links, array('id' => 'secondary', 'class' => 'links sub-menu')); } ?>
            <?php if (!empty($primary_links)){ print theme('links', $primary_links, array('id' => 'primary', 'class' => 'links main-menu')); } ?>
          </div> <!-- /navigation -->
        <?php endif; ?>

        <?php if ($left): ?>
          <div id="sidebar-first" class="column sidebar first">
            <div id="sidebar-first-inner" class="inner">
              <?php print $left; ?>
            </div>
          </div>
        <?php endif; ?> <!-- /sidebar-left -->

        <?php if ($right): ?>
          <div id="sidebar-second" class="column sidebar second">
            <div id="sidebar-second-inner" class="inner">
              <?php print $right; ?>
            </div>
          </div>
        <?php endif; ?> <!-- /sidebar-second -->

      </div> <!-- /main -->

      <!-- ______________________ FOOTER _______________________ -->

      <?php if(!empty($footer_message) || !empty($footer) || !empty($footer_second)): ?>
        <div id="footer">
          <?php print $footer; ?>
          <?php print $footer_second; ?>
          <?php print $footer_message; ?>
        </div> <!-- /footer -->
      <?php endif; ?>

    </div> <!-- /page -->
    <?php print $closure; ?>
  </body>
</html>