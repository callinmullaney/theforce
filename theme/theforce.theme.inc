<?php

/**
 * Implements hook_preprocess_theforce_top().
 */
function template_preprocess_theforce(&$vars){
  $region = $vars['region'];
  $settings = theforce_settings();
  $vars['attributes_array']['id'] = 'theforce-' . $region;
  $vars['location_attributes_array'] = array();
  $vars['classes_array'][] = 'theforce-region';
  if(isset($settings[$region . '_style'])){
    $vars['classes_array'][] = 'theforce-style-' . $settings[$region . '_style'];
  }

  array_unshift($vars['locations'], 'mini');
  $vars['location_attributes_array']['mini']['class'][] = 'theforce-mini-location';
  $vars['content'] = array(
    'mini' => array(
      '#theme' => 'theforce_location',
      '#content' => array(
        array(
          '#theme' => 'theforce_item',
          '#content' => array('#markup' => '<div class="theforce-elements"><a href="" class="theforce-element theforce-mini-toggle"><i class="theforce-icon fa fa-bars"></i></a></div>'),
        ),
      ),
    )
  ) + $vars['content'];

  foreach($vars['locations'] as $location){
    $vars['location_attributes_array'][$location]['id'] = 'theforce-' . $region . '-' . $location;
    $vars['location_attributes_array'][$location]['class'][] = 'theforce-location';
  }

  if(theforce_is_management() && !empty($vars['locations'])){
    foreach($vars['locations'] as $location){
      $weight = $location == 'bottom' || $location == 'right' ? -1 : 100;
      $vars['content'][$location]['ops']['#weight'] = $weight;
      $vars['content'][$location]['ops']['add'] = array(
        '#theme' => 'theforce_location',
        '#region' => $vars['region'],
        '#location' => 'ops-' . $location,
        '#content' => array(
          array(
            '#theme' => 'theforce_item',
            '#content' => array('#markup' => '<a href="' . url("admin/config/user-interface/theforce/plugin/activate/$region/$location") . '" class="theforce-element use-theforce"><i class="fa fa-plus"></i></a>'),
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
  $vars['location_attributes'] = array();
  foreach($vars['locations'] as $location){
    $vars['location_attributes'][$location] = drupal_attributes($vars['location_attributes_array'][$location]);
  }
}

/**
 * Default implementation of theme_theforce_location().
 */
function theme_theforce_location($vars){
  return '<ul' . drupal_attributes($vars['attributes']) . '>' . drupal_render($vars['content']) . '</ul>';
}

/**
 * Implements hook_preprocess_theforce_location().
 */
function template_preprocess_theforce_location(&$vars){
  $vars['attributes']['class'][] = 'theforce-items';
}

/**
 * Default implementation of theme_theforce_item().
 */
function theme_theforce_item($vars){
  return '<li' . drupal_attributes($vars['attributes']) . '>' . drupal_render($vars['content']) . '</li>';
}

/**
 * Default implementation of theme_theforce_menu().
 */
function theme_theforce_menu($vars) {
  return '<ul class="theforce-menu">' . $vars['tree'] . '</ul>';
}

/**
 * Implements hook_preprocess_theforce_menu().
 */
function template_preprocess_theforce_menu(&$vars) {
  $vars['tree'] = $vars['tree']['#children'];
}

/**
 * Default implementation of theme_theforce_menu_link().
 */
function theme_theforce_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
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
      $region = $vars['plugin']['region'];
      $location = $vars['plugin']['region'];
      $vars['attributes']['class'][] = 'theforce-ui-item';
      $vars['attributes']['data-id'] = $vars['plugin']['id'];
      if(!empty($vars['plugin']['configurable'])){
        $vars['attributes']['data-config'] = 1;
      }
      if(empty($vars['plugin']['default']) && !empty($vars['plugin']['custom'])){
        $vars['attributes']['data-trash'] = 1;
      }
      $vars['attributes']['data-region'] = $location;
    }
  }
  $vars['attributes']['class'][] = 'theforce-item';
}

/**
 * Implements default theme_theforce_plugin_form().
 */
// function theme_theforce_plugin_form($vars){
//   static $count;
//   $count++;
//   $element = $vars['element'];
//   $header = array(t('Enabled'), t('Name'), t('Weight'));
//   $rows = array();
//   $table_id = 'theforce-items-' . $count;
//   $row_class = 'theforce-weight-' . $count;
//   foreach(element_children($element) as $id){
//     $element[$id]['weight']['#attributes']['class'] = array($row_class);
//     $rows[] = array(
//       'data' => array(
//         array('data' => drupal_render($element[$id]['status']), 'class' => array('status')),
//         drupal_render($element[$id]['title']),
//         drupal_render($element[$id]['weight']),
//       ),
//       'class' => array('draggable'),
//     );
//   }

//   $output = theme('table', array(
//     'header' => $header,
//     'rows' => $rows,
//     'attributes' => array('id' => $table_id),
//   ));

//   $output .= drupal_render($form);

//   drupal_add_tabledrag($table_id, 'order', 'sibling', $row_class, null, null, true);
//   return $output;
// }
