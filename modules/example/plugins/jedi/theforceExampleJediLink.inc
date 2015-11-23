<?php

$plugin = array(
  'title' => 'EXAMPLE | Link',
  'configurable' => 1,
  'handler' => array(
    'class' => 'theforceExampleJediLink',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediLink extends theforceLink {

  protected $text = 'Link with really long title';
  protected $icon = 'rebel';

  /**
   * Link path can be defined in a variable.
   */
  protected $linkPath = 'http://www.theforce.net';

  /**
   * Content can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function linkPath(){
    return $this->linkPath;
  }

}
