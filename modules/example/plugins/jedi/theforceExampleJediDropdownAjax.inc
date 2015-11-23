<?php

$plugin = array(
  'title' => 'EXAMPLE | Dropdown (ajax)',
  'configurable' => 1,
  'handler' => array(
    'class' => 'theforceExampleJediDropdownAjax',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediDropdownAjax extends theforceDropdown {

  protected $text = 'Dropdown (ajax)';
  protected $icon = 'refresh';
  protected $ajax = 1;

  /**
   * Dropdown content can be defined in a variable.
   */
  protected $dropdown = 'The ajax-loaded Force is strong with this one.';

  /**
   * Dropdown content can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function dropdown(){
    return $this->dropdown;
  }

}
