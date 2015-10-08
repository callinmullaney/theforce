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
   * The default values for this plugin.
   */
  public function valuesDefinition(){
    return array(
      'dropdown' => $this->dropdown,
      'dropdownAttributes' => $this->dropdownAttributes,
      'dropdownPrefix' => $this->dropdownPrefix,
      'dropdownSuffix' => $this->dropdownSuffix,
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
      $this->values['linkAfter'] = ' <i class="fa fa-angle-down"></i>';
      $this->values['dropdownAttributes']['class'][] = 'theforce-dropdown-content';

      if($this->values['ajax']){
        // ID used by theforce_content_ajax() to place content.
        $this->values['dropdownAttributes']['id'] = 'ajax-' . $this->info['id'];
        // Use built-in ajaxification.
        $this->values['linkAttributes']['class'][] = 'use-theforce';
        // No loading bar.
        $this->values['linkAttributes']['data-progress'] = 'none';
        // Run ajax request only once.
        $this->values['linkAttributes']['data-once'] = 1;
        $this->values['linkPath'] = 'system/theforce/content/' . $this->info['id'];
        $content = '<div class="theforce-dropdown-loader"><img src="' . url(drupal_get_path('module', 'theforce')) . '/images/loader.svg" /></div>';
      }

      if(!is_array($content)){
        $content = array('#markup' => $content);
      }
      $output = array(
        '#prefix' => '<div' . drupal_attributes($this->values['dropdownAttributes']) . '>',
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
    $output['#prefix'] = $this->values['dropdownPrefix'];
    $output['#suffix'] = $this->values['dropdownSuffix'];
    return $output;
  }

  /**
   * Render output for ajax callback.
   */
  protected function outputAjax(){
    return $this->dropdownRenderAjax();
  }
}
