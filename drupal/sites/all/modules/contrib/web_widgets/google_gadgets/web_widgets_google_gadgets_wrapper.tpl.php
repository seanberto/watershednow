<?php
// $Id: web_widgets_google_gadgets_wrapper.tpl.php,v 1.1 2009/06/24 07:38:58 aronnovak Exp $
/**
 * @file
 * Inline embedding means that we use the views output only
 */
?>
<?php print '<?xml version="1.0" encoding="UTF-8" ?>' ?>
<Module>
  <ModulePrefs title="<?php print $title ?>" />
  <Content type="html">
     <![CDATA[ 
<?php print $content ?>
     ]]>
  </Content> 
</Module>
