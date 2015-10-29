<?php

/**
 * Base class for a The Force top bar content.
 */
class theforce {

  public $info;
  public $settings;
  public $values;
  public $force;

  // ELement to wrap each item.
  protected $itemElement = 'span';
  protected $itemElementAttributes = array();

  protected $ajax = 0;
  protected $access = true;
  protected $isManagement = false;
  protected $elementWrapperClass = 'theforce-element';

  /**
   * Constructor for top bar content.
   */
  public function __construct($info, $args = array()) {
    $this->info = $info;
    $this->settings = drupal_array_merge_deep($this->valuesDefinition(), $this->info['settings']);
    $this->values = drupal_array_merge_deep($this->valuesProcess(), $this->info['settings']);
    $this->isManagement = theforce_is_management();
    foreach(theforce_regions() as $region => $locations){
      $this->force[$region] = false;
    }
  }

  /**
   * The default values for this plugin.
   */
  public function valuesDefinition(){
    return array(
      'ajax' => $this->ajax,
    );
  }

  /**
   * Prepare values and call methods for processing.
   */
  private function valuesProcess(){
    $this->values = $this->settings;
    foreach($this->values as $key => &$value){
      if(method_exists($this, $key)){
        $value = $this->{$key}();
      }
    }
    return $this->values;
  }

  /**
   * Prepare values during an ajax request. We do this so that ajax values
   * are not processed during regular requests.
   */
  private function valuesProcessAjax(){
    foreach($this->values as $key => &$value){
      $key .= 'Ajax';
      if(method_exists($this, $key)){
        $value = $this->{$key}();
      }
    }
    return $this->values;
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    return $form;
  }

  /**
   * Output that will be rendered.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $output = array();
    return $output;
  }

  /**
   * Output that will be rendered for AJAX requests.
   *
   * @return
   *   A Drupal render array.
   */
  protected function outputAjax(){
    $output = array();
    return $output;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  final public function render(){
    $output = $this->output();
    // Check access
    if(!$this->access && !$this->isManagement){
      return null;
    }
    if(empty($output) && $this->isManagement){
      $output['#markup'] = '<div class="theforce-label">' . t('@title Placeholder', array('@title' => $this->info['title'])) . '</div>';
    }
    if(!empty($this->itemElement)){
      $this->itemElementAttributes['class'][] = $this->elementWrapperClass;
      $output['#prefix'] = '<' . $this->itemElement . drupal_attributes($this->itemElementAttributes) . '>';
      $output['#suffix'] = '</' . $this->itemElement . '>';
    }
    return $output;
  }

  /**
   * Render output for ajax callback.
   */
  final public function renderAjax(){
    $this->values = drupal_array_merge_deep($this->valuesProcessAjax(), $this->info['settings']);
    return $this->outputAjax();
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
