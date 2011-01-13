<?php

/**
 * Implementation of hook_content_default_fields().
 */
function wn_donations_content_default_fields() {
  $fields = array();

  // Exported field: field_attachments
  $fields['donation-field_attachments'] = array(
    'field_name' => 'field_attachments',
    'type_name' => 'donation',
    'display_settings' => array(
      'weight' => 0,
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
    'list_default' => 1,
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
      'filefield_sources' => array(),
      'filefield_source_attach_path' => 'file_attach',
      'filefield_source_attach_absolute' => '0',
      'filefield_source_attach_mode' => 'copy',
      'filefield_source_autocomplete' => '1',
      'insert' => 1,
      'insert_styles' => array(
        'link' => 'link',
        'auto' => 0,
        'image' => 0,
        'imagecache_galleryformatter_slide' => 0,
        'imagecache_galleryformatter_thumb' => 0,
      ),
      'insert_default' => 'link',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Attachments',
      'weight' => 0,
      'description' => 'Attach supporting documents and files to this donation page.',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_body
  $fields['donation-field_body'] = array(
    'field_name' => 'field_body',
    'type_name' => 'donation',
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
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_body][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Body',
      'weight' => '-3',
      'description' => 'Enter the content of this donation campaign.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_get_involved
  $fields['donation-field_get_involved'] = array(
    'field_name' => 'field_get_involved',
    'type_name' => 'donation',
    'display_settings' => array(
      'weight' => '-2',
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
      'weight' => '-2',
      'description' => 'Select up to 5 donation, take action, or event opportunities associated with this donation page. Start typing the title of the page to find those available. You can drag-and-drop selected pages to change their order in the list.',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_image_assets
  $fields['donation-field_image_assets'] = array(
    'field_name' => 'field_image_assets',
    'type_name' => 'donation',
    'display_settings' => array(
      'weight' => '7',
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
      'slide_preset' => '0',
      'thumb_preset' => '0',
      'style' => 'nostyle',
      'link_to_full' => 0,
      'link_to_full_preset' => '0',
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
        'link' => 0,
      ),
      'insert_default' => 'auto',
      'insert_class' => '',
      'insert_width' => '',
      'wysiwyg_imagefield' => TRUE,
      'label' => 'Image assets',
      'weight' => '7',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_sidebar
  $fields['donation-field_sidebar'] = array(
    'field_name' => 'field_sidebar',
    'type_name' => 'donation',
    'display_settings' => array(
      'weight' => '-1',
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
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '3000',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '2',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_sidebar][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Sidebar content',
      'weight' => '-1',
      'description' => 'Optionally, can enter up to 3000 characters of additional content to be displayed in the sidebar on this donation campaign page.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_teaser
  $fields['donation-field_teaser'] = array(
    'field_name' => 'field_teaser',
    'type_name' => 'donation',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'default',
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
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '3',
      'size' => '200',
      'default_value' => array(
        '0' => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_teaser][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Teaser',
      'weight' => '-4',
      'description' => 'Enter a short teaser for this donation page (200 characters max). This teaser will be used in sidebars as well as "widgets".',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Associated "Get Involved" opportunities');
  t('Attachments');
  t('Body');
  t('Image assets');
  t('Sidebar content');
  t('Teaser');

  return $fields;
}
