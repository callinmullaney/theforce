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
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'nodeCreateInclude' => array(),
      'nodeCreateExclude' => array(),
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
    $item = menu_get_item('node/add');
    $content = system_admin_menu_block($item);

    $include = array_filter($this->values['nodeCreateInclude']);
    $include = !empty($include) ? $this->values['nodeCreateInclude'] : null;
    $exclude = array_filter($this->values['nodeCreateExclude']);
    $exclude = !empty($this->values['nodeCreateExclude']) ? $this->values['nodeCreateExclude'] : null;

    $items = array();
    foreach($content as $data){
      $parts = explode('/', $data['link_path']);
      $type = str_replace('-','_',$parts[2]);
      if($include){
        if(empty($include[$type])){
          continue;
        }
      }
      if($exclude){
        if(!empty($exclude[$type])){
          continue;
        }
      }
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
