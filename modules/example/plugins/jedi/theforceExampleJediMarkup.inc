<?php

$plugin = array(
  'title' => 'EXAMPLE | Markup',
  'configurable' => 1,
  'handler' => array(
    'file' => 'theforceExampleJediMarkup.inc',
    'class' => 'theforceExampleJediMarkup',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediMarkup extends theforceMarkup {

  /**
   * Content can be defined in a variable.
   */
  protected $markup = '<strong>Mark</strong><em>up</em>';

  /**
   * Content can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function markup(){
    $markup = $this->markup;
    return $markup;
  }

}
