<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceTopTestDropdownList extends theforceDropdownList {

  protected $listTitle = 'Sexy';

  protected function items(){
    return array(
      'asdf',
      'asdf2'
    );
  }

  protected function content(){
    return '<i class="fa fa-rebel"></i>';
  }

}
