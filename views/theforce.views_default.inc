<?php

/**
 * Implements hook_views_default_views().
 */
function theforce_views_default_views() {
  $views = array();
  $views['theforce_content'] = theforce_views_default_theforce_content();

  if(module_exists('asset')){
    $views['theforce_assets'] = theforce_views_default_theforce_assets();
  }
  return $views;
}

/**
 * Node Management
 */
function theforce_views_default_theforce_content() {

  $view = new view();
  $view->name = 'theforce_content';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'The Force | Manage Content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage Content';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'status' => 'status',
    'created' => 'created',
    'changed' => 'changed',
    'ops' => 'ops',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'ops' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Content: Creation */
  $handler->display->display_options['header']['creation']['id'] = 'creation';
  $handler->display->display_options['header']['creation']['table'] = 'node';
  $handler->display->display_options['header']['creation']['field'] = 'creation';
  $handler->display->display_options['header']['creation']['label'] = '';
  $handler->display->display_options['header']['creation']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'strong';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'custom';
  $handler->display->display_options['fields']['status']['type_custom_true'] = '<i class="fa fa-check text-success"></i>';
  $handler->display->display_options['fields']['status']['type_custom_false'] = '<i class="fa fa-times text-warning"></i>';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  if(module_exists('views_ops')){
    /* Field: Content: Operation links */
    $handler->display->display_options['fields']['ops']['id'] = 'ops';
    $handler->display->display_options['fields']['ops']['table'] = 'node';
    $handler->display->display_options['fields']['ops']['field'] = 'ops';
    $handler->display->display_options['fields']['ops']['label'] = '';
    $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
  }
  else{
    /* Field: Content: Edit link */
    $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
    $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
    $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
    $handler->display->display_options['fields']['edit_node']['label'] = '';
    $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['edit_node']['text'] = '<i class="fa fa-edit"></i> Edit';
    /* Field: Content: Delete link */
    $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
    $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
    $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
    $handler->display->display_options['fields']['delete_node']['label'] = '';
    $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['delete_node']['text'] = '<i class="fa fa-trash-o"></i> Delete';
  }
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';

  foreach(node_type_get_types() as $type => $data){

    /* Display: Full */
    $handler = $view->new_display('page', $data->name . ' Full', $type . '_full');
    $handler->display->display_options['path'] = 'admin/content/' . $type;
    $handler->display->display_options['defaults']['access'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'edit any ' . $type . ' content';
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Content: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'node';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      $type => $type,
    );
    $handler->display->display_options['filters']['type']['group'] = 1;
    /* Filter criterion: Content: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['group'] = 1;
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

    /* Display: Mini */
    $handler = $view->new_display('page', $data->name . ' Mini', $type . '_mini');
    $handler->display->display_options['path'] = 'admin/content/' . $type;
    $handler->display->display_options['defaults']['access'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'edit any ' . $type . ' content';
    $handler->display->display_options['defaults']['use_more'] = FALSE;
    $handler->display->display_options['use_more'] = TRUE;
    $handler->display->display_options['defaults']['use_more_always'] = FALSE;
    $handler->display->display_options['use_more_always'] = TRUE;
    $handler->display->display_options['defaults']['use_more_text'] = FALSE;
    $handler->display->display_options['use_more_text'] = 'View All';
    $handler->display->display_options['defaults']['pager'] = FALSE;
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '6';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['header'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
    $handler->display->display_options['fields']['title']['element_type'] = 'strong';
    /* Field: Content: Published */
    $handler->display->display_options['fields']['status']['id'] = 'status';
    $handler->display->display_options['fields']['status']['table'] = 'node';
    $handler->display->display_options['fields']['status']['field'] = 'status';
    $handler->display->display_options['fields']['status']['type'] = 'custom';
    $handler->display->display_options['fields']['status']['type_custom_true'] = '<i class="fa fa-check text-success"></i>';
    $handler->display->display_options['fields']['status']['type_custom_false'] = '<i class="fa fa-times text-warning"></i>';
    $handler->display->display_options['fields']['status']['not'] = 0;
    if(module_exists('views_ops')){
      /* Field: Content: Operation links */
      $handler->display->display_options['fields']['ops']['id'] = 'ops';
      $handler->display->display_options['fields']['ops']['table'] = 'node';
      $handler->display->display_options['fields']['ops']['field'] = 'ops';
      $handler->display->display_options['fields']['ops']['label'] = '';
      $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
    }
    else{
      /* Field: Content: Edit link */
      $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
      $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
      $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
      $handler->display->display_options['fields']['edit_node']['label'] = '';
      $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
      $handler->display->display_options['fields']['edit_node']['text'] = '<i class="fa fa-edit"></i> Edit';
      /* Field: Content: Delete link */
      $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
      $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
      $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
      $handler->display->display_options['fields']['delete_node']['label'] = '';
      $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
      $handler->display->display_options['fields']['delete_node']['text'] = '<i class="fa fa-trash-o"></i> Delete';
    }
    /* Filter criterion: Content: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'node';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      $type => $type,
    );
    $handler->display->display_options['filters']['type']['group'] = 1;
    /* Filter criterion: Content: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['group'] = 1;
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  }

  return $view;
}


/**
 * Asset Management
 */
function theforce_views_default_theforce_assets() {

  $view = new view();
  $view->name = 'theforce_assets';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'asset';
  $view->human_name = 'The Force | Manage Assets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage Assets';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'status' => 'status',
    'created' => 'created',
    'changed' => 'changed',
    'asset_ops' => 'asset_ops',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'asset_ops' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Asset: Asset Creation */
  $handler->display->display_options['header']['asset_creation']['id'] = 'asset_creation';
  $handler->display->display_options['header']['asset_creation']['table'] = 'asset';
  $handler->display->display_options['header']['asset_creation']['field'] = 'asset_creation';
  $handler->display->display_options['header']['asset_creation']['label'] = '';
  $handler->display->display_options['header']['asset_creation']['element_label_colon'] = FALSE;
  /* Field: Asset: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'asset';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Sort criterion: Asset: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'asset';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';

  foreach(asset_get_types() as $type => $data){

    /* Display: Page */
    $handler = $view->new_display('page', $data->title . ' Full', $type . '_full');
    $handler->display->display_options['path'] = 'admin/assets/' . $type;
    $handler->display->display_options['defaults']['access'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'edit any ' . $type . ' asset';
    $handler->display->display_options['defaults']['fields'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Field: Asset: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'asset';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = !empty($data->data['title_label']) ? $data->data['title_label'] : 'Title';
    if(!empty($data->data['allow_status'])){
      /* Field: Asset: Published */
      $handler->display->display_options['fields']['status']['id'] = 'status';
      $handler->display->display_options['fields']['status']['table'] = 'asset';
      $handler->display->display_options['fields']['status']['field'] = 'status';
      $handler->display->display_options['fields']['status']['type'] = 'custom';
      $handler->display->display_options['fields']['status']['type_custom_true'] = '<i class="fa fa-check text-success"></i>';
      $handler->display->display_options['fields']['status']['type_custom_false'] = '<i class="fa fa-times text-warning"></i>';
      $handler->display->display_options['fields']['status']['not'] = 0;
    }
    /* Field: Asset: Created */
    $handler->display->display_options['fields']['created']['id'] = 'created';
    $handler->display->display_options['fields']['created']['table'] = 'asset';
    $handler->display->display_options['fields']['created']['field'] = 'created';
    $handler->display->display_options['fields']['created']['date_format'] = 'short';
    $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
    /* Field: Asset: Changed */
    $handler->display->display_options['fields']['changed']['id'] = 'changed';
    $handler->display->display_options['fields']['changed']['table'] = 'asset';
    $handler->display->display_options['fields']['changed']['field'] = 'changed';
    $handler->display->display_options['fields']['changed']['label'] = 'Updated';
    $handler->display->display_options['fields']['changed']['date_format'] = 'short';
    $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
    /* Field: Asset: Operation links */
    $handler->display->display_options['fields']['asset_ops']['id'] = 'asset_ops';
    $handler->display->display_options['fields']['asset_ops']['table'] = 'asset';
    $handler->display->display_options['fields']['asset_ops']['field'] = 'asset_ops';
    $handler->display->display_options['fields']['asset_ops']['label'] = '';
    /* Filter criterion: Asset: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'asset';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      $type => $type,
    );
    /* Filter criterion: Asset: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'asset';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

    /* Display: Mini */
    $handler = $view->new_display('page', $data->title . ' Mini', $type . '_mini');
    $handler->display->display_options['path'] = 'admin/assets/' . $type;
    $handler->display->display_options['defaults']['access'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = 'edit any ' . $type . ' asset';
    $handler->display->display_options['defaults']['use_more'] = FALSE;
    $handler->display->display_options['use_more'] = TRUE;
    $handler->display->display_options['defaults']['use_more_always'] = FALSE;
    $handler->display->display_options['use_more_always'] = TRUE;
    $handler->display->display_options['defaults']['use_more_text'] = FALSE;
    $handler->display->display_options['use_more_text'] = 'View All';
    $handler->display->display_options['defaults']['pager'] = FALSE;
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '6';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['defaults']['header'] = FALSE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Field: Asset: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'asset';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = !empty($data->data['title_label']) ? $data->data['title_label'] : 'Title';
    if(!empty($data->data['allow_status'])){
      /* Field: Asset: Published */
      $handler->display->display_options['fields']['status']['id'] = 'status';
      $handler->display->display_options['fields']['status']['table'] = 'asset';
      $handler->display->display_options['fields']['status']['field'] = 'status';
      $handler->display->display_options['fields']['status']['type'] = 'custom';
      $handler->display->display_options['fields']['status']['type_custom_true'] = '<i class="fa fa-check text-success"></i>';
      $handler->display->display_options['fields']['status']['type_custom_false'] = '<i class="fa fa-times text-warning"></i>';
      $handler->display->display_options['fields']['status']['not'] = 0;
    }
    /* Field: Asset: Operation links */
    $handler->display->display_options['fields']['asset_ops']['id'] = 'asset_ops';
    $handler->display->display_options['fields']['asset_ops']['table'] = 'asset';
    $handler->display->display_options['fields']['asset_ops']['field'] = 'asset_ops';
    $handler->display->display_options['fields']['asset_ops']['label'] = '';
    /* Filter criterion: Asset: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'asset';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      $type => $type,
    );
    /* Filter criterion: Asset: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'asset';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

  }


  return $view;
}
