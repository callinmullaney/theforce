<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceTopUser extends theforceDropdownList {

  // protected $listStyle = 'mini';

  /**
   * The default settings for this plugin.
   */
  public function settingsDefinition(){
    return array(
      'show_avatar' => 1,
    );
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    $form['show_avatar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Avatar'),
      '#default_value' => $this->settings['show_avatar'],
    );

    return $form;
  }

  /**
   * Content that will be placed in the toolbar.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function content(){
    global $user;
    $output = parent::content();
    $account = user_load($user->uid);
    if($this->settings['show_avatar']){
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
  protected function items(){
    global $user;
    $output = array();

    $output[] = l('<i class="fa fa-user"></i> Profile', 'user/' . $user->uid, array('html' => TRUE));
    $output[] = l('<i class="fa fa-edit"></i> Settings', 'user/' . $user->uid . '/edit', array('html' => TRUE));
    $output[] = l('<i class="fa fa-sign-out"></i> Logout', 'user/logout', array('html' => TRUE));

    return $output;
  }



}
