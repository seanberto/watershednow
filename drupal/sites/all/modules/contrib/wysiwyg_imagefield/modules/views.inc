<?php
// $Id: views.inc,v 1.1 2010/08/06 11:05:26 deciphered Exp $
/**
 * @file
 * Adds Views integration.
 */

function wysiwyg_imagefield_views_query_alter(&$view, &$query) {
  if ($view->name == 'wysiwyg_imagefield') {
    // Filepath.
    if (!isset($query->fields['files_filepath'])) {
      $query->fields['files_filepath'] = array(
        'table' => 'files',
        'field' => 'filepath',
        'alias' => 'files_filepath',
      );
    }

    // Filename.
    if (!isset($query->fields['files_filename'])) {
      $query->fields['files_filename'] = array(
        'table' => 'files',
        'field' => 'filename',
        'alias' => 'files_filename',
      );
    }
  }
}

function wysiwyg_imagefield_preprocess_views_view(&$vars) {
  if ($vars['view']->name == 'wysiwyg_imagefield') {
    if ($vars['view']->display['default']->display_options['style_plugin'] == 'table') {
      $thumbnails = '';
      foreach ($vars['view']->result as $delta => $result) {
        $file = array(
          'filepath' => $result->files_filepath,
          'filename' => $result->files_filename,
        );
        $thumbnails .= "<div class='wysiwyg_imagefield-thumbnail-{$result->fid}'>" . theme('imagefield_admin_thumbnail', $file) . "</div>";
      }
      $vars['rows'] = "<div class='widget-preview'><div class='imagefield-preview'></div></div>{$vars['rows']}<div class='wysiwyg_imagefield-thumbnails wysiwyg_imagefield-hidden'>{$thumbnails}</div>";
    }
  }
}

function wysiwyg_imagefield_preprocess_views_view_table(&$vars) {
  if ($vars['view']->name == 'wysiwyg_imagefield') {
    foreach ($vars['row_classes'] as $delta => &$classes) {
      $classes[] = "wysiwyg_imagefield-fid-{$vars['result'][$delta]->fid}";
    }
  }
}

/**
 * Implementation of hook_views_default_views().
 */
function wysiwyg_imagefield_views_default_views() {
  /*
   * View 'wysiwyg_imagefield'
   */
  $view = new view;
  $view->name = 'wysiwyg_imagefield';
  $view->description = 'Display all available files for use with FileField Sources in the WYSIWYG ImageField module.';
  $view->tag = 'WYSIWYG ImageField';
  $view->view_php = '';
  $view->base_table = 'files';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'fid' => array(
      'label' => 'File ID',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_file' => 0,
      'exclude' => 0,
      'id' => 'fid',
      'table' => 'files',
      'field' => 'fid',
      'relationship' => 'none',
    ),
    'filename' => array(
      'label' => 'Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_file' => 0,
      'exclude' => 0,
      'id' => 'filename',
      'table' => 'files',
      'field' => 'filename',
      'relationship' => 'none',
    ),
    'filesize' => array(
      'label' => 'Size',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'file_size_display' => 'formatted',
      'exclude' => 0,
      'id' => 'filesize',
      'table' => 'files',
      'field' => 'filesize',
      'relationship' => 'none',
    ),
    'timestamp' => array(
      'label' => 'Upload date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'timestamp',
      'table' => 'files',
      'field' => 'timestamp',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'fid' => array(
      'order' => 'ASC',
      'id' => 'fid',
      'table' => 'files',
      'field' => 'fid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'filemime' => array(
      'operator' => 'contains',
      'value' => 'image',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 1,
      'id' => 'filemime',
      'table' => 'files',
      'field' => 'filemime',
      'relationship' => 'none',
    ),
    'status' => array(
      'operator' => 'in',
      'value' => array(
        '1' => '1',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'files',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'fid' => 'fid',
      'filemime' => 'filemime',
      'filename' => 'filename',
      'filepath' => 'filepath',
      'filesize' => 'filesize',
      'timestamp' => 'timestamp',
    ),
    'info' => array(
      'fid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'filemime' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'filename' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'filepath' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'filesize' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'timestamp' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'fid',
  ));
  $views[$view->name] = $view;

  return $views;
}
