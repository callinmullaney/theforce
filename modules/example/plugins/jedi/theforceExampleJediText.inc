<?php

$plugin = array(
  'title' => 'EXAMPLE | Text',
  'configurable' => 1,
  'handler' => array(
    'class' => 'theforceExampleJediText',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediText extends theforceText {

  protected $icon = 'empire';
  protected $badge = 1;
  protected $badgeColor = 'sky';

  /**
   * Content can be defined in a variable.
   */
  protected $text = 'Text';

  /**
   * Content can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function text(){
    $output = array('#markup' => $this->text);
    return $output;
  }

}
