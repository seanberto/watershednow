<?php
// $Id: feeds_defaults.defaults.inc,v 1.3 2010/04/17 03:56:47 alexb Exp $

/**
 * @file
 * Actual function bodies for default hook definitions in
 * feeds_defaults.features.inc.
 */

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _feeds_defaults_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "data" && $api == "data_default") {
    return array("version" => 1);
  }
  else if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
}

/**
 * Helper to implementation of hook_data_default().
 */
function _feeds_defaults_data_default() {
  $export = array();
  $data_table = new stdClass;
  $data_table->disabled = FALSE; /* Edit this to true to make a default data_table disabled initially */
  $data_table->api_version = 1;
  $data_table->title = 'Fast feed';
  $data_table->name = 'feeds_data_feed_fast';
  $data_table->table_schema = array(
    'fields' => array(
      'feed_nid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'id' => array(
        'type' => 'serial',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'timestamp' => array(
        'description' => 'The Unix timestamp for the data.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'title' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
      ),
      'description' => array(
        'type' => 'text',
        'not null' => FALSE,
      ),
      'url' => array(
        'type' => 'text',
        'not null' => FALSE,
      ),
      'guid' => array(
        'type' => 'text',
        'not null' => FALSE,
      ),
    ),
    'indexes' => array(
      'feed_nid' => array(
        '0' => 'feed_nid',
      ),
      'id' => array(
        '0' => 'id',
      ),
      'timestamp' => array(
        '0' => 'timestamp',
      ),
      'url' => array(
        '0' => array(
          '0' => 'url',
          '1' => 255,
        ),
      ),
      'guid' => array(
        '0' => array(
          '0' => 'guid',
          '1' => 255,
        ),
      ),
    ),
    'primary key' => array(
      '0' => 'id',
    ),
  );
  $data_table->meta = array(
    'fields' => array(
      'feed_nid' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'id' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'timestamp' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field_date',
        'views_filter_handler' => 'views_handler_filter_date',
        'views_argument_handler' => 'views_handler_argument_date',
        'views_sort_handler' => 'views_handler_sort_date',
      ),
      'title' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument_string',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'description' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'url' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field_url',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'guid' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
    ),
  );

  $export['feeds_data_feed_fast'] = $data_table;
  return $export;
}

/**
 * Helper to implementation of hook_feeds_importer_default().
 */
function _feeds_defaults_feeds_importer_default() {
  $export = array();
  $feeds_importer = new stdClass;
  $feeds_importer->disabled = TRUE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'feed';
  $feeds_importer->config = array(
    'name' => 'Feed',
    'description' => 'Import RSS or Atom feeds, create nodes from feed items.',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'feed_item',
        'update_existing' => 0,
        'expire' => '-1',
        'mappings' => array(
          '0' => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          '1' => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          '2' => array(
            'source' => 'timestamp',
            'target' => 'created',
            'unique' => FALSE,
          ),
          '3' => array(
            'source' => 'url',
            'target' => 'url',
            'unique' => TRUE,
          ),
          '4' => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => TRUE,
          ),
        ),
      ),
    ),
    'content_type' => 'feed',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );

  $export['feed'] = $feeds_importer;

  // Expose a default configuration for Data if enabled.
  if (module_exists('data')) {

    $feeds_importer = new stdClass;
    $feeds_importer->disabled = TRUE; /* Edit this to true to make a default feeds_importer disabled initially */
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'feed_fast';
    $feeds_importer->config = array(
      'name' => 'Fast feed',
      'description' => 'Create light weight database records from feed items. Faster than aggregating nodes.',
      'fetcher' => array(
        'plugin_key' => 'FeedsHTTPFetcher',
        'config' => array(
          'auto_detect_feeds' => FALSE,
        ),
      ),
      'parser' => array(
        'plugin_key' => 'FeedsSyndicationParser',
        'config' => array(),
      ),
      'processor' => array(
        'plugin_key' => 'FeedsDataProcessor',
        'config' => array(
          'update_existing' => 0,
          'expire' => '7257600',
          'mappings' => array(
            '0' => array(
              'source' => 'title',
              'target' => 'title',
              'unique' => 0,
            ),
            '1' => array(
              'source' => 'description',
              'target' => 'description',
              'unique' => 0,
            ),
            '2' => array(
              'source' => 'url',
              'target' => 'url',
              'unique' => 1,
            ),
            '3' => array(
              'source' => 'guid',
              'target' => 'guid',
              'unique' => 1,
            ),
          ),
        ),
      ),
      'content_type' => 'feed_fast',
      'update' => 0,
      'import_period' => '1800',
      'expire_period' => 3600,
      'import_on_create' => 1,
    );

    $export['feed_fast'] = $feeds_importer;
  }

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = TRUE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'node';
  $feeds_importer->config = array(
    'name' => 'Node import',
    'description' => 'Import nodes from CSV file.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'story',
        'update_existing' => 1,
        'expire' => '-1',
        'mappings' => array(
          '0' => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          '1' => array(
            'source' => 'body',
            'target' => 'body',
            'unique' => FALSE,
          ),
          '2' => array(
            'source' => 'published',
            'target' => 'created',
            'unique' => FALSE,
          ),
          '3' => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );

  $export['node'] = $feeds_importer;
  $feeds_importer = new stdClass;
  $feeds_importer->disabled = TRUE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'opml';
  $feeds_importer->config = array(
    'name' => 'OPML import',
    'description' => 'Import subscriptions from OPML files. Use together with "Feed" configuration.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsOPMLParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsFeedNodeProcessor',
      'config' => array(
        'content_type' => 'feed',
        'update_existing' => 0,
        'mappings' => array(
          '0' => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          '1' => array(
            'source' => 'xmlurl',
            'target' => 'source',
            'unique' => 1,
          ),
        ),
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );

  $export['opml'] = $feeds_importer;
  $feeds_importer = new stdClass;
  $feeds_importer->disabled = TRUE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'user';
  $feeds_importer->config = array(
    'name' => 'User import',
    'description' => 'Import users from CSV file.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsUserProcessor',
      'config' => array(
        'roles' => array(),
        'update_existing' => FALSE,
        'status' => 1,
        'mappings' => array(
          '0' => array(
            'source' => 'name',
            'target' => 'name',
            'unique' => 0,
          ),
          '1' => array(
            'source' => 'mail',
            'target' => 'mail',
            'unique' => 1,
          ),
          '2' => array(
            'source' => 'created',
            'target' => 'created',
            'unique' => FALSE,
          ),
        ),
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );

  $export['user'] = $feeds_importer;
  return $export;
}
