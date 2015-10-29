<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediMenuManage extends theforceSide {

  protected $menus = array();

  protected $text = 'Menus';
  protected $icon = 'list';

  protected $sideTitle = 'Manage Menus';
  protected $sideDescription = 'Select the menu to manage.';

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'menus' => $this->menus,
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    unset($form['linkPath']);

    $form['menus'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Menus'),
      '#default_value' => $this->values['menus'],
      '#options' => menu_get_menus(),
    );

    return $form;
  }

  public function access(){
    if(module_exists('menu_admin_per_menu')){
      return _menu_admin_per_menu_access();
    }
    return user_access('administer menu');
  }

  /**
   * Pull in the markup for rendering.
   *
   * @return [type]
   */
  protected function side(){
    $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
    $menus = array_filter($this->values['menus']);
    foreach ($result as $menu) {
      if(empty($menus) || !empty($menus[$menu['menu_name']])){
        if(module_exists('menu_admin_per_menu') && !_menu_admin_per_menu_menu_link_access($menu)){
          continue;
        }
        $icon = 'list';
        $items[] = l('<i class="'. $this->iconPrefix . ' ' . $this->iconPrefix . '-' . $icon . '"></i> ' . $menu['title'], 'admin/structure/menu/manage/' . $menu['menu_name'] . '/edit', array('html' => true, 'attributes' => array('class' => array('use-theforce'))));
      }
    }

    return array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array(
        'class' => array('theforce-grid'),
      ),
    );


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
