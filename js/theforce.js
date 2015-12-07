(function ($, Drupal) {

var theForce = {
  $window: null,
  $body: null,
  $loader: null,
  once: 1,
  region: {}
}

theForce.attach = function (context, settings) {
  var self = this;
  if(settings.theforce){

    if(self.once){
      self.$body = $('body');
      self.$window = $(window);
      self.$loader = $('.theforce-ui-loader');
    }

    // Get our regions
    $('.theforce-region').once('theforce-region-processed').each(function(){
      var $this = $(this);
      var id = $this.attr('id').replace('theforce-', '');
      self.region[id] = $this;
      var method = 'region' + (id.charAt(0).toUpperCase() + id.slice(1));
      if(self[method].init){
        self[method].$element = $this;
        self[method].init();
      }
    });

    // Ajax link bindings.
    self.linkAjaxBind(context, settings);
    // Ink link effect
    self.inkBind(context, settings);
    // Dropdown dropdown
    self.dropdown.init(context, settings);
    // Mini initialization
    self.mini.init(context, settings);
    // A flag used to allow child methods to do things only once as the above
    // methods are called for each ajax call.
    self.once = 0;
  }
};

/**
 * Prepare mini mode
 */
 theForce.mini = {};

 theForce.mini.init = function () {
  console.log('init mini');
 }

/**
 * Prepare the top region.
 */
theForce.regionTop = {
  $element: null
};

theForce.regionTop.init = function () {
  var self = this;
}

/**
 * Prepare the side region.
 */
theForce.regionSide = {
  $element: null,
  delay: 400,
  active: 0,
  locked: 0,
  timeout: null
};

theForce.regionSide.init = function () {
  var self = this;
  self.$element.on('mouseenter', function (e) {
    if(!self.locked){
      self.timeout = setTimeout(function(){
        self.open();
      }, self.delay);
    }
  }).on('mouseleave', function (e) {
    if(!self.locked){
      clearTimeout(self.timeout);
      self.close();
    }
  });
}

theForce.regionSide.open = function () {
  var self = this;
  if(!self.active){
    self.active = 1;
    theForce.$body.addClass('theforce-side-active').trigger('theforce-side:open');
    self.$element.afterTransition(function (e) {
      theForce.$body.trigger('theforce-side:openEnd');
    });
  }
}

theForce.regionSide.close = function () {
  var self = this;
  if(self.active){
    self.active = 0;
    self.unlock();
    theForce.$body.removeClass('theforce-side-active').trigger('theforce-side:close');
    self.$element.afterTransition(function (e) {
      theForce.$body.trigger('theforce-side:closeEnd');
    });
  }
}

theForce.regionSide.lock = function () {
  this.locked = 1;
}

theForce.regionSide.unlock = function () {
  this.locked = 0;
}

/**
 * Setup ajax binding for all links using use-theforce.
 */
theForce.linkAjaxBind = function (context, settings) {
  $('.use-theforce:not(.used-theforce)', context).addClass('used-theforce').each(function () {
    var element_settings = {};
    var progress = $(this).data('progress') || 'theforce';
    // Clicked links look better with the throbber than the progress bar.
    element_settings.progress = { 'type': progress };
    var base = $(this).attr('id') || 'use-theforce-' + (self.forceUses++);
    // For anchor tags, these will go to the target of the anchor rather
    // than the usual location.
    if ($(this).attr('href')) {
      element_settings.theforce = 1;
      element_settings.url = $(this).attr('href') + '/ajax?destination=' + encodeURIComponent(settings.theforce.basePath);
      element_settings.event = 'click.theforce';
      element_settings.once = $(this).data('once') || 0;
      element_settings.base = base;
      element_settings.effect = 'fade';
      element_settings.speed = 600;
    }
    Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
  });
}

/**
 * Dropdown handler.
 */
theForce.dropdown = {
  $triggers: null,
  $dropdown: null,
  $active: null
};

theForce.dropdown.init = function (context, settings) {
  var self = this;
  self.$triggers = $('.theforce-dropdown-trigger', context).once();
  if(self.$triggers.length){
    self.$triggers.on('click', self.click);
  }

  if(theForce.once){
    theForce.$body.on('theforce-dropdown:open.theforce-dropdown', function(e){
      self.close();
    }).on('theforce-side:open.theforce-dropdown', function(){
      self.close();
    }).on('theforce-side:close.theforce-dropdown', function(){
      self.close();
    });
  }
}

theForce.dropdown.click = function(e){
  var self = theForce.dropdown;
  e.preventDefault();

  self.$dropdown = $(this).closest('.theforce-dropdown');
  if(self.$dropdown.length){
    self.open();
  }
};

theForce.dropdown.open = function(e){
  var self = this, current = self.$dropdown.hasClass('open');
  theForce.$body.trigger('theforce-dropdown:open');
  if(!current){
    self.$active = self.$dropdown;
    self.$active.addClass('open');
    setTimeout(function(){
      theForce.$window.on('click.theforce-dropdown', function(e){
        if(!$(e.target).closest('.theforce-dropdown-content').length){
          self.close();
        }
      });
    }, 10);
  }
};

theForce.dropdown.close = function(e){
  var self = this;
  if(self.$active){
    self.$active.removeClass('open');
    self.$active = null;
    theForce.$window.off('click.theforce-dropdown');
  }
};

/**
 * Ink handler.
 */
theForce.inkBind = function(context, settings) {
  var $parent, $ink, d, x, y;
  $(".theforce-item a").once('theforce-ink').click(function(e){
    $parent = $(this).parent();
    //create .ink element if it doesn't exist
    if($parent.find(".ink").length == 0){
      $parent.prepend("<span class='ink-wrap'><span class='ink'></span></span>");
    }

    $ink = $parent.find(".ink");
    //incase of quick double clicks stop the previous animation
    $ink.removeClass("animate");

    //set size of .ink
    if(!$ink.height() && !$ink.width()){
      //use parent's width or height whichever is larger for the diameter to make a circle which can cover the entire element.
      d = Math.max($parent.outerWidth(), $parent.outerHeight());
      $ink.css({height: d, width: d});
    }

    //get click coordinates
    //logic = click coordinates relative to page - parent's position relative to page - half of self height/width to make it controllable from the center;
    x = e.pageX - $parent.offset().left - $ink.width()/2;
    y = e.pageY - $parent.offset().top - $ink.height()/2;

    //set the position and add class .animate
    $ink.css({top: y+'px', left: x+'px'}).addClass("animate");
  });
}

/**
 * Overlay
 */
theForce.overlay = {
  $element: null
};

theForce.overlay.build = function () {
  self = this;
  self.$element = $('<div id="theforce-overlay" class="theforce"></div>').appendTo($('#theforce-wrap'));

  theForce.$window.on('click.theforce-overlay', function(e){
    if(!$(e.target).closest('.theforce-percist').length){
      self.close();
    }
  });

  setTimeout(function(){
    theForce.shade.open();
    theForce.$body.addClass('has-theforce-overlay');
    self.$element.addClass('animate');
  }, 10);
}

theForce.overlay.open = function (html, settings) {
  self = this;
  settings = settings || Drupal.settings;
  self.close();
  if(!self.$element){
    self.build();
  }
  self.$element.html('<div id="theforce-overlay-inner" class="theforce-percist"><div class="theforce-overlay-content">' + html + '</div></div>');
  Drupal.attachBehaviors(self.$element, settings);
}

theForce.overlay.close = function () {
  self = this;
  if(self.$element){
    theForce.shade.close();
    theForce.$body.removeClass('has-theforce-overlay');
    theForce.$window.off('click.theforce-overlay');
    self.$element.removeClass('animate').afterTransition(function (e) {
      self.$element.remove();
      self.$element = null;
    });
  }
}

/**
 * Command to insert new content into an overlay.
 */
Drupal.ajax.prototype.commands.theforceOverlay = function (ajax, response, status) {
  var settings = response.settings || ajax.settings || Drupal.settings;
  theForce.overlay.open(response.data, settings);
};

/**
 * Command to close an active overlay.
 */
Drupal.ajax.prototype.commands.theforceOverlayClose = function (ajax, response, status) {
  theForce.overlay.close();
};




/**
 * Side Content
 */

theForce.side = {
  $element: null
};

theForce.side.build = function () {
  self = this;
  self.$wrapper = $('#theforce-side');
  self.$element = $('<div id="theforce-side-content" class="theforce-percist"></div>').appendTo(self.$wrapper);

  $(window).on('click.theforce-side-content', function(e){
    if(!$(e.target).closest('.theforce-percist').length){
      self.close();
    }
  });

  setTimeout(function(){
    theForce.shade.open();
    self.$element.addClass('animate');
  }, 20);
}

theForce.side.open = function (html, settings) {
  self = this;
  settings = settings || Drupal.settings;
  if(!self.$element){
    self.build();
  }
  self.$element.html(html);
  Drupal.attachBehaviors(self.$element, settings);
  theForce.regionSide.lock();
  theForce.regionSide.open();
}

theForce.side.close = function () {
  self = this;
  if(self.$element){
    self.$element.removeClass('animate').afterTransition(function (e) {
      if(self.$element){
        self.$element.remove();
        self.$element = null;
      }
    });
    theForce.regionSide.unlock();
    theForce.regionSide.close();
    theForce.shade.close();
    theForce.$window.off('click' + '.theforce-side-content');
  }
}

/**
 * Command to insert new content into shelf.
 */
Drupal.ajax.prototype.commands.theforceSide = function (ajax, response, status) {
  var settings = response.settings || ajax.settings || Drupal.settings;
  theForce.side.open(response.data, settings);
};

Drupal.ajax.prototype.commands.theforceSideClose = function (ajax, response, status) {
  theForce.side.close();
};


/**
 * Inset and shade the content area.
 */
theForce.shade = {
  count: 0,
};

theForce.shade.open = function() {
  var self = this;
  if(!self.count){
    theForce.$body.addClass('theforce-inset');
  }
  self.count++;
}

theForce.shade.close = function() {
  var self = this;
  self.count--;
  if(!self.count){
    theForce.$body.removeClass('theforce-inset');
  }
}

/**
 * Global loader
 */
theForce.loader = {};

theForce.loader.show = function(){
  var self = this;
  theForce.$loader.addClass('show');
  setTimeout(function(){
    theForce.$loader.addClass('animate');
  }, 10);
}

theForce.loader.hide = function(){
  var self = this;
  theForce.$loader.removeClass('animate').afterTransition(function (e) {
    theForce.$loader.removeClass('show');
  });
}

Drupal.behaviors.theforce = theForce;








/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
Drupal.ajax.prototype.beforeSendOriginal = Drupal.ajax.prototype.beforeSend;
Drupal.ajax.prototype.beforeSend = function (xmlhttprequest, options){
  this.beforeSendOriginal(xmlhttprequest, options);
  if (this.progress.type == 'theforce') {
    theForce.loader.show();
  }
}

/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
var successOriginal = Drupal.ajax.prototype.success;
Drupal.ajax.prototype.success = function (response, status){
  if (this.progress.type == 'theforce') {
    theForce.loader.hide();
  }
  // Unbind events if once is set.
  if (this.theforce && this.once) {
    delete Drupal.ajax[this.base];
    $(this.element).unbind('click.theforce');
  }
  successOriginal.call(this, response, status);
}

})(jQuery, Drupal);
