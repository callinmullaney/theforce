<?php

/**
 * Base class for a The Force top bar text.
 */
class theforceText extends theforce {

  protected $text = '';
  protected $textHtml = 0;

  // Element to wrap each item.
  protected $textPrefix = '<span class="theforce-text">';
  protected $textSuffix = '</span>';

  protected $icon = '';
  protected $iconPrefix = 'fa';

  protected $badge = 0;
  protected $badgeColor = 'green';

  /**
   * The default settings for this plugin. Settings are exportable.
   */
  public function valuesDefinition(){
    return array(
      'icon' => $this->icon,
      'text' => $this->text,
      'badge' => $this->badge,
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    if(module_exists('fawesome')){
      $form['icon'] = array(
        '#type' => 'fawesome_select',
        '#title' => t('Icon'),
        '#default_value' => $this->values['icon'],
      );
    }
    else{
      $form['icon'] = array(
        '#type' => 'textfield',
        '#title' => t('Icon'),
        '#default_value' => $this->values['icon'],
      );
    }

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => $this->values['text'],
    );

    return $form;
  }

  /**
   * Define the text that will be placed in the toolbar.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function text(){
    return $this->text;
  }

  /**
   * Define the icon that will be placed in the toolbar.
   *
   * @return
   *   An icon class name to be used as an icon in the toolbar.
   */
  protected function icon(){
    return $this->icon;
  }

  /**
   * Define the badge count that will be placed in the toolbar.
   *
   * @return
   *   A value to be placed in a badge in the toolbar.
   */
  protected function badge(){
    return $this->badge;
  }

  /**
   * Pull in the text for rendering.
   *
   * @return [type]
   */
  private function textRender(){
    $text = $this->values['text'];
    if(!is_array($text)){
      if(!$this->textHtml){
        $text = check_plain($text);
      }
      $text = array('#markup' => $text);
    }
    $text['#prefix'] = $this->textPrefix;
    $text['#suffix'] = $this->textSuffix;
    return $text;
  }

  /**
   * Pull in the icon for rendering.
   *
   * @return [type]
   */
  private function iconRender(){
    $icon = $this->values['icon'];
    if(!empty($icon)){
      $icon = array('#markup' => '<i class="theforce-icon ' . $this->iconPrefix . ' ' . $this->iconPrefix . '-' . $this->values['icon'] . '"></i>');
    }
    else{
      $icon = array();
    }
    return $icon;
  }

  /**
   * Pull in the badge for rendering.
   *
   * @return [type]
   */
  private function badgeRender(){
    $badge = $this->values['badge'];
    if(!empty($badge)){
      $badge = array('#markup' => '  <em class="theforce-badge ' . $this->badgeColor . '">' . $badge . '</em>');
    }
    else{
      $badge = array();
    }
    return $badge;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $output = array();
    $output['icon'] = $this->iconRender();
    $output['text'] = $this->textRender();
    $output['badge'] = $this->badgeRender();
    $output['#attributes']['class'][] = 'theforce-text';
    return $output;
  }

}
