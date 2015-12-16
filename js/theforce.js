(function ($, Drupal) {

var theForce = {
  $window: null,
  $body: null,
  $loader: null,
  once: 1,
  region: {},
  forceUses: 0,
  miniWidth: 600
}

theForce.attach = function (context, settings) {
  var self = this;
  if(settings.theforce){

    if(self.once){
      self.$body = $('body');
      self.$window = $(window);
      self.$loader = $('.theforce-ui-loader');
      // Set background color to reflect body background color.
      $('#theforce-content').css('backgroundColor', $('body').css('backgroundColor'));
    }

    // Get our regions
    $('.theforce-region').once('theforce-region').each(function(){
      var $this = $(this);
      var id = $this.attr('id').replace('theforce-', '');
      self.region[id] = $this;
      var method = 'region' + (id.charAt(0).toUpperCase() + id.slice(1));
      if(self[method].init){
        self[method].$element = $this;
        self[method].init(settings);
      }
    });

    // Ajax link bindings.
    self.linkBind(context, settings);
    // Ink link effect
    self.inkBind(context, settings);
    // Dropdown dropdown
    self.dropdown.init(context, settings);
    // Only perform when NOT in management mode
    if(!settings.theforce.isManagement){
      // Mini initialization
      self.mini.init(settings);
    }
    // A flag used to allow child methods to do things only once as the above
    // methods are called for each ajax call.
    self.once = 0;
  }
};

/**
 * Prepare mini mode
 */
 theForce.mini = {};

 theForce.mini.init = function (settings) {
  var self = this;

  for (var id in theForce.region){
    var method = 'region' + (id.charAt(0).toUpperCase() + id.slice(1));
    if(theForce[method].mini){
      theForce[method].mini(settings);
    }
    $('a.theforce-mini-toggle', theForce[method].$element).once('theforce-mini').on('click', {region:id}, function(e){
      e.preventDefault();
      var region = e.data.region;
      // theForce[method].$element.toggleClass('open');
      theForce.$body.toggleClass('theforce-' + region + '-mini-open');
    });
    if(theForce[method].miniWatch){
      theForce[method].miniWatch();
    }
  }

  theForce.$window.on('resize.theforce-mini', function() {
    for (var id in theForce.region){
      var method = 'region' + (id.charAt(0).toUpperCase() + id.slice(1));
      if(theForce[method].miniWatch){
        theForce[method].miniWatch();
      }
    }
  });
 }

/**
 * Prepare the top region.
 */
theForce.regionTop = {
  $element: null,
  $items: null,
  width: 0,
};

theForce.regionTop.init = function (settings) {
  var self = this;
  self.$items = $('.theforce-item', self.$element);
}

theForce.regionTop.mini = function (settings) {
  var self = this, $largest;

  if(self.$element.once('theforce-mini').length){
    if(settings.theforce.topIconOnly){
      theForce.$body.removeClass('theforce-top-icon-only');
      self.$items.each(function(){
        var $item = $(this), itemWidth = $item.outerWidth();
        if(itemWidth > self.width){
          self.width = itemWidth;
          $largest = $item;
        }
      });
      theForce.$body.addClass('theforce-top-icon-only');
    }

    self.$items.each(function(){
      var $item = $(this);
      if(!$largest || $item[0] != $largest[0]){
        self.width += $item.outerWidth();
        self.width += parseInt($item.css("marginLeft"));
        self.width += parseInt($item.css("marginRight"));
      }
    });

    if(theForce.region.side.length){
      self.width += parseInt(settings.theforce.sideWidth);
    }

    theForce.$body.on('theforce-side:open.theforce-top-mini', function(e){
      self.width += (parseInt(settings.theforce.sideWidthActive) - parseInt(settings.theforce.sideWidth));
      self.miniWatch();
    });

    theForce.$body.on('theforce-side:closeEnd.theforce-top-mini', function(e){
      self.width -= (parseInt(settings.theforce.sideWidthActive) - parseInt(settings.theforce.sideWidth));
      self.miniWatch();
    });
  }
}

theForce.regionTop.miniWatch = function () {
  var self = this;
  theForce.$body.removeClass('theforce-top-mini');
  if(theForce.$window.width() < theForce.miniWidth || self.width > theForce.$window.width()){
    theForce.$body.addClass('theforce-top-mini');
  }
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

theForce.regionSide.init = function (settings) {
  var self = this, clientX;
  self.$element.on('touchstart', function (e) {
    self.open();
  });

  // Use mousemove event to prevent mouse from immediately trigger side
  // active events.
  theForce.$body.on('mousemove.theforce-side', function(e){
    clientX = e.clientX;
    theForce.$body.off('mousemove.theforce-side');
  });

  self.$element.on('mouseenter', function (e) {
    if(!self.locked && clientX){
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
    self.$element.on('allTransitionEnd', function(e){
      self.$element.off('allTransitionEnd');
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
    self.$element.on('allTransitionEnd', function(e){
      self.$element.off('allTransitionEnd');
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

theForce.regionSide.mini = function (settings) {
  var self = this;
}

theForce.regionSide.miniWatch = function () {
  var self = this;
  theForce.$body.removeClass('theforce-side-mini');
  if(theForce.$window.width() < theForce.miniWidth){
    theForce.$body.addClass('theforce-side-mini');
  }
}

/**
 * Setup ajax binding for all links using use-theforce.
 */
theForce.linkBind = function (context, settings) {
  var self = this;
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
  // Side close buttons
  $('.theforce-side-close:not(.used-theforce)', context).addClass('used-thefoce').on('click', function(e){
    e.preventDefault();
    theForce.regionSide.close();
  });
  // Side content close buttons
  $('.theforce-side-content-close:not(.used-theforce)', context).addClass('used-thefoce').on('click', function(e){
    e.preventDefault();
    theForce.side.close();
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
      $parent.prepend('<span class="ink-wrap"><span class="ink"></span></span>');
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
    $ink.css({top: y+'px', left: x+'px'}).addClass('animate');
  });
}

/**
 * Overlay
 */
theForce.overlay = {
  $element: null
};

theForce.overlay.build = function () {
  var self = this;
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
  var self = this;
  settings = settings || Drupal.settings;
  if(!self.$element){
    self.build();
  }
  self.$element.html('<div id="theforce-overlay-inner" class="theforce-percist"><div class="theforce-overlay-content">' + html + '</div></div>');
  Drupal.attachBehaviors(self.$element, settings);
}

theForce.overlay.close = function () {
  var self = this;
  if(self.$element){
    theForce.shade.close();
    theForce.$body.removeClass('has-theforce-overlay');
    theForce.$window.off('click.theforce-overlay');
    self.$element.removeClass('animate').on('allTransitionEnd', function(e){
      self.$element.off('allTransitionEnd');
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
  $wrapper: null,
  $element: null
};

theForce.side.build = function () {
  var self = this;
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
  var self = this;
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
  var self = this;
  me = this;
  if(self.$element){
    self.$element.removeClass('animate').on('allTransitionEnd', function(e){
      self.$element.off('allTransitionEnd');
      self.$element.remove();
      self.$element = null;
    });
    theForce.regionSide.unlock();
    theForce.regionSide.close();
    theForce.shade.close();
    theForce.$window.off('click.theforce-side-content');
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
  theForce.$loader.removeClass('animate').on('allTransitionEnd', function(e){
    theForce.$loader.off('allTransitionEnd');
    theForce.$loader.removeClass('show');
  });
}

Drupal.behaviors.theforce = theForce;





/**
 * Redirect content area
 */
Drupal.ajax.prototype.commands.theforceRedirect = function (ajax, response, status) {
  var $element = $('<div />');
  var element_settings = {};
  element_settings.progress = { 'type': 'theforce' };
  var base = 'theforce-redirect';
  // For anchor tags, these will go to the target of the anchor rather
  // than the usual location.
  element_settings.theforce = 1;
  element_settings.url = response.url;
  element_settings.event = 'onload';
  element_settings.base = base;
  element_settings.effect = 'fade';
  element_settings.speed = 600;
  Drupal.ajax[base] = new Drupal.ajax(base, $element, element_settings);
  $element.trigger('onload');
  $element.remove();
};


/**
 * Reload content area
 */
Drupal.ajax.prototype.commands.theforceReload = function (ajax, response, status) {
  var $element = $('#theforce-content');
  var element_settings = {};
  element_settings.progress = { 'type': 'theforce' };
  var base = 'theforce-reload';
  // For anchor tags, these will go to the target of the anchor rather
  // than the usual location.
  element_settings.theforce = 1;
  element_settings.url = window.location.protocol + '//'
    + window.location.hostname + '/'
    + window.location.pathname + '/'
    + '?theforce=1'
    + window.location.hash.replace('#','','g');
  element_settings.event = 'onload';
  element_settings.base = base;
  element_settings.effect = 'fade';
  element_settings.speed = 600;
  Drupal.ajax[base] = new Drupal.ajax(base, $element, element_settings);
  $element.trigger('onload');
};





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
