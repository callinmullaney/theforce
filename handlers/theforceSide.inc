<?php

/**
 * Link class for a The Force top bar content.
 */
class theforceSide extends theforceLink {

  protected $text = 'Control';
  protected $icon = 'cog';
  protected $ajax = 1;

  protected $sideTitle = '';
  protected $sideDescription = '';

  protected $linkAttributes = array('class' => array('theforce-side-trigger'));

  /**
   * Constructor for top bar content.
   */
  public function __construct($info, $args = array()) {
    parent::__construct($info, $args);
    // Make sure sidebar is visible.
    $this->force['side'] = true;
  }

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'sideTitle' => $this->sideTitle,
      'sideDescription' => $this->sideDescription,
    ) + parent::valuesDefinition();
  }

  /**
   * Link path
   */
  protected function sideTitle(){
    return t($this->sideTitle);
  }

  /**
   * Link path
   */
  protected function sideDescription(){
    return t($this->sideDescription);
  }

  /**
   * Pull in the markup for rendering.
   *
   * @return [type]
   */
  protected function side(){
    return '';
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $this->values['linkPath'] = 'system/theforce/side/' . $this->info['id'];
    return parent::output();
  }

  /**
   * Render output for ajax callback.
   */
  protected function outputAjax(){
    $output = $this->side();
    if(!is_array($output)){
      $output = array('#markup' => $output);
    }
    return $output;
  }

}
