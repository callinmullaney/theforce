<?php

/**
 * Base class for a The Force top bar content.
 */
class theforceMasterView extends theforceDropdown {

  protected $ajax = 1;

  public function settingsDefinition(){
    return array(
      'view' => '',
      'viewDisplay' => '',
    );
  }

  public function settingsForm($form, &$form_state){
    $form = parent::settingsForm($form, $form_state);

    unset($form['dropdown']);

    if(!empty($form_state['values'])){
      $parents = $form_state['triggering_element']['#parents'];
      $this->values['view'] = drupal_array_get_nested_value($form_state['values'], $parents);
    }

    $settings_id = 'theforce-' . $this->info['name'];
    $options = views_get_views_as_options(TRUE, 'all', NULL, FALSE, TRUE);
    $form['view'] = array(
      '#type' => 'select',
      '#title' => t('View'),
      '#description' => t('Select a view to embed.'),
      '#empty_option' => t('- Select -'),
      '#default_value' => $this->values['view'],
      '#options' => $options,
      '#ajax' => array(
        'callback' => 'theforceMasterViewDisplayRefresh',
        'wrapper' => $settings_id,
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    if($this->values['view']){
      $view = views_get_view($this->values['view']);
      $options = array();
      foreach ($view->display as $name => $display) {
        $options[$name] = $display->display_title;
      }

      $form['viewDisplay'] = array(
        '#type' => 'select',
        '#title' => t('Display'),
        '#description' => t('Select a view display to use.'),
        '#empty_option' => t('- Select -'),
        '#default_value' => $this->values['viewDisplay'],
        '#options' => $options,
        '#prefix' => '<div id="' . $settings_id . '">',
        '#suffix' => '</div>',
      );
    }
    else{
      $form['viewDisplay'] = array(
        '#type' => 'container',
        '#id' => $settings_id,
      );
    }
    return $form;
  }

  public function dropdownAjax(){
    $view = views_get_view($this->values['view']);
    $view->set_display($this->values['viewDisplay']);
    // Enable ajax view by default.
    $view->display_handler->set_option('use_ajax', true);
    return $view->preview($this->values['viewDisplay']);
  }

}

function theforceMasterViewDisplayRefresh($form, $form_state){
  $parents = $form_state['triggering_element']['#parents'];
  array_pop($parents);
  $parents[] = 'viewDisplay';
  return drupal_array_get_nested_value($form, $parents);
}
