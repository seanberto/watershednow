<?php
// $Id: media_youtube.media_mover.inc,v 1.1.2.4 2010/05/12 20:56:21 aaron Exp $

/**
 * @file
 * Functions to implement Media Mover behavior for Media: YouTube.
 */

/**
 * Upload a video to YouTube through Media Mover.
 */
function media_youtube_upload_video($file, $configuration) {
  _media_youtube_set_include_path();

  $path = media_youtube_zend_path();
  Zend_Loader::loadClass('Zend_Gdata_ClientLogin', $path);
  Zend_Loader::loadClass('Zend_Gdata_YouTube', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_Exception', $path);
  Zend_Loader::loadClass('Zend_Gdata_App_HttpException', $path);

  $title = check_plain($configuration['media_youtube_default_title']);
  $description = check_plain($configuration['media_youtube_default_description']);
  $tags = '';
  $node = NULL;

  if ($file['nid']) {
    $node = node_load($file['nid']);
    if ($node) {
      $title = check_plain($node->title);

      $description_field = $configuration['media_youtube_description_field'];

      if ($configuration['media_youtube_description_field'] == 'body') {
        $clean_description = check_plain($node->body);
      }
      else if ($node->{$configuration['media_youtube_description_field']}) {
        $clean_description = check_plain($node->{$configuration['media_youtube_description_field']}[0]['value']);
      }
      if ($clean_description) {
        $description = $clean_description;
      }

      $tags = array();
      if ($node->taxonomy) {
        foreach ($node->taxonomy as $term) {
          if ($configuration['media_youtube_vocabs'][$term->vid]) {
            $ntags = explode(' ', $term->name);
            $tags = array_merge($tags, $ntags);
          }
        }
      }
      $tags = array_filter($tags, '_media_youtube_filter_tags');
      $tags = check_plain(implode(', ', $tags));
    }
    else {
      watchdog('media_youtube', 'File !file belongs to node !nid which does not exist anymore.', array('!file' => $filepath, '!nid' => $file['nid']), WATCHDOG_ERROR);
    }
  }
  if ($tags == '') {
    $tags = check_plain($configuration['media_youtube_default_tags']);
  }

  $username = media_youtube_variable_get('youtube_username');
  $password = media_youtube_variable_get('youtube_password');
  $devkey = media_youtube_variable_get('api_key');

  $category = check_plain($configuration['media_youtube_category']);

  $filepath = media_mover_api_config_current_file($file);
  $fileinfo = pathinfo($filepath);

  try {
    $httpclient = Zend_Gdata_ClientLogin::getHttpClient($username, $password, 'youtube', NULL, MEDIA_YOUTUBE_APP_ID, NULL, NULL, MEDIA_YOUTUBE_AUTH_URL);
  }
  catch (Exception $e) {
    watchdog('media_youtube', 'Cannot authenticate. Error: @error', array('@error' => strip_tags($e->getMessage())), WATCHDOG_ERROR);
    return FALSE;
  }

  $youtube = new Zend_Gdata_YouTube($httpclient, MEDIA_YOUTUBE_APP_ID, NULL, $devkey);
  $upload = new Zend_Gdata_YouTube_VideoEntry();

  $filesource = $youtube->newMediaFileSource($filepath);
  $filesource->setContentType('video/avi');
  $filesource->setSlug($filepath);

  $upload->setMediaSource($filesource);
  $upload->setVideoTitle($title);
  $upload->setVideoDescription($description);
  $upload->setVideoCategory($category);
  $upload->setVideoTags($tags);

  try {
    $video = $youtube->insertEntry($upload, MEDIA_YOUTUBE_UPLOAD_URL, 'Zend_Gdata_YouTube_VideoEntry');
    $url = media_youtube_video_url($video->getVideoId());
    watchdog('media_youtube', 'Uploaded file !file to Youtube: !video', array('!file' => $fileinfo['basename'], '!video' => l($url, $url)), WATCHDOG_NOTICE, l($title, 'http://www.youtube.com/watch?v='. $story_id, array('absolute' => TRUE)));
    return $url;
  }
  catch (Zend_Gdata_App_HttpException $httpException) {
    watchdog('media_youtube', 'Upload failed. Error: @error. Exception: @exception.', array('@error' => strip_tags($httpException->getMessage()), '@exception' => strip_tags($httpException->getRawResponseBody())), WATCHDOG_ERROR, $node ? l($node->title, 'node/' . $node->nid) : NULL);
    return FALSE;
  }
  catch (Zend_Gdata_App_Exception $e) {
    watchdog('media_youtube', print_r($file, TRUE));
    watchdog('media_youtube', 'Upload failed. Error: @error.', array('@error' => strip_tags($e->getMessage())), WATCHDOG_ERROR, $node ? l($node->title, 'node/' . $node->nid) : NULL);
    return FALSE;
  }
}

/**
 * Media Mover configuration form element for Media: YouTube.
 *
 * @see media_youtube_validate_configuration().
 */
function media_youtube_config($configuration) {
  $youtube_username = isset($configuration['media_youtube_username']) ? $configuration['media_youtube_username'] : media_youtube_variable_get('youtube_username');
  $youtube_password = isset($configuration['media_youtube_password']) ? $configuration['media_youtube_password'] : media_youtube_variable_get('youtube_password');

  $form['media_youtube_conf'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload to Youtube configuration'),
    '#element_validate' => array('media_youtube_validate_configuration', array('media_youtube_conf')),
  );

//   $form['media_youtube_conf']['media_youtube_username'] = array(
//     '#title' => t('Username'),
//     '#type' => 'textfield',
//     '#default_value' => $youtube_username,
//     '#description' => t('Your Youtube username.'),
//   );
//
//   $form['media_youtube_conf']['media_youtube_password'] = array(
//     '#title' => t('Password'),
//     '#type' => 'password',
//     '#default_value' => $youtube_password,
//     '#description' => t('Your Youtube password.'),
//   );
//
//   $form['media_youtube_conf']['media_youtube_key'] = array(
//     '#title' => t('Developer Key'),
//     '#type' => 'textfield',
//     '#default_value' => $configuration['media_youtube_key'] ? $configuration['media_youtube_key'] : media_youtube_variable_get('api_key'),
//     '#description' => t('Your Youtube developer key.'),
//   );

  $form['media_youtube_conf']['media_youtube_category'] = array(
    '#title' => t('Category'),
    '#type' => 'textfield',
    '#default_value' => $configuration['media_youtube_category'] ? $configuration['media_youtube_category'] : '',
    '#description' => t('The category where videos will be added. This has to be a Youtube category.'),
  );

  $form['media_youtube_conf']['media_youtube_default_title'] = array(
    '#title' => t('Default title'),
    '#type' => 'textfield',
    '#default_value' => $configuration['media_youtube_default_title'] ? $configuration['media_youtube_default_title'] : '',
    '#description' => t('Videos which do not belong to a node will be given this title.'),
  );

  $options = array(
    'body' => t('<Body>'),
  );
  foreach (content_fields() as $field) {
    $options[$field['field_name']] = $field['widget']['label'];
  }
  $form['media_youtube_conf']['media_youtube_description_field'] = array(
    '#title' => t('Description field'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $configuration['media_youtube_description_field'] ? $configuration['media_youtube_description_field'] : 'body',
    '#description' => t('The video description will be pulled from this field.'),
  );

  $form['media_youtube_conf']['media_youtube_default_description'] = array(
    '#title' => t('Default description'),
    '#type' => 'textfield',
    '#default_value' => $configuration['media_youtube_default_description'] ? $configuration['media_youtube_default_description'] : 'Default description',
    '#description' => t('Videos which do not have a node, or text in the body or description field above will be given this description.'),
  );

  $form['media_youtube_conf']['media_youtube_default_tags'] = array(
    '#title' => t('Default tags'),
    '#type' => 'textfield',
    '#default_value' => $configuration['media_youtube_default_tags'] ? $configuration['media_youtube_default_tags'] : '',
    '#description' => t('Videos which do not belong to a node or which do not have tags will be given these tags. Separate them by space.'),
  );

  $vocabs = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabs as $vocab) {
    $options[$vocab->vid] = $vocab->name;
  }

  $form['media_youtube_conf']['media_youtube_vocabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Take tags from'),
    '#description' => t('Tags will be taken from the selected vocabularies.'),
    '#options' => $options,
    '#default_value' => $configuration['media_youtube_vocabs'] ? $configuration['media_youtube_vocabs'] : array(),
  );

  return $form;
}

function _media_youtube_filter_tags($val) {
  if ($val == 'si') {
    return FALSE;
  }
  return TRUE;
}
