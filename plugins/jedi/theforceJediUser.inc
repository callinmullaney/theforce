<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceJediUser extends theforceDropdownList {

  // protected $listStyle = 'mini';
  protected $textHtml = 1;
  protected $textPrefix = '';
  protected $textSuffix = '';

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'show_avatar' => 1,
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    // $form = parent::settingsForm($form, $form_state);

    $form['show_avatar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Avatar'),
      '#default_value' => $this->values['show_avatar'],
    );

    return $form;
  }

  /**
   * Content that will be placed in the toolbar.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function text(){
    global $user;
    $output = parent::text();
    $account = user_load($user->uid);
    if($this->values['show_avatar']){
      $output['image'] = $this->userImage();
    }
    $output['name']['#markup'] = '<span>' . $account->name . '</span>';
    return $output;
  }

  /**
   * The content that will be placed in the dropdown list.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function listItems(){
    global $user;
    $output = array();

    if(drupal_valid_path('user/' . $user->uid)){
      $output[] = l('<i class="fa fa-user"></i> Profile', 'user/' . $user->uid, array('html' => TRUE));
    }
    if(drupal_valid_path('user/' . $user->uid . '/edit')){
      $output[] = l('<i class="fa fa-edit"></i> Settings', 'user/' . $user->uid . '/edit', array('html' => TRUE));
    }
    $output[] = l('<i class="fa fa-sign-out"></i> Logout', 'user/logout', array('html' => TRUE));

    return $output;
  }



}
