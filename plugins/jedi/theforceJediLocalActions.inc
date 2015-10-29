<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediLocalActions extends theforce {

  protected $itemElement = '';

  protected function localActionsRender(){
    $output = array();

    $actions = menu_local_actions();

    // Remove tasks from the rest of the page output.
    $tasks = &drupal_static('menu_local_tasks');
    unset($tasks['actions']);

    if(empty($actions)){
      $this->access = false;
    }
    else{
      $plugin = theforce_query()->filterId('theforceMasterLink')->loadHandler()->execute();
      $this->handler = $plugin['handler'];

      foreach($actions as $key => $data){
        $output[$key] = $this->localActionsLinkRender($data['#link']);
      }

      if(!empty($output)){
        $output['#prefix'] = '<div class="theforce-label">' . t('Actions') . '</div><ul class="theforce-actions theforce-elements">';
        $output['#suffix'] = '</ul>';
      }
    }
    return $output;

    return $output;
  }

  protected function localActionsLinkRender($link){
    $output = array();
    $handler = $this->handler;
    $attributes = array();
    $icon = 'angle-right';
    if(function_exists('fawesome_match') && $match = fawesome_match($link['title'])){
      $icon = $match['icon'];
    }
    $handler->values['icon'] = $icon;
    $handler->values['text'] = $link['title'];
    $handler->values['linkPath'] = $link['href'];
    $handler->values['linkOptions'] = $link['localized_options'];
    $output = $handler->render();
    $output['#prefix'] = '<li'.drupal_attributes($attributes).'>';
    $output['#suffix']  = '</li>';
    return $output;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $output = array();
    if($actions = $this->localActionsRender()){
      $output['actions'] = $actions;
    }
    return $output;
  }

}
