<?php

/**
 * Implements hook_preprocess_theforce_top().
 */
function template_preprocess_theforce(&$vars){
  $type = $vars['type'];
  $vars['attributes_array']['id'] = 'theforce-' . $type;
  $vars['classes_array'][] = 'theforce';
  $vars['region_attributes_array'] = array();
  foreach($vars['regions'] as $region){
    $vars['region_attributes_array'][$region]['id'] = 'theforce-' . $type . '-' . $region;
  }

  if(theforce_is_management() && !empty($vars['regions'])){
    foreach($vars['regions'] as $region){
      $vars['content'][$region]['ops']['add'] = array(
        '#theme' => 'theforce_region',
        '#type' => $vars['type'],
        '#region' => 'ops-' . $region,
        '#content' => array(
          array(
            '#theme' => 'theforce_item',
            '#content' => array('#markup' => '<a href="' . url("admin/config/user-interface/theforce/plugin/activate/$type/$region") . '" class="use-theforce"><i class="fa fa-plus"></i></a>'),
            '#attributes' => array(
              'class' => array('theforce-add'),
            ),
          ),
        ),
      );
    }
  }
}

/**
 * Implements hook_process_theforce_top().
 */
function template_process_theforce(&$vars){
  $vars['region_attributes'] = array();
  foreach($vars['regions'] as $region){
    $vars['region_attributes'][$region] = drupal_attributes($vars['region_attributes_array'][$region]);
  }
}

/**
 * Implements hook_preprocess_theforce_side().
 */
function template_preprocess_theforce_side(&$vars){
  $vars['attributes_array']['id'] = 'theforce-side';
  $vars['classes_array'][] = 'theforce';
}

/**
 * Default implementation of theme_theforce_region().
 */
function theme_theforce_region($vars){
  return '<ul' . drupal_attributes($vars['attributes']) . '>' . drupal_render($vars['content']) . '</ul>';
}

/**
 * Implements hook_preprocess_theforce_region().
 */
function template_preprocess_theforce_region(&$vars){
  // $vars['attributes']['id'] = 'theforce-' . $vars['type'] . '-' . $vars['region'];
  $vars['attributes']['class'][] = 'theforce-region';
}

/**
 * Default implementation of theme_theforce_item().
 */
function theme_theforce_item($vars){
  return '<li' . drupal_attributes($vars['attributes']) . '>' . drupal_render($vars['content']) . '</li>';
}

/**
 * Implements hook_preprocess_theforce_top().
 */
function template_preprocess_theforce_item(&$vars){
  if(isset($vars['content']['#attributes'])){
    // Merge in attributes specific by the content.
    $vars['attributes'] = drupal_array_merge_deep($vars['attributes'], $vars['content']['#attributes']);
    unset($vars['content']['#attributes']);
  }
  if(theforce_is_management()){
    if(!empty($vars['plugin'])){
      $type = $vars['plugin']['type'];
      $region = $vars['plugin']['region'];
      $vars['attributes']['class'][] = 'theforce-ui-item';
      $vars['attributes']['data-id'] = $vars['plugin']['id'];
      // Definition is used to determine if plugin has it's own settings.
      $has_definition = $vars['plugin']['handler']->settingsDefinition();
      if(!empty($has_definition)){
        $vars['attributes']['data-config'] = 1;
      }
      $vars['attributes']['data-region'] = $region;
    }
  }
  $vars['attributes']['class'][] = 'theforce-item';
}

/**
 * Implements default theme_theforce_plugin_form().
 */
function theme_theforce_plugin_form($vars){
  static $count;
  $count++;
  $element = $vars['element'];
  $header = array(t('Enabled'), t('Name'), t('Weight'));
  $rows = array();
  $table_id = 'theforce-items-' . $count;
  $row_class = 'theforce-weight-' . $count;
  foreach(element_children($element) as $id){
    $element[$id]['weight']['#attributes']['class'] = array($row_class);
    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($element[$id]['status']), 'class' => array('status')),
        drupal_render($element[$id]['title']),
        drupal_render($element[$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  $output .= drupal_render($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', $row_class, null, null, true);
  return $output;
}
