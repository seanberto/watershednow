<?php

/**
 * Implementation of hook_default_box().
 */
function wn_hp_main_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'homepage_promo';
  $box->plugin_key = 'simple';
  $box->title = 'Welcome to your new Watershed Now website';
  $box->description = 'Homepage promo';
  $box->options = array(
    'body' => '<p>This is your primary homepage callout, which you can edit by clicking the "Edit" link that you see when logged into the site as an admin, and hovering your mouse over this content area. The purpose of this homepage call out is to let people know what you do. You are encouraged to keep this callout fresh, though it\'s probably not content that you are going to change everyday. You homepage can also have a rotating image-based callout above - as well as pull in dynamic side bar listings of upcoming events and news.</p>',
    'format' => '3',
  );

  $export['homepage_promo'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'homepage_secondary';
  $box->plugin_key = 'simple';
  $box->title = 'Sidebar callout';
  $box->description = 'Homepage secondary callout';
  $box->options = array(
    'body' => '<p>This is a secondary callout on the homepage. If you don\'t need this callout, feel free to click the edit link, and then delete the title and body of this block content. You can always add this content back from the <a href="/admin/dashboard">Dashboard</a>.</p>',
    'format' => '3',
  );

  $export['homepage_secondary'] = $box;
  return $export;
}
