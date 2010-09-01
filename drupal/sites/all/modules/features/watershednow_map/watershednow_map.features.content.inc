<?php

/**
 * Implementation of hook_content_default_fields().
 */
function watershednow_map_content_default_fields() {
  $fields = array();

  // Exported field: field_attachments
  $fields['map_feature-field_attachments'] = array(
    'field_name' => 'field_attachments',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '1',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
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
      'weight' => '3',
      'description' => 'Attach supporting documents and files to this donation page.',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_get_involved
  $fields['map_feature-field_get_involved'] = array(
    'field_name' => 'field_get_involved',
    'type_name' => 'map_feature',
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
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Associated "Get Involved" opportunities',
      'weight' => '2',
      'description' => 'Select those "Take Action" and donation pages associated with this watershed feature.',
      'type' => 'nodereference_buttons',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_image_assets
  $fields['map_feature-field_image_assets'] = array(
    'field_name' => 'field_image_assets',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '8',
      'parent' => '',
      'label' => array(
        'format' => 'above',
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
      'weight' => '10',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_image_uploads
  $fields['map_feature-field_image_uploads'] = array(
    'field_name' => 'field_image_uploads',
    'type_name' => 'map_feature',
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
    'multiple' => '10',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '1',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => 'map/images',
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
      'filefield_sources' => array(
        'reference' => 'reference',
        'remote' => 0,
        'attach' => 0,
      ),
      'filefield_source_attach_path' => 'file_attach',
      'filefield_source_attach_absolute' => '0',
      'filefield_source_attach_mode' => 'move',
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
      'label' => 'Image Uploads',
      'weight' => '1',
      'description' => 'Upload images to be placed on the map.',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_video
  $fields['map_feature-field_video'] = array(
    'field_name' => 'field_video',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
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
    'type' => 'emvideo',
    'required' => '0',
    'multiple' => '0',
    'module' => 'emvideo',
    'active' => '1',
    'widget' => array(
      'video_width' => '425',
      'video_height' => '350',
      'video_autoplay' => 0,
      'preview_width' => '425',
      'preview_height' => '350',
      'preview_autoplay' => 0,
      'thumbnail_width' => '120',
      'thumbnail_height' => '90',
      'thumbnail_default_path' => '',
      'thumbnail_link_title' => 'See video',
      'providers' => array(
        'flickr_sets' => 'flickr_sets',
        'google' => 'google',
        'vimeo' => 'vimeo',
        'youtube' => 'youtube',
      ),
      'default_value' => array(
        '0' => array(
          'embed' => '',
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'media_flickr_ir_repeat_video' => 'list',
      'media_flickr_ir_rotatetime_video' => '5',
      'media_flickr_ir_shownavigation_video' => 1,
      'media_flickr_ir_showicons_video' => 1,
      'media_flickr_ir_shuffle_video' => 0,
      'media_flickr_ir_allowfullscreen_video' => 1,
      'media_flickr_ir_logo_video' => '',
      'media_flickr_ir_transition_video' => 'random',
      'media_flickr_player_video' => 'flash',
      'media_flickr_playlist_video' => 'right',
      'media_flickr_playlistsize_video' => '86',
      'media_flickr_repeat_video' => 'list',
      'media_flickr_delay_video' => '10',
      'media_flickr_controlbar_video' => 'none',
      'media_flickr_allowfullscreen_video' => 1,
      'media_flickr_logo_video' => '',
      'media_flickr_ir_repeat_preview' => 'list',
      'media_flickr_ir_rotatetime_preview' => '5',
      'media_flickr_ir_shownavigation_preview' => 1,
      'media_flickr_ir_showicons_preview' => 1,
      'media_flickr_ir_shuffle_preview' => 0,
      'media_flickr_ir_allowfullscreen_preview' => 1,
      'media_flickr_ir_logo_preview' => '',
      'media_flickr_ir_transition_preview' => 'random',
      'media_flickr_player_preview' => 'flash',
      'media_flickr_playlist_preview' => 'right',
      'media_flickr_playlistsize_preview' => '86',
      'media_flickr_repeat_preview' => 'list',
      'media_flickr_delay_preview' => '10',
      'media_flickr_controlbar_preview' => 'none',
      'media_flickr_allowfullscreen_preview' => 1,
      'media_flickr_logo_preview' => '',
      'label' => 'Video content',
      'weight' => 0,
      'description' => 'Enter the URL of a video or Flickr slideshow to be placed on this map.',
      'type' => 'emvideo_textfields',
      'module' => 'emvideo',
    ),
  );

  // Exported field: field_wkt
  $fields['map_feature-field_wkt'] = array(
    'field_name' => 'field_wkt',
    'type_name' => 'map_feature',
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
        'format' => 'openlayers_map_map_picker',
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
    'type' => 'openlayers_wkt',
    'required' => '1',
    'multiple' => '1',
    'module' => 'openlayers_cck',
    'active' => '1',
    'openlayers_cck_feature_types' => array(
      'point' => 'point',
      'path' => 'path',
      'polygon' => 'polygon',
    ),
    'widget' => array(
      'openlayers_cck_preset_map' => 'map_picker',
      'openlayers_behaviors' => array(
        'openlayers_behavior_zoomtolayer' => array(
          'enabled' => 0,
          'options' => array(
            'zoomtolayer' => 'openlayers_cck_vector_layer',
            'point_zoom_level' => '5',
          ),
        ),
      ),
      'keep_points' => 0,
      'position' => '10',
      'default_value' => array(
        '0' => array(
          'openlayers_wkt' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Geolocation',
      'weight' => '-2',
      'description' => 'Enter the location(s) of this feature. You may create any combination of points, polylines, and polygons. Your entry will be stored as <a hre="http://en.wikipedia.org/wiki/Well_Known_Text" target="_blank">well-known text</a>. You can also search for an address to get started, but each time you search, your previous entry will be erased. (<em>Hint:</em> When drawing shapes, double-click the map to complete a line or polygon shape.)',
      'type' => 'openlayers_geocoder_widget',
      'module' => 'openlayers_geocoder',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Associated "Get Involved" opportunities');
  t('Attachments');
  t('Geolocation');
  t('Image Uploads');
  t('Image assets');
  t('Video content');

  return $fields;
}
