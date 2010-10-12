<?php

/**
 * Implementation of hook_content_default_fields().
 */
function wn_common_content_default_fields() {
  $fields = array();

  // Exported field: field_attachments
  $fields['page-field_attachments'] = array(
    'field_name' => 'field_attachments',
    'type_name' => 'page',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '1',
    'list_default' => 0,
    'description_field' => '1',
    'widget' => array(
      'file_extensions' => 'txt pdf doc jpg png gif xls',
      'file_path' => 'attachments',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'wn_hp_callout_desc' => NULL,
      'wn_hp_callout_desc_help' => NULL,
      'custom_wn_hp_callout_desc' => NULL,
      'custom_wn_hp_callout_desc_style' => NULL,
      'custom_wn_hp_callout_desc_required' => NULL,
      'filefield_sources' => array(
        'reference' => 'reference',
        'attach' => 0,
        'remote' => 0,
      ),
      'filefield_source_attach_path' => 'file_attach',
      'filefield_source_attach_absolute' => '0',
      'filefield_source_attach_mode' => 'copy',
      'filefield_source_autocomplete' => '1',
      'insert' => 0,
      'insert_styles' => array(
        'auto' => 0,
        'link' => 0,
        'image' => 0,
        'imagecache_thumbnail_large' => 0,
        'imagecache_thumbnail_medium' => 0,
        'imagecache_thumbnail_small' => 0,
        'imagecache_web' => 0,
      ),
      'insert_default' => 'auto',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Attachments',
      'weight' => '-1',
      'description' => 'Attach supporting documents and files to this page.',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_get_involved
  $fields['page-field_get_involved'] = array(
    'field_name' => 'field_get_involved',
    'type_name' => 'page',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '0',
    'multiple' => '5',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'donation' => 'donation',
      'event' => 'event',
      'take_action' => 'take_action',
      'page' => 0,
      'update' => 0,
      'update_feed' => 0,
      'map_feature' => 0,
      'webform' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_get_involved][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Associated "Get Involved" opportunities',
      'weight' => '-3',
      'description' => 'Select up to 5 donation, take action, or event opportunities associated with this page. Start typing the title of the page to find those available. You can drag-and-drop selected pages to change their order in the list.',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_image_assets
  $fields['page-field_image_assets'] = array(
    'field_name' => 'field_image_assets',
    'type_name' => 'page',
    'display_settings' => array(
      'weight' => '6',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => 'image_assets',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 1,
      'title' => '',
      'custom_title' => 1,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'wn_hp_callout_desc' => NULL,
      'wn_hp_callout_desc_help' => NULL,
      'custom_wn_hp_callout_desc' => NULL,
      'custom_wn_hp_callout_desc_style' => NULL,
      'custom_wn_hp_callout_desc_required' => NULL,
      'filefield_sources' => array(
        'reference' => 'reference',
        'attach' => 'attach',
        'remote' => 0,
      ),
      'filefield_source_attach_path' => 'file_attach',
      'filefield_source_attach_absolute' => '0',
      'filefield_source_attach_mode' => 'copy',
      'filefield_source_autocomplete' => '1',
      'insert' => 1,
      'insert_styles' => array(
        'auto' => 'auto',
        'image' => 'image',
        'imagecache_thumbnail_large' => 'imagecache_thumbnail_large',
        'imagecache_thumbnail_medium' => 'imagecache_thumbnail_medium',
        'imagecache_thumbnail_small' => 'imagecache_thumbnail_small',
        'imagecache_web' => 'imagecache_web',
        'link' => 0,
      ),
      'insert_default' => 'imagecache_web',
      'insert_class' => '',
      'insert_width' => '',
      'wysiwyg_imagefield' => TRUE,
      'label' => 'Image assets',
      'weight' => '6',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Associated "Get Involved" opportunities');
  t('Attachments');
  t('Image assets');

  return $fields;
}
