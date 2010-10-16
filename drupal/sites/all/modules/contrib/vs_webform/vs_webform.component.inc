<?php
// $Id: vs_webform.component.inc,v 1.2.2.16 2010/06/07 21:30:55 troynt Exp $

/**
 * @file
 * Vote Smart Webform component.
 */

/**
 * Specify the default properties of vs_webform component
 *
 * @return An array defining the default structure of a component.
 */
function _webform_defaults_vs_webform() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'pid' => 0,
    'weight' => 0,
    'type' => 'hidden',
    'extra' => array(
      'email' => 1,
      'allow_candidates_without_email' => 1,
      'hide_no_email_warnings' => 1,
      'attributes' => array(),
      'name_format' => '!last !suffix, !first',
      'zip' => '',
      'hide_zip' => TRUE,
      'state' => '',
      'office_types' => array(),
      'hide_state' => FALSE
    )
  );
}

/**
 * Generate the form for editing vs_webform component
 * @param $component
 *   A Webform component array.
 * @return
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_vs_webform($component) {
  include('vs_webform.defines.inc');
  
  $form = array('extra' => array(
  ));
  $form['extra']['state'] = array(
    '#type' => 'select',
    '#title' => t('Default State'),
    '#description' => t('Pick a default state. This field will be hidden if zip is shown or specified below'),
    '#weight' => -1.2,
    '#default_value' => $component['extra']['state'],
    '#element_validate' => array('_webform_edit_vs_webform_default_state_validate'),
    '#options' => array_merge(array('' => ''), $VS_WEBFORM_US_STATES)
  );

  $form['extra']['hide_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide state on form, don\'t allow user to change value.'),
    '#weight'  => -1.15,
    '#default_value' => $component['extra']['hide_state']
  );

  $form['extra']['office_types'] = array(
    '#type' => 'checkboxes',
    '#multiple' => TRUE,
    '#title' => t('Default Office(s)'),
    '#description' => t('Show only officials in these offices. Leave blank to show all offices.'),
    '#weight' => -1.2,
    '#default_value' => $component['extra']['office_types'],
    '#options' => vs_webform_get_office_types()
  );

  $form['extra']['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Zip'),
    '#description' => t('Leave blank if you wish to use the state dropdown on form.'),
    '#weight' => -1.1,
    '#size' => '10',
    '#maxlength' => 10,
    '#default_value' => $component['extra']['zip'],
    '#element_validate' => array('_webform_edit_vs_webform_default_zip_validate'),
  );

  $form['extra']['hide_zip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide zip on form, don\'t allow user to change value.'),
    '#weight'  => -1.05,
    '#default_value' => $component['extra']['hide_zip']
  );

  $form['extra']['name_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Candidate name format'),
    '#weight' => 0,
    '#description' => t('valid tokens include: !title !first !middle !last !suffix !nick ... extra spaces are trimmed'),
    '#default_value' => $component['extra']['name_format']
  );

  return $form;
}

/**
 * Validate the Default Zip Code
 * @param $zip_element The zip form element.
 * @param $form_state The full form state for the webform.
 */
function _webform_edit_vs_webform_default_zip_validate($element, &$form_state) {
  $zip = $element['#value'];//alias
  if ( empty($zip) ) return;

  if ( !vs_webform_is_zip($zip) ) {
    form_error($element, t('!value is not a valid zip code.', array('!value' => $zip)));
  }
}

/**
 * Validate the Zip Code BLANK NOT ALLOWED
 * @param $element The zip form element.
 * @param $form_state The full form state for the webform.
 */
function _webform_edit_vs_webform_zip_validate($element, &$form_state) {
  $zip = $element['#value'];//alias
  if ( empty($zip) ) {
    return;
  }

  if ( !vs_webform_is_zip($zip) ) {
    form_error($element, t('!value is not a valid zip code.', array('!value' => $zip)));
  }
}

/**
 * Validate the Default State
 * @param $element The state form element.
 * @param $form_state The full form state for the webform.
 */
function _webform_edit_vs_webform_default_state_validate($element, &$form_state) {
  $state = $element['#value'];//alias
  if ( empty($state) ) return;

  if ( !vs_webform_is_us_state($state) ) {
    form_error($element, t('!value is not a valid state.', array('!value' => $state)));
  }
}

/**
 * Validate the State BLANK NOT ALLOWED
 * @param $element The state form element.
 * @param $form_state The full form state for the webform.
 */
function _webform_edit_vs_webform_state_validate($element, &$form_state) {
  $state = $element['#value'];//alias
  if ( empty($state) )
    form_error($element, t('Please select a state.'));

  if ( !vs_webform_is_us_state($state) ) {
    form_error($element, t('!value is not a valid state.', array('!value' => $state)));
  }
}

/**
 * Render a Webform component to be part of a form.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   If editing an existing submission or resuming a draft, this will contain
 *   an array of values to be shown instead of the default in the component
 *   configuration. This value will always be an array, keyed numerically for
 *   each value saved in this field.
 */
function _webform_render_vs_webform($component, $value = NULL) {
  if( arg(2) == 'submission' && arg(4) == 'edit' ) {
    //don't allow admins to edit reps
    return _webform_display_vs_webform($component, $value, 'html');
  }
  include('vs_webform.defines.inc');

  if ( !empty($value) )
    $value = unserialize($value[0]);

  if ( isset($value['vs_webform']) ) {
    if ( isset($value['vs_webform']['zip']) )
      $component['extra']['zip'] = $value['vs_webform']['zip'];
    if ( isset($value['vs_webform']['state']) )
      $component['extra']['state'] = $value['vs_webform']['state'];
    if ( isset($value['vs_webform']['office_types']) )
      $component['extra']['office_types'] = $value['vs_webform']['office_types'];

  }
  
  $dom_id = 'webform-component-' . $component['form_key'];
  $form = array(
    '#prefix' => '<div id="' . $dom_id . '" class="webform-component-vs-webform">',
    'vs_webform' => array(),
    '#suffix' => '</div><!--/#' . $dom_id . ' .webform-component-vs-webform-->',
    '#weight' => $component['weight']
  );

  //HACK: Need to find a better way
  $vs_webform_post = _vs_webform_array_key_search($_POST, 'vs_webform');
  if ( $vs_webform_post ) {
    if ( isset($vs_webform_post['vs_webform']) ) {
      $vs_webform_post = $vs_webform_post['vs_webform'];
    }
    $component['extra']['zip'] = $vs_webform_post['zip'];
    $component['extra']['state'] = $vs_webform_post['state'];
    $component['extra']['office_types'] = $vs_webform_post['office_types'];
    if ( isset($component['extra']['office_types']) )
      $component['extra']['office_types'] = unserialize($component['extra']['office_types']);
  }

  $form['vs_webform']['zip'] = array(
    '#type' => $component['extra']['hide_zip'] ? 'hidden' : 'textfield',
    '#title' => t('Zip'),
    '#required' => !$component['extra']['hide_zip'],
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => $component['extra']['zip'],
    '#description'   => t('Enter Postal Code in format 12345 or 12345-1234.'),
    '#element_validate' => array('_webform_edit_vs_webform_zip_validate'),
    '#ahah' => array(
      'event' => 'change',
      'wrapper' => $dom_id,
      'path' => 'vs_webform/ahah/get_candidates',
      'progress' => array('type' => 'throbber', 'message' => t('Getting candidates ...'))
    )
  );

  $form['vs_webform']['state'] = array(
    '#type' => $component['extra']['hide_state'] ? 'hidden' : 'select',
    '#title' => t('State'),
    '#options' => array_merge(array('' => ''), $VS_WEBFORM_US_STATES),
    '#multiple' => FALSE,
    '#required' => !$component['extra']['hide_state'],
    '#default_value' => $component['extra']['state'],
    '#element_validate' => array('_webform_edit_vs_webform_state_validate'),
    '#ahah' => array(
      'event' => 'change',
      'wrapper' => $dom_id,
      'path' => 'vs_webform/ahah/get_candidates',
      'progress' => array('type' => 'throbber')
    )
  );
  
  //Showing a zip field, hide the state
  if( $form['vs_webform']['zip']['#type'] != 'hidden' ) {
    $form['vs_webform']['state']['#type'] = 'hidden';
    $form['vs_webform']['state']['#required'] = FALSE;
    $form['vs_webform']['state']['#element_validate'] = array();
  }
/*  
  //default zip specified remove state field
  if( !empty($component['extra']['zip']) ) {
    unset($form['vs_webform']['state']);
  }
*/
  $form['vs_webform']['office_types'] = array(
    '#type' => 'hidden',
    '#value' => serialize($component['extra']['office_types'])
  );

  $zip = $component['extra']['zip']; //alias
  $state = $component['extra']['state']; //alias
  $office_types = array();
  if ( !empty($component['extra']['office_types']) ) {
    $office_types = $component['extra']['office_types'];
  }
  $form['vs_webform']['jumper'] = array();//used to jump to various offices
 
  $candidates = array();

  $fetch_candidates = TRUE;
  if ( $form['vs_webform']['state']['#type'] == 'hidden' && empty($zip) ) {
    $fetch_candidates = FALSE;
  }
  
  if ( $fetch_candidates ) {
    $candidates = vs_webform_candidate_form_elements(array('state' => $state, 'zip' => $zip, 'officeTypes' => $office_types), $component, $form['vs_webform']['jumper']);
  }

  if ( !empty($candidates) ) {
    $cls = 'vs-webform-candidate-wrapper';
    if ( !empty($form['vs_webform']['jumper']) ) {
      $cls .= ' vs-webform-candidate-wrapper-has-jumper';
    }
    $candidate_count = 0;
    foreach( $candidates as $cand ) {
      if( $cand['#type'] == 'checkbox' ) {
        $candidate_count++;
      }
    }
    $form['vs_webform']['candidates_prefix'] = array(
      '#type' => 'markup',
      '#value' => '<div class="' . $cls . '"><span class="vs-webform-count">' . t('Showing %candidate_count Officials', array('%candidate_count' => $candidate_count)) . '</span>'
    );

    $form['vs_webform']['candidates'] = $candidates;
    $form['vs_webform']['candidate_suffix'] = array(
      '#type' => 'markup',
      '#value' => '</div><!--/.vs-webform-candidate-wrapper-->'
    );
  }

  if ( is_array($value['vs_webform']['candidates']) )
    foreach ($value['vs_webform']['candidates'] as $k => $v )
      $form['vs_webform']['candidates'][$k]['#default_value'] = $v;

  return $form;
}

/**
 * @param $args array containing conditions the officials must meet to be included
 * @param $component array containing component options
 * @param $jumper_element html element containing list used to jump to various branches in the list
 * @return array of candidate form elements
 */
function vs_webform_candidate_form_elements($args, $component, &$jumper_element = NULL) {
  include('vs_webform.defines.inc');

  $candidates = $candidate_elements = $cand_names = array();

  $candidates = vs_webform_get_candidates($args);
  if ( empty($candidates) ) return array();

  $cand_names = array();
  foreach ($candidates as $id => $c) {
    if ( !isset($cand_names[$c['officeTypeId']]) )
      $cand_names[$c['officeTypeId']] = array();
    $cand_names[$c['officeTypeId']][$id] = _vs_webform_format_name($c, $component['extra']['name_format']);
  }

  $needs_jumper = count($cand_names) > 1 && count($candidates) > 10;

  if ( $needs_jumper ) {
    $jumper_element = array(
      '#type' => 'markup',
      '#value' => '<ul class="vs-webform-jumper">'
    );
  }

  asort($cand_names);
  $office_types = vs_webform_get_office_types();
  foreach ($cand_names as $office => $cands ) {
    $anchor_name = 'vs-webform-office-type-' . strtolower($office);
    $office_label = t($office_types[$office]);
    if ( $needs_jumper )
      $jumper_element['#value'] .= '<li><a title="jump to ' . $office_label . '" href="#' . $anchor_name . '">' . $office_label . '</a></li>';
    $candidate_elements[] = array(
      '#type' => 'markup',
      '#value' => '<h4><a name="' . $anchor_name . '">' . $office_label . '</a></h4>'
    );
    asort($cands);
    foreach ($cands as $cand_id => $cand_name ) {
      $element = array(
        '#type' => 'checkbox',
        '#title' => $cand_name,
      );

      if ( !$component['extra']['hide_no_email_warnings'] && !isset($candidates[$cand_id]['email']) ) {     
        if ( !$component['extra']['allow_candidates_without_email'] )
          $element['#disabled'] = TRUE;
        $element['#description'] = t('No email found for !candidate.', array('!candidate' => $cand_name));

        if ( isset($candidates[$cand_id]['webmail']) || isset($candidate[$cand_id]['website']) ) {
          if ( isset($candidates[$cand_id]['webmail']) ) {       
            $website = $candidates[$cand_id]['webmail'];
          }      
          else
            $website = $candidates[$cand_id]['website'];

          $element['#description'] = t('No email found for !candidate. Try contacting them at <a target="_blank" rel="nofollow" href="!website">their&nbsp;website</a>.', array(
            '!candidate' => $cand_name,
            '!website' => $website
          ));

        }  
      }

      $candidate_elements[$cand_id] = $element;
    }
  }//foreach 

  if ( $needs_jumper )
    $jumper_element['#value'] .= '</ul>';

 
  return $candidate_elements;
}

/**
 * Display the result of a submission for a component.
 * 
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param $format
 *   Either 'html' or 'text' . Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 * @return
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
function _webform_display_vs_webform($component, $value, $format = 'html') {
  if ( !in_array($format, array('html', 'table-cell', 'text')) || empty($value) ) {
    return '';
  }

  $ret = array(
    '#weight' => $component['weight'],
    '#theme' => 'markup',
  );

  if ( $format == 'html' ) {
    $ret['#prefix'] = '<fieldset><legend>' . t($component['name']) . '</legend>';
    $ret['#suffix'] = '</fieldset>';
  }

  $value[0] = unserialize($value[0]);

  $candidates = $value[0]['vs_webform']['candidates'];
  if ( !$candidates ) $candidates = array();
  $cand_names = array();

  if ( is_array($candidates) ) {
    foreach ($candidates as $cand_id => $checked ) {
      $c = vs_webform_get_candidate($cand_id);
      //sanity check
      if ( $checked ) {
        $name = _vs_webform_format_name($c, $component['extra']['name_format']);
        if ( !empty($c['email']) ) {
          $e = $c['email'];
          $name .= $format == 'text' ? ' ' . $e : ' ' . l($e, 'mailto:' . $e);
        }
        $cand_names[] = $name;
      }
    }//foreach
  }//if is_array
  $o = '';


  if ( !empty($value[0]['vs_webform']['state']) ) {
    $o = t('State') . ': ' . $value[0]['vs_webform']['state'] . "<br />\r\n";
  }

  if ( !empty($value[0]['vs_webform']['zip']) ) {
    $o = t('Zip') . ': ' . $value[0]['vs_webform']['zip'] . "<br />\r\n";
  }

  unset($value[0]);

  $o .= t('Officials Selected') . ': ';

  if ( $format == 'text' ) {
    $o .= "\r\n" . implode("\r\n", $cand_names) . "\r\n";
    $ret['#value'] = strip_tags($o);
    return array();//hack to keep this out of emails
    return $ret;//must return an array
  }
  else{
    $o .= theme('item_list', $cand_names);
  }

  if ( $format == 'table-cell' ) {
    return $o;
  }
 
  $ret['#value'] = $o;

  return $ret;
}

/**
 * Implementation of _webform_submit_component().
 *
 * NOTE: When webform sends out emails it will loop
 * through the returned array, validating the emails;
 * If the email is valid webform will send an email.
 */
function _webform_submit_vs_webform($component, &$value) {
  if ( empty($value) || !is_array($value['vs_webform']['candidates']) ) return NULL;

  $ret = array(0 => ''); //placeholder for serialized value
  foreach ($value['vs_webform']['candidates'] as $cand_id => $checked) {
    if ( !$checked ) {
      unset($value['vs_webform']['candidates'][$cand_id]);
    }
    else{
      $candidate = vs_webform_get_candidate($cand_id, NULL, TRUE);
      if ( !empty($candidate['email']) ) {
        $ret[] = $candidate['email'];
      }
    }
  }
  $ret[0] = serialize($value);
  return $ret;
}

function vs_webform_ahah_get_candidates() {
  $form_state = array('storage' =>  NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  if ( !is_array($args) ) return drupal_json(array('status' => FALSE, 'data' => ''));

  $form_id = array_shift($args);
  $form['#post'] = $form_state['post'] = $_POST;
  $form['#redirect'] = $form['#programmed'] = FALSE;
  
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  _vs_webform_validate($form, $form_state, TRUE);
  $vs_webform = vs_webform_get_vs_webform_element($form);
  $output = theme('status_messages');  
  $errors = form_get_errors();
  if( !empty($errors) ) {
    unset(
      $vs_webform['candidates_prefix'],
      $vs_webform['candidates'],
      $vs_webform['candidates_suffix'],
      $vs_webform['jumper']
    );
  }
  
  if ( $vs_webform ) {
    $output .= drupal_render($vs_webform);
  }

  drupal_json(array('status' => TRUE, 'data' => $output));
}

function vs_webform_form_alter(&$form, $form_state, $form_id) {
  if ( preg_match('@webform_(.*)@', $form_id, $matches) ) {
    if ( $vs_webform = vs_webform_get_vs_webform_element($form) ) {
      $form['#validate'][] = '_vs_webform_validate';
    }
  }
}

function _vs_webform_element_children_search($form, $key) {
  $element_children = element_children($form);
  $found = array_search($key, $element_children);
  if ( $found ) return $form[$key];

  if ( !is_array($element_children) ) return FALSE;

  foreach ($element_children as $e) {
    $result = _vs_webform_element_children_search($form[$e], $key);
    if ( $result != FALSE ) return $result;
  }
  return FALSE;
}

function _vs_webform_array_key_search($arr, $key) {
  if ( !is_array($arr) ) return FALSE;

  $found = array_key_exists($key, $arr);
  if ( $found ) return $arr[$key];

  if ( !is_array($arr) ) return FALSE;

  foreach ($arr as $e) {
    $result = _vs_webform_array_key_search($e, $key);
    if ( $result != FALSE ) return $result;
  }
  return FALSE;
}

function vs_webform_get_vs_webform_element( $form ) {
  $vs_webform = &_vs_webform_element_children_search($form, 'vs_webform');
  if( isset($vs_webform['vs_webform']) ) {
    $vs_webform = $vs_webform['vs_webform'];
  }
  return $vs_webform;
}

function _vs_webform_validate($form, &$form_state, $allow_no_candidates_selected = FALSE) {
  $vs_webform = vs_webform_get_vs_webform_element($form);

  if ( !empty($vs_webform['zip']['#value']) && !vs_webform_is_zip($vs_webform['zip']['#value']) ) {
    form_error($vs_webform['zip'], t('!value is not a valid zip code.', array('!value' => $vs_webform['zip']['#value'])));
  }

  if ( empty($vs_webform['zip']['#value']) && $vs_webform['zip']['#type'] != 'hidden' ) {
    form_error($vs_webform['zip'], t('Please enter a zip code.'));
  }

  
  if ( ( isset($vs_webform['zip']) && form_get_error($vs_webform['zip']) )
      || ( isset($vs_webform['state']) && form_get_error($vs_webform['state']) )
  ) {
    return; // wait until we have a valid zip or state
  }

  $allowed_candidates = vs_webform_get_candidates(array(
    'state' => $vs_webform['state']['#value'],
    'zip' => $vs_webform['zip']['#value']
  ));
  
  if ( empty($allowed_candidates) ) {
    form_set_error('vs_webform',t('Unable to find officials with information provided.'));
    return;
  }
  
  $candidates = element_children($vs_webform['candidates']);//alias
  if( !$allow_no_candidates_selected ) {
    if ( empty($vs_webform['candidates']) ) {
      form_set_error('vs_webform', t('Please select one or more officials.'));
      return;
    }
  
    if ( !is_array($candidates) ) {
      form_set_error('vs_webform', t('Please select one or more officials.'));
      return;
    }
  }

  /**
   * Both zip and state supplied. Zip was used to look up candidates,
   * check to see if an official found in the zip is also present in state,
   * to prove that the zip is within that state.
   */
  if ( !empty($vs_webform['zip']['#value']) && !empty($vs_webform['state']['#value']) ) {
    $allowed_cands_from_state = vs_webform_get_candidates(array(
        'state' => $vs_webform['state']['#value'],
    ));
    $first_cand_id_from_zip = current(array_keys($allowed_candidates));
    if ( !isset($allowed_cands_from_state[$first_cand_id_from_zip]) ) {
      form_set_error('vs_webform', t('Zip not found in !state.',array('!state' => $vs_webform['state']['#value'])));
    }
  }
  
  $has_one_candidate_selected = FALSE;
  if ( !empty($candidates) ) {
    foreach ( $candidates as $cand_id ) {
      if ( !isset($allowed_candidates[$cand_id]) ) {
        $vs_webform['candidates'][$cand_id]['#value'] = 0;
      }
      
      if ( !$has_one_candidate_selected && $vs_webform['candidates'][$cand_id]['#value'] == 1 ) {
        $has_one_candidate_selected = TRUE;
      }
    }
  }

  if ( !$allow_no_candidates_selected && !$has_one_candidate_selected ) {
    form_set_error('vs_webform', t('Please select one or more officials.'));
  }
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_vs_webform($component, $value) {
  return _webform_display_vs_webform($component, $value, 'table-cell');
}


/**
 * Implementation of _webform_csv_headers_component().
 */
function _webform_csv_headers_vs_webform($component, $export_options) {
  $name = 'vs_webform';
  return array(
    0 => array('', '', ''),
    1 => array($name, $name, $name),
    2 => array($name . '_state', $name . '_zip', 'officials_selected')
  );
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_vs_webform($component, $export_options, $value) {
  if ( !isset($value[0]) ) return '';

  $v = unserialize($value[0]);
  $v = $v['vs_webform'];

  $cand_ids = '';
  if ( !empty($v['candidates']) && is_array($v['candidates']) ) {
    $cand_ids = implode(';', array_keys($v['candidates']));
  }

  return array(
    $v['state'],
    $v['zip'],
    $cand_ids
  );
}

