(function($) {

Drupal.behaviors.theforceAdmin = {
  once: 0,
  attach: function(context, settings) {
    var self = this;
    if(!this.once){
      this.once = 1;
      clouds();
    }
  }
}






var theforceAdminUI = {
  once: 0,
  $itemsTop: {},
  $loader: null,
  transEndEventNames: 'transitionend webkitTransitionEnd oTransitionEnd otransitionend MSTransitionEnd'
};

theforceAdminUI.attach = function(context, settings){
  var self = this;

  if(!self.once){
    self.once = 1;
    self.$loader = $('.theforce-ui-loader');
  }

  self.$itemsTop = $('#theforce-top .theforce-ui-item:not(.theforce-processed)');

  self.itemsTopInit(settings);
  self.itemsTopSort();
}

theforceAdminUI.itemsTopInit = function(settings){
  var self = this, $item, $ops, $drag, $remove, $edit, url, id, config, base, vars;

  self.$itemsTop.addClass('theforce-processed').each(function(){
    $item = $(this);
    $ops = $('<div class="theforce-ui-ops"></div>');
    config = $item.data('config');
    id = $item.data('id');

    // Drag
    $drag = $('<div class="theforce-ui-drag theforce-ui-op"><i class="fa fa-arrows"></i></div>').appendTo($ops);

    // Configure
    if(config){
      base = 'theforce-ui-edit-' + id;
      url = settings.basePath + settings.pathPrefix + settings.theforce.basePath + '/plugin/settings/' + id;
      $edit = $('<a href="'+url+'" class="theforce-ui-edit theforce-ui-op"><i class="fa fa-cog"></i></a>').appendTo($ops);
      vars = {url: url + '/nojs',event: 'click',progress: {type: 'theforce'}};
      Drupal.ajax[base] = new Drupal.ajax(base, $edit, vars);
    }

    // Remove
    base = 'theforce-ui-remove-' + id;
    url = settings.basePath + settings.pathPrefix + settings.theforce.basePath + '/plugin/remove/' + id;
    $remove = $('<a href="' + url + '" class="theforce-ui-remove theforce-ui-op"><i class="fa fa-minus-circle"></i></a>').appendTo($ops);
    vars = {url: url + '/nojs',event: 'click',progress: {type: 'theforce'}};
    Drupal.ajax[base] = new Drupal.ajax(base, $remove, vars);
    // $lock = $('<div class="theforce-ui-lock theforce-ui-op"><i class="fa fa-lock"></i></div>').appendTo($ops);
    $ops.appendTo($item);
  });

  // Bind Ajax behaviors to all items showing the class.
  $('.use-theforce:not(.theforce-processed)').addClass('theforce-processed').each(function () {
    var element_settings = {};
    // Clicked links look better with the throbber than the progress bar.
    element_settings.progress = { 'type': 'theforce' };

    // For anchor tags, these will go to the target of the anchor rather
    // than the usual location.
    if ($(this).attr('href')) {
      element_settings.url = $(this).attr('href') + '/ajax';
      element_settings.event = 'click';
    }
    var base = $(this).attr('id');
    Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
  });
}

theforceAdminUI.itemsTopSort = function(){
  var self = this;
  var els = document.getElementsByClassName('theforce-ui-sort');
  if(els){
    [].forEach.call(els, function (el) {
      var sortable = Sortable.create(el, {
        handle: '.theforce-ui-drag',
        store: {
          get: function (sortable) {
            return [];
          },
          set: function (sortable) {
            var order = sortable.toArray();
            $.ajax({
              type: 'POST',
              url: Drupal.settings.basePath + Drupal.settings.theforce.basePath + '/plugin/weights',
              dataType: 'json',
              data: {weights: JSON.stringify(order)},
              beforeSend: function (xmlhttprequest, options) {
                self.loading();
              },
              success: function(response, status) {
                self.success();
              }
            });
          }
        }
      });
    });
  }
}

theforceAdminUI.loading = function(){
  var self = this;
  self.$loader.addClass('show');
  setTimeout(function(){
    self.$loader.addClass('animate');
  },10);
}

theforceAdminUI.success = function(){
  var self = this;
  self.$loader.removeClass('animate').one(self.transEndEventNames, function(){
    self.$loader.removeClass('show');
  });
}

Drupal.behaviors.theforceAdminUI = theforceAdminUI;




/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
Drupal.ajax.prototype.beforeSendOriginal = Drupal.ajax.prototype.beforeSend;
Drupal.ajax.prototype.beforeSend = function (xmlhttprequest, options){
  this.beforeSendOriginal(xmlhttprequest, options);
  if (this.progress.type == 'theforce') {
    theforceAdminUI.loading();
  }
}

/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
Drupal.ajax.prototype.successOriginal = Drupal.ajax.prototype.success;
Drupal.ajax.prototype.success = function (response, status){
  if (this.progress.type == 'theforce') {
    theforceAdminUI.success();
  }
  this.successOriginal(response, status);
}


























var clouds = function () {

  (function() {
    var lastTime = 0;
    var vendors = ['ms', 'moz', 'webkit', 'o'];
    for(var x = 0; x < vendors.length && !window.requestAnimationFrame; ++x) {
      window.requestAnimationFrame = window[vendors[x]+'RequestAnimationFrame'];
      window.cancelRequestAnimationFrame = window[vendors[x]+
        'CancelRequestAnimationFrame'];
    }

    if (!window.requestAnimationFrame)
      window.requestAnimationFrame = function(callback, element) {
        var currTime = new Date().getTime();
        var timeToCall = Math.max(0, 16 - (currTime - lastTime));
        var id = window.setTimeout(function() { callback(currTime + timeToCall); },
          timeToCall);
        lastTime = currTime + timeToCall;
        return id;
      };

    if (!window.cancelAnimationFrame)
      window.cancelAnimationFrame = function(id) {
        clearTimeout(id);
      };
  }())

  var layers = [],
    objects = [],

    world = document.getElementById( 'world' ),
    viewport = document.getElementById( 'viewport' ),

    d = 0,
    p = 400,
    worldXAngle = 0,
    worldYAngle = 0;

  viewport.style.webkitPerspective = p;
  viewport.style.MozPerspective = p;
  viewport.style.oPerspective = p;

  generate();

  function createCloud() {

    var div = document.createElement( 'div'  );
    div.className = 'cloudBase';
    var x = 500 - ( Math.random() * 1000 );
    var y = 500 - ( Math.random() * 1000 );
    var z = 500 - ( Math.random() * 1000 );
    var t = 'translateX( ' + x + 'px ) translateY( ' + y + 'px ) translateZ( ' + z + 'px )';
    div.style.webkitTransform = t;
    div.style.MozTransform = t;
    div.style.oTransform = t;
    world.appendChild( div );

    for( var j = 0; j < 2 + Math.round( Math.random() * 10 ); j++ ) {
      var cloud = document.createElement( 'img' );
      cloud.style.opacity = 0;
      var r = Math.random();
      var src = '/sites/all/modules/private/theforce/images/settings.cloud.png';
      ( function( img ) { img.addEventListener( 'load', function() {
        img.style.opacity = .8;
      } ) } )( cloud );
      cloud.setAttribute( 'src', src );
      cloud.className = 'cloudLayer';

      var x = 500 - ( Math.random() * 1000 );
      var y = 500 - ( Math.random() * 1000 );
      var z = 100 - ( Math.random() * 200 );
      var a = Math.random() * 360;
      var s = .25 + Math.random();
      x *= .2; y *= .2;
      cloud.data = {
        x: x,
        y: y,
        z: z,
        a: a,
        s: s,
        speed: .1 * Math.random()
      };
      var t = 'translateX( ' + x + 'px ) translateY( ' + y + 'px ) translateZ( ' + z + 'px ) rotateZ( ' + a + 'deg ) scale( ' + s + ' )';
      cloud.style.webkitTransform = t;
      cloud.style.MozTransform = t;
      cloud.style.oTransform = t;

      div.appendChild( cloud );
      layers.push( cloud );
    }

    return div;
  }

  // window.addEventListener( 'mousewheel', onContainerMouseWheel );
  // window.addEventListener( 'DOMMouseScroll', onContainerMouseWheel );

  window.addEventListener( 'mousemove', function( e ) {
    worldYAngle = -( .5 - ( e.clientX / window.innerWidth ) ) * 20;
    worldXAngle = ( .5 - ( e.clientY / window.innerHeight ) ) * 20;
    updateView();
  } );

  function generate() {
    objects = [];
    if ( world.hasChildNodes() ) {
      while ( world.childNodes.length >= 1 ) {
        world.removeChild( world.firstChild );
      }
    }
    for( var j = 0; j < 5; j++ ) {
      objects.push( createCloud() );
    }
  }

  function updateView() {
    var t = 'translateZ( ' + d + 'px ) rotateX( ' + worldXAngle + 'deg) rotateY( ' + worldYAngle + 'deg)';
    world.style.webkitTransform = t;
    world.style.MozTransform = t;
    world.style.oTransform = t;
  }

  function onContainerMouseWheel( event ) {

    event = event ? event : window.event;
    d = d - ( event.detail ? event.detail * -5 : event.wheelDelta / 8 );
    updateView();

  }

  function update (){

    for( var j = 0; j < layers.length; j++ ) {
      var layer = layers[ j ];
      layer.data.a += layer.data.speed;
      var t = 'translateX( ' + layer.data.x + 'px ) translateY( ' + layer.data.y + 'px ) translateZ( ' + layer.data.z + 'px ) rotateY( ' + ( - worldYAngle ) + 'deg ) rotateX( ' + ( - worldXAngle ) + 'deg ) rotateZ( ' + layer.data.a + 'deg ) scale( ' + layer.data.s + ')';
      layer.style.webkitTransform = t;
      layer.style.MozTransform = t;
      layer.style.oTransform = t;
    }

    requestAnimationFrame( update );

  }

  update();

}

})(jQuery);
