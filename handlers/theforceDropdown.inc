<?php

/**
 * Dropdown class for a The Force top bar content.
 */
class theforceDropdown extends theforceLink {

  protected $dropdown = '';
  protected $dropdownAttributes = array('class' => array('padded'));
  protected $linkAttributes = array('class' => array('theforce-dropdown-trigger'));

  /**
   * The content that will be placed in the dropdown.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function dropdown(){
    return $this->dropdown;
  }

  /**
   * Pull in the content for rendering.
   *
   * @return [type]
   */
  private function dropdownRender(){
    $output = array();
    $content = $this->dropdown();
    if(!empty($content)){
      $this->linkSuffix = ' <i class="fa fa-angle-down"></i>';
      if(!is_array($content)){
        $content = array('#markup' => $content);
      }
      $this->dropdownAttributes['class'][] = 'theforce-dropdown-content';
      $output = array(
        '#prefix' => '<div' . drupal_attributes($this->dropdownAttributes) . '>',
        '#suffix' => '</div>',
      );
      $output['content'] = $content;
    }
    return $output;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  public function render(){
    $dropdown = $this->dropdownRender();
    $output = parent::render();
    $output['dropdown'] = $dropdown;
    $output['#attributes']['class'][] = 'theforce-dropdown';
    return $output;
  }
}
