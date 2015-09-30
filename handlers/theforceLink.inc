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
  protected $linkPrefix = '';
  protected $linkSuffix = '';

  // Remove wrap as <a> will server as the wrap.
  protected $contentPrefix = '';
  protected $contentSuffix = '';

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
    return array(
      '#theme' => 'link',
      '#text' => $this->linkPrefix . drupal_render($output) . $this->linkSuffix,
      '#path' => $this->linkPath(),
      '#options' => array(
        'attributes' => $this->linkAttributes,
        'fragment' => $this->linkFragment,
        'query' => $this->linkQuery,
        'html' => $this->linkHtml,
      ),
    );
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  public function render(){
    $content = parent::render();
    $output = array();
    $output['link'] = $this->renderLink($content);
    return $output;
  }

}
