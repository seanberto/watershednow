<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function watershednow_multimedia_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function watershednow_multimedia_node_info() {
  $items = array(
    'multimedia' => array(
      'name' => t('Multimedia asset'),
      'module' => 'features',
      'description' => t('<em>Multimedia assets</em> contain images, videos, text, links, and other content that is used to generate slideshows and other galleries. How multimedia assets are used on a site changes frequently. This content type will inevitably morph as part of the development needs of a particular organization.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => t('Multimedia assets are videos, images, and other dynamic content used to create galleries.'),
    ),
  );
  return $items;
}
