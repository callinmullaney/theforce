<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceSideTestLink extends theforceLink {

  protected $badgeColor = 'purple';

  public function content(){
    return array('#markup' => '<i class="fa fa-bell-o"></i>');
  }

  public function badge(){
    return 2;
  }

}
