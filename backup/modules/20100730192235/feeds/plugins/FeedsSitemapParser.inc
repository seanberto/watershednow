<?php
// $Id: FeedsSitemapParser.inc,v 1.1 2010/05/16 21:35:08 alexb Exp $

/**
 * A parser for the Sitemap specification http://www.sitemaps.org/protocol.php
 */
class FeedsSitemapParser extends FeedsParser {
  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    // Yes, using a DOM parser is a bit inefficient, but will do for now
    $xml = new SimpleXMLElement($batch->getRaw());
    foreach ($xml->url as $url) {
      $item = array('url' => (string) $url->loc);
      if ($url->lastmod) {
        $item['lastmod'] = strtotime($url->lastmod);
      }
      if ($url->changefreq) {
        $item['changefreq'] = $url->changefreq;
      }
      if ($url->priority) {
        $item['priority'] = $url->priority;
      }
      $batch->addItem($item);
    }
  }

  /**
   * Implementation of FeedsParser::getMappingSources().
   */
  public function getMappingSources() {
    return array(
      'url' => array(
        'name' => t('Item URL (link)'),
        'description' => t('URL of the feed item.'),
      ),
      'lastmod' => array(
        'name' => t('Last modification date'),
        'description' => t('Last modified date as UNIX time GMT of the feed item.'),
      ),
      'changefreq' => array(
        'name' => t('Change frequency'),
        'description' => t('How frequently the page is likely to change.'),
      ),
      'priority' => array(
        'name' => t('Priority'),
        'description' => t('The priority of this URL relative to other URLs on the site.'),
      ),
    );
  }
}
