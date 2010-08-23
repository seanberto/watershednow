<?php
// $Id: help.inc,v 1.2 2010/08/09 23:53:11 deciphered Exp $
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
      return '<h2>' . t('Configuration') . '</h2>'
           . '<p>' . t('WYSIWYG ImageField must be enabled and configured on each desire Content type, follow these simple steps to do so:') . '</p>'
           . '<ol>'
           . '  <li>' . t('Create a new ImageField or edit a pre-defined ImageField on the desire Content types !manage_fields page.', array('!manage_fields' => '<strong>\'' . t('Manage fields') . '\'</strong>')) . '</li>'
           . '  <li>' . t('Check the !wif_checkbox checkbox.', array('!wif_checkbox' => '<strong>\'' . t('Use with WYSIWYG ImageField?') . '\'</strong>')) . '<br />' . ('Note: Only one ImageField per Content type can be used.') . '</li>'
           . '  <li>' . t('Under the !insert section, check the !insert_btn checkbox and configure the !enabled_styles and !default_styles sections.', array('!insert' => '<strong>\'' . t('Insert') . '\'</strong>', '!insert_btn' => '<strong>\'' . t('Enable insert button') . '\'</strong>', '!enabled_styles' => '<strong>\'' . t('Enabled insert styles') . '\'</strong>', '!default_styles' => '<strong>\'' . t('Default insert styles') . '\'</strong>')) . '</li>'
           . '  <li>' . t('(optional*) Under the !file_sources section, check the !autocomplete checkbox.', array('!file_sources' => '<strong>\'' . t('File sources') . '\'</strong>', '!autocomplete' => '<strong>\'' . t('Autocomplete reference textfield') . '\'</strong>')) . '</li>'
           . '  <li>' . t('Set !values to !unlimited.', array('!values' => '<strong>\'' . t('Number of values') . '\'</strong>', '!unlimited' => '<strong>\'' . t('Unlimited') . '\'</strong>')) . '</li>'
           . '</ol>'
           . '<p>' . t('Once WYSIWYG ImageField is configured in your desired Content types, enable the WYSIWYG plugin on your desired WYSIWYG profile and you are ready to go.') . '</p>'
           . '<p><em>' . t('* Requires !filefield_sources and !views to provide Image Library functionality.', array('!filefield_sources' => l(t('FileField Sources module'), 'http://drupal.org/project/filefield_sources'), '!views' => l(t('Views module'), 'http://drupal.org/project/views'))) . '</em></p>'
           . '<p>&nbsp;</p>'
           . '<h2>' . t('Frequently asked questions') . '</h2>'
           . '<dl>'
           . '  <dt><strong>' . t('Where did my ImageField go?') . '</strong></dt>'
           . '  <dd>'
           . '    ' . t('To make the process of adding inline images as streamlined as possible, WYSIWYG ImageField removes the ImageField widget from the user interface, making in visible only when needed.') . '<br /><br />'
           . '    ' . t('It is recommended that if you require a ImageField accessible outside of the WYSIWYG ImageField experience that you create an ImageField for the sole purpose of using it with ImageField instead of using your existing ImageField.')
           . '    <p>&nbsp;</p>'
           . '  </dd>'
           . '  <dt><strong>' . t('How can I add a search to the \'Reference existing files\' tab?') . '</strong></dt>'
           . '  <dd>'
           . '    ' . t('If you have both the FileField Sources and Views modules installed, the !existing_files section will be displayed as a Views table. This View can be modified by the administrator (or approved roles) to add or remove columns, filters and a number of options.', array('!existing_files' => '<strong>\'' . t('Reference existing files') . '\'</strong>')) . '<br /><br />'
           . '    ' . t('By adding an Exposed filter, you are essentially adding a search engine to the existing file list.') . '<br /><br />'
           . '    ' . t('The View can be modified at:') . '<br />'
           . '    ' . l(url('admin/build/views/edit/wysiwyg_imagefield', array('absolute' => TRUE)), 'admin/build/views/edit/wysiwyg_imagefield')
           . '    <p>&nbsp;</p>'
           . '  </dd>'
           . '  <dt><strong>' . t('How can I see all the files uploaded to the WYSIWYG ImageField widget?') . '</strong></dt>'
           . '  <dd>'
           . '    ' . t('Currently this is not available, for the sole purpose of keeping the system as simple as possible.') . '<br /><br />'
           . '    ' . t('Ideas have been put forward and there will be a solution for this in a future release.')
           . '    <p>&nbsp;</p>'
           . '  </dd>'
           . '</dl>';
  }
}
