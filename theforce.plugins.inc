<?php

/**
 * Jedi plugins.
 */
function _theforce_theforce_jedi() {
  $path = drupal_get_path('module', 'theforce') . '/plugins/jedi';
  $plugins = array();
  $plugins['theforceJediUser'] = array(
    'title' => 'User Info',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceJediUser.inc',
      'class' => 'theforceJediUser',
    ),

    /**
     * If limit_region is set, this plugin will only be available in regions
     * containing the specified region ids.
     */
    // 'limit_region' => array('top'),

    /**
     * If limit_location is set, this plugin will only be available in locations
     * containing the specified location ids. Can be used with limit_region to
     * limit a plugin specifically.
     */
    // 'limit_location' => array('right'),
  );
  $plugins['theforceJediNodeCreate'] = array(
    'title' => 'Node Creation',
    'handler' => array(
      'path' => $path,
      'file' => 'theforceJediNodeCreate.inc',
      'class' => 'theforceJediNodeCreate',
    ),
  );
  $plugins['theforceJediMenuManage'] = array(
    'title' => 'Menu Manage',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceJediMenuManage.inc',
      'class' => 'theforceJediMenuManage',
    ),
  );
  $plugins['theforceJediLocalTabs'] = array(
    'title' => 'Local Tabs',
    'handler' => array(
      'path' => $path,
      'file' => 'theforceJediLocalTabs.inc',
      'class' => 'theforceJediLocalTabs',
    ),
  );
  $plugins['theforceJediLocalActions'] = array(
    'title' => 'Local Actions',
    'handler' => array(
      'path' => $path,
      'file' => 'theforceJediLocalActions.inc',
      'class' => 'theforceJediLocalActions',
    ),
  );
  return $plugins;
}

/**
 * Master plugins.
 */
function _theforce_theforce_master() {
  $path = drupal_get_path('module', 'theforce') . '/plugins/master';
  $plugins = array();
  $plugins['theforceMasterMarkup'] = array(
    'title' => 'Markup',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceMasterMarkup.inc',
      'class' => 'theforceMasterMarkup',
    ),
  );
  $plugins['theforceMasterMenu'] = array(
    'title' => 'Menu',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceMasterMenu.inc',
      'class' => 'theforceMasterMenu',
    ),
  );
  $plugins['theforceMasterText'] = array(
    'title' => 'Text',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceMasterText.inc',
      'class' => 'theforceMasterText',
    ),
  );
  $plugins['theforceMasterLink'] = array(
    'title' => 'Link',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceMasterLink.inc',
      'class' => 'theforceMasterLink',
    ),
  );
  $plugins['theforceMasterDropdown'] = array(
    'title' => 'Dropdown',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceMasterDropdown.inc',
      'class' => 'theforceMasterDropdown',
    ),
  );
  $plugins['theforceMasterView'] = array(
    'title' => 'View',
    'configurable' => 1,
    'handler' => array(
      'path' => $path,
      'file' => 'theforceMasterView.inc',
      'class' => 'theforceMasterView',
    ),
  );
  return $plugins;
}
