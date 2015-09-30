<?php

/**
 * Top bar plugins.
 */
function _theforce_theforce_top() {
  $path = drupal_get_path('module', 'theforce');
  $plugins = array();
  $plugins['theforceTopUser'] = array(
    'title' => 'User Info',
    'handler' => array(
      'path' => $path . '/plugins/top',
      'file' => 'theforceTopUser.inc',
      'class' => 'theforceTopUser',
    ),
  );
  return $plugins;
}

/**
 * Side bar plugins.
 */
function _theforce_theforce_side() {
  $path = drupal_get_path('module', 'theforce');
  $plugins = array();
  // $plugins['theforceSideTestlink'] = array(
  //   'title' => 'SIDE Test Link',
  //   'handler' => array(
  //     'path' => $path . '/plugins/side',
  //     'file' => 'theforceSideTestLink.inc',
  //     'class' => 'theforceSideTestLink',
  //   ),
  // );
  // $plugins['theforceSide'] = array(
  //   'handler' => array(
  //     'path' => $path . '/plugins/side',
  //     'file' => 'theforceSide.inc',
  //     'class' => 'theforceSide',
  //   ),
  // );
  return $plugins;
}
