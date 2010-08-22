<?php
// $Id: help.inc,v 1.1 2010/08/06 11:05:26 deciphered Exp $
/**
 * @file
 * Adds Help integration.
 */

/**
 * Implements hook_help().
 */
function wysiwyg_imagefield_help($path, $arg) {
  switch ($path) {
    // Main module help for the WYSIWYG ImageField module
    case 'admin/help#wysiwyg_imagefield':
      return '<p>' . t('WYSIWYG ImageField must be enabled and configured on each desire Content type, follow these simple steps to do so:') . '</p>'
           . '<ol>'
           . '  <li>' . t('Create a new ImageField or edit a pre-defined ImageField on the desire Content types !manage_fields page.', array('!manage_fields' => '<strong>\'' . t('Manage fields') . '\'</strong>')) . '</li>'
           . '  <li>' . t('Check the !wif_checkbox checkbox.', array('!wif_checkbox' => '<strong>\'' . t('Use with WYSIWYG ImageField?') . '\'</strong>')) . '<br />' . ('Note: Only one ImageField per Content type can be used.') . '</li>'
           . '  <li>' . t('Under the !insert section, check the !insert_btn checkbox and configure the !enabled_styles and !default_styles sections.', array('!insert' => '<strong>\'' . t('Insert') . '\'</strong>', '!insert_btn' => '<strong>\'' . t('Enable insert button') . '\'</strong>', '!enabled_styles' => '<strong>\'' . t('Enabled insert styles') . '\'</strong>', '!default_styles' => '<strong>\'' . t('Default insert styles') . '\'</strong>')) . '</li>'
           . '  <li>' . t('(optional*) Under the !file_sources section, check the !autocomplete checkbox.', array('!file_sources' => '<strong>\'' . t('File sources') . '\'</strong>', '!autocomplete' => '<strong>\'' . t('Autocomplete reference textfield') . '\'</strong>')) . '</li>'
           . '  <li>' . t('Set !values to !unlimited.', array('!values' => '<strong>\'' . t('Number of values') . '\'</strong>', '!unlimited' => '<strong>\'' . t('Unlimited') . '\'</strong>')) . '</li>'
           . '</ol>'
           . '<p>' . t('* Requires !filefield_sources and !views to provide Image Library functionality.', array('!filefield_sources' => l(t('FileField Sources module'), 'http://drupal.org/project/filefield_sources'), '!views' => l(t('Views module'), 'http://drupal.org/project/views'))) . '</p>';
  }
}
