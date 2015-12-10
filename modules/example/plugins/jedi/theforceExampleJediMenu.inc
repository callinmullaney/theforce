<?php

$plugin = array(
  'title' => 'EXAMPLE | Menu',
  'configurable' => 0,
  'handler' => array(
    'file' => 'theforceExampleJediMenu.inc',
    'class' => 'theforceExampleJediMenu',
  ),
);

/**
 * Base class for a The Force top bar content.
 */
class theforceExampleJediMenu extends theforce {
  protected $itemPrefix = '';
  protected $itemSuffix = '';

  /**
   * Content can be defined in a variable.
   */
  protected $markup = '<strong>Mark</strong><em>up is really something very cool</em>';

  /**
   * Content can be defined in a method. If both a variable and a method are
   * defined then the method is used.
   */
  protected function markup(){

    // $plugin = theforce_query()->filterId('theforceMasterLink')->loadHandler()->execute();
    // dsm($plugin);

    $markup = array('#markup' => $this->markup);
    return $markup;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $tree = menu_tree_all_data('theforce', null, 3);
    $plugin = theforce_query()->filterId('theforceMasterLink')->loadHandler()->execute();
    $this->linkHandler = $plugin['handler'];
    $plugin = theforce_query()->filterId('theforceMasterDropdown')->loadHandler()->execute();
    $this->dropdownHandler = $plugin['handler'];

    $output = array();

    foreach($tree as $key => $data){
      $output[] = $this->tempLinkRender($data);
      // $has_children = !empty($data['below']);
      // $handler = $has_children ? $dropdown : $link;
      // if(isset($data['link']['options']['fawesome'])){
      //   $handler->values['icon'] = $data['link']['options']['fawesome'];
      // }
      // else{
      //   $handler->values['icon'] = 'circle-o';
      // }
      // $handler->values['text'] = $data['link']['title'];
      // $handler->values['linkPath'] = $data['link']['link_path'];
      // if($has_children){
      //   $handler->values['dropdownPrefix'] = '<span class="theforce-dropdown">';
      //   $handler->values['dropdownSuffix'] = '</span>';
      //   foreach($data['below'] as $below){
      //     $handler->values['dropdown'][] =
      //   }
      //   $handler->values['dropdown'] = 'hi';
      // }
      // $output[] = $handler->output();
    }

    return $output;
  }

  private function tempLinkRender($data){
    $has_children = !empty($data['below']);
    $handler = $has_children ? $this->dropdownHandler : $this->linkHandler;
    if($has_children){
      $handler->values['dropdownPrefix'] = '<span class="theforce-dropdown">';
      $handler->values['dropdownSuffix'] = '</span>';
      foreach($data['below'] as $below){
        $handler->values['dropdown'][] = $this->tempLinkRender($below);
      }
    }
    if(isset($data['link']['options']['fawesome'])){
      $handler->values['icon'] = $data['link']['options']['fawesome'];
    }
    else{
      $handler->values['icon'] = 'circle-o';
    }
    $handler->values['text'] = $data['link']['title'];
    $handler->values['linkPath'] = $data['link']['link_path'];
    return $handler->output();
  }

}
