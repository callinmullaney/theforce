<?php

/**
 * Query hander for Force plugins.
 */
class theforceQuery{

  // The database query results.
  static private $results = array();
  // All ctools plugins of $type.
  static private $plugins = array();
  // The query results.
  static private $query = array();
  // A flag used to return a single result. Default returns an array of results.
  static private $single;
  // The value to return if no results are found.
  static private $empty;

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

  final public static function filterRegion($region){
    self::$query = array_filter(self::$query, function($val) use( &$region) {
      return $val['region'] == $region;
    });
    return new static;
  }

  final public static function loadHandler(){
    foreach(self::$query as $id => &$plugin){
      $plugin['handler'] = theforce_plugin_get($plugin['type'], $id, $plugin['settings']);
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
    $results = self::results();
    $plugins = self::plugins();
    foreach($plugins as $id => &$plugin){
      $plugin['active'] = isset($results[$id]) ? 1: 0;
      $plugin['region'] = isset($results[$id]->region) ? $results[$id]->region : '';
      $plugin['weight'] = isset($results[$id]->weight) ? $results[$id]->weight : 0;
      $plugin['settings'] = isset($results[$id]->settings) ? $results[$id]->settings : array();
      // Rename type for ease of use.
      $plugin['type'] = $plugin['plugin type'];
      unset($plugin['plugin type']);
      $plugin['id'] = $plugin['name'];
      unset($plugin['name']);
    }
    uasort($plugins, 'drupal_sort_weight');
    return $plugins;
  }

  final private static function results(){
    if(empty(self::$results)){
      $results = db_select('theforce_plugin', 'f')
        ->fields('f')
        ->execute()
        ->fetchAll();
      foreach($results as $result){
        $result->settings = !empty($result->settings) ? unserialize($result->settings) : array();
        self::$results[$result->id] = $result;
      }
    }
    return self::$results;
  }

  final private static function plugins(){
    if(empty(self::$plugins)){
      ctools_include('plugins');
      foreach(theforce_ctools_plugin_type() as $type => $info){
        $plugins = ctools_get_plugins('theforce', $type);
        self::$plugins += $plugins;
      }
    }
    return self::$plugins;
  }
}
