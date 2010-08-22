<?php

/**
 * Implementation of hook_strongarm().
 */
function drupalrivers_common_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'admin_theme';
  $strongarm->value = 'rubik';

  $export['admin_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'clean_url';
  $strongarm->value = '1';

  $export['clean_url'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_page';
  $strongarm->value = 0;

  $export['comment_page'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'configurable_timezones';
  $strongarm->value = '1';

  $export['configurable_timezones'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_default_timezone';
  $strongarm->value = '-25200';

  $export['date_default_timezone'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_first_day';
  $strongarm->value = '0';

  $export['date_first_day'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_long';
  $strongarm->value = 'l, F j, Y - g:ia';

  $export['date_format_long'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_long_custom';
  $strongarm->value = 'l, F j, Y - H:i';

  $export['date_format_long_custom'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_medium';
  $strongarm->value = 'D, m/d/Y - g:ia';

  $export['date_format_medium'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_medium_custom';
  $strongarm->value = 'D, m/d/Y - H:i';

  $export['date_format_medium_custom'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_short';
  $strongarm->value = 'm/d/Y - g:ia';

  $export['date_format_short'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_short_custom';
  $strongarm->value = 'm/d/Y - H:i';

  $export['date_format_short_custom'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'drupal_http_request_fails';
  $strongarm->value = FALSE;

  $export['drupal_http_request_fails'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'file_directory_temp';
  $strongarm->value = '/tmp';

  $export['file_directory_temp'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'filter_html_1';
  $strongarm->value = 1;

  $export['filter_html_1'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'image_toolkit';
  $strongarm->value = 'gd';

  $export['image_toolkit'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imageapi_image_toolkit';
  $strongarm->value = 'imageapi_gd';

  $export['imageapi_image_toolkit'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'jquery_update_replace';
  $strongarm->value = TRUE;

  $export['jquery_update_replace'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_confirm_email_at_registration';
  $strongarm->value = '0';

  $export['logintoboggan_confirm_email_at_registration'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_immediate_login_on_register';
  $strongarm->value = 1;

  $export['logintoboggan_immediate_login_on_register'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_login_successful_message';
  $strongarm->value = '0';

  $export['logintoboggan_login_successful_message'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_login_with_email';
  $strongarm->value = '1';

  $export['logintoboggan_login_with_email'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_minimum_password_length';
  $strongarm->value = '0';

  $export['logintoboggan_minimum_password_length'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_override_destination_parameter';
  $strongarm->value = 0;

  $export['logintoboggan_override_destination_parameter'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_pre_auth_role';
  $strongarm->value = '2';

  $export['logintoboggan_pre_auth_role'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_purge_unvalidated_user_interval';
  $strongarm->value = '0';

  $export['logintoboggan_purge_unvalidated_user_interval'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_redirect_on_confirm';
  $strongarm->value = '';

  $export['logintoboggan_redirect_on_confirm'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'logintoboggan_redirect_on_register';
  $strongarm->value = '';

  $export['logintoboggan_redirect_on_register'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_admin_theme';
  $strongarm->value = 0;

  $export['node_admin_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_page';
  $strongarm->value = array(
    0 => 'status',
  );

  $export['node_options_page'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_ignore_words';
  $strongarm->value = 'a,an,as,at,before,but,by,for,from,is,in,into,like,of,off,on,onto,per,since,than,the,this,that,to,up,via,with';

  $export['pathauto_ignore_words'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_indexaliases';
  $strongarm->value = FALSE;

  $export['pathauto_indexaliases'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_indexaliases_bulkupdate';
  $strongarm->value = FALSE;

  $export['pathauto_indexaliases_bulkupdate'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_max_component_length';
  $strongarm->value = '100';

  $export['pathauto_max_component_length'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_max_length';
  $strongarm->value = '100';

  $export['pathauto_max_length'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_modulelist';
  $strongarm->value = array(
    0 => 'node',
    1 => 'user',
    2 => 'taxonomy',
  );

  $export['pathauto_modulelist'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_applytofeeds';
  $strongarm->value = '';

  $export['pathauto_node_applytofeeds'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_bulkupdate';
  $strongarm->value = FALSE;

  $export['pathauto_node_bulkupdate'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_forum_pattern';
  $strongarm->value = '';

  $export['pathauto_node_forum_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_image_pattern';
  $strongarm->value = '';

  $export['pathauto_node_image_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_page_pattern';
  $strongarm->value = '';

  $export['pathauto_node_page_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_pattern';
  $strongarm->value = 'content/[title-raw]';

  $export['pathauto_node_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_story_pattern';
  $strongarm->value = '';

  $export['pathauto_node_story_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_punctuation_hyphen';
  $strongarm->value = 1;

  $export['pathauto_punctuation_hyphen'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_punctuation_quotes';
  $strongarm->value = 0;

  $export['pathauto_punctuation_quotes'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_separator';
  $strongarm->value = '-';

  $export['pathauto_separator'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_1_pattern';
  $strongarm->value = '';

  $export['pathauto_taxonomy_1_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_2_pattern';
  $strongarm->value = '';

  $export['pathauto_taxonomy_2_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_applytofeeds';
  $strongarm->value = FALSE;

  $export['pathauto_taxonomy_applytofeeds'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_bulkupdate';
  $strongarm->value = FALSE;

  $export['pathauto_taxonomy_bulkupdate'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_pattern';
  $strongarm->value = 'category/[vocab-raw]/[catpath-raw]';

  $export['pathauto_taxonomy_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_supportsfeeds';
  $strongarm->value = '0/feed';

  $export['pathauto_taxonomy_supportsfeeds'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_update_action';
  $strongarm->value = '2';

  $export['pathauto_update_action'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_user_bulkupdate';
  $strongarm->value = FALSE;

  $export['pathauto_user_bulkupdate'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_user_pattern';
  $strongarm->value = 'users/[user-raw]';

  $export['pathauto_user_pattern'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_user_supportsfeeds';
  $strongarm->value = '';

  $export['pathauto_user_supportsfeeds'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_verbose';
  $strongarm->value = FALSE;

  $export['pathauto_verbose'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_403';
  $strongarm->value = 'toboggan/denied';

  $export['site_403'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_default';
  $strongarm->value = 'watershed';

  $export['theme_default'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_rubik_settings';
  $strongarm->value = array(
    'toggle_logo' => 1,
    'toggle_name' => 1,
    'toggle_slogan' => 0,
    'toggle_mission' => 1,
    'toggle_node_user_picture' => 0,
    'toggle_comment_user_picture' => 0,
    'toggle_search' => 0,
    'toggle_favicon' => 1,
    'toggle_primary_links' => 1,
    'toggle_secondary_links' => 1,
    'default_logo' => 1,
    'logo_path' => '',
    'logo_upload' => '',
    'default_favicon' => 1,
    'favicon_path' => '',
    'favicon_upload' => '',
  );

  $export['theme_rubik_settings'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_settings';
  $strongarm->value = array(
    'toggle_logo' => 1,
    'toggle_name' => 1,
    'toggle_slogan' => 0,
    'toggle_mission' => 0,
    'toggle_node_user_picture' => 0,
    'toggle_comment_user_picture' => 0,
    'toggle_search' => 0,
    'toggle_favicon' => 1,
    'toggle_primary_links' => 1,
    'toggle_secondary_links' => 1,
    'toggle_node_info_page' => 0,
    'default_logo' => 1,
    'logo_path' => '',
    'logo_upload' => '',
    'default_favicon' => 1,
    'favicon_path' => '',
    'favicon_upload' => '',
  );

  $export['theme_settings'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_watershed_settings';
  $strongarm->value = array(
    'watershed_zen_tabs' => 1,
    'watershed_wireframe' => 0,
    'watershed_block_editing' => 1,
    'watershed_rebuild_registry' => 0,
    'mission' => '',
    'default_logo' => 1,
    'logo_path' => '',
    'default_favicon' => 1,
    'favicon_path' => '',
    'primary_links' => 1,
    'secondary_links' => 1,
    'toggle_logo' => 1,
    'toggle_favicon' => 1,
    'toggle_name' => 1,
    'toggle_search' => 1,
    'toggle_slogan' => 0,
    'toggle_mission' => 0,
    'toggle_node_user_picture' => 0,
    'toggle_comment_user_picture' => 0,
    'toggle_primary_links' => 1,
    'toggle_secondary_links' => 1,
    'logo_upload' => '',
    'favicon_upload' => '',
  );

  $export['theme_watershed_settings'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_email_verification';
  $strongarm->value = TRUE;

  $export['user_email_verification'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'wysiwyg_imageupload_imgstyles';
  $strongarm->value = 'Spacing 20px Border=imgupl_styles_spacing_20_black_border\\n\\rBlack Border=imgupl_styles_black_border\\n\\rSpacing 5px=imgupl_styles_spacing_5\\n\\rSpacing 10px=imgupl_styles_spacing_10';

  $export['wysiwyg_imageupload_imgstyles'] = $strongarm;
  return $export;
}
