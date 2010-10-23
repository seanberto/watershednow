<?php
// $Id: views_handler_field_taxonomy_image.inc,v 1.1.2.2 2008/11/19 16:02:10 nancyw Exp $

/**
 * @file
 * Views integration for Taxonomy Image module.
 */

/**
 * Field handler to provide an embedded image.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_taxonomy_image extends views_handler_field {
  /**
   * Define options available for this field.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['imagecache_preset'] = array('default' => '');
    $options['link_to_taxonomy'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Build option configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this image to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    // If ImageCache module is found, add its presets as available options
    // for how to display the image.
    if (module_exists('imagecache')) {
      $raw_presets = imagecache_presets();
      $presets[''] = t('Default');
      foreach ($raw_presets as $preset_id => $preset_info) {
        $preset = $preset_info['presetname'];
        $presets[$preset] = $preset;
      }
      $form['imagecache_preset'] = array(
        '#type' => 'select',
        '#title' => t('ImageCache preset'),
        '#options' => $presets,
        '#default_value' => $this->options['imagecache_preset'],
      );
    }
  }

  /**
   * Render field output to the browser.
   */
  function render($values) {
    $image = '';
    $tid = $values->{$this->field_alias};
    if (!$tid) {
      return;
    }

    // Render image. If ImageCache preset is specified, use it.
    if ($this->options['imagecache_preset']) {
      $image = taxonomy_image_display($tid, NULL, $this->options['imagecache_preset']);
    }
    else {
      $image = taxonomy_image_display($tid);
    }

    // Output image as a link, if option is set.
    if ($this->options['link_to_taxonomy']) {
      $image = l($image, drupal_get_path_alias(taxonomy_term_path(taxonomy_get_term($tid))), array('html' => TRUE));
    }

    return $image;
  }
}
