<?php

/**
 * @file
 * AJAX handlers for The Force.
 */

/**
 * Return ajax html output from plugins.
 */
function theforce_html_ajax($plugin){
  $commands = array();
  $html = '';
  if($plugin['handler']->access()){
    $html = drupal_render($plugin['handler']->renderAjax());
  }
  if($messages = theme('status_messages')){
    $html = $messages . $html;
  }
  $commands[] = ajax_command_html('#ajax-' . $plugin['id'], $html);
  $result['#type'] = 'ajax';
  $result['#commands'] = $commands;
  return $result;
}

/**
 * Return ajax console output from plugins.
 */
function theforce_side_ajax($plugin){
  $commands = array();
  $commands[] = ajax_command_theforce_side(drupal_render($plugin['handler']->renderAjax()), $plugin['handler']->values['sideTitle'], $plugin['handler']->values['sideDescription']);
  $result['#type'] = 'ajax';
  $result['#commands'] = $commands;
  return $result;
}


/**
 * Reload the content area.
 */
function ajax_command_theforce_redirect($url) {
  return array(
    'command' => 'theforceRedirect',
    'url' => url($url),
  );
}


/**
 * Reload the content area.
 */
function ajax_command_theforce_reload() {
  return array(
    'command' => 'theforceReload',
  );
}


/**
 * Insert HTML into a shelf that sits just below the top bar.
 *
 * @param $html
 *   Html to insert into overlay.
 * @param $settings
 *   An optional array of settings that will be used for this command only.
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function ajax_command_theforce_overlay($html, $title = '', $description = '', $settings = null) {
  $title = !empty($title) ? $title : drupal_get_title();
  if(empty($title)){
    $menu = menu_get_item();
    $title = $menu['title'];
  }
  if(empty($description)){
    $menu = menu_get_item();
    $description = $menu['description'];
  }
  if($messages = theme('status_messages')){
    $html = $messages . $html;
  }
  $html = '<div class="theforce-overlay-title">' . $title . '</div><div class="theforce-overlay-description">' . $description . '</div>' . $html;
  return array(
    'command' => 'theforceOverlay',
    'data' => $html,
    'settings' => $settings,
  );
}

/**
 * Close any open overlay.
 */
function ajax_command_theforce_overlay_close() {
  return array(
    'command' => 'theforceOverlayClose',
  );
}


/**
 * Insert HTML into the side region.
 *
 * @param $html
 *   Html to insert into shelf.
 * @param $settings
 *   An optional array of settings that will be used for this command only.
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function ajax_command_theforce_side($html, $title = '', $description = '', $settings = null) {
  $title = !empty($title) ? $title : drupal_get_title();
  if(empty($title)){
    $menu = menu_get_item();
    $title = $menu['title'];
  }
  if(empty($description)){
    $menu = menu_get_item();
    $description = $menu['description'];
  }
  if($messages = theme('status_messages')){
    $html = $messages . $html;
  }
  $html = '<div class="theforce-side-title">' . $title . '</div><div class="theforce-side-description">' . $description . '</div>' . $html;
  return array(
    'command' => 'theforceSide',
    'data' => $html,
    'settings' => $settings,
  );
}

/**
 * Close any open shelf.
 */
function ajax_command_theforce_side_close() {
  return array(
    'command' => 'theforceSideClose',
  );
}

/**
 * Update part of The Force.
 *
 * @param $type
 *   The type of plugin to update. (top || side)
 * @param $region
 *   The region to update. (left || right || top || bottom)
 */
function ajax_command_theforce_update($region, $location = null, $settings = null) {
  if(!$location){
    $selector = '#theforce-' . $type;
    $content = theforce_content($type);
  }else{
    $selector = "#theforce-$region-$location-content";
    $content = theforce_content_region($region, $location);
  }
  return ajax_command_replace($selector, drupal_render($content));
}
