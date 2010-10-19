<?php

/**
 * Now that we've installed a bunch of modules, let's capture some other config, create some roles,
 * set up some permissions, etc.
 *
 * @TODO: More ond more of what's stuff into this custom function is abstractable with Profiler module.
 * So, we need to remember to come back and prune sometimes.
 */
function _watershednow_configure() {
  
  // @FIXME - Set wysiwyg imagefield flag on appropriate content types.
  // This should be accomplishable via Features strongarm export. And if not there, in profiler.info.
  // But neither approach works for some reason. So, we call it in this last step of the install process.
  $has_assets = array('page', 'donation', 'take_action', 'update', 'event'); 
  foreach ($has_assets as $type) { // Set variable for types with image_assets field.
    variable_set('wysiwyg_imagefield_' . $type, 'field_image_assets');
  }


	// Leverage Install Profile API to create roles and perms.
	require_once('libraries/install_profile_api/core/user.inc');
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
	
	// Remove the default content type that Webform Module adds. It's cruft.
	node_type_delete('webform');

	// Set up basic contact form with Install Profile API
	require_once('libraries/install_profile_api/core/contact.inc');
	require_once('libraries/install_profile_api/core/menu.inc');
	$category = 'website feedback';
	$recipients = variable_get('site_mail', 'admin@'. $_SERVER['HTTP_HOST']); // Grab site email, if not create from URL.
	install_contact_add_category($category, $recipients, $reply = '', $weight = 0, $selected = 1);
	// Add contact form to secondary links.
	install_menu_create_menu_item('contact', 'Contact Us', 'Get in touch with us.', 'secondary_links');
	
	// @FIXME - For some reason, this value isn't being obeyed in wn_common's strongarm settings.
	// Set pathauto default for nodes. Do so before creating nodes below.
  variable_set('pathauto_node_pattern', '[title-raw]');

	// Create a few placeholder nodes and add them to the menu structure.
	// @TODO: Find the correct input filter. Right now, we rely on _watershednow_filters_wysiwyg() being called first..	
	require_once('libraries/install_profile_api/core/node.inc');
	require_once('libraries/install_profile_api/core/menu.inc');
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

  // Do some additional clean up.
	db_query("UPDATE {blocks} SET status = 0, region = ''"); // disable all DB blocks
	db_query("UPDATE {system} SET status = 0 WHERE type = 'theme' and name ='%s'", 'garland');
	db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", 'watershed');
	variable_set('admin_theme', 'seven'); // set the admin theme to Rubik
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