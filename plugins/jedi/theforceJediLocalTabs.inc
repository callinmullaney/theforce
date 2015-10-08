<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediLocalTabs extends theforceMarkup {

  protected function markup(){
    // $tasks = menu_local_tasks();
    return array(
      '#theme' => 'menu_local_tasks',
      '#primary' => menu_primary_local_tasks(),
      '#secondary' => menu_secondary_local_tasks(),
    );
    return $markup;
  }

}
