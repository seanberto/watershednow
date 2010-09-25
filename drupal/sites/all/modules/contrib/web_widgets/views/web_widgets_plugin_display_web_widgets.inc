<?php
// $Id: web_widgets_plugin_display_web_widgets.inc,v 1.1.2.5 2010/08/01 02:28:43 diggersf Exp $
/**
 * @file
 * Implementation of widget display.
 */

/**
 * The plugin that handles a feed, such as RSS or atom.
 *
 * For the most part, feeds are page displays but with some subtle differences.
 */
class web_widgets_plugin_display_web_widgets extends views_plugin_display_page {

  function has_path() {
    return TRUE;
  }

  function uses_breadcrumb() {
    return FALSE;
  }

  /**
   * web_widgets do not go through the normal page theming mechanism. Instead, they
   * go through their own little theme function and then return NULL so that
   * Drupal believes that the page has already rendered itself...which it has.
   */
  function execute() {
    $output = $this->view->render();
    if (empty($output)) {
      return drupal_not_found();
    }
    print $output;
  }

  function preview() {
    return $this->view->render();
  }

  /**
   * Instead of going through the standard views_view.tpl.php, delegate this
   * to the style handler.
   */
  function render() {
    // Ensure that the embed style functions are available
    $current_style = $this->get_option('embed_style');
    $current_style = empty($current_style) ? 'iframe' : $current_style;
    require_once web_widgets_get_style_inc($current_style);

    // See if the render() function is "overloaded" or not
    web_widgets_inside_widget(TRUE);
    $render_func = 'web_widgets_'. $current_style .'_render';
    if (function_exists($render_func)) {
      $output = $render_func($this);
    }
    else {
      // Fallback to default way
      $content = theme($this->theme_functions(), $this->view);
      $code = '';
      $track = $this->get_option('track');
      if (!empty($track)) {
        $code = theme('web_widgets_tracking_code', $track);
      }
      $title = $this->view->get_title();
      $output = web_widgets_render_widget($current_style, $code . $content, $title);
    }
    web_widgets_inside_widget(FALSE);
    return $output;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'rss';
    $options['style_options']['default']  = array('mission_description' => FALSE, 'description' => '');
    $options['sitename_title']['default'] = FALSE;
    $options['row_plugin']['default'] = '';
    $options['defaults']['default']['style_plugin'] = 'views_plugin_style_list';
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = 'views_plugin_row_fields';
    $options['defaults']['default']['row_options'] = FALSE;

    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here.
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'displays':
        $form['#title'] .= t('Attach embed code to');
        $displays = array();
        foreach ($this->view->display as $display_id => $display) {
          if (!empty($display->handler) && $display->handler->accept_attachments()) {
            $displays[$display_id] = $display->display_title;
          }
        }
        $form['displays'] = array(
          '#type' => 'checkboxes',
          '#description' => t('The embed code will be displayed on the selected displays.'),
          '#options' => $displays,
          '#default_value' => $this->get_option('displays'),
        );
        break;
      case 'dimension':
        $form['width'] = array(
          '#type' => 'textfield',
          '#description' => t('The width of the widget in pixels (not respected by all embed styles).'),
          '#default_value' => $this->get_option('width'),
        );
        $form['height'] = array(
          '#type' => 'textfield',
          '#description' => t('The height of the widget in pixels (not respected by all embed styles).'),
          '#default_value' => $this->get_option('height'),
        );
        break;
      case 'embed_style':
        $form['embed_style'] = array(
          '#type' => 'select',
          '#options' => web_widgets_get_styles(),
          '#description' => 'The method being used to embed a widget from this site in another site.',
          '#default_value' => $this->get_option('embed_style'),
        );
        break;
      case 'path':
        $form['path']['#description'] = t('This view will be displayed by visiting this path on your site. It is recommended that the path be something like "path/%/%/widget" or "path/%/%/widget/[embed-style]", putting one % in the path for each argument you have defined in the view.');
        break;
      case 'track':
        $form['track'] = array(
          '#type' => 'textfield',
          '#default_value' => $this->get_option('track'),
          '#description' => t('Supply a valid Analytics domain ID if you want to track the usage of the widget in the Google Analytics ecosystem. If the field is empty, the tracking is turned off.')
        );
        break;
    }
  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit($form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'displays':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]);
        $this->set_option('attach_from', $form_state['view']->current_display);
        break;
      case 'dimension':
        $this->set_option('width', $form_state['values']['width']);
        $this->set_option('height', $form_state['values']['height']);
        break;
      case 'embed_style':
        $this->set_option('embed_style', $form_state['values']['embed_style']);
        break;
      case 'track':
        $this->set_option('track', $form_state['values']['track']);
        break;
    }
  }

  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    // I don't think we want to give feeds menus directly.
    unset($options['menu']);

    $displays = array_filter($this->get_option('displays'));
    if (count($displays) > 1) {
      $attach_to = t('Multiple displays');
    }
    else if (count($displays) == 1) {
      $display = array_shift($displays);
      if (!empty($this->view->display[$display])) {
        $attach_to = $this->view->display[$display]->display_title;
      }
    }

    if (!isset($attach_to)) {
      $attach_to = t('None');
    }

    $options['displays'] = array(
      'category' => 'page',
      'title' => t('Attach embed code to'),
      'value' => $attach_to,
    );

    $x = $this->get_option('width');
    $y = $this->get_option('height');
    if (!empty($x) && !empty($y)) {
      $dim = t("X: !x Y: !y", array('!x' => $x, '!y' => $y));
    }
    else {
      $dim = t('Not set yet');
    }
    $options['dimension'] = array(
      'category' => 'page',
      'title' => t('Widget dimensions'),
      'value' => $dim,
    );

    $current_style = $this->get_option('embed_style');
    // iframe is the default, built-in style, always available
    $current_style = empty($current_style) ? 'iframe' : $current_style;
    $style_names = web_widgets_get_styles();
    $options['embed_style'] = array(
      'category' => 'page',
      'title' => t('Embed style'),
      'value' => $style_names[$current_style],
    );

    $track = $this->get_option('track');
    $track = empty($track) ? t('Tracking is turned off') : $track;
    $options['track'] = array(
      'category' => 'page',
      'title' => t('Google Tracking code'),
      'value' => $track,
    );
  }

  /**
   * Attach to another view.
   */
  function attach_to($display_id) {
    $attach_to = $this->get_option('displays');
    if ($attach_to[$display_id]) {
      $current_style = $this->get_option('embed_style');
      $current_style = empty($current_style) ? 'iframe' : $current_style;
      $path = url($this->view->get_url(NULL, $this->get_path()), array('absolute' => TRUE));
      $width = $this->get_option('width');
      $height = $this->get_option('height');
      $this->view->attachment_after .= web_widgets_render_embed_code($current_style, $path, $width, $height);
    }
  }

  /**
   * Display validation.
   */
  function validate() {
    $errors = parent::validate();
    return $errors;
  }
}
