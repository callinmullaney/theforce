<?php

/**
 * Menu management interface.
 */
function theforce_admin_menu_edit($menu){
  $form = drupal_get_form('theforce_admin_menu_edit_form', $menu);
  drupal_set_title('Manage ' . $menu['title']);

  $commands = array();
  $commands[] = ajax_command_theforce_overlay(drupal_render($form));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function theforce_admin_menu_edit_form($form, &$form_state, $menu){
  global $menu_admin;
  form_load_include($form_state, 'inc', 'menu', 'menu.admin');
  $form['#theme'] = 'menu_overview_form';
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu' => $menu['menu_name']), array('fetch' => PDO::FETCH_ASSOC));
  $links = array();
  foreach ($result as $item) {
    $links[] = $item;
  }
  $tree = menu_tree_data($links);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;

  $form = array_merge($form, _theforce_menu_overview_tree_form($tree));
  $form['#menu'] =  $menu;

  if (element_children($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#ajax' => array(
        'callback' => 'theforce_admin_menu_edit_form_redirect_ajax',
        'progress' => 'theforce',
      ),
      '#redirect' => 'admin/structure/menu/manage/' . $menu['menu_name'] . '/edit/ajax',
    );
    $form['#submit'][] = 'menu_overview_form_submit';
  }
  else {
    $form['#empty_text'] = t('There are no menu links yet. <a href="@link">Add link</a>.', array('@link' => url('admin/structure/menu/manage/'. $form['#menu']['menu_name'] .'/add')));
  }
  return $form;
}

/**
 * Recursive helper function for menu_overview_form().
 *
 * @param $tree
 *   The menu_tree retrieved by menu_tree_data.
 */
function _theforce_menu_overview_tree_form($tree) {
  $form = &drupal_static(__FUNCTION__, array('#tree' => TRUE));
  foreach ($tree as $data) {
    $title = '';
    $item = $data['link'];
    // Don't show callbacks; these have $item['hidden'] < 0.
    if ($item && $item['hidden'] >= 0) {
      $mlid = 'mlid:' . $item['mlid'];
      $form[$mlid]['#item'] = $item;
      $form[$mlid]['#attributes'] = $item['hidden'] ? array('class' => array('menu-disabled')) : array('class' => array('menu-enabled'));
      $form[$mlid]['title']['#markup'] = l($item['title'], $item['href'], $item['localized_options']);
      if ($item['hidden']) {
        $form[$mlid]['title']['#markup'] .= ' (' . t('disabled') . ')';
      }
      elseif ($item['link_path'] == 'user' && $item['module'] == 'system') {
        $form[$mlid]['title']['#markup'] .= ' (' . t('logged in users only') . ')';
      }

      $form[$mlid]['hidden'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable @title menu link', array('@title' => $item['title'])),
        '#title_display' => 'invisible',
        '#default_value' => !$item['hidden'],
      );
      $form[$mlid]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => $item['weight'],
        '#title_display' => 'invisible',
        '#title' => t('Weight for @title', array('@title' => $item['title'])),
      );
      $form[$mlid]['mlid'] = array(
        '#type' => 'hidden',
        '#value' => $item['mlid'],
      );
      $form[$mlid]['plid'] = array(
        '#type' => 'hidden',
        '#default_value' => $item['plid'],
      );
      // Build a list of operations.
      $operations = array();
      $title = t('Edit');
      if(module_exists('fawesome')){
        $title = '<i class="fa fa-edit"></i> ' . $title;
      }
      $operations['edit'] = array('#type' => 'link', '#title' => $title, '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/edit', '#options' => array('html' => true, 'attributes' => array('class' => array('use-theforce'))));
      // Only items created by the menu module can be deleted.
      if ($item['module'] == 'menu' || $item['updated'] == 1) {
        $title = t('Delete');
        if(module_exists('fawesome')){
          $title = '<i class="fa fa-trash-o"></i> ' . $title;
        }
        $operations['delete'] = array('#type' => 'link', '#title' => $title, '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/delete', '#options' => array('html' => true, 'attributes' => array('class' => array('use-theforce'))), '#prefix' => '&nbsp;&nbsp;&nbsp;');
      }
      // Set the reset column.
      elseif ($item['module'] == 'system' && $item['customized']) {
        $title = t('Reset');
        if(module_exists('fawesome')){
          $title = '<i class="fa fa-undo"></i> ' . $title;
        }
        $operations['reset'] = array('#type' => 'link', '#title' => $title, '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/reset', '#options' => array('html' => true, 'attributes' => array('class' => array('use-theforce'))), '#prefix' => '&nbsp;&nbsp;&nbsp;');
      }
      $form[$mlid]['operations'][] = $operations;
    }

    if ($data['below']) {
      _theforce_menu_overview_tree_form($data['below']);
    }
  }
  return $form;
}

/**
 * Close overlay and refresh page.
 */
function theforce_admin_menu_edit_form_refresh_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $commands = array();
  // $commands[] = ajax_command_theforce_overlay_close();
  $commands[] = ajax_command_theforce_reload();
  $commands[] = ajax_command_theforce_overlay(drupal_render($form));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Close overlay and refresh page.
 */
function theforce_admin_menu_edit_form_redirect_ajax($form, &$form_state){
  if (form_get_errors()){
    $commands = array();
    $commands[] = ajax_command_theforce_overlay(drupal_render($form));
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  $trigger = $form_state['triggering_element'];
  $commands = array();
  $commands[] = ajax_command_theforce_reload();
  $commands[] = ajax_command_theforce_redirect($trigger['#redirect']);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Menu item edit interface.
 */
function theforce_admin_menu_edit_item($item){
  $form = drupal_get_form('theforce_admin_menu_edit_item_form', $item);
  $commands = array();
  $commands[] = ajax_command_theforce_overlay(drupal_render($form));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function theforce_admin_menu_edit_item_form($form, &$form_state, $item){
  global $menu_admin;
  form_load_include($form_state, 'inc', 'menu', 'menu.admin');
  $form = menu_edit_item($form, $form_state, 'edit', $item, NULL);
  $form['#validate'][] = 'menu_edit_item_validate';
  $form['#submit'][] = 'menu_edit_item_submit';
  $form['actions']['submit']['#ajax'] = array(
    'callback' => 'theforce_admin_menu_edit_form_redirect_ajax',
    'progress' => 'theforce',
  );
  $form['actions']['submit']['#redirect'] = 'admin/structure/menu/manage/' . $item['menu_name'] . '/edit/ajax';
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/structure/menu/manage/' . $item['menu_name'] . '/edit/ajax',
    '#options' => array('html' => true, 'attributes' => array('class' => array('use-theforce'))),
  );
  if ($item['module'] == 'menu') {
    $title = t('Delete');
    if(module_exists('fawesome')){
      $title = '<i class="fa fa-trash-o"></i> ' . $title;
    }
    $form['actions']['delete'] = array(
      '#type' => 'link',
      '#title' => $title,
      '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/delete',
      '#options' => array('html' => true, 'attributes' => array('class' => array('use-theforce'))),
      '#weight' => 10,
    );
  }
  return $form;
}

/**
 * Menu item delete interface.
 */
function theforce_admin_menu_delete_item($item){
  $form = drupal_get_form('theforce_admin_menu_delete_item_form', $item);
  $commands = array();
  $commands[] = ajax_command_theforce_overlay(drupal_render($form));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function theforce_admin_menu_delete_item_form($form, &$form_state, $item) {
  form_load_include($form_state, 'inc', 'menu', 'menu.admin');
  $form['#item'] = $item;
  $form['#submit'][] = 'menu_item_delete_form_submit';
  $form = theforce_confirm_form($form, t('Are you sure you want to delete the custom menu link %item?', array('%item' => $item['link_title'])), 'admin/structure/menu/manage/' . $item['menu_name'] . '/edit');
  $form['actions']['submit']['#ajax'] = array(
    'callback' => 'theforce_admin_menu_edit_form_redirect_ajax',
    'progress' => 'theforce',
  );
  $form['actions']['submit']['#redirect'] = 'admin/structure/menu/manage/' . $item['menu_name'] . '/edit/ajax';
  return $form;
}

/**
 * Menu item reset interface.
 */
function theforce_admin_menu_reset_item($item){
  $form = drupal_get_form('theforce_admin_menu_reset_item_form', $item);
  $commands = array();
  $commands[] = ajax_command_theforce_overlay(drupal_render($form));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function theforce_admin_menu_reset_item_form($form, &$form_state, $item) {
  form_load_include($form_state, 'inc', 'menu', 'menu.admin');
  $form['item'] = array('#type' => 'value', '#value' => $item);
  $form['#submit'][] = 'menu_reset_item_confirm_submit';
  $form = theforce_confirm_form($form, t('Are you sure you want to reset the link %item to its default values?', array('%item' => $item['link_title'])), 'admin/structure/menu/manage/' . $item['menu_name'] . '/edit', t('Any customizations will be lost. This action cannot be undone.'), t('Reset'));
  $form['actions']['submit']['#ajax'] = array(
    'callback' => 'theforce_admin_menu_edit_form_redirect_ajax',
    'progress' => 'theforce',
  );
  $form['actions']['submit']['#redirect'] = 'admin/structure/menu/manage/' . $item['menu_name'] . '/edit/ajax';
  return $form;
}

/**
 * Confirm form with AJAX support.
 */
function theforce_confirm_form($form, $question, $path, $description = NULL, $yes = NULL, $no = NULL, $name = 'confirm') {
  $description = isset($description) ? $description : t('This action cannot be undone.');

  // Prepare cancel link.
  if (isset($_GET['destination'])) {
    $options = drupal_parse_url(urldecode($_GET['destination']));
  }
  elseif (is_array($path)) {
    $options = $path;
  }
  else {
    $options = array('path' => $path);
  }
  $options['attributes']['class'][] = 'use-theforce';

  drupal_set_title($question, PASS_THROUGH);

  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array('#markup' => '<div class="theforce-overlay-description">' . $description . '</div>');
  $form[$name] = array('#type' => 'hidden', '#value' => 1);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $yes ? $yes : t('Confirm'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => $no ? $no : t('Cancel'),
    '#href' => $options['path'],
    '#options' => $options,
  );
  // By default, render the form using theme_confirm_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'confirm_form';
  }
  return $form;
}
