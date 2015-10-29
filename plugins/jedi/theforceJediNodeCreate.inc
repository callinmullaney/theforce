<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediNodeCreate extends theforceSide {

  protected $text = 'Create';
  protected $icon = 'plus-circle';

  protected $sideTitle = 'Create Content';
  protected $sideDescription = 'Select the content type to create.';

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $this->linkAttributes['class'][] = 'theforce-node-create';
    return parent::output();
  }

  /**
   * Pull in the markup for rendering.
   *
   * @return [type]
   */
  protected function side(){
    $item = menu_get_item('node/add');
    $content = system_admin_menu_block($item);

    $items = array();
    foreach($content as $data){
      $icon = !empty($data['options']['fawesome']) ? $data['options']['fawesome'] : 'square-o';
      $items[] = l('<i class="'. $this->iconPrefix . ' ' . $this->iconPrefix . '-' . $icon . '"></i> ' . $data['title'], $data['link_path'], array('html' => true));
    }

    return array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array(
        'class' => array('theforce-grid'),
      ),
    );
  }

}
