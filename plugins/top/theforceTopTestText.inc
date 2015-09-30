<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceTopTestText extends theforceText {

  protected function content(){
    return array('#markup' => '<i class="fa fa-chrome"></i>');
  }

  protected function badge(){
    return 1;
  }

}
