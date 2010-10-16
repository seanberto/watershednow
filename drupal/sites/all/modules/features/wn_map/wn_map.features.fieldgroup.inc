<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function wn_map_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_general
  $groups['map_feature-group_general'] = array(
    'group_type' => 'standard',
    'type_name' => 'map_feature',
    'group_name' => 'group_general',
    'label' => 'General information',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => 'Please describe this watershed feature.',
      ),
      'display' => array(
        'weight' => '-2',
        'label' => 'hidden',
        'teaser' => array(
          'format' => 'hidden',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'no_style',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_description',
      '1' => 'field_contact',
      '2' => 'field_hours',
    ),
  );

  // Exported group: group_location
  $groups['map_feature-group_location'] = array(
    'group_type' => 'standard',
    'type_name' => 'map_feature',
    'group_name' => 'group_location',
    'label' => 'Location Information',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => 'Tell site visitors how to visit/see this watershed feature.',
      ),
      'display' => array(
        'weight' => '0',
        'label' => 'above',
        'teaser' => array(
          'format' => 'hidden',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'no_style',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_wkt',
      '1' => 'field_mile',
      '2' => 'field_address',
    ),
  );

  // Exported group: group_multimedia
  $groups['map_feature-group_multimedia'] = array(
    'group_type' => 'standard',
    'type_name' => 'map_feature',
    'group_name' => 'group_multimedia',
    'label' => 'Multimedia',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => 'Are there videos, images, or files that are associated with this watershed feature?',
      ),
      'display' => array(
        'weight' => '4',
        'label' => 'hidden',
        'teaser' => array(
          'format' => 'hidden',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'no_style',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '4',
    'fields' => array(
      '0' => 'field_videos',
      '1' => 'field_images',
      '2' => 'field_files',
    ),
  );

  // Exported group: group_options
  $groups['map_feature-group_options'] = array(
    'group_type' => 'standard',
    'type_name' => 'map_feature',
    'group_name' => 'group_options',
    'label' => 'Services and amenities',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => 'What services, amenities and other options are available when visiting this watershed feature?',
      ),
      'display' => array(
        'weight' => '3',
        'label' => 'above',
        'teaser' => array(
          'format' => 'hidden',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'no_style',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '3',
    'fields' => array(
      '0' => 'field_amenities',
      '1' => 'field_services',
      '2' => 'field_camping_options',
      '3' => 'field_reservation',
      '4' => 'field_fees',
      '5' => 'field_restrooms',
      '6' => 'field_ada',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('General information');
  t('Location Information');
  t('Multimedia');
  t('Services and amenities');

  return $groups;
}
