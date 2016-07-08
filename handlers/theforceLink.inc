<?php

/**
 * Link class for a The Force top bar content.
 */
class theforceLink extends theforceText {

  protected $linkPath = '';
  protected $linkAttributes = array();
  protected $linkOptions = array();
  protected $linkFragment = '';
  protected $linkQuery = array();
  protected $linkHtml = true;
  protected $linkBefore = '';
  protected $linkAfter = '';
  protected $linkPrefix = '';
  protected $linkSuffix = '';

  // Remove wrap as <a> will server as the wrap.
  protected $itemElement = '';

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'linkPath' => $this->linkPath(),
      'linkOptions' => $this->linkOptions(),
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    $form['linkPath'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => $this->values['linkPath'],
    );

    $form['linkOptions']['attributes']['target'] = array(
      '#type' => 'checkbox',
      '#title' => t('Open link in new tab'),
      '#default_value' => !empty($this->values['linkOptions']['attributes']['target']),
      '#return_value' => '_blank',
    );

    return $form;
  }

  /**
   * Link path
   */
  protected function linkPath(){
    return $this->linkPath;
  }

  /**
   * Link options
   */
  protected function linkOptions(){
    return $this->linkOptions;
  }

  /**
   * Wrap output in a link.
   *
   * @param $output
   *   A Drupal render array that will be rendered inside the link.
   * @param $path
   *   The path the link should point to.
   * @param $attributes
   *   Attributes that will be added to the link.
   * @param $html
   *
   * @return
   *   A Drupal render array.
   */
  private function renderLink($output){
    $output['#prefix'] = $this->linkBefore;
    $output['#suffix'] = $this->linkAfter;
    if($this->elementWrapperClass){
      $this->linkAttributes['class']['wrap'] = $this->elementWrapperClass;
    }
    if($this->values['ajax']){
      // Use built-in ajaxification.
      $this->linkAttributes['class']['ajax'] = 'use-theforce';
    }
    $link = array();
    $linkOptions = $this->values['linkOptions'] + array(
      'attributes' => array(),
      'html' => $this->linkHtml,
    );
    $linkOptions['attributes'] = array_filter(drupal_array_merge_deep($linkOptions['attributes'], $this->linkAttributes));

    $link['#markup'] = l(drupal_render($output), $this->values['linkPath'], $linkOptions);
    $link['#prefix'] = $this->linkPrefix;
    $link['#suffix'] = $this->linkSuffix;
    return $link;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $output = array();
    if (empty($this->values['linkPath']) || drupal_valid_path($this->values['linkPath'])) {
      $content = parent::output();
      $output['link'] = $this->renderLink($content);
      $output['#attributes']['id'] = drupal_html_id($this->camel2dashed($this->info['id']));
    }
    return $output;
  }

}
