<?php

/**
 * Dropdown class for a The Force top bar content.
 */
class theforceDropdownList extends theforceDropdown {

  protected $listItems = array();
  protected $listTitle = '';

  // List style (full, mini)
  protected $listStyle = 'full';

  // Remove the padded class as the list has it's own padding.
  protected $dropdownAttributes = array();

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'listItems' => $this->listItems,
      'listStyle' => $this->listStyle,
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    unset($form['dropdown']);

    $form['listStyle'] = array(
      '#type' => 'radios',
      '#title' => t('Style'),
      '#options' => array('full' => t('Full'), 'mini' => t('Mini')),
      '#default_value' => $this->values['listStyle'],
    );

    return $form;
  }

  /**
   * The content that will be placed in the item list.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function listItems(){
    return $this->listItems;
  }

  /**
   * Pull in the list for rendering.
   *
   * @return [type]
   */
  private function listItemsRender(){
    $attributes = array();
    $attributes['class'][] = 'theforce-list';
    $attributes['class'][] = $this->values['listStyle'];

    $output = array(
      '#theme' => 'item_list',
      '#title' => $this->listTitle,
      '#items' => $this->values['listItems'],
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
    return $this->listItemsRender();
  }

}
