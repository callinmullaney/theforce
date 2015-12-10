<?php

$plugin = array(
  'title' => 'EXAMPLE | Dropdown',
  'configurable' => 1,
  'handler' => array(
    'file' => 'theforceExampleJediDropdown.inc',
    'class' => 'theforceExampleJediDropdown',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediDropdown extends theforceDropdown {

  protected $text = 'Dropdown';
  protected $icon = 'drupal';

  /**
   * Dropdown content can be defined in a variable.
   */
  protected $dropdown = 'The Force is strong with this one.';

  /**
   * Dropdown content can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function dropdown(){
    return $this->dropdown;
  }

}
