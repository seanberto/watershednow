$Id: README.txt,v 1.30 2010/07/05 23:51:36 alexb Exp $


"It feeds"


FEEDS
=====

An import and aggregation framework for Drupal.
http://drupal.org/project/feeds

Features
========

- Pluggable import configurations consisting of fetchers (get data) parsers
  (read and transform data) and processors (create content on Drupal).
-- HTTP upload (with optional PubSubHubbub support).
-- File upload.
-- CSV, RSS, Atom parsing.
-- Creates nodes or terms.
-- Creates lightweight database records if Data module is installed.
   http://drupal.org/project/data
-- Additional fetchers/parsers or processors can be added by an object oriented
   plugin system.
-- Granular mapping of parsed data to content elements.
- Import configurations can be piggy backed on nodes, thus using nodes as
  importers ("feed as node" approach) or they can be used on a standalone form.
- Unlimited number of import configurations.
- Export import configurations to code.
- Optional libraries module support.

Requirements
============

- CTools 1.x
  http://drupal.org/project/ctools
- Drupal 6.x
  http://drupal.org/project/drupal
- PHP safe mode is not supported, depending on your Feeds Importer configuration
  safe mode may cause no problems though.
- PHP 5.2.x recommended

Installation
============

- Install Feeds, Feeds Admin UI and Feeds defaults.
- Make sure cron is correctly configured http://drupal.org/cron
- Navigate to admin/build/feeds.
- Enable one or more importers, create your own by adding a new one, modify an
  existing one by clicking on 'override' or copy and modify an existing one by
  clicking on 'clone'.
- Go to import/ to import data.
- To use SimplePie parser, download SimplePie and place simplepie.inc into
  feeds/libraries. Recommended version: 1.2.
  http://simplepie.org/

PubSubHubbub support
====================

Feeds supports the PubSubHubbub publish/subscribe protocol. Follow these steps
to set it up for your site.
http://code.google.com/p/pubsubhubbub/

- Go to admin/build/feeds and edit (override) the importer configuration you
  would like to use for PubSubHubbub.
- Choose the HTTP Fetcher if it is not already selected.
- On the HTTP Fetcher, click on 'settings' and check "Use PubSubHubbub".
- Optionally you can use a designated hub such as http://superfeedr.com/ or your
  own. If a designated hub is specified, every feed on this importer
  configuration will be subscribed to this hub, no matter what the feed itself
  specifies.

Libraries support
=================

If you are using Libraries module, you can place external libraries in the
Libraries module's search path (for instance sites/all/libraries. The only
external library used at the moment is SimplePie.

Libraries found in the libraries search path are preferred over libraries in
feeds/libraries/.

Transliteration support
=======================

If you plan to store files with Feeds - for instance when storing podcasts
or images from syndication feeds - it is recommended to enable the
Transliteration module to avoid issues with non-ASCII characters in file names.
http://drupal.org/project/transliteration

API Overview
============

See "The developer's guide to Feeds":
http://drupal.org/node/622700

Testing
=======

See "The developer's guide to Feeds":
http://drupal.org/node/622700

Debugging
=========

Set the Drupal variable 'feeds_debug' to TRUE (i. e. using drush). This will
create a file /tmp/feeds_[my_site_location].log. Use "tail -f" on the command
line to get a live view of debug output.

Note: at the moment, only PubSubHubbub related actions are logged.

Performance
===========

See "The site builder's guide to Feeds":
http://drupal.org/node/622698

Hidden settings
===============

Hidden settings are variables that you can define by adding them to the $conf
array in your settings.php file.

Name:        feeds_debug
Default:     FALSE
Description: Set to TRUE for enabling debug output to
             /DRUPALTMPDIR/feeds_[sitename].log

Name:        feeds_importer_class
Default:     'FeedsImporter'
Description: The class to use for importing feeds.

Name:        feeds_source_class
Default:     'FeedsSource'
Description: The class to use for handling feed sources.

Name:        feeds_scheduler_class
Default:     'FeedsScheduler'
Description: The class to use for scheduling feed refreshing.

Name:        feeds_worker_time
Default:     15
Description: Execution time for a queue worker, only effective if used with
             drupal_queue.

Name:        feeds_schedule_num
Default:     5
Description: The number of feeds to import on cron time.
             Only has an effect if Drupal Queue is *not* enabled.
             http://drupal.org/project/drupal_queue

Name:        feeds_schedule_queue_num
Default:     200
Description: The number of feeds to queue on cron time. Only has an effect if
             Drupal Queue is enabled.
             http://drupal.org/project/drupal_queue

Name:        feeds_data_$importer_id
Default:     feeds_data_$importer_id
Description: The table used by FeedsDataProcessor to store feed items. Usually a
             FeedsDataProcessor builds a table name from a prefix (feeds_data_)
             and the importer's id ($importer_id). This default table name can
             be overridden by defining a variable with the same name.

Name:        feeds_node_batch_size
Default:     50
             The number of nodes feed node processor creates or deletes in one
             page load.

Glossary
========

See "Feeds glossary":
http://drupal.org/node/622710
