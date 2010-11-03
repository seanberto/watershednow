<?php
require_once('libraries/install_profile_api/core/user.inc');
require_once('libraries/install_profile_api/core/contact.inc');
require_once('libraries/install_profile_api/core/menu.inc');
require_once('libraries/install_profile_api/core/node.inc');

/**
 * Now that we've installed a bunch of modules, let's capture some other config, create some roles,
 * set up some permissions, etc.
 *
 * @TODO: More ond more of what's stuff into this custom function is abstractable with Profiler module.
 * So, we need to remember to come back and prune sometimes.
 */
function _watershednow_configure( $config ) {
  // @FIXME - Set wysiwyg imagefield flag on appropriate content types.
  // This should be accomplishable via Features strongarm export. And if not there, in profiler.info.
  // But neither approach works for some reason. So, we call it in this last step of the install process.
  $has_assets = array('page', 'donation', 'take_action', 'update', 'event', 'map_feature');
  foreach ($has_assets as $type) { // Set variable for types with image_assets field.
    variable_set('wysiwyg_imagefield_' . $type, 'field_image_assets');
  }

  _watershednow_configure_roles();

  // Configure Input formats and WYSIWYG editor
  _watershednow_filters_wysiwyg();

  // Remove the default content type that Webform Module adds. It's cruft.
  node_type_delete('webform');

  _watershednow_create_nodes(); // Call after setting up filters.

  _watershednow_create_menu_items();

  _watershednow_create_follow_links( $config['follow'] );

  // Do some additional clean up.
  db_query("UPDATE {blocks} SET status = 0, region = ''"); // disable all DB blocks
  db_query("UPDATE {system} SET status = 0 WHERE type = 'theme' and name ='%s'", 'garland');
  variable_set('admin_theme', 'seven'); // set the admin theme to Rubik
  variable_set('node_admin_theme', 1); // Use the admin theme for content editing
}

/**
 * Follow Links
 *
 * This function is used to parse the follow components in the install profile
 * info file.
 *
 * We chose not to use another module, to cut down on the # of modules.
 *
 * The syntax in the info file looks like the following:
 * follow[uid][network] = url
 * 0 for uid is sitewide
 *
 * Declare in the order you want them presented.
 *
 * @param $follow_links array of follow links to be saved.
 * array($uid  => array($network => $url))
 */
function _watershednow_create_follow_links($follow_links) {
  foreach($follow_links as $uid => $links ) {
    $weight = 0;
    foreach($links as $name => $url ) {
      $follow_link = array(
        'uid'  => $uid,
        'url' => $url,
        'name' => $name,
        'weight' => $weight++
      );
      $follow_link = (object) $follow_link;
      follow_link_save( $follow_link );
    }
  }
}

function _watershednow_create_menu_items() {
  menu_rebuild();

  // Create a homepage link in the primary navigation
  install_menu_create_menu_item('<front>', 'Home', 'Return to the homepage.', 'primary_links', '0', '-10');

  // Set up basic contact form with Install Profile API
  $recipients = variable_get('site_mail', 'admin@'. $_SERVER['HTTP_HOST']); // Grab site email, if not create from URL.
  install_contact_add_category('website feedback', $recipients, $reply = '', $weight = 0, $selected = 1);
  // Add contact form to primary links.
  install_menu_create_menu_item('contact', 'Contact Us', 'Get in touch with us.', 'primary_links', '0', '50');

  $secondary_links = array(
    'about-us'    => 'About Us',
    'map'     => 'Explore',
    'updates'     => 'Blog',
    'events'      => 'Events',
    'take-action' => 'Take Action',
    'donate'      => 'Donate',
    'contact'     => 'Contact'
  );

  $weight = 0;
  foreach( $secondary_links as $path => $title ) {
    install_menu_create_menu_item($path, $title, NULL, 'secondary_links', '0', $weight++);
  }
}

function _watershednow_create_nodes() {
  // @FIXME - Strongarm settings don't seem to be picked up until after the install process runs.
  // Set pathauto default for nodes. Do so before creating nodes below.
  variable_set('pathauto_node_pattern', '[title-raw]');
  // While at it, set pathauto default for taxonomy terms.
  variable_set('pathauto_taxonomy_pattern', '[vocab-raw]/[catpath-raw]');

  // Create a few placeholder nodes and add them to the menu structure.
  // @TODO: Find the correct input filter. Right now, we rely on _watershednow_filters_wysiwyg() being called first..
  $pages = array();

  $page = new stdClass;
  $page->title = 'About Us';
  $page->body = 'This is a placeholder "about us" page. Here readers can learn more about this website and the orgaization behind it. When you are logged into the site as an administrator, you will see an "edit" link below the title of this page. Follow that link to edit/delete this page content. You can easily embed images, videos, forms and other rich HTML. Or, you can simply use our "WYSIWYG" (What you see is what you get) editor to edit this page like you would the body of an email with Google or Yahoo mail. After you get to know the basics, there are many additional content management features that you are sure to enjoy - such as the ability to create menu links to specific pages and the ability to add sidebar content per page.';
  $page->format = 1;
  $page->menu = 'primary_links';
  $page->menuitem_description = 'Learn more about us';
  $page->parent_title = NULL;
  $pages['about'] = $page;

  $page = new stdClass;
  $page->title = 'Staff';
  $page->body = 'This is a placeholder page about "our staff." It\'s provided here to show how subpages work. When you are logged into the site as an administrator, you will see an "edit" link below the title of this page. Follow that link to edit/delete this page content. You can easily embed images, videos, forms and other rich HTML. Or, you can simply use our "WYSIWYG" (What you see is what you get) editor to edit this page like you would the body of an email with Google or Yahoo mail. After you get to know the basics, there are many additional content management features that you are sure to enjoy - such as the ability to create menu links to specific pages and the ability to add sidebar content per page.';
  $page->format = 1;
  $page->menu = 'primary_links';
  $page->menuitem_description = 'Get to know our staff. Feel free to rename or even delete this page. It\'s provided here to show how subpages work.';
  $page->parent_title = 'About Us';
  $pages['staff'] = $page;

  $page = new stdClass;
  $page->title = 'History';
  $page->body = 'This is a placeholder page about our "organizational history." It\'s provided here to show how subpages work. When you are logged into the site as an administrator, you will see an "edit" link below the title of this page. Follow that link to edit/delete this page content. You can easily embed images, videos, forms and other rich HTML. Or, you can simply use our "WYSIWYG" (What you see is what you get) editor to edit this page like you would the body of an email with Google or Yahoo mail. After you get to know the basics, there are many additional content management features that you are sure to enjoy - such as the ability to create menu links to specific pages and the ability to add sidebar content per page.';
  $page->format = 1;
  $page->menu = 'primary_links';
  $page->menuitem_description = 'Get to know our history.';
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
}

/*
 * Defining default permissions for all roles.
 */
function _watershednow_define_perms() {
  $default_perms = array();

  $default_perms['anonymous'] = array(
    'access comments',
    'post comments',
    'post comments without approval',
    'subscribe to comments',
    'access site-wide contact form',
    'access content',
    'search content',
    'access taxonomy images',
    'change own username',
    'sign up for content'
    );

  $default_perms['authenticated'] = array(
    'access comments',
    'post comments',
    'post comments without approval',
    'subscribe to comments',
    'access site-wide contact form',
    'access content',
    'search content',
    'access taxonomy images',
    'change own username',
    'sign up for content',
    'edit own signups',
    'cancel own signups'
    );

  $default_perms['support'] = array(
    'access wn dashboard',
    'access administration menu',
    'access donation reports',
    'view advanced help index',
    'view advanced help popup',
    'view advanced help topic',
    'collapse format fieldset by default',
    'collapsible format selection',
    'show format selection for blocks',
    'show format selection for comments',
    'show format selection for nodes',
    'show more format tips link',
    'administer blocks',
    'administer comments',
    'administer comment notify',
    'administer site-wide contact form',
    'administer votesmart settings',
    'Allow Reordering',
    'create donation content',
    'create event content',
    'create map_feature content',
    'create map_kml content',
    'create page content',
    'create take_action content',
    'create update content',
    'create update_feed content',
    'create wn_hp_callout content',
    'delete any donation content',
    'delete any event content',
    'delete any map_feature content',
    'delete any map_kml content',
    'delete any page content',
    'delete any take_action content',
    'delete any update content',
    'delete any update_feed content',
    'delete any wn_hp_callout content',
    'delete own donation content',
    'delete own event content',
    'delete own map_feature content',
    'delete own map_kml content',
    'delete own page content',
    'delete own take_action content',
    'delete own update content',
    'delete own update_feed content',
    'delete own wn_hp_callout content',
    'edit any donation content',
    'edit any event content',
    'edit any map_feature content',
    'edit any map_kml content',
    'edit any page content',
    'edit any take_action content',
    'edit any update content',
    'edit any update_feed content',
    'edit any wn_hp_callout content',
    'edit own donation content',
    'edit own event content',
    'edit own map_feature content',
    'edit own map_kml content',
    'edit own page content',
    'edit own take_action content',
    'edit own update content',
    'edit own update_feed content',
    'edit own wn_hp_callout content',
    'administer feeds',
    'clear update_feed feeds',
    'import update_feed feeds',
    'administer filters',
    'administer follow',
    'change follow link titles',
    'edit site follow links',
    'administer google analytics',
    'administer menu',
    'administer content types',
    'administer nodes',
    'delete revisions',
    'revert revisions',
    'view revisions',
    'administer openlayers',
    'administer url aliases',
    'create url aliases',
    'administer pathauto',
    'assign all roles',
    'assign staff role',
    'assign support role',
    'assign web admin role',
    'use advanced search',
    'administer all signups',
    'administer signups for own content',
    'cancel own signups',
    'cancel signups',
    'edit own signups',
    'email all signed up users',
    'email users signed up for own content',
    'sign up for content',
    'view all signups',
    'administer simple payments',
    'access administration pages',
    'access site reports',
    'administer site configuration',
    'select different theme',
    'administer taxonomy',
    'administer taxonomy images',
    'access user profiles',
    'administer permissions',
    'administer users',
    'access all webform results',
    'access own webform results',
    'access own webform submissions',
    'delete all webform submissions',
    'delete own webform submissions',
    'edit all webform submissions',
    'edit own webform submissions'
    );

  $default_perms['staff'] = array(
    'access wn dashboard',
    'access administration menu',
    'access donation reports',
    'collapse format fieldset by default',
    'collapsible format selection',
    'show more format tips link',
    'administer blocks',
    'administer comments',
    'Allow Reordering',
    'create donation content',
    'create event content',
    'create map_feature content',
    'create map_kml content',
    'create page content',
    'create take_action content',
    'create update content',
    'create update_feed content',
    'create wn_hp_callout content',
    'delete any donation content',
    'delete any event content',
    'delete any map_feature content',
    'delete any map_kml content',
    'delete any page content',
    'delete any take_action content',
    'delete any update content',
    'delete any update_feed content',
    'delete any wn_hp_callout content',
    'delete own donation content',
    'delete own event content',
    'delete own map_feature content',
    'delete own map_kml content',
    'delete own page content',
    'delete own take_action content',
    'delete own update content',
    'delete own update_feed content',
    'delete own wn_hp_callout content',
    'edit any donation content',
    'edit any event content',
    'edit any map_feature content',
    'edit any map_kml content',
    'edit any page content',
    'edit any take_action content',
    'edit any update content',
    'edit any update_feed content',
    'edit any wn_hp_callout content',
    'edit own donation content',
    'edit own event content',
    'edit own map_feature content',
    'edit own map_kml content',
    'edit own page content',
    'edit own take_action content',
    'edit own update content',
    'edit own update_feed content',
    'edit own wn_hp_callout content',
    'import update_feed feeds',
    'administer menu',
    'delete revisions',
    'revert revisions',
    'view revisions',
    'administer url aliases',
    'create url aliases',
    'use advanced search',
    'administer all signups',
    'administer signups for own content',
    'cancel own signups',
    'cancel signups',
    'edit own signups',
    'email all signed up users',
    'email users signed up for own content',
    'sign up for content',
    'view all signups',
    'access administration pages',
    'access user profiles',
    'access all webform results',
    'access own webform results',
    'access own webform submissions',
    'delete all webform submissions',
    'delete own webform submissions',
    'edit all webform submissions',
    'edit own webform submissions'
    );

  $default_perms['web_admin'] = array(
    'access wn dashboard',
    'access administration menu',
    'access donation reports',
    'collapse format fieldset by default',
    'collapsible format selection',
    'show more format tips link',
    'administer blocks',
    'administer comments',
    'administer votesmart settings',
    'administer site-wide contact form',
    'Allow Reordering',
    'create donation content',
    'create event content',
    'create map_feature content',
    'create map_kml content',
    'create page content',
    'create take_action content',
    'create update content',
    'create update_feed content',
    'create wn_hp_callout content',
    'delete any donation content',
    'delete any event content',
    'delete any map_feature content',
    'delete any map_kml content',
    'delete any page content',
    'delete any take_action content',
    'delete any update content',
    'delete any update_feed content',
    'delete any wn_hp_callout content',
    'delete own donation content',
    'delete own event content',
    'delete own map_feature content',
    'delete own map_kml content',
    'delete own page content',
    'delete own take_action content',
    'delete own update content',
    'delete own update_feed content',
    'delete own wn_hp_callout content',
    'edit any donation content',
    'edit any event content',
    'edit any map_feature content',
    'edit any map_kml content',
    'edit any page content',
    'edit any take_action content',
    'edit any update content',
    'edit any update_feed content',
    'edit any wn_hp_callout content',
    'edit own donation content',
    'edit own event content',
    'edit own map_feature content',
    'edit own map_kml content',
    'edit own page content',
    'edit own take_action content',
    'edit own update content',
    'edit own update_feed content',
    'edit own wn_hp_callout content',
    'import update_feed feeds',
    'administer follow',
    'change follow link titles',
    'edit site follow links',
    'administer google analytics',
    'administer menu',
    'delete revisions',
    'revert revisions',
    'view revisions',
    'administer openlayers',
    'administer url aliases',
    'create url aliases',
    'assign staff role',
    'assign web admin role',
    'use advanced search',
    'administer all signups',
    'administer signups for own content',
    'cancel own signups',
    'cancel signups',
    'edit own signups',
    'email all signed up users',
    'email users signed up for own content',
    'sign up for content',
    'view all signups',
    'administer simple payments',
    'access administration pages',
    'access site reports',
    'administer site configuration',
    'select different theme',
    'administer taxonomy',
    'administer taxonomy images',
    'access user profiles',
    'administer users',
    'access all webform results',
    'access own webform results',
    'access own webform submissions',
    'delete all webform submissions',
    'delete own webform submissions',
    'edit all webform submissions',
    'edit own webform submissions'
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
 * @FIXME: This is all really fragile. Roles are hardcoded...
 */
function _watershednow_filters_wysiwyg() {

  // Setting up Filter Formats
  db_query("DELETE FROM {filter_formats}");
  db_query("INSERT INTO {filter_formats} (`format`,`name`,`roles`,`cache`)
  VALUES
    (1,'Filtered HTML',',1,2,',1),
    (2,'Full HTML - Used for editing node content',',3,4,5,',1),
    (3,'Block HTML - Special formatter for Blocks. Same as Full HTML. (Needed b/c using a different WYSIWYG editor)',',3,4,5,',1)"
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
    (4,'block',3,1,-24),
    (5,'node',2,1,-26),
    (5,'comment',2,1,-26),
    (5,'block',3,1,-26)"
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
    'settings' => array(
      'default' => 1,
      'user_choose' => 0,
      'show_toggle' => 1,
      'theme' => 'advanced',
      'language' => 'en',
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Underline' => 1,
          'Strike' => 1,
          'JustifyLeft' => 1,
          'JustifyCenter' => 1,
          'JustifyRight' => 1,
          'JustifyBlock' => 1,
          'BulletedList' => 1,
          'NumberedList' => 1,
          'Outdent' => 1,
          'Indent' => 1,
          'Undo' => 1,
          'Redo' => 1,
          'Link' => 1,
          'Unlink' => 1,
          'Anchor' => 1,
          'Image' => 1,
          'Superscript' => 1,
          'Subscript' => 1,
          'RemoveFormat' => 1,
          'Format' => 1,
          'Table' => 1,
        ),
        'drupal' => array(
          'wysiwyg_imagefield' => 1,
          ),
        ),
      'toolbar_loc' => 'top',
      'toolbar_align' => 'left',
      'path_loc' => 'bottom',
      'resizing' => 1,
      'verify_html' => 1,
      'preformatted' => 0,
      'convert_fonts_to_spans' => 1,
      'remove_linebreaks' => 1,
      'apply_source_formatting' => 0,
      'paste_auto_cleanup_on_paste' => 0,
      'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
      'css_setting' => 'default',
      'css_path' => '',
      'css_classes' => '',
      )
    );

  $wysiwyg_profiles['3'] = array(
    'format' => 3,
    'editor' => 'ckeditor',
    'settings' => array(
      'default' => 0,
      'user_choose' => 0,
      'show_toggle' => 1,
      'theme' => 'advanced',
      'language' => 'en',
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Underline' => 1,
          'JustifyLeft' => 1,
          'JustifyCenter' => 1,
          'JustifyRight' => 1,
          'Link' => 1,
          'Unlink' => 1,
          'Format' => 1,
          ),
        ),
      'toolbar_loc' => 'top',
      'toolbar_align' => 'left',
      'path_loc' => 'bottom',
      'resizing' => 1,
      'verify_html' => 1,
      'preformatted' => 0,
      'convert_fonts_to_spans' => 1,
      'remove_linebreaks' => 1,
      'apply_source_formatting' => 0,
      'paste_auto_cleanup_on_paste' => 0,
      'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
      'css_setting' => 'default',
      'css_path' => '',
      'css_classes' => '',
      )
    );

  db_query("DELETE FROM {wysiwyg}");
  foreach ($wysiwyg_profiles as $profile) {
    db_query("INSERT INTO {wysiwyg} (`format`,`editor`,`settings`) VALUES (%d, '%s', '%s')", $profile['format'], $profile['editor'], serialize($profile['settings']));
  };
}

/**
 * Helper function to configure roles.
 */

function _watershednow_configure_roles() {
  // Leverage Install Profile API to create perms and ad to roles.
  $default_roles = user_roles(); // getting roles and perms.
  $default_perms = _watershednow_define_perms(); // Grabbing array of permissions.
  foreach ($default_roles as $rid => $role) {
    switch( $role ) {
      case 'support':
        install_add_permissions($rid, $default_perms['support']);
        break;
      case 'staff':
        install_add_permissions($rid, $default_perms['staff']);
        break;
      case 'web admin':
        install_add_permissions($rid, $default_perms['web_admin']);
        break;
    }
  }
  install_add_permissions('1', $default_perms['anonymous']);
  install_add_permissions('2', $default_perms['authenticated']);

  // Now remove some crufy permissions
  install_remove_permissions('1', $default_perms['to_remove']);
  install_remove_permissions('2', $default_perms['to_remove']);
}

/**
 * Installing themes is a PITA. It's easy to enable themes, but setting their variables is not straight forward.
 * @TODO - There's gotta be a better way to set these variables.
 */
function _watershednow_install_themes( $config ) {
  $config = $config['theme_config'];

  $all_themes = system_theme_data();
  $wn_themes = array(
    'watershed' => $all_themes['watershed'],
    'wn_neuse' =>$all_themes['wn_neuse'],
    'wn_rogue' => $all_themes['wn_rogue'],
    'wn_whiteoak' => $all_themes['wn_whiteoak']
    );
  $defaults = theme_get_settings(); // Get site's default settings as an array.
  foreach($wn_themes as $current_theme => $theme) {
    $settings = $theme->info['settings'];

    if( is_array($config[$current_theme]) ) {
      foreach( $config[$current_theme] as $setting => $value ) {
        $settings[$current_theme . '_' . $setting] = $value;
      }
    }
    foreach($theme->info['features'] as $feature) {
      $settings['toggle_' . $feature] = 1;
    }
    $settings = array_merge($defaults, $settings);
    variable_set('theme_' . $current_theme . '_settings', $settings);
    db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", $current_theme);
  }
}

/**
 * Helper module to create a Update Feed node on install. This function creates a new node, and then
 * runs it's import process to create additional update node(s).
 *
 * NOTE: Would prefer to use node_save() instead of drupal_execute(), but node_save() doesn't create
 * feeds job appropriately.
 *
 * For more information on creating complex nodes via drupal_execute, see:
 * http://thedrupalblog.com/programmatically-create-any-node-type-using-drupal-execute
 */
function _watershednow_add_sample_update_feed($feeds) {
  foreach ($feeds as $feed) {
    // Create a new node. Note: we don't need to pass a node title b/c Feeds module looks it up for us.
    $form_state = array();
    module_load_include('inc', 'node', 'node.pages'); // Process node form submit.
    module_load_include('inc', 'node', 'feeds.pages'); // Needed to run node through Feed module's secret sauce.
    $node = array('type' => 'update_feed'); // Our targetted content type.
    $form_state['values']['feeds'] = array('FeedsHTTPFetcher' => array('source' => $feed)); // The RSS feed we want to add.
    $form_state['values']['name'] = 'admin'; // Node author.
    $form_state['values']['op'] = t('Save');
    drupal_execute('update_feed_node_form', $form_state, (object)$node);
  }
}

/**
 * Helper function for adding homepage callout node. Note, needs to be called after flushing caches.
 */
function _watershednow_add_callouts() {
  // Create a default callout node.
  // @FIXME - This should really be part of the install profile, but for some reason isn't adding the imagefields
  // correctly.
  $node = new stdClass;
  $node->title = 'Example Callout';
  $node->type = 'wn_hp_callout';
  // Search examples directory to attach some images.
  $callouts_dir = drupal_get_path('module', 'wn_hp_callout').'/imgs/examples/';
  $callout_imgs = glob($callouts_dir.'*.{jpg,jpeg,png,gif}',GLOB_BRACE);
  // Now add the images and provide imagefield extended additional text.
  foreach($callout_imgs as $img) {
    $img_info = pathinfo($img);
    $field = field_file_save_file($img, array(), file_directory_path() .'/example_callouts/');
    if( !isset($field['data']) ) {
      $field['data'] = array();
    }
    $field['data']['title'] = ucwords(str_replace('_',' ',$img_info['filename']));
    $field['data']['alt'] = 'This is alt text.';
    $field['data']['wn_hp_callout_desc'] = array(
      'body' => 'This is text associated with this homepage callout image.',
      'format' => '1',
      'style' => 'formatted'
    );
    $node->field_wn_hp_callout_image[] = $field;
   }
   $node = node_submit($node);
   node_save($node);
}