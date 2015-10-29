<?php

/**
 * Dropdown class for a The Force top bar content.
 */
class theforceDropdown extends theforceLink {

  protected $dropdown = '';
  protected $dropdownAttributes = array('class' => array('padded'));
  protected $dropdownPrefix = '';
  protected $dropdownSuffix = '';

  protected $linkAttributes = array('class' => array('theforce-dropdown-trigger'));

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'dropdown' => $this->dropdown,
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    // $form['linkPath']['#access'] = false;
    unset($form['linkPath']);

    $form['dropdown'] = array(
      '#type' => 'textarea',
      '#title' => t('Dropdown'),
      '#default_value' => $this->values['dropdown'],
      '#rows' => 3,
    );

    $form['ajax'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Ajax'),
      '#default_value' => $this->values['ajax'],
      '#weight' => 100,
    );

    return $form;
  }

  /**
   * The content that will be placed in the dropdown.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function dropdown(){
    return $this->values['ajax'] ? true : $this->dropdown;
  }

  /**
   * The content that will be placed in the dropdown during ajax request.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function dropdownAjax(){
    return $this->dropdown;
  }

  /**
   * Pull in the content for rendering.
   *
   * @return [type]
   */
  private function dropdownRender(){
    $output = array();
    $content = $this->values['dropdown'];

    if(!empty($content)){
      $this->linkAfter = ' <i class="fa fa-angle-down"></i>';
      $this->dropdownAttributes['class'][] = 'theforce-dropdown-content';

      if($this->values['ajax']){
        // ID used by theforce_html_ajax() to place content.
        $this->dropdownAttributes['id'] = 'ajax-' . $this->info['id'];
        // No loading bar.
        $this->linkAttributes['data-progress'] = 'none';
        // Run ajax request only once.
        $this->linkAttributes['data-once'] = 1;
        // Set URL
        $this->values['linkPath'] = 'system/theforce/html/' . $this->info['id'];
        $content = '<div class="theforce-dropdown-loader"><img src="' . url(drupal_get_path('module', 'theforce')) . '/images/loader.svg" /></div>';
      }

      if(!is_array($content)){
        $content = array('#markup' => $content);
      }
      $output = array(
        '#prefix' => '<div' . drupal_attributes($this->dropdownAttributes) . '>',
        '#suffix' => '</div>',
      );
      $output['content'] = $content;
    }
    return $output;
  }

  /**
   * Pull in the content for rendering via ajax.
   *
   * @return [type]
   */
  private function dropdownRenderAjax(){
    $output = array();
    $content = $this->values['dropdown'];
    if($content){
      if(!is_array($content)){
        $content = array('#markup' => $content);
      }
      $output = $content;
    }
    return $output;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $dropdown = $this->dropdownRender();
    $output = parent::output();
    $output['dropdown'] = $dropdown;
    $output['#attributes']['class'][] = 'theforce-dropdown';
    $output['#prefix'] = $this->dropdownPrefix;
    $output['#suffix'] = $this->dropdownSuffix;
    return $output;
  }

  /**
   * Render output for ajax callback.
   */
  protected function outputAjax(){
    return $this->dropdownRenderAjax();
  }
}
