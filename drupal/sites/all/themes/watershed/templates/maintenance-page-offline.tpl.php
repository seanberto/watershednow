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
    <div id="page">

      <!-- ______________________ HEADER _______________________ -->

      <div id="header">

        <?php if ($newsletter && ($newsletter_position == 'header')) { print $newsletter; } ?>

        <div id="logo-title">
          <div id="name-and-slogan">
            <?php if (!empty($site_name)): ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
                  <?php $wn_logo_large = drupal_get_path('theme', 'watershed') . '/images/wn_logo_large.png'; ?>
                  <img src="/sites/all/themes/watershed/images/wn_logo_large.png" alt="Watershed Now" />
                </a>
              </h1>
            <?php endif; ?>
          </div> <!-- /name-and-slogan -->

        </div> <!-- /logo-title -->

      </div> <!-- /header -->

      <!-- ______________________ MAIN _______________________ -->

      <div id="main" class="clearfix">

        <div id="content">
          <div id="content-inner" class="inner column center">

            <div id="content-area">
              <p>The site is currently not available due to technical problems. Please try again later. Thank you for your understanding.</p>
            </div> <!-- /#content-area -->

            <?php if ($content_bottom): ?>
              <div id="content-bottom">
                <?php print $content_bottom; ?>
              </div><!-- /#content-bottom -->
            <?php endif; ?>

            </div>
          </div> <!-- /content-inner /content -->

        </div> <!-- /main -->

    </div> <!-- /page -->

    <!-- ______________________ FOOTER _______________________ -->

    <div id="footer" class="clear-block">
      <div id="footer-inner">
        <div id="footer-message"><?php print $footer_message . $wn_credit; ?></div><!-- /#footer-message -->
      </div> <!-- /footer-inner -->
    </div> <!-- /footer -->

    <?php print $closure; ?>
  </body>
</html>