<?php
// $Id: ndtest.ds_default.inc,v 1.1.2.8 2010/02/03 08:33:10 swentel Exp $

/**
 * @file
 * Display suite default settings.
 */

function _ndtest_ds_default_settings() {
  $data = array(
    'nd' => array(
      'page' => array(
        'teaser' => array(
          'fields' => array(
            'title' => array(
              'weight' => '-19',
              'format' => 'nd_title_h2_link',
              'region' => 'header',
              'label_value' => 'Title',
              'labelformat' => 'hidden',
            ),
            'author' => array(
              'weight' => '-18',
              'format' => 'ds_author_nolink',
              'region' => 'disabled',
              'label_value' => 'Author',
              'labelformat' => 'hidden',
            ),
            'links' => array(
              'weight' => '-17',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Links',
              'labelformat' => 'hidden',
            ),
            'read_more' => array(
              'weight' => '-18',
              'format' => '',
              'region' => 'footer',
              'label_value' => 'Read more',
              'labelformat' => 'hidden',
            ),
            'post_date' => array(
              'weight' => '-19',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Post date',
              'labelformat' => 'hidden',
            ),
            'body' => array(
              'weight' => '-20',
              'format' => 'nd_bodyfield',
              'region' => 'middle',
              'label_value' => 'Core body',
              'labelformat' => 'hidden',
            ),
            'terms' => array(
              'weight' => '-20',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Taxonomy',
              'labelformat' => 'hidden',
            ),
            'test_key' => array(
              'weight' => '-19',
              'format' => 'ds_eval_code',
              'region' => 'disabled',
              'label_value' => 'Test key',
              'labelformat' => 'hidden',
            ),
          ),
          'status' => 1,
        ),
        'full' => array(
          'fields' => array(
            'title' => array(
              'weight' => '-15',
              'format' => 'nd_title_h1_nolink',
              'region' => 'disabled',
              'label_value' => 'Title',
              'labelformat' => 'hidden',
            ),
            'author' => array(
              'weight' => '-19',
              'format' => 'ds_author_nolink',
              'region' => 'header',
              'label_value' => 'Author',
              'labelformat' => 'hidden',
            ),
            'links' => array(
              'weight' => '-16',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Links',
              'labelformat' => 'hidden',
            ),
            'read_more' => array(
              'weight' => '-18',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Read more',
              'labelformat' => 'hidden',
            ),
            'post_date' => array(
              'weight' => '-20',
              'format' => '',
              'region' => 'header',
              'label_value' => 'Post date',
              'labelformat' => 'hidden',
            ),
            'body' => array(
              'weight' => '-19',
              'format' => 'nd_bodyfield',
              'region' => 'middle',
              'label_value' => 'Core body',
              'labelformat' => 'hidden',
            ),
            'terms' => array(
              'weight' => '-19',
              'format' => '',
              'region' => 'disabled',
              'label_value' => 'Taxonomy',
              'labelformat' => 'hidden',
            ),
            'test_key' => array(
              'weight' => '-20',
              'format' => 'ds_eval_code',
              'region' => 'footer',
              'label_value' => 'Test key',
              'labelformat' => 'hidden',
            ),
          ),
          'status' => 1,
        ),
      ),
    ),
  );
  return $data;
}

function _ndtest_ds_fields() {
  $data = array(
    'nd' => array(
      'test_key' => array(
        'title' => 'Test key',
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_DEFAULT,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '<?php
echo "Test key: ". $object->nid;
?>'),
        'exclude' => array(
          'story' => 'story',
          'page' => 0,
        ),
      ),
    ),
  );
  return $data;
}
