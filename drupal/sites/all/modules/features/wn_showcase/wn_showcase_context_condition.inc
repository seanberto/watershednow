<?php
class wn_showcase_context_condition extends context_condition_path {
  protected function match($subject, $patterns, $path = FALSE) {
    $subject = end(explode('/',path_to_theme()));
    return parent::match($subject, $patterns, $path);
  }
}
