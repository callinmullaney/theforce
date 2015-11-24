<?php

/**
 * Content creation widget.
 */
class theforceJediCreate extends theforceSide {

  protected $text = 'Create';
  protected $icon = 'plus-circle';

  protected $sideTitle = 'Create Content';
  protected $sideDescription = 'Select the type of content to create.';

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'nodeCreateInclude' => array(),
      'nodeCreateExclude' => array(),
      'assetCreateInclude' => array(),
      'assetCreateExclude' => array(),
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){

    $options = array();
    foreach(node_type_get_types() as $type => $data){
      $options[$type] = $data->name;
    }

    $form['nodeCreateInclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Content Types'),
      '#description' => t('If nothing is selected, all content types will be included.'),
      '#default_value' => $this->values['nodeCreateInclude'],
      '#options' => $options,
    );

    $form['nodeCreateExclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Exclude Content Types'),
      '#description' => t('If nothing is selected, no content types will be excluded.'),
      '#default_value' => $this->values['nodeCreateExclude'],
      '#options' => $options,
    );

    if(module_exists('asset')){
      $options = array();
      foreach (asset_get_types() as $type) {
        $options[$type->type] = $type->title;
      }

      $form['assetCreateInclude'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Include Asset Types'),
        '#description' => t('If nothing is selected, all content types will be included.'),
        '#default_value' => $this->values['assetCreateInclude'],
        '#options' => $options,
      );

      $form['assetCreateExclude'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Exclude Asset Types'),
        '#description' => t('If nothing is selected, no content types will be excluded.'),
        '#default_value' => $this->values['assetCreateExclude'],
        '#options' => $options,
      );
    }

    return $form;
  }

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

  public function access(){
    return _node_add_access();
  }

  /**
   * Pull in the markup for rendering.
   *
   * @return [type]
   */
  protected function side(){
    $items = $this->getItems('node', 'node/add');
    if(module_exists('asset')){
      $items += $this->getItems('asset', 'admin/content/assets/add');
    }
    ksort($items);

    return array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array(
        'class' => array('theforce-grid'),
      ),
    );
  }

  protected function getItems($type, $url){
    $item = menu_get_item($url);
    $content = system_admin_menu_block($item);

    $include = array_filter($this->values[$type . 'CreateInclude']);
    $include = !empty($include) ? $this->values[$type . 'CreateInclude'] : null;
    $exclude = array_filter($this->values[$type . 'CreateExclude']);
    $exclude = !empty($this->values[$type . 'CreateExclude']) ? $this->values[$type . 'CreateExclude'] : null;

    $items = array();
    foreach($content as $key => $data){
      $parts = explode('/', $data['link_path']);
      $bundle = str_replace('-','_',end($parts));
      if($include){
        if(empty($include[$bundle])){
          continue;
        }
      }
      if($exclude){
        if(!empty($exclude[$bundle])){
          continue;
        }
      }
      if($type == 'asset' && ($asset_type = asset_type_load($bundle))){
        $icon = $asset_type->data['icon'];
      }
      else{
        $icon = !empty($data['options']['fawesome']) ? $data['options']['fawesome'] : 'square-o';
      }
      $items[$key] = l('<i class="'. $this->iconPrefix . ' ' . $this->iconPrefix . '-' . $icon . '"></i> ' . $data['title'], $data['link_path'], array('html' => true));
    }
    return $items;
  }

}
