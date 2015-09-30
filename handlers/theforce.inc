<?php

/**
 * Base class for a The Force top bar content.
 */
class theforce {

  public $info;
  public $settings;

  /**
   * Constructor for top bar content.
   */
  public function __construct($type, $args = array()) {
    $this->info = $args[0];
    $this->settings = drupal_array_merge_deep($this->settingsDefinition(), $args[1]);
  }

  /**
   * The default settings for this plugin.
   */
  public function settingsDefinition(){
    return array();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    return $form;
  }

  /**
   * Returns the user photo.
   */
  public function userImage($style_name = 'theforce_top') {
    global $user;
    $account = user_load($user->uid);
    $image = array();
    $uri = '';
    $wrapper = entity_metadata_wrapper('user', $account);
    if(isset($wrapper->field_image)){
      $image = $wrapper->field_image->value();
    }
    elseif(isset($wrapper->field_image_asset) && isset($wrapper->field_image_asset->field_image)){
      $image = $wrapper->field_image_asset->field_image->value();
    }
    if(!empty($image)){
      $uri = $image['uri'];
    }
    else{
      $uri = 'public://theforce/profile.png';
    }
    return array(
      '#theme' => 'image_style',
      '#style_name' => $style_name,
      '#path' => $uri,
      '#attributes' => array(
        'class' => array(
          'theforce-avatar'
        ),
      ),
    );
  }

}
