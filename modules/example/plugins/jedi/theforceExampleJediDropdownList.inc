<?php

$plugin = array(
  'title' => 'EXAMPLE | Dropdown List',
  'configurable' => 1,
  'handler' => array(
    'class' => 'theforceExampleJediDropdownList',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediDropdownList extends theforceDropdownList {

  protected $text = 'List';
  protected $icon = 'apple';
  protected $listTitle = 'Example Dropdown List';

  /**
   * Dropdown list items can be defined in a variable.
   */
  protected $listItems = array('First Item', 'Second Item', '<a href="#">Third Item</a>');

  /**
   * Dropdown list items can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function listItems(){
    return $this->listItems;
  }

}
