<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediLocalTabs extends theforce {

  protected $itemElement = '';

  protected function localTabsRender(){
    $output = array();

    if($this->isManagement){
      return $output;
    }

    $primary = menu_primary_local_tasks();
    $secondary = menu_secondary_local_tasks();

    // Remove tasks from the rest of the page output.
    $current = &drupal_static('menu_local_tasks');
    $current['tabs'] = array();

    if(empty($primary) && empty($secondary)){
      // Disable access of this plugin if no tabs exist.
      $this->access = false;
    }
    else{
      $plugin = theforce_query()->filterId('theforceMasterLink')->loadHandler()->execute();
      $this->handler = $plugin['handler'];

      foreach($primary as $key => $data){
        $output[$key] = $this->localTabsLinkRender($data['#link']);
        if(!empty($data['#active']) && !empty($secondary)){
          $childOutput = array();
          foreach($secondary as $childKey => $childData){
            $childOutput[$childKey] = $this->localTabsLinkRender($childData['#link']);
          }
          if(!empty($childOutput)){
            $childOutput['#prefix'] = '<ul class="theforce-tabs-secondary theforce-elements">';
            $childOutput['#suffix'] = '</ul>';
            $output[$key]['secondary'] = $childOutput;
          }
        }
      }

      if(!empty($output)){
        // Label for the Local Tabs group isnt necessary given the clear button labels
        // $output['#prefix'] = '<div class="theforce-label">' . t('Tasks') . '</div><ul class="theforce-tabs-primary theforce-elements">';
        $output['#prefix'] = '<ul class="theforce-tabs-primary theforce-elements">';
        $output['#suffix'] = '</ul>';
      }
    }
    return $output;
  }

  protected function localTabsLinkRender($link){
    $output = array();
    $handler = $this->handler;
    $handler->info['id'] = 'theforceTab'.ucwords($link['title']);
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
    if($tabs = $this->localTabsRender()){
      $output['tabs'] = $tabs;
      $output['#attributes']['class'][] = 'theforce-tabs';
    }
    return $output;
  }

}
