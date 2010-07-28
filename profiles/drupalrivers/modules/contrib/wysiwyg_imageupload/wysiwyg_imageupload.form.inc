<?php
// $Id:$
// Copyright (c) 2010 KontextWork GbR
// Author: Eugen Mayer

/**
* Implementation of hook_form_alter
* Adding jquery_ui_dialog javascript / css definitions to the form.
*/
function wysiwyg_imageupload_form_alter(&$form, $form_state, $form_id) {
  
  if ($form['#id'] == 'node-form' || $form['#id'] == 'comment-form' ) {
    if(variable_get('wysiwyg_imageupload_skip_dependencies_check', true)) {
      _wysiwyg_imageupload_check_dependencies();
    }
    // SO UGLY: we have to use this, as FAPI has a special case with cached forms
    // If you save or preview the form and a validation error occurs, form_alter is not called
    // in that case all our javascript would be missing
    $form['#after_build'][] = '_wysiwyg_imageupload_prepare_main_form';
    // Add this as in some cases like comments drupal is not inlcude it by default

    // In the case of an preview, where we dont have a validation error
    // the form gets rebuild and gets a new form_build_id. In this case we have to transfer our cache
    // to the new form_buil_id so we can later restore the cache properly in our _wysiwyg_imageupload_transfer_cache method
    if($form_state['submitted'] == true && $form_state['rebuild'] == true) {
      // The form has been saved or is preview, transfer the cache to the new form_build_id
      _wysiwyg_imageupload_transfer_cache($form, $form_state);
    }
    else if ($form_state['submitted'] == true) {
      // From submitted, but not rebuild. We dont need to do anything here
    }
    else {
      // Load already existing files and put them in the array before the form is shown.
      // This must be done when nodes or comments are edited, to ensure that already uploaded images
      // are in the cache.
      if (
        $form['#id'] == 'comment-form' &&   // Should only match, if we have a comment formular.
        $form['cid']['#value'] != NULL      // If its a creation of a new comment, we dont need to load any files.
      ) {
        // We need this special case, as there is no "op=load" in the hook_comment hook_comment
        // so we cant load extra data like our files. We only call this on the first load of the edit form
        // so on a preview or similar, we already have written this cache and only load (transfer) it.
        $files = _wysiwyg_imageupload_commment_load($form['cid']['#value']);
      }
      else if (
        $form['#id'] == 'node-form' &&
        $form['#node']->nid != NULL &&
        !empty($form['#node']->imgupload_images)
      ) {
        $files = $form['#node']->imgupload_images;
      }
      // Save them in the cache, if there are any.
      if(is_array($files) && count($files) > 0) {
        cache_set('wysiwyg_imageupload_'.$form['#build_id'],$files,'cache',CACHE_TEMPORARY);
      }
    }
    // Comment form is incompatible with form caching see http://drupal.org/node/644222
    if($form['#id'] != 'comment-form') {
      // Thats is mandatory now, as we handle or cache ourselfs
      $form['#cache'] = TRUE;
    }

    $form['#validate'][] = 'wysiwyg_imageupload_form_validate';
  }
}

/**
* Inlcude all js libs and css we need for the form
*/
function _wysiwyg_imageupload_prepare_main_form(&$form,$form_state) {
  drupal_add_js('misc/jquery.form.js');

  $settings = array(
    'current_form' => $form['form_build_id']['#value'],
    // only set revisions new, when we actually edit a node. It does not make sense for new node nor for comments
    'revisions' => (int) (($form['#id'] != 'comment-form') && _wysiwyg_imagegupload_revisions_activated($form['#node']->type) && !empty($form['#node']->nid))
  );

  drupal_add_js(array('wysiwyg_imageupload' => $settings), 'setting');
  jquery_ui_dialog_parent_js();
  return $form;
}

/**
* This method transfers the cache of the old from_build_id to a new cache setting
* using the new form_build_id. This is need due every preview and save of a non cached form
* is causing the form_build_id to change and therefor we could later not load our uploaded files
* for making them persisten.
*/
function _wysiwyg_imageupload_transfer_cache($form, $form_state) {
  $old_form_build_id = $form_state['values']['form_build_id'];
  $new_form_build_id = $form['#build_id'];
  // Lets get the old cache and set it to the new cache
  $old = cache_get('wysiwyg_imageupload_'.$old_form_build_id);
  // Now apply it using the new from_build_id
  cache_set('wysiwyg_imageupload_'.$new_form_build_id, $old->data,'cache', CACHE_TEMPORARY);
}

/**
* Called on every node or comment save
* This method will load the cache and put the loaded files into the comment-object for use in insert/update
* where those files will be made persistent and added to the relation table.
*/
function wysiwyg_imageupload_form_validate(&$form, &$form_state) {
  if ($form_state['submitted'] == true) {
    $value = cache_get('wysiwyg_imageupload_' . $form_state['values']['form_build_id'], 'cache');
    $form_state['values']['imgupload_images'] = $value->data;
  }
}

/**
* Returns the general image upload form.
* This form is shown before an image has been uploaded,
* afterwards it is replaced by the image details form
*/
function wysiwyg_imageupload_upload_form(&$form_state, $parent_build_id) {
  $m = drupal_get_path('module','wysiwyg_imageupload');
  
  $parent_build_id = check_plain($parent_build_id);
  jquery_ui_dialog_child_js();
  drupal_set_title(t('Upload image'));
  drupal_add_js("$m/js/wysiwyg_imageupload_uploadform.js");

  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['image_upload'] = array(
    '#type' => 'fieldset',
    '#title'=> t('Upload image'),
    '#prefix' => '<div id="file_upload_group">',
    '#suffix' => '</div>'
  );

  $form['parent_build_id'] = array(
    '#type' => 'value',
    '#value' => $parent_build_id
  );

  $form['image_upload']['wysiwyg_imageupload_file'] = array(
    '#type' => 'file',
    '#title' => t('Select an image to upload'),
    '#size' => 22,
  );
  return $form;
}

function wysiwyg_imageupload_upload_form_validate(&$form, &$state) {
  $iid = _wysiwyg_imageupload_entity_upload($state['values']['parent_build_id']);
  if ($iid == false) {
    form_set_error('wysiwyg_imageupload_file',t('This file could not be uploaded'));
    return;
  }
  // If the upload was fine, redirect to the details form
  drupal_goto("wysiwyg_imageupload/edit/$iid/".$state['values']['parent_build_id']);
}


/**
* Called when a image was selected / uploaded and the details form should be shown.
*/
function wysiwyg_imageupload_edit_form(&$form_state,$iid, $parent_build_id, $rebuild_entity = false) {
  jquery_ui_dialog_child_js(array('width' => 740));
  drupal_set_title(t('Image details'));
  if($rebuild_entity) {
    $img_obj = _wysiwyg_imageupload_load_inline_entity($iid);
    $iid = _wysiwyg_imageupload_create_inline_entity($img_obj,true, array('title' => $img_obj->title, 'imagecache' => $img_obj->imagecache,'style' => $img_obj->style));
    $img_obj->iid= $iid;
    $img_obj->nid = null;
    $img_obj->vid = null;
    $img_obj->vid = null;
    
    $old = cache_get('wysiwyg_imageupload_'.$parent_build_id, 'cache');
    $old = $old->data;
    // Add the new entity
    $old[$img_obj->iid] = $img_obj;
    cache_set('wysiwyg_imageupload_'.$parent_build_id, $old,'cache', CACHE_TEMPORARY);
  }
  
  $img_obj = _wysiwyg_imageupload_load_inline_entity($iid);
  if($img_obj == false) {
    drupal_set_message(t('Could not find image in the media-database'),'warning');
    return array();
  }

  $defaults = array (
    'filepath' => $img_obj->filepath,
    'imagecache' => $img_obj->imagecache,
    'alignment' => $img_obj->alignment,
    'style' => $img_obj->style,
    'title' => $img_obj->title,
  );
  $attributes = array(
    'class' => 'wysiwyg_imageupload_preview',
  );

  // Create the form image detail form.
  $form['#cache'] = TRUE;
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  // Render the image for the preview in the form.
  $img = theme('imagecache', 'wysiwyg_imageupload_preview', $defaults['filepath'], $iid, $defaults['title'], $attributes, TRUE, FALSE);
  // Load the details form.
  $form += _wysiwyg_imageupload_details_form($img,$iid,$defaults);

  return $form;
}

function wysiwyg_imageupload_edit_form_validate($form, &$state) {
  if (isset($_POST['revisions']) && $_POST['revisions'] == 1) {
    // As this node is going to have a new revision AND we just EDITED an image (NOT updated)
    // we create a new inline entity with the new settings and dont touch the old one.
    // This way the old inline image stays the same in older revisions.
    $old_iid = $state['values']['iid'];
    $img_obj = _wysiwyg_imageupload_load_inline_entity($old_iid);
    // Create a new inline entity and save it to our form_state
    // so the the code below handles the right entity.
    $state['values']['iid'] = _wysiwyg_imageupload_create_inline_entity($img_obj,true);
    // Remove the old inline id and place the new one.
    $old = cache_get('wysiwyg_imageupload_' . $_POST['parent_build_id'], 'cache');;
    $old = $old->data;
    $new_file = $old[$old_iid];
    // remove the old
    unset($old[$old_iid]);
    $new_file->iid = $state['values']['iid'];
    // Create the new.
    $old[$new_file->iid] = $new_file;
    // update the cache
    cache_set('wysiwyg_imageupload_'.$_POST['parent_build_id'], $old,'cache', CACHE_TEMPORARY);
  }
}

/**
* Handles the submit of the edit form
* Adds the details into the database for the inline-id
*/
function wysiwyg_imageupload_edit_form_submit($form, &$state) {
  $update = array(
    'iid',
  );
  $record = array(
    'style' => $state['values']['style'],
    'imagecache' => $state['values']['imagecache'],
    'alignment' => $state['values']['alignment'],
    'title' => $state['values']['title'],
    'iid' => $state['values']['iid'],
  );
  // Update the details of the inline entity.
  drupal_write_record('wysiwyg_imageupload_entity',$record, $update);
  // Call all our api hooks.
  module_invoke_all('wysiwyg_imageupload_entity_save',$state['values'],$state['values']['iid']);
  $GLOBALS['devel_shutdown'] = FALSE;
  print _wysiwyg_imageupload_to_js(array('status' => TRUE, 'data' => array('iid' => $state['values']['iid'])));
  exit();
}

/**
* Return the form to be presented as image details form.
* Title, floating description and so forth
*/
function _wysiwyg_imageupload_details_form($img, $iid, $defaults = array()){
  $form = array();

  $form['image_upload_details'] = array( 
    '#type' => 'fieldset',
    '#title'=> t('Image details'),
    '#prefix' => '<div id="image_details">',
    '#suffix' => '</div>'
  );
  // Title
  $form['image_upload_details']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $defaults['title'],
    '#size' => 50
  );
  // Alignment / Floating
  $form['image_upload_details']['alignment'] = array(
    '#type' => 'select',
    '#title' => t('Image alignment'),
    '#options' => array(
      'imgupl_floating_none' => t('Image is inline with the text (none)'),
      'imgupl_floating_left' => t('Image on left, text on right'),
      'imgupl_floating_right' => t('Image on right, text on left')
    ),
    '#default_value' => $defaults['alignment']
  );
  // Imagecache preset
  $form['image_upload_details']['imagecache'] = array(
    '#type' => 'select',
    '#title' => t('Size'),
    '#default_value' => $defaults['imagecache'],
    '#options' => _wysiwyg_imagegupload_allowed_presets(),
  );
  
  // We group all those extras into this fieldset to hide them from the general UI.
  $form['image_upload_details']['extras'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extras'),
    '#collapsed' => true,
    '#collapsible' => true,
  );

  // image styles.
  $styles = _wysiwyg_imageupload_get_image_styles();
  if (count($styles) > 0) {
    array_unshift($styles, t('None'));
    $form['image_upload_details']['extras']['style'] = array(
      '#type' => 'select',
      '#title' => t('Style'),
      '#options' => $styles,
      '#default_value' => $defaults['style'],
    );
  }

  // this is the container for our preview-image
  $form['image_details_form']['preview'] = array (
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#attributes' => array('id' => 'image_preview'),
    '#collapsible' => false
  );
  $form['image_details_form']['preview'] ['uploadedImage'] = array (
    '#prefix' => '<div >',
    '#suffix' => '</div>',
    '#value' => $img
  );
  
  // This is a fake submit button, thats why "access false" is used.
  // This is due we use ajax form submit.
  $form['image_details_form']['buttons']['submitimagedetails'] = array(
    '#type' => 'submit',
    '#value' => 'JSinsert', // t() not needed here, see comment below.
    '#access' => FALSE,
  );

  $form['image_upload_details']['iid'] = array(
    '#type' => 'value',
    '#value' => $iid,
    '#default_value' => $iid
  );
  return $form;
}
