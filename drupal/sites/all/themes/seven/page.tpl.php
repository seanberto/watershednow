<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php echo $language->language ?>" lang="<?php echo $language->language ?>" dir="<?php echo $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>

  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if lte IE 7]>
    <style type="text/css" media="all">@import "<?php echo $base_path . path_to_theme() ?>/ie.css";</style>
  <![endif]-->
  <!--[if lte IE 6]>
    <style type="text/css" media="all">@import "<?php echo $base_path . path_to_theme() ?>/ie6.css";</style>
  <![endif]-->
  <?php print $scripts; ?>

</head>

<body class="<?php print $body_classes; ?>">
  
  <div id="skip-link">
    <a href="#main-content"><?php print t('Skip to main content'); ?></a>
  </div>
  
  <div class="element-invisible"><a id="main-content"></a></div>
  
  <div id="branding" class="clearfix">
  
    <?php print $breadcrumb; ?>
    
    <?php if ($title): ?>
      <h1 class="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    
    <?php if ($primary_local_tasks): ?><ul class="tabs primary"><?php print $primary_local_tasks; ?></ul><?php endif; ?>
    
  </div>

  <div id="page">
    <?php if ($secondary_local_tasks): ?><ul class="tabs secondary"><?php print $secondary_local_tasks; ?></ul><?php endif; ?>

    <div id="content" class="clearfix">
      
      <?php if ($messages): ?>
        <div id="console" class="clearfix"><?php print $messages; ?></div>
      <?php endif; ?>
      
      <?php if ($help): ?>
        <div id="help">
          <?php print $help; ?>
        </div>
      <?php endif; ?>
      
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      
      <?php print $content; ?>
      
    </div>

    <div id="footer">
      <?php print $feed_icons; ?>
    </div>

  </div>
  
  <?php print $closure; ?>
  
</body>
</html>
