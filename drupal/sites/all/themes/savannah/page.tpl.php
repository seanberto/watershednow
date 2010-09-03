<?php // $Id: page.tpl.php,v 1.3 2010/04/10 23:59:10 ishmaelsanchez Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <!--[if IE]>
    <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/css/ie.css" type="text/css">
  <![endif]-->
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <div id="wrapper">
    <a href="#skip" class="invisible" title="To Content">Content</a>
    <?php if ($site_name && $is_front): ?>
      <h1 id="site-name"><a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" rel="home">
				<?php print $site_name; ?>
      </a></h1>
    <?php else: ?>
      <div id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" rel="home">
					<?php print $site_name; ?>
        </a>
      </div> <!-- site-name -->
    <?php endif; ?>
    
    <div id="content">
      <div class="content-wrapper">
        <div class="content-box">
          <a href="skip"></a>
          
          <?php if ($callout): ?>
            <div id="callout">
              <?php print $callout; ?>
            </div> <!-- /#callout -->
          <?php endif; ?>
          
          <div id="content-inner">
            <?php if ($content_top): ?>
              <div id="content-top">
                <?php print $content_top; ?>
              </div> <!-- content-top -->
            <?php endif; ?>
         
            <?php if ($title && !$is_front): ?>
              <h1 class="title">
                <?php print $title; ?>
              </h1>
            <?php endif; ?>
          
            <?php if ($tabs || $help || $messages || $notes || $preface): ?>
              <?php print $messages; ?>
              <?php if ($notes): ?>
                <div id="notes"><?php print $notes; ?></div>
              <?php endif; ?>
              <?php if ($preface): ?>
                <div id="preface"><?php print $preface; ?></div>
              <?php endif; ?>
              <?php if ($tabs): ?><?php print $tabs; ?><?php endif; ?>
              <?php print $help; ?>
            <?php endif; ?>
          
            <?php print $content; ?>
          
            <?php if ($content_bottom): ?>
              <div id="content-bottom">
                <?php print $content_bottom; ?>
              </div> <!-- content-bottom -->
            <?php endif; ?>
          </div> <!-- /content-inner -->
          
        </div><!-- content-box -->
        
        <?php if ($left || $right) : ?>
          <div id="sidebars">
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
          </div><!-- /sidebars -->
        <?php endif; ?>
        
      </div><!-- content-wrapper -->
    </div><!-- content -->
		
    <div id="top-bar">
      <?php if ($primary_links): ?>
        <div id="primary">
        	<?php print theme('links', $primary_links, array('id' => 'highlighter', 'class' => 'links')) ?>
        </div> <!-- primary -->
      <?php endif; ?>
      <?php if ($header): ?>
        <div id="banner">
          <?php print $header; ?>
        </div> <!-- header -->
      <?php endif; ?>
    </div> <!-- top-bar -->
    
    <div id="bottom-bar">
      <div id="footer-first">
				<?php if ($footer_first || $footer_message || $logo): ?>
					<?php print $footer_first; ?>
          <div id="logo">
          	<a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" rel="home">
            	<img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
            </a>
           </div> <!-- logo -->
          <div id="footer-message">
						<?php print $footer_message; ?>
          </div> <!-- footer-message -->
				<?php endif; ?>
      </div> <!-- footer-first -->
      
      <div id="footer-second">
       	<?php if ($search_box): ?>
					<?php print $search_box; ?>
				<?php endif; ?>
				<?php if ($secondary_links): ?>
        	<div id="footer-links">
						<?php print theme('links', $secondary_links); ?>
        	</div> <!-- footer-links -->
				<?php endif; ?>
				<?php if ($footer_second): ?>
					 <?php print $footer_second; ?>
         <?php endif; ?>
      </div> <!-- footer-second -->
      
      <div id="footer-bottom">
			 	<?php if ($footer): ?>
					<?php print $footer; ?>
				<?php endif; ?>
      </div> <!-- footer-bottom-->
      
    </div> <!-- bottom-bar -->
    
    <div id="bg-image">
      <div>
        <table cellspacing="0" cellpadding="0">
          <tr>
            <td>
              <?php if($is_front): ?>
                <img id="main-image" src="<?php print $base_path . $directory; ?>/images/home-bg.jpg" alt="<?php print $site_name; ?>" />
              <?php else: ?>
                <img id="content-image" src="<?php print $base_path . $directory; ?>/images/content-bg.jpg" alt="<?php print $site_name; ?>" />
              <?php endif; ?>
            </td>
          </tr>
        </table>
      </div>
    </div>
    
  </div> <!-- wrapper -->
<?php print $closure; ?>
</body>
</html>
