<?php
// $Id: ds.admin.inc,v 1.1.2.8 2010/06/08 00:22:20 swentel Exp $

/**
 * @file
 * General file for administrative functions.
 */

/**
 * General page.
 */
function ds_general_page() {

  // Child pages.
  $item = menu_get_item('admin/build/ds');
  if ($content = system_admin_menu_block($item)) {
    $childs = theme('admin_block_content', $content);
  }
  else {
    $childs = t('You do not have any administrative items.');
  }

  $fieldset = array();
  $fieldset['#title'] = 'General';
  $fieldset['#children'] = $childs;
  $fieldset['#attributes'] = array();
  $output = theme('fieldset', $fieldset);

  return $output;
}