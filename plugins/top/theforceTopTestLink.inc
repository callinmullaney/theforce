<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceTopTestLink extends theforceLink {

  protected $badgeColor = 'purple';

  protected function content(){
    return array('#markup' => '<i class="fa fa-bell-o"></i>');
  }

  protected function badge(){
    return 2;
  }

}
