<?php
// $Id: feeds.test.inc,v 1.15 2010/09/15 19:27:42 alexb Exp $

/**
 * @file
 * Common functionality for all Feeds tests.
 */

/**
 * Test basic Data API functionality.
 */
class FeedsWebTestCase extends DrupalWebTestCase {

  /**
   * Debug utility. Shows current screen.
   */
  public function show($path = NULL) {
    if ($path !== NULL) {
      $this->drupalGet($path);
    }
    // Use error, this will make sure we don't forget accidental calls to show()
    // in our tests.
    $this->error($this->content);
  }

  /**
   * Absolute path to Drupal root.
   */
  public function absolute() {
    return realpath(getcwd());
  }

  /**
   * Get the absolute directory path of the feeds module.
   */
  public function absolutePath() {
    return  $this->absolute() .'/'. drupal_get_path('module', 'feeds');
  }

  /**
   * Generate an OPML test feed.
   *
   * The purpose of this function is to create a dynamic OPML feed that points
   * to feeds included in this test.
   */
  public function generateOPML() {
    $path = $GLOBALS['base_url'] .'/'. drupal_get_path('module', 'feeds') .'/tests/feeds/';

  $output =
'<?xml version="1.0" encoding="utf-8"?>
<opml version="1.1">
<head>
    <title>Feeds test OPML</title>
    <dateCreated>Fri, 16 Oct 2009 02:53:17 GMT</dateCreated>
    <ownerName></ownerName>
</head>
<body>
  <outline text="Feeds test group" >
       <outline title="Development Seed - Technological Solutions for Progressive Organizations" text="" xmlUrl="'. $path .'developmentseed.rss2" type="rss" />
       <outline title="Magyar Nemzet Online - H’rek" text="" xmlUrl="'. $path .'feed_without_guid.rss2" type="rss" />
       <outline title="Drupal planet" text="" type="rss" xmlUrl="'. $path .'drupalplanet.rss2" />
    </outline>
</body>
</opml>';

    // UTF 8 encode output string and write it to disk
    $output = utf8_encode($output);
    $file = $this->absolute() .'/'. file_directory_path() .'/test-opml-'. $this->randomName() .'.opml';
    $handle = fopen($file, 'w');
    fwrite($handle, $output);
    fclose($handle);
    return $file;
  }

  /**
   * Create an importer configuration.
   *
   * @param $name
   *   The natural name of the feed.
   * @param $id
   *   The persistent id of the feed.
   * @param $edit
   *   Optional array that defines the basic settings for the feed in a format
   *   that can be posted to the feed's basic settings form.
   */
  public function createImporterConfiguration($name = 'Syndication', $id = 'syndication') {
    // Create new feed configuration.
    $this->drupalGet('admin/build/feeds');
    $this->clickLink('New importer');
    $edit = array(
      'name' => $name,
      'id' => $id,
    );
    $this->drupalPost('admin/build/feeds/create', $edit, 'Create');

    // Assert message and presence of default plugins.
    $this->assertText('Your configuration has been created with default settings.');
    $this->assertPlugins($id, 'FeedsHTTPFetcher', 'FeedsSyndicationParser', 'FeedsNodeProcessor');
    // Per default attach to page content type.
    $this->setSettings($id, NULL, array('content_type' => 'page'));
  }

  /**
   * Choose a plugin for a importer configuration and assert it.
   *
   * @param $id
   *   The importer configuration's id.
   * @param $plugin_key
   *   The key string of the plugin to choose (one of the keys defined in
   *   feeds_feeds_plugins()).
   */
  public function setPlugin($id, $plugin_key) {
    if ($type = feeds_plugin_type($plugin_key)) {
      $edit = array(
        'plugin_key' => $plugin_key,
      );
      $this->drupalPost("admin/build/feeds/edit/$id/$type", $edit, 'Save');

      // Assert actual configuration.
      $config = unserialize(db_result(db_query("SELECT config FROM {feeds_importer} WHERE id = '%s'", $id)));
      $this->assertEqual($config[$type]['plugin_key'], $plugin_key, 'Verified correct '. $type .' ('. $plugin_key .').');
    }
  }

  /**
   * Set importer or plugin settings.
   *
   * @param $id
   *   The importer configuration's id.
   * @param $plugin
   *   The plugin (class) name, or NULL to set importer's settings
   * @param $settings
   *   The settings to set.
   */
  public function setSettings($id, $plugin, $settings) {
    $this->drupalPost('admin/build/feeds/edit/'. $id .'/settings/'. $plugin, $settings, 'Save');
    $this->assertText('Your changes have been saved.');
  }

  /**
   * Create a test feed node. Test user has to have sufficient permissions:
   *
   * * create [type] content
   * * use feeds
   *
   * Assumes that page content type has been configured with
   * createImporterConfiguration() as a feed content type.
   *
   * @return
   *   The node id of the node created.
   */
  public function createFeedNode($id = 'syndication', $feed_url = NULL, $title = '', $content_type = NULL) {
    if (empty($feed_url)) {
      $feed_url = $GLOBALS['base_url'] .'/'. drupal_get_path('module', 'feeds') .'/tests/feeds/developmentseed.rss2';
    }
    // If content type not given, retrieve it.
    if (!$content_type) {
      $config = unserialize(db_result(db_query("SELECT config FROM {feeds_importer} WHERE id = '%s'", $id)));
      $content_type = $config['content_type'];
      $this->assertFalse(empty($content_type), 'Valid content type found: '. $content_type);
    }

    // Create a feed node.
    $edit = array(
      'title' => $title,
      'feeds[FeedsHTTPFetcher][source]' => $feed_url,
    );
    $this->drupalPost('node/add/'. str_replace('_', '-', $content_type), $edit, 'Save');
    $this->assertText('has been created.');

    // Get the node id from URL.
    $nid = $this->getNid($this->getUrl());

    // Check whether feed got recorded in feeds_source table.
    $this->assertEqual(1, db_result(db_query("SELECT COUNT(*) FROM {feeds_source} WHERE id = '%s' AND feed_nid = %d", $id, $nid)));
    $source = db_fetch_object(db_query("SELECT * FROM {feeds_source} WHERE id = '%s' AND feed_nid = %d", $id, $nid));
    $config = unserialize($source->config);
    $this->assertEqual($config['FeedsHTTPFetcher']['source'], $feed_url, t('URL in DB correct.'));
    return $nid;
  }

  /**
   * Edit the configuration of a feed node to test update behavior.
   *
   * @param $nid
   *   The nid to edit.
   * @param $feed_url
   *   The new (absolute) feed URL to use.
   * @param $title
   *   Optional parameter to change title of feed node.
   */
  public function editFeedNode($nid, $feed_url, $title = '') {
    $edit = array(
      'title' => $title,
      'feeds[FeedsHTTPFetcher][source]' => $feed_url,
    );
    // Check that the update was saved.
    $this->drupalPost('node/' . $nid . '/edit', $edit, 'Save');
    $this->assertText('has been updated.');

    // Check that the URL was updated in the feeds_source table.
    $source = db_fetch_object(db_query("SELECT * FROM {feeds_source} WHERE feed_nid = %d", $nid));
    $config = unserialize($source->config);
    $this->assertEqual($config['FeedsHTTPFetcher']['source'], $feed_url, t('URL in DB correct.'));
  }

  /**
   * Batch create a variable amount of feed nodes. All will have the
   * same URL configured.
   *
   * @return
   *   An array of node ids of the nodes created.
   */
  public function createFeedNodes($id = 'syndication', $num = 20, $content_type = NULL) {
    $nids = array();
    for ($i = 0; $i < $num; $i++) {
      $nids[] = $this->createFeedNode($id, NULL, $this->randomName(), $content_type);
    }
    return $nids;
  }

  /**
   * Import a URL through the import form. Assumes FeedsHTTPFetcher in place.
   */
  public function importURL($id, $feed_url = NULL) {
    if (empty($feed_url)) {
      $feed_url = $GLOBALS['base_url'] .'/'. drupal_get_path('module', 'feeds') .'/tests/feeds/developmentseed.rss2';
    }
    $edit = array(
      'feeds[FeedsHTTPFetcher][source]' => $feed_url,
      );
    $nid = $this->drupalPost('import/'. $id, $edit, 'Import');

    // Check whether feed got recorded in feeds_source table.
    $this->assertEqual(1, db_result(db_query("SELECT COUNT(*) FROM {feeds_source} WHERE id = '%s' AND feed_nid = 0", $id)));
    $source = db_fetch_object(db_query("SELECT * FROM {feeds_source} WHERE id = '%s' AND feed_nid = 0", $id));
    $config = unserialize($source->config);
    $this->assertEqual($config['FeedsHTTPFetcher']['source'], $feed_url, t('URL in DB correct.'));

    // Check whether feed got properly added to scheduler.
    $this->assertEqual(1, db_result(db_query("SELECT COUNT(*) FROM {job_schedule} WHERE type = '%s' AND id = 0 AND callback = 'feeds_source_import' AND last <> 0 AND scheduled = 0", $id)));
    // There must be only one entry for callback 'expire' - no matter what the feed_nid is.
    $this->assertEqual(0, db_result(db_query("SELECT COUNT(*) FROM {job_schedule} WHERE type = '%s' AND callback = 'feeds_importer_expire' AND last <> 0 AND scheduled = 0", $id)));
  }

  /**
   * Import a file through the import form. Assumes FeedsFileFetcher in place.
   */
  public function importFile($id, $file) {

    $this->assertTrue(file_exists($file), 'Source file exists');
    $edit = array(
      'files[feeds]' => $file,
    );
    $this->drupalPost('import/'. $id, $edit, 'Import');
  }

  /**
   * Assert a feeds configuration's plugins.
   *
   * @deprecated:
   *   Use setPlugin() instead.
   *
   * @todo Refactor users of assertPlugin() and make them use setPugin() instead.
   */
  public function assertPlugins($id, $fetcher, $parser, $processor) {
    // Assert actual configuration.
    $config = unserialize(db_result(db_query("SELECT config FROM {feeds_importer} WHERE id = '%s'", $id)));

    $this->assertEqual($config['fetcher']['plugin_key'], $fetcher, 'Correct fetcher');
    $this->assertEqual($config['parser']['plugin_key'], $parser, 'Correct parser');
    $this->assertEqual($config['processor']['plugin_key'], $processor, 'Correct processor');
  }

  /**
   * Add mappings to a given configuration.
   *
   * @param $mappings
   *   An array of mapping arrays. Each mapping array must have a source and
   *   an target key and can have a unique key.
   *
   * @see FeedsRSStoDataTest class.
   */
  public function addMappings($id, $mappings) {

    $path = 'admin/build/feeds/edit/'. $id .'/mapping';

    // Iterate through all mappings and add the via the form.
    foreach ($mappings as $i => $mapping) {

      // Get unique flag and unset it - otherwise drupalPost will complain that
      // there is no form element named "unique".
      $unique = !empty($mapping['unique']);
      unset($mapping['unique']);
      $this->drupalPost($path, $mapping, t('Add'));

      // If unique was set, set the last mapping's unique flag.
      if ($unique) {
        $edit = array(
          'unique_flags['. $i .']' => 1,
        );
        $this->drupalPost($path, $edit, t('Save'));
      }
    }
  }

  /**
   * Helper function, retrieves node id from a URL.
   */
  public function getNid($url) {
    $matches = array();
    preg_match('/node\/(\d+?)$/', $url, $matches);
    $nid = $matches[1];
    if (!is_numeric($nid)) {
      $this->error(t('Could not find node id, found @nid instead.', array('@nid' => $nid)));
    }
    return $nid;
  }
}
