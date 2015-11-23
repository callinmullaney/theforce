<?php

////////////////////////////////////////////////////////////////////////////////
/// Activate
////////////////////////////////////////////////////////////////////////////////

/**
 * Activate a plugin in a given type/region.
 */
function theforce_admin_plugin_activate($region, $location, $js = false){
  $form = drupal_get_form('theforce_admin_plugin_activate_form', $region, $location, $js);

  if($js == 'ajax'){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    print ajax_render($commands);
    drupal_exit();
  }

  return $form;
}

function theforce_admin_plugin_activate_form($form, &$form_state, $region, $location, $js = false){
  $plugins = theforce_query()->filterPublic()->filterInactive()->filterInvalid($region, $location)->execute();

  if(empty($plugins)){
    drupal_set_message(t('There are no available plugins for this region.'), 'warning');
  }
  else{
    $form_state['region'] = $region;
    $form_state['location'] = $location;
    $options = array();
    foreach($plugins as $id => $plugin){
      $options[$id] = $plugin['title'];
    }
    $form['plugins'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Plugin'),
      '#options' => $options,
      '#required' => true,
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
    );
    if($js == 'ajax'){
      $form['actions']['submit']['#ajax'] = array(
        'callback' => 'theforce_admin_plugin_activate_form_ajax',
        'progress' => 'theforce',
      );
    }
  }

  $plugins = theforce_query()->filterPrivate()->filterInvalid($region, $location)->execute();
  if(!empty($plugins)){
    $attributes = array();
    if($js == 'ajax'){
      $attributes['class'][] = 'use-theforce';
    }
    $form['actions']['create'] = array(
      '#type' => 'markup',
      '#markup' => l('<i class="fa fa-plus"></i> Create New', "admin/config/user-interface/theforce/plugin/create/$region/$location", array('html' => true, 'attributes' => $attributes)),
    );
  }

  return $form;
}

function theforce_admin_plugin_activate_form_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $commands = array();
  $commands[] = ajax_command_theforce_overlay_close();
  $commands[] = ajax_command_theforce_update($form_state['region'], $form_state['location']);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function theforce_admin_plugin_activate_form_submit($form, &$form_state){
  foreach(array_filter($form_state['values']['plugins']) as $id){
    theforce_plugin_activate($id, $form_state['region'], $form_state['location']);
  }
  $form_state['redirect'] = 'admin/config/user-interface/theforce';
}


////////////////////////////////////////////////////////////////////////////////
/// Settings
////////////////////////////////////////////////////////////////////////////////

/**
 * Settings for a single plugin.
 */
function theforce_admin_plugin_settings($plugin, $js = false){
  $form = drupal_get_form('theforce_admin_plugin_settings_form', $plugin, $js);

  if($js == 'ajax'){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    print ajax_render($commands);
    drupal_exit();
  }

  return $form;
}

function theforce_admin_plugin_settings_form($form, &$form_state, $plugin, $js = false){
  $plugin_form = $plugin['handler']->settingsForm($form, $form_state);

  if(empty($plugin_form)){
    drupal_set_message(t('There are no available settings for this plugin.'), 'warning');
  }
  else{
    $form_state['plugin'] = $plugin;
    $form_state['plugin_id'] = $plugin['id'];

    $form[$plugin['id']] = $plugin_form + array(
      '#type' => 'container',
      '#tree' => true,
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    if($plugin['settings_custom']){
      $form['actions']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset'),
        '#submit' => array('theforce_admin_plugin_settings_form_submit_reset'),
      );
    }
  }

  if($js == 'ajax'){
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'theforce_admin_plugin_settings_form_ajax',
      'progress' => 'theforce',
    );
    if(isset($form['actions']['reset'])){
      $form['actions']['reset']['#ajax'] = array(
        'callback' => 'theforce_admin_plugin_settings_form_ajax',
        'progress' => 'theforce',
      );
    }
  }
  return $form;
}

function theforce_admin_plugin_settings_form_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $plugin = $form_state['plugin'];

  $commands = array();
  $commands[] = ajax_command_theforce_overlay_close();
  $commands[] = ajax_command_theforce_update($plugin['region'], $plugin['location']);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function theforce_admin_plugin_settings_form_submit($form, &$form_state){
  $plugin = $form_state['plugin'];
  $id = $plugin['id'];
  theforce_plugin_update($id, null, $form_state['values'][$id]);
  $form_state['redirect'] = 'admin/config/user-interface/theforce';
}

function theforce_admin_plugin_settings_form_submit_reset($form, &$form_state){
  $plugin = $form_state['plugin'];
  theforce_plugin_update($plugin['id'], null, array());
  $form_state['redirect'] = 'admin/config/user-interface/theforce';
}


////////////////////////////////////////////////////////////////////////////////
/// Weights
////////////////////////////////////////////////////////////////////////////////

/**
 * Ajax post handler that will bulk save the weights of a plugins.
 */
function theforce_admin_plugin_weights_save(){
  if(isset($_POST['weights'])){
    $weights = json_decode($_POST['weights']);
    foreach($weights as $weight => $id){
      theforce_plugin_weight_save($id, $weight);
    }
  }
  $output = array('status' => 'OK');
  drupal_json_output($output);
  drupal_exit();
}


////////////////////////////////////////////////////////////////////////////////
/// Remove
////////////////////////////////////////////////////////////////////////////////

/**
 * Remove a plugin page callback.
 */
function theforce_admin_plugin_remove($plugin, $js = false){
  $form = drupal_get_form('theforce_admin_plugin_remove_form', $plugin, $js);

  if($js == 'ajax'){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    print ajax_render($commands);
    drupal_exit();
  }

  return $form;
}

function theforce_admin_plugin_remove_form($form, &$form_state, $plugin, $js = false){
  $form_state['plugin'] = $plugin;

  drupal_set_message(t('Are you sure you would like to remove %name?', array('%name' => $plugin['title'])), 'warning');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
  );

  if($js == 'ajax'){
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'theforce_admin_plugin_remove_form_ajax',
      'progress' => 'theforce',
    );
  }
  return $form;
}

function theforce_admin_plugin_remove_form_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $plugin = $form_state['plugin'];
  $commands = array();
  $commands[] = ajax_command_theforce_overlay_close();
  $commands[] = ajax_command_theforce_update($plugin['region'], $plugin['location']);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function theforce_admin_plugin_remove_form_submit($form, &$form_state){
  $plugin = $form_state['plugin'];
  theforce_plugin_deactivate($plugin['id']);
  $form_state['redirect'] = 'admin/config/user-interface/theforce';
}


////////////////////////////////////////////////////////////////////////////////
/// Create
////////////////////////////////////////////////////////////////////////////////

/**
 * Create a plugin
 */
function theforce_admin_plugin_create($js = false){
  $form = drupal_get_form('theforce_admin_plugin_create_form', null, null, $js);

  if($js == 'ajax'){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    print ajax_render($commands);
    drupal_exit();
  }

  return $form;
}

function theforce_admin_plugin_create_active($region, $location, $js = false){
  $form = drupal_get_form('theforce_admin_plugin_create_form', $region, $location, $js);

  if($js == 'ajax'){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    print ajax_render($commands);
    drupal_exit();
  }

  return $form;
}

function theforce_admin_plugin_create_form($form, &$form_state, $region = null, $location = null, $js = false){
  $plugins = theforce_query()->filterPrivate()->filterInvalid($region, $location)->execute();

  if(empty($plugins)){
    drupal_set_message(t('There are no configurable plugins available.'), 'warning');
  }
  else{
    $settings_id = 'theforce-plugin-settings';

    $form_state['region'] = $region;
    $form_state['location'] = $location;

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('A descriptive label for this item used for administrative purposes.'),
      '#required' => true,
    );

    $form['id'] = array(
      '#type' => 'machine_name',
      '#title' => t('System ID'),
      // theforcePadawan is added in submit.
      '#maxlength' => 113,
      '#required' => true,
      '#machine_name' => array(
        'exists' => 'theforce_plugin_exists',
        'source' => array('title'),
        'label' => t('System ID'),
        'replace_pattern' => '[^a-zA-Z0-9_]+',
        'replace' => '_',
      ),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'theforce') . '/js/theforce.machine-name.js' => array('weight' => 1000),
        ),
      ),
    );

    $options = array();
    foreach($plugins as $id => $plugin){
      $options[$id] = $plugin['title'];
    }
    $form['master'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => $options,
      '#required' => true,
      '#ajax' => array(
        'callback' => 'theforce_admin_plugin_create_form_plugin_ajax',
        'wrapper' => $settings_id,
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    $id = isset($form_state['values']) && !empty($form_state['values']['master']) ? $form_state['values']['master'] : null;
    if($id && $plugin = theforce_query()->filterId($id)->loadHandler()->execute()){
      $plugin_form = $plugin['handler']->settingsForm(array(), $form_state);
      if(!empty($plugin_form)){
        $form['settings'] = $plugin_form + array(
          '#type' => 'fieldset',
          '#title' => t('Settings'),
          '#id' => $settings_id,
          '#tree' => TRUE,
        );
      }
    }
    if(empty($form['settings'])){
      $form['settings'] = array(
        '#type' => 'container',
        '#id' => $settings_id,
      );
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $region && $location ? t('Create and Activate') : t('Create'),
    );
    if($js == 'ajax'){
      $form['actions']['submit']['#ajax'] = array(
        'callback' => 'theforce_admin_plugin_create_form_ajax',
        'progress' => 'theforce',
      );
    }
  }
  return $form;
}

function theforce_admin_plugin_create_form_plugin_ajax($form, $form_state){
  return $form['settings'];
}

function theforce_admin_plugin_create_form_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $commands = array();
  $commands[] = ajax_command_theforce_overlay_close();
  $commands[] = ajax_command_theforce_update($form_state['region'], $form_state['location']);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function theforce_admin_plugin_create_form_submit($form, &$form_state){
  $values = $form_state['values'];
  $id = theforce_plugin_id_prepare($values['id']);
  $title = $values['title'];
  $master = $values['master'];
  $settings = !empty($values['settings']) ? $values['settings'] : array();
  $region = $form_state['region'];
  $location = $form_state['location'];

  theforce_plugin_create($id, $title, $master, $settings);

  if($region && $location){
    theforce_plugin_activate($id, $region, $location);
  }

  $form_state['redirect'] = 'admin/config/user-interface/theforce';
}


////////////////////////////////////////////////////////////////////////////////
/// Delete
////////////////////////////////////////////////////////////////////////////////

/**
 * Delete a plugin page callback.
 */
function theforce_admin_plugin_delete($plugin, $js = false){
  $form = drupal_get_form('theforce_admin_plugin_delete_form', $plugin, $js);

  if($js == 'ajax'){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    print ajax_render($commands);
    drupal_exit();
  }

  return $form;
}

function theforce_admin_plugin_delete_form($form, &$form_state, $plugin, $js = false){

  if(empty($plugin['custom'])){
    drupal_set_message(t('This is not a configurable plugin.'), 'warning');
  }
  else{
    $form_state['plugin'] = $plugin;

    drupal_set_message(t('Are you sure you would like to permanently delete %name?', array('%name' => $plugin['title'])), 'error');

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );

    if($js == 'ajax'){
      $form['actions']['submit']['#ajax'] = array(
        'callback' => 'theforce_admin_plugin_delete_form_ajax',
        'progress' => 'theforce',
      );
    }
  }
  return $form;
}

function theforce_admin_plugin_delete_form_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $plugin = $form_state['plugin'];
  $commands = array();
  $commands[] = ajax_command_theforce_overlay_close();
  $commands[] = ajax_command_theforce_update($plugin['region'], $plugin['location']);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function theforce_admin_plugin_delete_form_submit($form, &$form_state){
  $plugin = $form_state['plugin'];
  theforce_plugin_deactivate($plugin['id']);
  theforce_plugin_delete($plugin['id']);
  $form_state['redirect'] = 'admin/config/user-interface/theforce';
}
