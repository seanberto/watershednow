<?php

// As a child theme, we inherit all customization's in Watershed template.php.

/*
 *	 This function creates the body classes that are relative to each page
 *
 *	@param $vars
 *	  A sequential array of variables to pass to the theme template.
 *	@param $hook
 *	  The name of the theme function being called ("page" in this case.)
 */
function wn_neuse_preprocess_page(&$vars, $hook) {
  // See parent theme preprocess_page function to understand logic here.
  // Basically, we use this as sort of a "switch" to change search box/block usage in page.tpl.php.
  // This lets us cut out the usage of an extra page.tpl.php file for this child theme.
  unset($vars['search_block']);
}