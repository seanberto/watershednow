<?php
// $Id: admin.inc,v 1.1 2008/11/12 23:22:19 dww Exp $


/**
 * @file
 * Theme functions for signup administration.
 */

/**
 * Controls the output of the signup administration overview page.
 *
 * This page is located at admin/content/signup, and allows site-wide signup
 * administrators to view signup-related information, close/open signups, etc,
 * for all signup-enabled nodes on the site.  There's a form to filter the
 * results by signup status, which is rendered at the top of the page by
 * default.
 *
 * @param $filter_status_form
 *   HTML representation of the signup status filter form.
 * @param $signup_admin_form
 *   HTML representation of the main signup administration form.
 *
 * @return
 *   Themed output for the signup administration overview page.
 */
function theme_signup_admin_page($filter_status_form, $signup_admin_form) {
  $output = $filter_status_form;
  $output .= $signup_admin_form;
  return $output;
}

function theme_signup_filter_status_form($form) {
  return '<div class="container-inline">'. drupal_render($form) .'</div>';
}

function theme_signup_admin_form($form) {
  if (!isset($form['nids'])) {
    $type = $_SESSION['signup_status_filter'];
    switch ($type) {
      case 'open':
        $filter = t('open');
        break;

      case 'closed':
        $filter = t('closed');
        break;

      default:
        $filter = t('enabled');
        break;
    }
    return t('No content is currently !status for signups.', array('!status' => $filter));
  }
  foreach ($form['nids'] as $nid => $node_form) {
    if (!is_numeric($nid)) {
      continue;
    }
    $row = array();
    if (isset($node_form['start'])) {
      $row[] = drupal_render($form['nids'][$nid]['start']);
    }
    $row[] = drupal_render($form['nids'][$nid]['title']);
    $row[] = drupal_render($form['nids'][$nid]['total']);
    $row[] = drupal_render($form['nids'][$nid]['limit']);
    $row[] = drupal_render($form['nids'][$nid]['status']);
    $row[] = drupal_render($form['nids'][$nid]['operations']);
    $rows[] = $row;
  }
  $header = $form['header']['#value'];
  unset($form['header']);
  $output = theme('table', $header, $rows, array('style' => 'width:100%'));
  $output .= drupal_render($form);
  $pager = theme('pager', NULL, 25, 0);
  if (!empty($pager)) {
    $output .= $pager;
  }
  return $output;
}

