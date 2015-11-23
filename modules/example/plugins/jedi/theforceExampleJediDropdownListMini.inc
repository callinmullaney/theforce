<?php

$plugin = array(
  'title' => 'EXAMPLE | Dropdown List (Mini)',
  'configurable' => 1,
  'handler' => array(
    'class' => 'theforceExampleJediDropdownListMini',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediDropdownListMini extends theforceDropdownList {

  protected $text = 'List (mini)';
  protected $icon = 'chrome';
  protected $listStyle = 'mini';

  /**
   * Dropdown items can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function listItems(){
    return array(
      l('Mini First Item', '</front>'),
      l('Mini Second Item (which is really long)', '<front>'),
      'Mini Third Item',
    );
  }

}
