<?php

/**
 * Base class for a The Force top bar markup.
 */
class theforceMenu extends theforce {

  protected $menuId = 'theforce';

  // Remove wrap as <ul> will server as the wrap.
  protected $itemElement = '';

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function settingsDefinition(){
    return array(
      'menuId' => $this->menuId,
    ) + parent::settingsDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    $available_menus = menu_get_menus();
    $form['menuId'] = array(
      '#type' => 'select',
      '#title' => t('Menu'),
      '#options' => $available_menus,
      '#default_value' => $this->values['menuId'],
      '#rows' => 3,
    );

    return $form;
  }

  /**
   * Pull in the markup for rendering.
   *
   * @return [type]
   */
  private function menuRender(){
    $menuId = $this->values['menuId'];
    $tree = menu_tree_all_data($menuId, null, 3);

    // We want to run all links through theforceLink for rendering.
    $plugin = theforce_query()->filterId('theforceMasterLink')->loadHandler()->execute();
    $this->linkHandler = $plugin['handler'];

    $markup = $this->menuPrepare($tree);
    return $markup;
  }




  private function menuPrepare($tree){
    $build = array();
    $items = array();

    // Pull out just the menu links we are going to render.
    foreach ($tree as $data) {
      if ($data['link']['access'] && !$data['link']['hidden']) {
        $items[] = $data;
      }
    }

    $router_item = menu_get_item();

    foreach($items as $i => $data){
      $attributes = array();
      $has_children = 0;
      if ($data['link']['has_children'] && $data['below']) {
        $attributes['class'][] = 'theforce-menu-expand';
        $has_children = 1;
      }
      $handler = $this->linkHandler;
      if(isset($data['link']['options']['fawesome'])){
        $handler->values['icon'] = $data['link']['options']['fawesome'];
      }
      else{
        $handler->values['icon'] = 'circle-o';
      }
      $handler->values['text'] = $data['link']['title'];
      $handler->values['linkPath'] = $data['link']['link_path'];
      $build[$data['link']['mlid']] = $handler->render();

      if($has_children){
        $build[$data['link']['mlid']]['below'] = $this->menuPrepare($data['below']);
      }


      $build[$data['link']['mlid']]['#prefix'] = '<li'.drupal_attributes($attributes).'>';
      $build[$data['link']['mlid']]['#suffix']  = '</li>';
    }

    if ($build) {
      $build['#prefix'] = '<ul class="theforce-menu">';
      $build['#suffix'] = '</ul>';
    }

    return $build;
  }




  /**
   * Build the menu tree into render array.
   *
   * @see menu_tree_output().
   */
  private function menuBuild($tree){
    $build = array();
    $items = array();

    // Pull out just the menu links we are going to render.
    foreach ($tree as $data) {
      if ($data['link']['access'] && !$data['link']['hidden']) {
        $items[] = $data;
      }
    }

    $router_item = menu_get_item();
    $num_items = count($items);
    foreach ($items as $i => $data) {
      $class = array();
      // Set a class for the <li>-tag. Since $data['below'] may contain local
      // tasks, only set 'expanded' class if the link also has children within
      // the current menu.
      if ($data['link']['has_children'] && $data['below']) {
        $class[] = 'theforce-below';
      }
      elseif ($data['link']['has_children']) {
        // $class[] = 'collapsed';
      }
      else {
        // $class[] = 'leaf';
      }
      $data['link']['localized_options']['attributes']['class'][] = 'theforce-element';
      // Set a class if the link is in the active trail.
      if ($data['link']['in_active_trail']) {
        $class[] = 'active-trail';
        $data['link']['localized_options']['attributes']['class'][] = 'active-trail';
      }
      // Normally, l() compares the href of every link with $_GET['q'] and sets
      // the active class accordingly. But local tasks do not appear in menu
      // trees, so if the current path is a local task, and this link is its
      // tab root, then we have to set the class manually.
      if ($data['link']['href'] == $router_item['tab_root_href'] && $data['link']['href'] != $_GET['q']) {
        $data['link']['localized_options']['attributes']['class'][] = 'active';
      }

      // Allow menu-specific theme overrides.
      $element['#theme'] = 'theforce_menu_link__' . strtr($data['link']['menu_name'], '-', '_');
      $element['#attributes']['class'] = $class;
      $element['#title'] = $data['link']['title'];
      $element['#href'] = $data['link']['href'];
      $element['#localized_options'] = !empty($data['link']['localized_options']) ? $data['link']['localized_options'] : array();
      $element['#below'] = $data['below'] ? $this->menuBuild($data['below']) : $data['below'];
      $element['#original_link'] = $data['link'];
      // Index using the link's unique mlid.
      $build[$data['link']['mlid']] = $element;
    }
    if ($build) {
      // Make sure drupal_render() does not re-order the links.
      $build['#sorted'] = TRUE;
      // Add the theme wrapper for outer markup.
      // Allow menu-specific theme overrides.
      $build['#theme_wrappers'][] = 'theforce_menu__' . strtr($data['link']['menu_name'], '-', '_');
    }
    return $build;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $output = array();
    $output['menu'] = $this->menuRender();
    return $output;
  }

}
