<?php

/**
 * Implementation of hook_content_default_fields().
 */
function wn_hp_callout_content_default_fields() {
  $fields = array();

  // Exported field: field_wn_hp_callout_image
  $fields['wn_hp_callout-field_wn_hp_callout_image'] = array(
    'field_name' => 'field_wn_hp_callout_image',
    'type_name' => 'wn_hp_callout',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'image_plain',
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
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 0,
      'title' => '',
      'custom_title' => 1,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'slide_preset' => NULL,
      'thumb_preset' => NULL,
      'style' => NULL,
      'link_to_full' => NULL,
      'link_to_full_preset' => NULL,
      'wn_hp_callout_desc' => '',
      'wn_hp_callout_desc_help' => '',
      'custom_wn_hp_callout_desc' => 1,
      'custom_wn_hp_callout_desc_style' => 'formatted',
      'custom_wn_hp_callout_desc_required' => 1,
      'filefield_sources' => array(
        'remote' => 0,
        'reference' => 0,
        'attach' => 0,
      ),
      'filefield_source_attach_path' => 'file_attach',
      'filefield_source_attach_absolute' => '0',
      'filefield_source_attach_mode' => 'move',
      'filefield_source_autocomplete' => '0',
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
      'label' => 'Callout Image',
      'weight' => '-4',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Callout Image');

  return $fields;
}
