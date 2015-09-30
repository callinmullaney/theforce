<?php

/**
 * Dropdown class for a The Force top bar content.
 */
class theforceDropdownList extends theforceDropdown {

  protected $items = array();
  protected $listTitle = '';

  // List style (full, mini)
  protected $listStyle = 'full';

  // Remove the padded class as the list has it's own padding.
  protected $dropdownAttributes = array();

  /**
   * The content that will be placed in the item list.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function items(){
    return $this->items;
  }

  private function listRender(){
    $attributes = array();
    $attributes['class'][] = 'theforce-list';
    $attributes['class'][] = $this->listStyle;

    $output = array(
      '#theme' => 'item_list',
      '#title' => $this->listTitle,
      '#items' => $this->items(),
      '#attributes' => $attributes,
    );
    return $output;
  }

  /**
   * The content that will be placed in the dropdown.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function dropdown(){
    return $this->listRender();
  }

}
