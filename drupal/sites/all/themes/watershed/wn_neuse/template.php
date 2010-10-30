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

  if( !empty($vars['search_box']) ) {
    $vars['search_box'] = $vars['search_box'];
  }
}