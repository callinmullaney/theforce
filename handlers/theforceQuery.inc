<?php

/**
 * Query hander for Force plugins.
 */
final class theforceQuery{

  private function __construct() {}
  // The query active.
  private static $query = array();
  // All prepared results.
  private static $results = null;
  // The database query active.
  private static $active = null;
  // The database query custom.
  private static $custom = null;
  // All ctools plugins of $type.
  private static $plugins = null;
  // A flag used to return a single result. Default returns an array of active.
  private static $single;
  // The value to return if no active are found.
  private static $empty;

  final public static function query(){
    self::$query = self::prepare();
    self::$single = false;
    self::$empty = array();
    return new static;
  }

  final public static function filterActive(){
    self::$query = array_filter(self::$query, function($val) {
      return !empty($val['active']);
    });
    return new static;
  }

  final public static function filterInactive(){
    self::$query = array_filter(self::$query, function($val) {
      return empty($val['active']);
    });
    return new static;
  }

  final public static function filterInvalid($region = null, $location = null){
    if($region){
      self::$query = array_filter(self::$query, function($val) use( &$region) {
        return !isset($val['limit_region']) || in_array($region, $val['limit_region']);
      });
    }
    if($location){
      self::$query = array_filter(self::$query, function($val) use( &$location) {
        return !isset($val['limit_location']) || in_array($location, $val['limit_location']);
      });
    }
    return new static;
  }

  final public static function filterId($id){
    self::$single = true;
    self::$query = array_filter(self::$query, function($val) use( &$id) {
      return $val['id'] == $id;
    });
    return new static;
  }

  final public static function filterType($type){
    self::$query = array_filter(self::$query, function($val) use( &$type) {
      return $val['type'] == $type;
    });
    return new static;
  }

  /**
   * Filter to only show public plugins. Plugins that are allowed to be added
   * to regions.
   *
   * A public plugin is either a jedi plugin or a generated plugin built off of
   * a master plugin.
   */
  final public static function filterPublic(){
    self::$query = array_filter(self::$query, function($val) use( &$type) {
      return $val['type'] == 'jedi' || !empty($val['custom']);
    });
    return new static;
  }

  /**
   * Filter to only show private plugins. Plugins that are not allowed to be
   * added directly to regions and must be generated as a custom plugin.
   */
  final public static function filterPrivate(){
    self::$query = array_filter(self::$query, function($val) use( &$type) {
      return $val['type'] == 'master' && empty($val['custom']);
    });
    return new static;
  }

  final public static function filterRegion($region){
    self::$query = array_filter(self::$query, function($val) use( &$region) {
      return $val['region'] == $region;
    });
    return new static;
  }

  final public static function filterLocation($location){
    self::$query = array_filter(self::$query, function($val) use( &$location) {
      return $val['location'] == $location;
    });
    return new static;
  }

  final public static function loadHandler(){
    foreach(self::$query as $id => &$plugin){
      $plugin['handler'] = theforce_plugin_get($plugin);
    }
    return new static;
  }

  final public static function returnOnEmpty($value){
    self::$empty = $value;
    return new static;
  }

  final public static function execute(){
    return empty(self::$query) ? self::$empty : ((self::$single) ? reset(self::$query) : self::$query);
  }

  final private static function prepare(){
    if(is_null(self::$results)){
      $active = self::active();
      $custom = self::custom();
      $plugins = self::plugins();
      foreach($custom as $id => $item){
        if(isset($plugins[$item->class])){
          $plugins[$id] = $plugins[$item->class];
          $plugins[$id]['custom'] = 1;
          $plugins[$id]['default'] = !empty($item->default);
          $plugins[$id]['id'] = $id;
          $plugins[$id]['title'] = $item->title;
          $plugins[$id]['settings'] = $item->settings;
          // if(isset($item->region) && isset($item->location) && isset($item->weight)){
          //   $active[$id] = (object) array(
          //     'id' => $id,
          //     'region' => $item->region,
          //     'location' => $item->location,
          //     'weight' => $item->weight,
          //     'settings' => $item->settings,
          //   );
          // }
        }
      }
      foreach($plugins as $id => &$plugin){
        $plugin['active'] = isset($active[$id]) ? 1: 0;
        $plugin['region'] = isset($active[$id]->region) ? $active[$id]->region : '';
        $plugin['location'] = isset($active[$id]->location) ? $active[$id]->location : '';
        $plugin['weight'] = isset($active[$id]->weight) ? $active[$id]->weight : 0;
        $plugin['settings'] = isset($plugin['settings']) && is_array($plugin['settings']) ? $plugin['settings'] : array();
        $plugin['settings_custom'] = 0;
        if(isset($active[$id]->settings) && is_array($active[$id]->settings) && !empty($active[$id]->settings)){
          $plugin['settings'] = drupal_array_merge_deep($plugin['settings'], $active[$id]->settings);
          $plugin['settings_custom'] = 1;
        }
        // Rename type for ease of use.
        $plugin['type'] = $plugin['plugin type'];
        // Rename class for ease of use
        $plugin['class'] = $plugin['name'];
        if(!isset($plugin['id'])){
          $plugin['id'] = $plugin['class'];
        }
      }
      uasort($plugins, 'drupal_sort_weight');
      self::$results = $plugins;
    }
    return self::$results;
  }

  final private static function active(){
    if(is_null(self::$active)){
      self::$active = array();
      $active = db_select('theforce_active', 'f')
        ->fields('f')
        ->execute()
        ->fetchAll();
      foreach($active as $result){
        $result->settings = !empty($result->settings) ? unserialize($result->settings) : array();
        self::$active[$result->id] = $result;
      }
    }
    return self::$active;
  }

  final private static function custom(){
    if(is_null(self::$custom)){
      self::$custom = array();

      // if ($cache = cache_get('theforce_custom', 'cache')) {
      //   self::$custom = $cache->data;
      // }
      foreach (module_implements('theforce_items') as $module) {
        $module_custom = module_invoke($module, 'theforce_items');
        foreach($module_custom as $id => $custom){
          $custom['id'] = $id;
          $custom['default'] = 1;
          self::$custom[$id] = (object) $custom;
        }
      }

      $custom = db_select('theforce_custom', 'f')
        ->fields('f')
        ->execute()
        ->fetchAll();
      foreach($custom as $result){
        $result->settings = !empty($result->settings) ? unserialize($result->settings) : array();
        self::$custom[$result->id] = $result;
      }

      drupal_alter('theforce_items', self::$custom);
      // cache_set('theforce_custom', self::$custom);
    }
    return self::$custom;
  }

  final private static function plugins(){
    if(is_null(self::$plugins)){
      self::$plugins = array();
      ctools_include('plugins');
      foreach(theforce_ctools_plugin_type() as $type => $info){
        $plugins = ctools_get_plugins('theforce', $type);
        self::$plugins += $plugins;
      }
    }
    return self::$plugins;
  }
}
