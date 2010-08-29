<?php

/**
 * Generate Watershed Dashboard page
 */
function watershednow_helper_dashboard() {
  $output .= _watershednow_helper_links();
  return $output;
}

/**
 * Build table with settings links
 */
function _watershednow_helper_links() {
  $rows = array(array('cell 1', 'cell 2'));
  return theme_table($rows, $rows);
}