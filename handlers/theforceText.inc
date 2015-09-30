<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceText extends theforce {

  protected $content = '';
  protected $contentPrefix = '<span>';
  protected $contentSuffix = '</span>';
  protected $badge = null;
  protected $badgeColor = 'green';

  /**
   * Define the content that will be placed in the toolbar.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function content(){
    return $this->content;
  }

  /**
   * Define the badge text that will be placed in the toolbar.
   *
   * @return
   *   A value to be placed in a badge in the toolbar.
   */
  protected function badge(){
    return $this->badge;
  }

  /**
   * Pull in the content for rendering.
   *
   * @return [type]
   */
  private function contentRender(){
    $content = $this->content();
    if(!is_array($content)){
      $content = array('#markup' => $content);
    }
    return $content;
  }

  /**
   * Pull in the content for rendering.
   *
   * @return [type]
   */
  private function badgeRender(){
    $content = $this->badge();
    if(!empty($content)){
      $content = array('#markup' => '  <em class="theforce-badge ' . $this->badgeColor . '">' . $content . '</em>');
    }
    else{
      $content = array();
    }
    return $content;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  public function render(){
    $output = array();
    $output['content'] = $this->contentRender();
    $output['content']['#prefix'] = $this->contentPrefix;
    $output['content']['#suffix'] = $this->contentSuffix;
    $output['badge'] = $this->badgeRender();
    return $output;
  }

}
