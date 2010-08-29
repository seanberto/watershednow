<?php
// $Id: watershednow.profile,v 1.22 2007/12/17 12:43:34 goba Exp $

/**
 * Implementation of hook_profile_details().
 */
function watershednow_profile_details() {
	return array(
		'name' => 'Watershed Now!',
		'description' => 'Select this profile to build a Drupal CMS developed for watershed organizations.'
	);
}

/**
 * Implementation of hook_profile_modules().
 */
function watershednow_profile_modules() {
	
	// When enabling a feature via an installation profile, dependancies are not automatically enabled. Have to enable them here.
	// @TODO: Programmatically check features' dependencies so that we only have to manage dependencies in one place.
	$modules = array(
		 // Drupal core - optional
		'comment',
		'contact',
		'help',
		'menu',
		'path',
		'search',
		'taxonomy',
		'update',
		// Admin
		'admin',
		// Views
		'views',
		'views_calc',
		'draggableviews',
		'web_widgets',
		// CTools
		'ctools',
		// Context
		'context',
		// Features
		'features',
		'strongarm',
		// Image
		'imageapi',
		'imageapi_gd',
		'imagecache',
		// CCK
		'content',
		'number',
		'text',
		'filefield',
		'imagefield',
		'emfield',
		'emvideo',
		'optionwidgets',
		'nodereference',
		// Date
		'date_api',
		'date',
		'date_popup',
		'date_timezone',
		// Other
		'advanced_help',
		'boxes',
		'logintoboggan',
		'menu_block',
		'pathauto',
		'token',
		'transliteration',
		'googleanalytics',
		// User interface and WYSIWYG
		'better_formats',
		'insert',
		'filefield_sources',
		'jquery_ui',
		'webform',
		'wysiwyg',
		'wysiwyg_imagefield',
		// Installation and initial config
		'install_profile_api',
		// Donation tools
		'simple_payments',
		'simple_payments_paypal',
		'simple_donations',
		// Event Sigups
		'signup',
		// Open Layers
		'openlayers',
		'openlayers_cck',
		'openlayers_views',
		'openlayers_geocoder',
		// Customizations
		'watershednow_helper',
		// Watershed Now features
		'watershednow_common',
		'watershednow_blog',
		'watershednow_getinvolved',
		'watershednow_map',
		// Development (NOTE: Comment out for production installs.)
		'devel',
		'diff',
		'context_ui',
		'feeds_ui',
		'imagecache_ui',
		'openlayers_ui',
		'views_ui',
		'variable_dump',
	);

	return $modules;
}

/**
 * Implementation of hook_profile_tasks
 */
function watershednow_profile_tasks(&$task, $url) {
	
	install_include(watershednow_profile_modules()); // Call all enabled modules' include files.
	
	$default_roles = array('web admin', 'staff'); // Adding roles and perms.
	$default_perms = _watershednow_define_perms(); // Grabbing array of permissions.
	foreach ($default_roles as $role) {
		install_add_role($role);
		$rid = install_get_rid($role); // Get the RID so we can add perms.
		if ($role == 'staff') {
			install_add_permissions($rid, $default_perms['staff']);
		}
		if ($role == 'web admin') {
			install_add_permissions($rid, $default_perms['web_admin']);
		}
	}
	install_add_permissions('1', $default_perms['anonymous']);
	install_add_permissions('2', $default_perms['authenticated']);
	
	// Now remove some crufy permissions
	install_remove_permissions('1', $default_perms['to_remove']);	 
	install_remove_permissions('2', $default_perms['to_remove']);	 

	// Configure Input formats and WYSIWYG editor
	_watershednow_fiters_wysiwyg();
	
	// Setup basic contact form
	$category = 'website feedback';
	$recipients = 'admin@'. $_SERVER['HTTP_HOST'];
	install_contact_add_category($category, $recipients, $reply = '', $weight = 0, $selected = 1);
	// Add contact form to secondary links.
	install_menu_create_menu_item('contact', 'Contact Us', 'Get in touch with us.', 'secondary_links');

	// Create a few placeholder nodes and add them to the menu structure.
	// @TODO: Find the correct input filter. Right now, we rely on _watershednow_filters_wysiwyg() being called first..
	$pages = array();
	
	$page = new stdClass;
	$page->title = 'About Us';
	$page->body = 'This is a placeholder "about us" page. Here readers can learn more about this website and the orgaization behind it.';
	$page->format = 2;
	$page->menu = 'secondary_links';
	$page->menuitem_description = 'Learn more about us';
	$page->parent_title = NULL;
	$pages['about'] = $page;
	
	$page = new stdClass;
	$page->title = 'Staff';
	$page->body = 'This is a placeholder page about our staff';
	$page->format = 2;
	$page->menu = 'secondary_links';
	$page->menuitem_description = 'Get to know our staff. Feel free to rename or even delete this page. It\'s provided here to show how subpages work.';
	$page->parent_title = 'About Us';
	$pages['staff'] = $page;
	
	$page = new stdClass;
	$page->title = 'History';
	$page->body = 'This is a placeholder page about our organizational history. Feel free to rename or even delete this page. It\'s provided here to show how subpages work.';
	$page->format = 2;
	$page->menu = 'secondary_links';
	$page->menuitem_description = 'Get to know our w.';
	$page->parent_title = 'About Us';
	$pages['history'] = $page;
	
	foreach ($pages as $node) {
		$created_node = install_create_node($node->title, $node->body);
		if (!empty($node->parent_title)) {
			$parent_item = install_menu_get_items(NULL, $node->parent_title);
			$plid = $parent_item[0]['mlid'];
		} else {
			$plid = 0;
		}
		
		install_menu_create_menu_item('node/' . $created_node->nid, $created_node->title, $node->menuitem_description, $node->menu, $plid);		 
	}

	// Borrowing from openatrium.profile to set default theme to watershed.
	// Rebuild key tables/caches
	drupal_flush_all_caches();
	// Set default theme. This must happen after drupal_flush_all_caches(), which
	// will run system_theme_data() without detecting themes in the install
	// profile directory.
	_watershednow_system_theme_data();
	db_query("UPDATE {blocks} SET status = 0, region = ''"); // disable all DB blocks
	db_query("UPDATE {system} SET status = 0 WHERE type = 'theme' and name ='%s'", 'garland');
	db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", 'watershed');
	db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", 'tao');
	db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", 'rubik');
	variable_set('theme_default', 'watershed'); // set the default theme to Basic
	variable_set('admin_theme', 'rubik'); // set the admin theme to Rubik
	variable_set('node_admin_theme', 1); // Use the admin theme for content editing

}

/*
 * Defining default permissions for all roles.
 */
function _watershednow_define_perms() {
	$default_perms = array();
	
	$default_perms['anonymous'] = array(
		'access comments',
		'access site-wide contact form',
		'access content',
		'search content',
		'change own username',
		);
		
	$default_perms['authenticated'] = array(
		'access comments',
		'post comments',
		'post comments without approval',
		'access site-wide contact form',
		'access content',
		'search content',
		'change own username',
		);
	
	$default_perms['staff'] = array(
		'use admin toolbar',
		'view advanced help index',
		'view advanced help popup',
		'view advanced help topic',
		'administer comments',
		'create page content',
		'delete any page content',
		'delete own page content',
		'delete revisions',
		'edit any page content',
		'edit own page content',
		'revert revisions',
		'view revisions',
		'create url aliases',
		'use advanced search',
		'access administration pages',
		'access user profiles',
		'use wysiwyg image upload',
		);
	
	$default_perms['web_admin'] = array(
		'use admin toolbar',
		'view advanced help index',
		'view advanced help popup',
		'view advanced help topic',
		'administer comments',
		'administer site-wide contact form',
		'administer menu',
		'administer nodes',
		'create page content',
		'delete any page content',
		'delete own page content',
		'delete revisions',
		'edit any page content',
		'edit own page content',
		'revert revisions',
		'view revisions',
		'create url aliases',
		'use advanced search',
		'access administration pages',
		'administer taxonomy',
		'access user profiles',
		'administer users',
		'use wysiwyg image upload',
		);
		
	// @FIXME - Hacky having this list in this array, but okay for now.
	$default_perms['to_remove'] = array(
		'collapse format fieldset by default',
		'collapsible format selection',
		'show format selection for blocks',
		'show format selection for comments',
		'show format selection for nodes',
		'show format tips',
		'show more format tips link',
		);
	
	return $default_perms;
}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function watershednow_form_alter(&$form, $form_state, $form_id) {
	if ($form_id == 'install_configure') {
		$form['site_information']['site_name']['#default_value'] = $_SERVER['HTTP_HOST'];
		$form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
		$form['admin_account']['account']['name']['#default_value'] = 'admin';
		$form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
	}
}

/**
 * Input filter and WYWIWYG editor configuration.
 *
 * @FIXME: This should really be managed via Exportables.module or Input_filters.module,
 * but they aren't production ready. There's probably 50 more eligant ways to handle this.
 * Also, for expediancy, not properly using db_query() below.
 *
 * @FIXME: Also, should use install_get_rid() to make sure that RIDs are consistent with those
 * created for "web admin" and "staff" roles. Fragile as is.
 */
function _watershednow_fiters_wysiwyg() {
	
	// Setting up Better Formats
	db_query("DELETE FROM {better_formats_defaults}");
	db_query("INSERT INTO {better_formats_defaults} (`rid`,`type`,`format`,`type_weight`,`weight`)
	VALUES
		(1,'node',1,1,-22),
		(1,'comment',1,1,-22),
		(1,'block',1,1,-22),
		(2,'node',1,1,-23),
		(2,'comment',1,1,-23),
		(2,'block',1,1,-23),
		(3,'node',2,1,-25),
		(3,'comment',2,1,-25),
		(3,'block',3,1,-25),
		(4,'node',2,1,-24),
		(4,'comment',2,1,-24),
		(4,'block',3,1,-24)"
		);
	
	// Setting up Filter Formats
	db_query("DELETE FROM {filter_formats}");
	db_query("INSERT INTO {filter_formats} (`format`,`name`,`roles`,`cache`)
	VALUES
		(1,'Filtered HTML',',1,2,',1),
		(2,'Full HTML - Used for editing node content',',4,3,',1),
		(3,'Block HTML - Special formatter for Blocks. Same as Full HTML. (Needed b/c using a different WYSIWYG editor)',',4,3,',1)"
	);
	
	// Setting up Filters
	db_query("DELETE FROM {filters}");
	db_query("INSERT INTO {filters} (`fid`,`format`,`module`,`delta`,`weight`)
	VALUES
		(1,1,'filter',2,0),
		(2,1,'filter',0,1),
		(3,1,'filter',1,2),
		(4,1,'filter',3,10),
		(29,2,'filter',2,0),
		(27,2,'filter',3,10),
		(28,2,'filter',1,1),
		(21,3,'filter',3,10),
		(22,3,'filter',1,2),
		(23,3,'filter',2,0)"
	);
	 
	// Setting up WYSIWYG profiles
	$wysiwyg_profiles = array();
	$wysiwyg_profiles['1'] = array(
		'format' => 1,
		'editor' => '',
		'settings' => NULL
		);
	$wysiwyg_profiles['2'] = array(
		'format' => 2,
		'editor' => 'ckeditor',
		'settings' => 'a:20:{s:7:"default";i:1;s:11:"user_choose";i:0;s:11:"show_toggle";i:1;s:5:"theme";s:8:"advanced";s:8:"language";s:2:"en";s:7:"buttons";a:2:{s:7:"default";a:23:{s:4:"Bold";i:1;s:6:"Italic";i:1;s:9:"Underline";i:1;s:6:"Strike";i:1;s:11:"JustifyLeft";i:1;s:13:"JustifyCenter";i:1;s:12:"JustifyRight";i:1;s:12:"JustifyBlock";i:1;s:12:"BulletedList";i:1;s:12:"NumberedList";i:1;s:7:"Outdent";i:1;s:6:"Indent";i:1;s:4:"Undo";i:1;s:4:"Redo";i:1;s:4:"Link";i:1;s:6:"Unlink";i:1;s:6:"Anchor";i:1;s:5:"Image";i:1;s:11:"Superscript";i:1;s:9:"Subscript";i:1;s:12:"RemoveFormat";i:1;s:6:"Format";i:1;s:5:"Table";i:1;}s:6:"drupal";a:1:{s:18:"wysiwyg_imagefield";i:1;}}s:11:"toolbar_loc";s:3:"top";s:13:"toolbar_align";s:4:"left";s:8:"path_loc";s:6:"bottom";s:8:"resizing";i:1;s:11:"verify_html";i:1;s:12:"preformatted";i:0;s:22:"convert_fonts_to_spans";i:1;s:17:"remove_linebreaks";i:1;s:23:"apply_source_formatting";i:0;s:27:"paste_auto_cleanup_on_paste";i:0;s:13:"block_formats";s:32:"p,address,pre,h2,h3,h4,h5,h6,div";s:11:"css_setting";s:5:"theme";s:8:"css_path";s:0:"";s:11:"css_classes";s:0:"";}'
		);
	$wysiwyg_profiles['3'] = array(
		'format' => 3,
		'editor' => 'ckeditor',
		'settings' => 'a:20:{s:7:"default";i:0;s:11:"user_choose";i:0;s:11:"show_toggle";i:1;s:5:"theme";s:8:"advanced";s:8:"language";s:2:"en";s:7:"buttons";a:1:{s:7:"default";a:9:{s:4:"Bold";i:1;s:6:"Italic";i:1;s:9:"Underline";i:1;s:11:"JustifyLeft";i:1;s:13:"JustifyCenter";i:1;s:12:"JustifyRight";i:1;s:4:"Link";i:1;s:6:"Unlink";i:1;s:6:"Format";i:1;}}s:11:"toolbar_loc";s:3:"top";s:13:"toolbar_align";s:4:"left";s:8:"path_loc";s:6:"bottom";s:8:"resizing";i:1;s:11:"verify_html";i:1;s:12:"preformatted";i:0;s:22:"convert_fonts_to_spans";i:1;s:17:"remove_linebreaks";i:1;s:23:"apply_source_formatting";i:0;s:27:"paste_auto_cleanup_on_paste";i:0;s:13:"block_formats";s:32:"p,address,pre,h2,h3,h4,h5,h6,div";s:11:"css_setting";s:5:"theme";s:8:"css_path";s:0:"";s:11:"css_classes";s:0:"";}'
		);
	
	db_query("DELETE FROM {wysiwyg}");
	foreach ($wysiwyg_profiles as $profile) {
		db_query("INSERT INTO {wysiwyg} (`format`,`editor`,`settings`) VALUES (%d, '%s', '%s')", $profile['format'], $profile['editor'], $profile['settings']);
	};
	
}

/**
 * Ripped off from Open Atrium's install profile.
 *
 * Re-implementation of system_theme_data(). The core function's static cache
 * is populated during install prior to active install profile awareness.
 * This workaround makes enabling themes in profiles/[profile]/themes possible.
 */
function _watershednow_system_theme_data() {
	global $profile;
	$profile = 'watershednow';

	$themes = drupal_system_listing('\.info$', 'themes');
	$engines = drupal_system_listing('\.engine$', 'themes/engines');

	$defaults = system_theme_default();

	$sub_themes = array();
	foreach ($themes as $key => $theme) {
		$themes[$key]->info = drupal_parse_info_file($theme->filename) + $defaults;

		if (!empty($themes[$key]->info['base theme'])) {
			$sub_themes[] = $key;
		}

		$engine = $themes[$key]->info['engine'];
		if (isset($engines[$engine])) {
			$themes[$key]->owner = $engines[$engine]->filename;
			$themes[$key]->prefix = $engines[$engine]->name;
			$themes[$key]->template = TRUE;
		}

		// Give the stylesheets proper path information.
		$pathed_stylesheets = array();
		foreach ($themes[$key]->info['stylesheets'] as $media => $stylesheets) {
			foreach ($stylesheets as $stylesheet) {
				$pathed_stylesheets[$media][$stylesheet] = dirname($themes[$key]->filename) .'/'. $stylesheet;
			}
		}
		$themes[$key]->info['stylesheets'] = $pathed_stylesheets;

		// Give the scripts proper path information.
		$scripts = array();
		foreach ($themes[$key]->info['scripts'] as $script) {
			$scripts[$script] = dirname($themes[$key]->filename) .'/'. $script;
		}
		$themes[$key]->info['scripts'] = $scripts;

		// Give the screenshot proper path information.
		if (!empty($themes[$key]->info['screenshot'])) {
			$themes[$key]->info['screenshot'] = dirname($themes[$key]->filename) .'/'. $themes[$key]->info['screenshot'];
		}
	}

	foreach ($sub_themes as $key) {
		$themes[$key]->base_themes = system_find_base_themes($themes, $key);
		// Don't proceed if there was a problem with the root base theme.
		if (!current($themes[$key]->base_themes)) {
			continue;
		}
		$base_key = key($themes[$key]->base_themes);
		foreach (array_keys($themes[$key]->base_themes) as $base_theme) {
			$themes[$base_theme]->sub_themes[$key] = $themes[$key]->info['name'];
		}
		// Copy the 'owner' and 'engine' over if the top level theme uses a
		// theme engine.
		if (isset($themes[$base_key]->owner)) {
			if (isset($themes[$base_key]->info['engine'])) {
				$themes[$key]->info['engine'] = $themes[$base_key]->info['engine'];
				$themes[$key]->owner = $themes[$base_key]->owner;
				$themes[$key]->prefix = $themes[$base_key]->prefix;
			}
			else {
				$themes[$key]->prefix = $key;
			}
		}
	}

	// Extract current files from database.
	system_get_files_database($themes, 'theme');
	db_query("DELETE FROM {system} WHERE type = 'theme'");
	foreach ($themes as $theme) {
		$theme->owner = !isset($theme->owner) ? '' : $theme->owner;
		db_query("INSERT INTO {system} (name, owner, info, type, filename, status, throttle, bootstrap) VALUES ('%s', '%s', '%s', '%s', '%s', %d, %d, %d)", $theme->name, $theme->owner, serialize($theme->info), 'theme', $theme->filename, isset($theme->status) ? $theme->status : 0, 0, 0);
	}
}