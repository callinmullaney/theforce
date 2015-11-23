<?php

/**
 * Implements hook_features_export_options().
 */
function theforce_features_export_options() {
  $options = array();
  foreach(theforce_query()->filterPublic()->execute() as $id => $plugin){
    $options[$id] = $plugin['title'] . ' -- ' . $id;
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function theforce_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['features'] = 'theforce';
  $map = features_get_default_map('theforce');
  $plugins = theforce_query()->filterPublic()->execute();
  foreach ($data as $id) {
    // If another module provides this plugin, add it as a dependency
    if (isset($map[$id]) && $map[$id] != $module_name) {
      $module = $map[$id];
      $export['dependencies'][$module] = $module;
    }
    // Otherwise, export the style
    elseif (isset($plugins[$id])) {
      $export['features']['theforce'][$id] = $id;
    }
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function theforce_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'theforce', 'includes/theforce.admin.plugin');
  $code = array();
  $code[] = '  $theforce = array();';
  $code[] = '';
  foreach ($data as $id) {
    if ($plugin = theforce_query()->filterPublic()->filterId($id)->loadHandler()->execute()) {
      $plugin['settings'] = $plugin['handler']->settings;
      features_sanitize($plugin['settings']);
      _theforce_features_sanitize($plugin);
      $theforce_export = features_var_export($plugin, '  ');
      $theforce_identifier = features_var_export($id);
      $code[] = "  // Exported theforce plugin: {$id}.";
      $code[] = "  \$theforce[{$theforce_identifier}] = {$theforce_export};";
      $code[] = "";
    }
  }
  $code[] = '  return $theforce;';
  $code = implode("\n", $code);
  return array('theforce_items' => $code);
}

/**
 * Implements hook_features_revert().
 */
function theforce_features_revert($module) {
  theforce_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function theforce_features_rebuild($module) {
  if ($defaults = features_get_default('theforce', $module)) {
    foreach ($defaults as $plugin) {
      if(!empty($plugin['region']) && !empty($plugin['location'])){
        theforce_plugin_activate($plugin['id'], $plugin['region'], $plugin['location'], $plugin['weight']);
      }
      else{
        theforce_plugin_deactivate($plugin['id']);
      }
    }
  }
}

/**
 * Remove unnecessary keys for export.
 */
function _theforce_features_sanitize(array &$plugin) {

  $plugin = array_intersect_key($plugin, array_flip(array(
    'id',
    'class',
    'title',
    'region',
    'location',
    'weight',
    'settings',
  )));

}
