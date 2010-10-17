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
 */
function _wn_admin_quicklinks() {
  $links = array();
  
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
  $link->title = 'Manage tags';
  $link->topic = 'Taxonomy';
  $link->path = 'admin/content/taxonomy';
  $link->description = 'Add/edit/delete taxonomy terms - in particular, <b>Map point icons</b>.';
  $links['taxonomy'] = $link;
  
  $link = new stdClass;
  $link->title = 'Take Action results';
  $link->topic = 'Take Action';
  $link->path = 'admin/content/taxonomy';
  $link->description = 'View, manage, download "Take Action" form responses.';
  $links['take-action'] = $link;
  
  $link = new stdClass;
  $link->title = 'Contact form settings';
  $link->topic = 'Contact';
  $link->path = 'admin/build/contact';
  $link->description = 'Manage contact form settings.';
  $links['contact'] = $link;
  
  $link = new stdClass;
  $link->title = 'Theme settings';
  $link->topic = 'Theme';
  $link->path = 'admin/build/themes';
  $link->description = 'Switch themes; change theme settings.';
  $links['theme'] = $link;
  
  $link = new stdClass;
  $link->title = 'Site Information';
  $link->topic = 'Site config';
  $link->path = 'admin/settings/site-information';
  $link->description = 'Manage your general site settings, such as the default email address and website name.';
  $links['siteinfo'] = $link;
  
  $link = new stdClass;
  $link->title = 'Set timezone';
  $link->topic = 'Site config';
  $link->path = 'admin/settings/date-time';
  $link->description = 'Set the timezone for your website.';
  $links['timezone'] = $link;
  
  foreach ($links as $link) {
    $rows[] = array($link->topic, l($link->title, $link->path), $link->description);
  }

  // Grab the rows created above and build the table of quick links. Add classes for javascript sorting.
  $header = array(array('data' =>'Topic', 'class' => 'sort-alpha'), array('data' => 'Task', 'class' => 'sort-alpha'), 'Description');
  $output = '<h2>Quick Links:</h2>';
  $output .= theme('table', $header, $rows, $attributes = array('class' => 'sortable'));
  $output = '<div class="quicklinks">' . $output . '</div>'; // Add some styles.
  
  return $output;
}