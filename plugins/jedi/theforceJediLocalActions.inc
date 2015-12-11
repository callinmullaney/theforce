<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediLocalActions extends theforce {

  protected $itemElement = '';

  protected function localActionsRender(){
    $output = array();

    if($this->isManagement){
      return $output;
    }

    $actions = menu_local_actions();

    // Remove tasks from the rest of the page output.
    $current = &drupal_static('menu_local_tasks');
    $current['actions'] = array();

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
        // Label for the Local Actions group isnt necessary given the clear button labels
        // $output['#prefix'] = '<div class="theforce-label">' . t('Actions') . '</div><ul class="theforce-elements">';
        $output['#prefix'] = '<ul class="theforce-elements">';
        $output['#suffix'] = '</ul>';
      }
    }
    return $output;
  }

  protected function localActionsLinkRender($link){
    $output = array();
    $handler = $this->handler;
    $handler->info['id'] = 'theforceAction'.ucwords($link['title']);
    $attributes = array();
    $attributes['class'][] = 'theforce-item';
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
      $output['#attributes']['class'][] = 'theforce-actions';
    }
    return $output;
  }

}
