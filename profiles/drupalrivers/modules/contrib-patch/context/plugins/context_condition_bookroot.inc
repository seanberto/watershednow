<?php
// $Id: context_condition_bookroot.inc,v 1.1.2.1 2010/01/10 07:07:45 yhahn Exp $

/**
 * Set the context on the basis of the node type of page's book root.
 */
class context_condition_bookroot extends context_condition_node {
  function execute($node, $op) {
    if (!empty($node->book['bid'])) {
      $type = db_result(db_query("SELECT type FROM {node} WHERE nid = %d", $node->book['bid']));
      $book = new stdClass();
      $book->type = $type;
      parent::execute($book, $op);
    }
  }
}
