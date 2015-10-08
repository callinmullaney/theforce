<?php

/**
 * Link class for a The Force top bar content.
 */
class theforceLink extends theforceText {

  protected $linkPath = '';
  protected $linkAttributes = array();
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
   * The default values for this plugin.
   */
  public function valuesDefinition(){
    return array(
      'linkPath' => $this->linkPath(),
      'linkAttributes' => $this->linkAttributes,
      'linkFragment' => $this->linkFragment,
      'linkQuery' => $this->linkQuery,
      'linkHtml' => $this->linkHtml,
      'linkBefore' => $this->linkBefore,
      'linkAfter' => $this->linkAfter,
      'linkPrefix' => $this->linkPrefix,
      'linkSuffix' => $this->linkSuffix,
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

    return $form;
  }

  /**
   * Link path
   */
  protected function linkPath(){
    return $this->linkPath;
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
    $output['#prefix'] = $this->values['linkBefore'];
    $output['#suffix'] = $this->values['linkAfter'];
    $this->values['linkAttributes']['class'][] = 'theforce-element';
    $link = array();
    $link['#markup'] = l(drupal_render($output), $this->values['linkPath'], array(
      'attributes' => $this->values['linkAttributes'],
      'fragment' => $this->values['linkFragment'],
      'query' => $this->values['linkQuery'],
      'html' => $this->values['linkHtml'],
    ));
    $link['#prefix'] = $this->values['linkPrefix'];
    $link['#suffix'] = $this->values['linkSuffix'];
    return $link;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $content = parent::output();
    $output = array();
    $output['link'] = $this->renderLink($content);
    return $output;
  }

}
