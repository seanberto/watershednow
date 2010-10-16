<?php

/**
 * Implementation of hook_content_default_fields().
 */
function wn_map_content_default_fields() {
  $fields = array();

  // Exported field: field_ada
  $fields['map_feature-field_ada'] = array(
    'field_name' => 'field_ada',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '6',
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_ada][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'ADA accessibility',
      'weight' => '6',
      'description' => 'If applicable, describe the ADA accessibility of this watershed feature.',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_address
  $fields['map_feature-field_address'] = array(
    'field_name' => 'field_address',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => 0,
      'parent' => 'group_location',
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
      'rows' => '4',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_address][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Address',
      'weight' => 0,
      'description' => 'If applicable, enter the street address of this watershed feature. This address will display as a block of text, as well as a link to Google Maps\'s Directions lookup.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_amenities
  $fields['map_feature-field_amenities'] = array(
    'field_name' => 'field_amenities',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => 0,
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'multiple' => '1',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'picnic tables
grills/fire rings
potable water
showers',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Amenities',
      'weight' => 0,
      'description' => 'What amenities are available at this location?',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_camping_options
  $fields['map_feature-field_camping_options'] = array(
    'field_name' => 'field_camping_options',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '2',
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'primitive
improved 
hookups',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Camping options',
      'weight' => '2',
      'description' => 'If applicable, what camping options are available at this location?',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_contact
  $fields['map_feature-field_contact'] = array(
    'field_name' => 'field_contact',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => 0,
      'parent' => 'group_general',
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
      'rows' => '3',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_contact][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Contact info',
      'weight' => 0,
      'description' => 'Enter contact information for people interested in visiting or learning more about this watershed feature.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_description
  $fields['map_feature-field_description'] = array(
    'field_name' => 'field_description',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => 'group_general',
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
          '_error_element' => 'default_value_widget][field_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Description',
      'weight' => '-1',
      'description' => 'Enter a description of this watershed feature.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_fees
  $fields['map_feature-field_fees'] = array(
    'field_name' => 'field_fees',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '4',
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'multiple' => '1',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'none
yes-camping
yes-launch
yes-parking',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Fees',
      'weight' => '4',
      'description' => 'Are there fees for visiting/using this location?',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_files
  $fields['map_feature-field_files'] = array(
    'field_name' => 'field_files',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '3',
      'parent' => 'group_multimedia',
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
      'file_extensions' => 'txt pdf doc xls cvs',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'wn_hp_callout_desc' => NULL,
      'wn_hp_callout_desc_help' => NULL,
      'custom_wn_hp_callout_desc' => NULL,
      'custom_wn_hp_callout_desc_style' => NULL,
      'custom_wn_hp_callout_desc_required' => NULL,
      'filefield_sources' => NULL,
      'filefield_source_attach_path' => NULL,
      'filefield_source_attach_absolute' => NULL,
      'filefield_source_attach_mode' => NULL,
      'filefield_source_autocomplete' => NULL,
      'insert' => NULL,
      'insert_styles' => NULL,
      'insert_default' => NULL,
      'insert_class' => NULL,
      'insert_width' => NULL,
      'label' => 'File uploads',
      'weight' => '3',
      'description' => 'Enter any file attachments associated with this watershed feature.',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_hours
  $fields['map_feature-field_hours'] = array(
    'field_name' => 'field_hours',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '1',
      'parent' => 'group_general',
      'label' => array(
        'format' => 'inline',
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
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_hours][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Hours of operation',
      'weight' => '1',
      'description' => 'Enter the hours of operation for this watershed feature. (Applicable for restaurants, visitor centers, parks, etc.)',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_images
  $fields['map_feature-field_images'] = array(
    'field_name' => 'field_images',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '2',
      'parent' => 'group_multimedia',
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
      'file_path' => 'watershed_images',
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
      'slide_preset' => 'map_gallery_slide',
      'thumb_preset' => 'map_gallery_thumb',
      'style' => 'Greenarrows',
      'link_to_full' => 1,
      'link_to_full_preset' => '0',
      'wn_hp_callout_desc' => NULL,
      'wn_hp_callout_desc_help' => NULL,
      'custom_wn_hp_callout_desc' => NULL,
      'custom_wn_hp_callout_desc_style' => NULL,
      'custom_wn_hp_callout_desc_required' => NULL,
      'filefield_sources' => NULL,
      'filefield_source_attach_path' => NULL,
      'filefield_source_attach_absolute' => NULL,
      'filefield_source_attach_mode' => NULL,
      'filefield_source_autocomplete' => NULL,
      'insert' => NULL,
      'insert_styles' => NULL,
      'insert_default' => NULL,
      'insert_class' => NULL,
      'insert_width' => NULL,
      'label' => 'Image Uploads',
      'weight' => '2',
      'description' => 'Upload one or more images associated with this watershed feature.',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_mile
  $fields['map_feature-field_mile'] = array(
    'field_name' => 'field_mile',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '-2',
      'parent' => 'group_location',
      'label' => array(
        'format' => 'inline',
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
    'type' => 'number_integer',
    'required' => '0',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '0',
    'max' => '1000',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_mile][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'River mile',
      'weight' => '-2',
      'description' => 'Enter the closest river mile marker to this watershed feature.',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_reservation
  $fields['map_feature-field_reservation'] = array(
    'field_name' => 'field_reservation',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '3',
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'yes
no',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Reservation requirement',
      'weight' => '3',
      'description' => 'If camping or lodging is available, does it require a reservation?',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_restrooms
  $fields['map_feature-field_restrooms'] = array(
    'field_name' => 'field_restrooms',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '5',
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'none
primitive
flush',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Restrooms',
      'weight' => '5',
      'description' => 'Are restrooms available at this location?',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_services
  $fields['map_feature-field_services'] = array(
    'field_name' => 'field_services',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '1',
      'parent' => 'group_options',
      'label' => array(
        'format' => 'inline',
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
    'multiple' => '1',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'water
grocery
equipment rental
guides
restaurant',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Services',
      'weight' => '1',
      'description' => 'What services are available at this location?',
      'type' => 'optionwidgets_buttons',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_videos
  $fields['map_feature-field_videos'] = array(
    'field_name' => 'field_videos',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '1',
      'parent' => 'group_multimedia',
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
    'type' => 'emvideo',
    'required' => '0',
    'multiple' => '1',
    'module' => 'emvideo',
    'active' => '1',
    'widget' => array(
      'video_width' => '350',
      'video_height' => '250',
      'video_autoplay' => 0,
      'preview_width' => '350',
      'preview_height' => '250',
      'preview_autoplay' => 0,
      'thumbnail_width' => '120',
      'thumbnail_height' => '90',
      'thumbnail_default_path' => '',
      'thumbnail_link_title' => 'See video',
      'providers' => array(
        'bliptv' => 'bliptv',
        'flickr_sets' => 'flickr_sets',
        'google' => 'google',
        'vimeo' => 'vimeo',
        'youtube' => 'youtube',
        'archive' => 0,
        'dailymotion' => 0,
        'guba' => 0,
        'imeem' => 0,
        'lastfm' => 0,
        'livevideo' => 0,
        'metacafe' => 0,
        'myspace' => 0,
        'revver' => 0,
        'sevenload' => 0,
        'spike' => 0,
        'tudou' => 0,
        'twistage' => 0,
        'ustream' => 0,
        'ustreamlive' => 0,
        'voicethread' => 0,
        'yahoomusic' => 0,
        'zzz_custom_url' => 0,
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
      'label' => 'Videos and slideshows',
      'weight' => '1',
      'description' => 'Enter the URL(s) of videos or Flickr slideshows that are associated with the watershed feature.<br /> (For videos, enter the URL of the individual video. For Flickr slideshows, enter the URL of a photo set.)',
      'type' => 'emvideo_textfields',
      'module' => 'emvideo',
    ),
  );

  // Exported field: field_wkt
  $fields['map_feature-field_wkt'] = array(
    'field_name' => 'field_wkt',
    'type_name' => 'map_feature',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => 'group_location',
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
    'type' => 'openlayers_wkt',
    'required' => '0',
    'multiple' => '1',
    'module' => 'openlayers_cck',
    'active' => '1',
    'openlayers_cck_feature_types' => array(
      'point' => 'point',
      'path' => 'path',
      'polygon' => 'polygon',
    ),
    'widget' => array(
      'openlayers_cck_preset_map' => 'picker_map',
      'openlayers_behaviors' => array(
        'openlayers_behavior_zoomtolayer' => array(
          'enabled' => 0,
          'options' => array(
            'zoomtolayer' => 'openlayers_cck_vector_layer',
            'point_zoom_level' => '5',
          ),
        ),
      ),
      'default_value' => array(
        '0' => array(
          'openlayers_wkt' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Location',
      'weight' => '-3',
      'description' => 'Use the map picker or enter <a href="http://en.wikipedia.org/wiki/Well-known_text" target="_blank">well-known text (WKT)</a> to geo-locate this watershed feature. You can enter individual points, polylines and/or polygons. To confirm a polyline or polygon drawing, double-click your last point. (Note: this map picker works best in Firefox and Chrome.)',
      'type' => 'openlayers_wkt_widget',
      'module' => 'openlayers_cck',
    ),
  );

  // Exported field: field_description
  $fields['map_kml-field_description'] = array(
    'field_name' => 'field_description',
    'type_name' => 'map_kml',
    'display_settings' => array(
      'weight' => '-3',
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
          'format' => '1',
          '_error_element' => 'default_value_widget][field_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Description',
      'weight' => '-3',
      'description' => 'Enter a description of this KML layer',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_kml
  $fields['map_kml-field_kml'] = array(
    'field_name' => 'field_kml',
    'type_name' => 'map_kml',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'path_plain',
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
    'required' => '1',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '1',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'kml',
      'file_path' => 'kml',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '1MB',
      'max_filesize_per_node' => '',
      'wn_hp_callout_desc' => NULL,
      'wn_hp_callout_desc_help' => NULL,
      'custom_wn_hp_callout_desc' => NULL,
      'custom_wn_hp_callout_desc_style' => NULL,
      'custom_wn_hp_callout_desc_required' => NULL,
      'filefield_sources' => NULL,
      'filefield_source_attach_path' => NULL,
      'filefield_source_attach_absolute' => NULL,
      'filefield_source_attach_mode' => NULL,
      'filefield_source_autocomplete' => NULL,
      'insert' => NULL,
      'insert_styles' => NULL,
      'insert_default' => NULL,
      'insert_class' => NULL,
      'insert_width' => NULL,
      'label' => 'KML file',
      'weight' => '-4',
      'description' => 'Upload a KML file to be added to OpenLayer\'s map presets. Keep in mind that you want to use small files, as larger files take longer to download and render in the web browsers of your site visitors.',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('ADA accessibility');
  t('Address');
  t('Amenities');
  t('Camping options');
  t('Contact info');
  t('Description');
  t('Fees');
  t('File uploads');
  t('Hours of operation');
  t('Image Uploads');
  t('KML file');
  t('Location');
  t('Reservation requirement');
  t('Restrooms');
  t('River mile');
  t('Services');
  t('Videos and slideshows');

  return $fields;
}
