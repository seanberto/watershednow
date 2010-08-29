<?php
// $Id: web_widgets_inline_wrapper.tpl.php,v 1.1 2009/06/24 07:38:58 aronnovak Exp $
/**
 * @file
 * Inline embedding means that we include the views output into a javascript file
 */
?>
window.onload = function() {
  document.getElementById(widgetContext['widgetid']).innerHTML = <?php print $js_string ?>;
};
