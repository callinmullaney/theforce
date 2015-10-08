<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediLocalActions extends theforceMarkup {

  protected function markup(){
    // $tasks = menu_local_tasks();
    return menu_local_actions();
  }

}
