<?php
// $Id: web_widgets_iframe.tpl.php,v 1.1 2009/06/24 07:38:58 aronnovak Exp $
/**
 * @file
 * Template for the code what to embed in external sites
 */
?>
<script type="text/javascript">
widgetContext = <?php print $js_variables ?>;
</script>
<script id="<?php print $wid ?>" src="<?php print $script_url ?>"></script>
