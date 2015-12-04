<?php

/**
 * The admin configuration form for the Force.
 */
function theforce_admin_form($form, &$form_state) {

  $settings = theforce_settings();
  $path = drupal_get_path('module', 'theforce');

  if(module_exists('sonar')){
    $bg = $path . '/images/settings.png';
    sonar_add_var('theforce-settings-bg-image', "'" . url($bg) . "'");
    $form['#attached']['css'][$path . '/css/theforce.admin.scss'] = array('weight' => 10);
  }

  $form['#attached']['js'][$path . '/js/theforce.admin.js'] = array();
  $form['#attached']['js'][$path . '/js/theforce.sortable.js'] = array();

  $form['theforce'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => - 10,
    '#tree' => true,
    '#prefix' => '
      <div id="theforce-settings-header">
        <div id="viewport" ></div>
        <div class="bg"></div>
        <div class="shine"></div>
        <div class="image"></div>
        <div class="lines"></div>
      </div>
      <div id="theforce-settings">
      <h1>' . t('The Force') . '</h1>
      <h4>' . t('The Force is what gives a Jedi his power. It\'s an energy field created by all living things. It surrounds us and penetrates us; it binds the galaxy together.') . '</h4>',
    '#suffix' => '</div>',
  );

  $form['#id'] = 'theforce-settings-wrapper';

  //////////////////////////////////////////////////////////////////////////////
  /// Theme
  //////////////////////////////////////////////////////////////////////////////

  if(module_exists('sonar')){
    $form['theforce']['theme'] = array(
      '#type' => 'fieldset',
      '#title' => t('Theme'),
      '#parents' => array('theforce'),
    );

    $top_bg = $settings['top_bg'];
    $top_primary = $settings['top_primary'];
    $side_bg = $settings['side_bg'];
    $side_primary = $settings['side_primary'];

    $themes = $items = array();
    $themes[] = array('Cobalt', array('#273035','#0997c3','#273035','#0997c3'));
    $themes[] = array('Midnight', array('#2B2B2B','#0997c3','#2B2B2B','#0997c3'));
    $themes[] = array('Albino', array('#ECECEC','#0997c3','#ECECEC','#0997c3'));
    $themes[] = array('Invert', array('#F1F3F4','#0997c3','#273035','#0997c3'));

    foreach($themes as $theme){
      $item_class = $theme[1][0] == $top_bg && $theme[1][1] == $top_primary && $theme[1][2] == $side_bg && $theme[1][3] == $side_primary ? 'active' : '';
      $item = '<a data-top-bg="' . $theme[1][0] . '" data-top-primary="' . $theme[1][1] . '" data-side-bg="' . $theme[1][2] . '" data-side-primary="' . $theme[1][3] . '" class="' . $item_class . '">';
      $item .= '<i class="fa fa-square" style="color:'.$theme[1][0].'"></i> ';
      $item .= '<i class="fa fa-square" style="color:'.$theme[1][1].'"></i> ';
      $item .= '<i class="fa fa-square" style="color:'.$theme[1][2].'"></i> ';
      $item .= '<i class="fa fa-square" style="color:'.$theme[1][3].'"></i> ';
      $item .= $theme[0];
      $item .= '</a>';
      $items[] = $item;
    }
    $form['theforce']['theme']['themes'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array('id' => 'theforce-theme-select'),
    );

    $form['theforce']['theme']['top_bg'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Top Background Color'),
      '#field_prefix'  => 'Hex or Color Name <i class="fa fa-square" style="color:'.$top_bg.';"></i>',
      '#default_value' => $top_bg,
    );

    $form['theforce']['theme']['top_primary'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Top Primary Color'),
      '#field_prefix'  => 'Hex or Color Name <i class="fa fa-square" style="color:'.$top_primary.';"></i>',
      '#default_value' => $top_primary,
    );

    $form['theforce']['theme']['side_bg'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Side Background Color'),
      '#field_prefix'  => 'Hex or Color Name <i class="fa fa-square" style="color:'.$side_bg.';"></i>',
      '#default_value' => $side_bg,
    );

    $form['theforce']['theme']['side_primary'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Side Primary Color'),
      '#field_prefix'  => 'Hex or Color Name <i class="fa fa-square" style="color:'.$side_primary.';"></i>',
      '#default_value' => $side_primary,
    );
  }

  //////////////////////////////////////////////////////////////////////////////
  /// Top Bar
  //////////////////////////////////////////////////////////////////////////////

  $form['theforce']['top'] = array(
    '#type' => 'fieldset',
    '#title' => t('Top Bar'),
    '#parents' => array('theforce'),
  );

  $form['theforce']['top']['top_icon_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Icons Only'),
    '#description' => t('Hide text labels and only show icons. The labels will show when hovering over an icon for a moment.'),
    '#default_value' => $settings['top_icon_only'],
  );

  $form['theforce']['top']['top_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Value should be entered in pixels. Example: 60px'),
    '#default_value' => $settings['top_height'],
  );

  //////////////////////////////////////////////////////////////////////////////
  /// Side Bar
  //////////////////////////////////////////////////////////////////////////////

  $form['theforce']['side'] = array(
    '#type' => 'fieldset',
    '#title' => t('Side Bar'),
    '#parents' => array('theforce'),
  );

  $form['theforce']['side']['side_position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => array('left' => t('Left'), 'right' => t('Right')),
    '#default_value' => $settings['side_position'],
  );

  $form['theforce']['side']['side_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Value should be entered in pixels. Example: 80px'),
    '#default_value' => $settings['side_width'],
  );

  $form['theforce']['side']['side_width_active'] = array(
    '#type' => 'textfield',
    '#title' => t('Width When Active'),
    '#description' => t('Value should be entered in pixels. Example: 335px'),
    '#default_value' => $settings['side_width_active'],
  );

  //////////////////////////////////////////////////////////////////////////////
  /// Tools
  //////////////////////////////////////////////////////////////////////////////

  $form['theforce']['tools'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tools'),
  );

  $title = t('Create Profile Image Field');
  $description = t('A quick way to attach an image field for use as a profile picture.');
  $exists = _theforce_admin_image_field_exists();
  if(module_exists('asset')){
    $description .= '<br><small>' . t('You can choose to use an image field directly attached to the user profile or an asset reference to an image asset. If you want user profile images to be able to be re-used in other areas of the site, you should use the asset field.') . '</small>';
  }
  if($exists){
    $title .= ' <small>(' . t('already exists') . ')</small>';
  }
  $form['theforce']['tools']['image'] = array(
    '#type' => 'submit',
    '#prefix' => '<label>' . $title . '</label><div class="description">' . $description . '</div>',
    '#value' => t('Create Field'),
    '#disabled' => $exists,
    '#submit' => array(
      'theforce_admin_form_tools_image_submit'
    ),
  );
  if(module_exists('asset')){
    $form['theforce']['tools']['image_asset'] = array(
      '#type' => 'submit',
      '#value' => t('Create Asset Field'),
      '#disabled' => $exists,
      '#asset' => TRUE,
      '#submit' => array(
        'theforce_admin_form_tools_image_submit'
      ),
    );
  }

  $form['#validate'][] = 'theforce_admin_form_validate';

  return system_settings_form($form);
}

/**
 * Validate settings form and clean up values.
 */
function theforce_admin_form_validate($form, &$form_state){
  $values = &$form_state['values']['theforce'];
  unset($values['theforce__active_tab'], $values['tools']);

  cache_clear_all('sonar', 'cache', TRUE);
}

/**
 * Create a profile image field.
 */
function theforce_admin_form_tools_image_submit(&$form, &$form_state) {
  if(!_theforce_admin_image_field_exists()){
    // Make sure default user profile pictures are turned off.
    variable_set('user_pictures', 0);

    $trigger = $form_state['triggering_element'];
    // Create asset field.
    if(!empty($trigger['#asset'])){
      _theforce_admin_form_tools_image_submit_create_asset_field();
    }
    else{
      _theforce_admin_form_tools_image_submit_create_field();
    }
  }
}

/**
 * Check if an image field exists on the user profile.
 */
function _theforce_admin_image_field_exists(){
  $instances = field_info_instances('user', 'user');
  $field_names = array('field_image', 'field_image_asset');
  $exists = FALSE;
  foreach($field_names as $field_name){
    if(array_key_exists($field_name, $instances)){
      $exists = TRUE;
    }
  }
  return $exists;
}

/**
 * Create an asset field and instance if neither exist.
 */
function _theforce_admin_form_tools_image_submit_create_asset_field(){
  $type = asset_type_load('image');
  if(empty($type)){
    $type = entity_import('asset_type', '{
      "type" : "image",
      "title" : "Image",
      "plural" : "Images",
      "data" : {
        "icon" : "photo",
        "path" : "admin\\/media\\/image",
        "auto_title" : 1,
        "usage_info" : 1,
        "instance_info" : 1,
        "widget" : {
          "assetWidgetPosition" : { "status" : 1 },
          "assetWidgetImageStyle" : {
            "status" : 1,
            "settings" : {}
          },
          "assetWidgetCaption" : { "status" : 1 },
          "assetWidgetLink" : { "status" : 1 }
        },
        "auto_title_pattern" : "",
        "render_title" : 0,
        "allow_status" : 0,
        "purge" : 0,
        "no_multiple" : 0,
        "require_instance" : 0,
        "title_label" : ""
      },
      "rdf_mapping" : [],
      "public" : true
    }');
    $type = entity_save('asset_type', $type);
  }
  _theforce_admin_form_tools_image_submit_create_field('field_image', 'asset', 'image');

  $instance = field_info_instance('user', 'field_image_asset', 'user');
  if(empty($instance)){
    $instance = array(
      'entity_type' => 'user',
      'bundle' => 'user',
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'entityreference',
          'settings' => array(
            'field_name' => 'field_image_asset_field_image',
            'image_style' => 'theforce',
          ),
          'type' => 'asset_image',
          'weight' => 0,
        ),
      ),
      'field_name' => 'field_image_asset',
      'label' => 'Profile Picture',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'inline_entity_form',
        'settings' => array(
          'fields' => array(),
          'type_settings' => array(
            'label_plural' => 'Profile Picture',
            'label_singular' => 'Profile Pictures',
            'override_labels' => 1,
            'delete_references' => 1,
            'auto_title' => 1,
          ),
        ),
        'type' => 'inline_entity_form_single',
        'weight' => 0,
      ),
    );
    field_create_instance($instance);

    // $default_permissions = array(
    //   "view own image asset",
    //   "edit own image asset",
    //   "delete own image asset",
    // );
    // user_role_grant_permissions(1, $default_permissions);
    // user_role_grant_permissions(2, $default_permissions);
  }
}

/**
 * Create a field and instance if neither exist.
 */
function _theforce_admin_form_tools_image_submit_create_field($field_name = 'field_image', $entity_type = 'user', $bundle = 'user'){
  $field = field_info_field($field_name);
  if(empty($field)){
    $field = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_image',
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => 0,
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => 0,
      'type' => 'image',
    );
    field_create_field($field);
  }

  $instance = field_info_instance($entity_type, $field_name, $bundle);
  if(empty($instance)){
    // Create the instance on the bundle.
    $instance = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'theforce',
          ),
          'type' => 'image',
          'weight' => 0,
        ),
      ),
      'field_name' => $field_name,
      'label' => 'Profile Picture',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1,
        'alt_field' => 0,
        'default_image' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'filefield_paths' => array(
          'active_updating' => 0,
          'file_name' => array(
            'options' => array(
              'pathauto' => 1,
              'transliterate' => 1,
            ),
            'value' => 'user-[user:uid]-[user:name].[file:ffp-extension-original]',
          ),
          'file_path' => array(
            'options' => array(
              'pathauto' => 0,
              'transliterate' => 0,
            ),
            'value' => 'images',
          ),
          'retroactive_update' => 0,
        ),
        'filefield_paths_enabled' => 1,
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => 0,
      ),
    );
    field_create_instance($instance);
  }
}
