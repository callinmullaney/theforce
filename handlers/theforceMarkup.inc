<?php

/**
 * Base class for a The Force top bar markup.
 */
class theforceMarkup extends theforce {

  protected $markup = '';

  /**
   * The default values for this plugin.
   */
  public function valuesDefinition(){
    return array(
      'markup' => $this->markup,
    ) + parent::valuesDefinition();
  }

  /**
   * The settings form.
   */
  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    $form['markup'] = array(
      '#type' => 'textarea',
      '#title' => t('Markup'),
      '#default_value' => $this->values['markup'],
      '#rows' => 3,
    );

    return $form;
  }

  /**
   * Define the markup that will be placed in the toolbar.
   *
   * @return
   *   A string or Drupal render array.
   */
  protected function markup(){
    return $this->markup;
  }

  /**
   * Pull in the markup for rendering.
   *
   * @return [type]
   */
  private function markupRender(){
    $markup = $this->values['markup'];
    if(!is_array($markup)){
      $markup = array('#markup' => $markup);
    }
    return $markup;
  }

  /**
   * Render output.
   *
   * @return
   *   A Drupal render array.
   */
  protected function output(){
    $output = array();
    $output['markup'] = $this->markupRender();
    return $output;
  }

}
