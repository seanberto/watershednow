<?php

/**
 * Generate Watershed Dashboard page
 */
function wn_admin_dashboard() {
  $output .= _wn_admin_quicklinks();
  return $output;
}

/**
 * Build table with settings links. Note that module adds some nice styles and javascript sorting.
 * Then build a list of blocks that an admin can edit.
 * @TODO - Manage with menu callbacks to respect permissions for each link.
 */
function _wn_admin_quicklinks() {
  $links = array();

  $link = new stdClass;
  $link->title = 'Manage users';
  $link->topic = 'Users';
  $link->path = 'admin/users/user';
  $link->description = 'Manage user accounts. (Note: Must have web admin role.)';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Manage content';
  $link->topic = 'Content';
  $link->path = 'admin/content/node';
  $link->description = 'Add/edit/delete content (pages, updates, campaigns, etc).';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Primary links';
  $link->topic = 'Menus';
  $link->path = 'admin/build/menu-customize/primary-links';
  $link->description = 'Add/edit/delete menu items in the primary links.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Secondary links';
  $link->topic = 'Menus';
  $link->path = 'admin/build/menu-customize/secondary-links';
  $link->description = 'Add/edit/delete menu items in the secondary links.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Social media links';
  $link->topic = 'Menus';
  $link->path = 'admin/build/follow';
  $link->description = 'Add/edit/delete "Follow us" social media links.';
  $links[$link->title] = $link;

  $link = new stdClass;
  $link->title = 'Manage blog comments';
  $link->topic = 'Comments';
  $link->path = 'admin/content/comment';
  $link->description = 'Review/delete comments written on your blog.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Manage tags';
  $link->topic = 'Taxonomy';
  $link->path = 'admin/content/taxonomy';
  $link->description = 'Add/edit/delete taxonomy terms - in particular, map point tags.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Take Action results';
  $link->topic = 'Surveys';
  $link->path = 'admin/content/webform';
  $link->description = 'View, manage, download "Take Action" form responses.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Donation campaign results';
  $link->topic = 'Donations';
  $link->path = 'admin/content/donations';
  $link->description = 'View and download donation data.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Administer sign-ups';
  $link->topic = 'Sign-ups';
  $link->path = 'admin/content/signup';
  $link->description = 'Administer sign-ups to your events.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Paypal configuration';
  $link->topic = 'Donations';
  $link->path = 'admin/settings/simple-payments/paypal';
  $link->description = 'Manage your paypal account settings.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Newsletter signup box';
  $link->topic = 'Newsletter';
  $link->path = 'admin/settings/site-information';
  $link->description = 'Edit the newsletter signup box';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Contact form settings';
  $link->topic = 'Contact';
  $link->path = 'admin/build/contact';
  $link->description = 'Manage contact form settings.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Map data';
  $link->topic = 'Mapping';
  $link->path = 'admin/content/map_data';
  $link->description = 'Manage all map data.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Manage map icons';
  $link->topic = 'Mapping';
  $link->path = 'admin/content/taxonomy/taxonomy_image';
  $link->description = 'Add/edit/delete Map point icons.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Map settings';
  $link->topic = 'Mapping';
  $link->path = 'admin/build/openlayers/presets';
  $link->description = 'Administer the map configurations. <b>(advanced)</b>';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Theme settings';
  $link->topic = 'Theme';
  $link->path = 'admin/build/themes';
  $link->description = 'Switch themes; change theme settings.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Site information';
  $link->topic = 'Site config';
  $link->path = 'admin/settings/site-information';
  $link->description = 'Manage your general site settings, such as the default email address and website name.';
  $links[$link->title] = $link;
  
  $link = new stdClass;
  $link->title = 'Google analytics configuration';
  $link->topic = 'Google analytics';
  $link->path = 'admin/settings/googleanalytics';
  $link->description = 'Manage your Google Analytics account information.';
  $links[$link->title] = $link;
  
  $rows = array();
  foreach ($links as $link) {
    $rows[] = array($link->topic, l($link->title, $link->path), $link->description);
  }

  // Grab the rows created above and build the table of quick links. Add classes for javascript sorting.
  $header = array(array('data' =>'Topic', 'class' => 'sort-alpha'), array('data' => 'Task', 'class' => 'sort-alpha'), 'Description');
  $output = '<h2>Quick Links:</h2>';
  $output .= theme('table', $header, $rows, $attributes = array('class' => 'sortable'));
  $output = '<div class="quicklinks">' . $output . '</div>'; // Add some styles.
  
  // Build a list of blocks that the admin can edit. Display if user has "administer block" permission.
  // @FIXME - Really fragile adding blocks to this list. Must manually edit $module_whitelist and $info_blacklist.
  if (user_access('administer blocks')) {
    $blocks = _block_rehash(); // Grab an array of all blocks in the system.
    // Show blocks created by specific modules.
    $module_whitelist = array('wn_map', 'wn_events', 'wn_takeaction', 'wn_donations', 'wn_common', 'boxes', 'wn_blog');
    // Hide modules with specific descriptions.
    $info_blacklist = array('Sidebar content', 'Add custom box');
    $rows = array();
    foreach($blocks as $block) {
      if (in_array($block['module'], $module_whitelist) && !(in_array($block['info'], $info_blacklist))) {
        $rows[] = array(l($block['info'], 'admin/build/block/configure/' . $block['module'] . '/' . $block['delta']));        
      }
    }
    // Grab the rows created above and build the table of block edit links. Add classes for javascript sorting.
    $header = array(array('data' =>'Block', 'class' => 'sort-alpha'));
    $output2 = '<h2>Block Editing Links:</h2>';
    $output2 .= theme('table', $header, $rows, $attributes = array('class' => 'sortable'));
    $output2 = '<div class="blocklinks">' . $output2 . '</div>'; // Add some styles.
  }
  
  return $output . $output2;
}