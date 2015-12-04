(function ($, Drupal) {

var theForce = {
  once: 0,
  $dropdown: null,
  $loader: null,
  forceUses: 0,
  sideActive: 0,
  transEndEventNames: 'transitionend webkitTransitionEnd oTransitionEnd otransitionend MSTransitionEnd'
}

theForce.attach = function(context, settings) {
  var self = this;
  if(settings.theforce){
    self.$body = $('body');

    if(!self.once){
      self.once = 1;
      // Set background color to reflect body background color.
      $('#theforce-content').css('backgroundColor', $('body').css('backgroundColor'));
      self.$loader = $('.theforce-ui-loader');
    }

    // Bind Ajax behaviors to all items showing the class.
    $('.use-theforce:not(.used-theforce)').addClass('used-theforce').each(function () {
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

    self.side(context, settings);
    self.dropdown(context, settings);
    // self.menu(context, settings);
    // self.fitH('#theforce-top', context, settings);
    self.ink(context, settings);
  }
};


  /**
   * Sizing handler.
   */
theForce.fit = {h:[],w:[]};
theForce.fitH = function(selector){
  var self = this;
  var $element = $(selector);
  var elementWidth = $element.width();
  var locationWidth = 0
  // var locationItemWidth;
  // $('.theforce-location', $element).each(function(){
  //   locationItemWidth = 0;
  //   $('.theforce-item', $(this)).each(function(){
  //     locationItemWidth += $(this).outerWidth();
  //   });
  //   $(this).width(locationItemWidth);
  //   locationWidth += locationItemWidth;
  // });

  $('.theforce-location', $element).each(function(){
    locationWidth += $(this).outerWidth();
  });

  // $element.css({minWidth: locationWidth + 'px'});
  $element.addClass('mini');
  // if(elementWidth < locationWidth){
  //   $element.addClass('mini');
  // }
  // else{
  //   $element.removeClass('mini');
  // }
  // theForce.fit.h.push(selector);
  // self.fitWatch();
}

theForce.fitWatchInit = 0;
theForce.fitWatch = function(){
  var self = this;

  if(!theForce.fitWatchInit){
    var timer;
    theForce.fitWatchInit = 1;
    $( window ).resize(function() {
      clearTimeout(timer);
      timer = setTimeout(function(){
        $.each(self.fit.h, function(index, selector){
          self.fitH(selector);
        });
      }, 200);

    });
  }
}


  /**
   * Dropdown handler.
   */
theForce.side = function(context, settings){
  var self = this;
  if(!settings.theforce.isManagement){
    var $side = $('#theforce-side:not(.theforce-side-processed)').addClass('theforce-side-processed');
    var timeout;
    if($side.length){
      var clientX;
      // Use mousemove event to prevent mouse from immediately trigger side
      // active events.
      $('body').on('mousemove.theforce', function(e){
        clientX = e.clientX;
        $('body.theforce').off('mousemove');
      });
      $side.on('mouseenter', function(e){
        if(!theForce.sideActive && clientX){
          timeout = setTimeout(function(){
            self.$body.addClass('theforce-side-active').trigger('theforce-side:show');
          }, 400);
        }
      }).on('mouseleave', function(){
        if(!theForce.sideActive){
          clearTimeout(timeout);
          self.$body.removeClass('theforce-side-active').trigger('theforce-side:hide');
        }
      });
    }
  }
};


/**
 * Dropdown handler.
 */
theForce.dropdown = function(context, settings){
  var self = this;
  var $dropdownTriggers = $('.theforce-dropdown-trigger', context);
  if($dropdownTriggers.length){

    self.$body.on('theforce-side:show.dropdown', function(){
      if(self.$dropdown){
        close(self.$dropdown);
      }
    });

    self.$body.on('theforce-side:hide.dropdown', function(){
      if(self.$dropdown){
        close(self.$dropdown);
      }
    });

    var open = function($dropdown){
      if(self.$dropdown){
        close(self.$dropdown);
      }
      $dropdown.addClass('open');
      self.$dropdown = $dropdown;
      setTimeout(function(){
        $(window).on('click' + '.theforce', function(e){
          if(!$(e.target).closest('.theforce-dropdown-content').length){
            close(self.$dropdown);
          }
        });
      }, 10);
    }

    var close = function($dropdown){
      $dropdown.removeClass('open');
      self.$dropdown = null;
      $(window).off('click' + '.theforce');
    }

    $dropdownTriggers.once('theforce-dropdown').on('click', function(e){
      e.preventDefault();
      var $dropdown = $(this).closest('.theforce-dropdown');
      if(self.$dropdown && $dropdown[0] == self.$dropdown[0]){
        close($dropdown);
      }
      else{
        open($dropdown);
      }
    });
  }
};


/**
 * Dropdown handler.
 */
theForce.menu = function(context, settings){
  var self = this, $this, $trigger;
  var $menuExpand = $('.theforce-menu-expand', context);
  if($menuExpand.length){
    $menuExpand.once('theforce-menu').each(function(){
      $this = $(this);
      // $trigger = $('a:first', $this);
      // console.log($trigger);
    });
  }
};

/**
 * Ink handler.
 */
theForce.ink = function(context, settings) {
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

theForce.loading = function(){
  var self = this;
  self.$loader.addClass('show');
  setTimeout(function(){
    self.$loader.addClass('animate');
  },10);
}

theForce.success = function(){
  var self = this;
  self.$loader.removeClass('animate').one(self.transEndEventNames, function(){
    self.$loader.removeClass('show');
  });
}

Drupal.behaviors.theforce = theForce;


/**
 * Inset and shade the content area.
 */
var theforceInset = {
  count: 0,
};

theforceInset.open = function() {
  var self = this;
  if(!self.count){
    $('body').addClass('theforce-inset');
  }
  self.count++;
}

theforceInset.close = function() {
  var self = this;
  self.count--;
  if(!self.count){
    $('body').removeClass('theforce-inset');
  }
}


/**
 * Reload content area
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
  element_settings.url = window.location + '?theforce=1';
  element_settings.event = 'onload';
  element_settings.base = base;
  element_settings.effect = 'fade';
  element_settings.speed = 600;
  Drupal.ajax[base] = new Drupal.ajax(base, $element, element_settings);
  $element.trigger('onload');
};

/**
 * Overlay
 */

/**
 * Command to insert new content into shelf.
 */
Drupal.ajax.prototype.commands.theforceOverlay = function (ajax, response, status) {
  var settings = response.settings || ajax.settings || Drupal.settings;
  // theforceSide.close();
  theforceOverlay.open(response.data, settings);
};

Drupal.ajax.prototype.commands.theforceOverlayClose = function (ajax, response, status) {
  theforceOverlay.close();
};

var theforceOverlay = {
  $overlay: null,
};

theforceOverlay.open = function(html, settings) {
  var self = this;
  if(!self.$overlay){
    self.$overlay = $('<div id="theforce-overlay" class="theforce"></div>').appendTo($('#theforce-wrap'));

    $(window).on('click' + '.theforceoverlay', function(e){
      if(!$(e.target).closest('.theforce-percist').length){
        theforceOverlay.close();
      }
    });

    setTimeout(function(){
      theforceInset.open();
      $('body').addClass('has-theforce-overlay');
      self.$overlay.addClass('animate');
    }, 20);
  }
  self.$overlay.html('<div id="theforce-overlay-inner" class="theforce-percist"><div class="theforce-overlay-content">' + html + '</div></div>');
  Drupal.attachBehaviors(self.$side, settings);
}

theforceOverlay.close = function(html) {
  var self = this;
  if(self.$overlay){
    self.$overlay.removeClass('animate');
    setTimeout(function(){
      self.$overlay.remove();
      self.$overlay = null;
    }, 300);
    theforceInset.close();
    $('body').removeClass('has-theforce-overlay');
    $(window).off('click' + '.theforceoverlay');
  }
}

/**
 * Side
 */

/**
 * Command to insert new content into shelf.
 */
Drupal.ajax.prototype.commands.theforceSide = function (ajax, response, status) {
  var settings = response.settings || ajax.settings || Drupal.settings;
  theforceOverlay.close();
  theforceSide.open(response.data, settings);
};

Drupal.ajax.prototype.commands.theforceSideClose = function (ajax, response, status) {
  theforceSide.close();
};

var theforceSide = {
  $side: null,
};

theforceSide.open = function(html, settings) {
  var self = this;
  if(!self.$side){
    theForce.sideActive = 1;
    self.$wrapper = $('#theforce-side');
    self.$side = $('<div id="theforce-side-content" class="theforce-percist"></div>').appendTo(self.$wrapper);

    $(window).on('click' + '.theforceside', function(e){
      if(!$(e.target).closest('.theforce-percist').length){
        theforceSide.close();
      }
    });

    setTimeout(function(){
      theforceInset.open();
      $('body').addClass('has-theforce-side-content').trigger('theforce-side:show');
      self.$side.addClass('animate');
    }, 20);
  }
  self.$side.html(html);
  Drupal.attachBehaviors(self.$side, settings);
  $('body').addClass('theforce-side-active');
}

theforceSide.close = function(html) {
  var self = this;
  if(self.$side){
    theForce.sideActive = 0;
    self.$side.removeClass('animate');
    setTimeout(function(){
      self.$side.remove();
      self.$side = null;
    }, 300);
    if(!Drupal.settings.theforce.isManagement){
      $('body').removeClass('theforce-side-active');
    }
    theforceInset.close();
    $('body').removeClass('has-theforce-side-content');
    $(window).off('click' + '.theforceside');
  }
}

/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
Drupal.ajax.prototype.beforeSendOriginal = Drupal.ajax.prototype.beforeSend;
Drupal.ajax.prototype.beforeSend = function (xmlhttprequest, options){
  this.beforeSendOriginal(xmlhttprequest, options);
  if (this.progress.type == 'theforce') {
    theForce.loading();
  }
}

/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
var successOriginal = Drupal.ajax.prototype.success;
Drupal.ajax.prototype.success = function (response, status){
  if (this.progress.type == 'theforce') {
    theForce.success();
  }
  // Unbind events if once is set.
  if (this.theforce && this.once) {
    delete Drupal.ajax[this.base];
    $(this.element).unbind('click.theforce');
  }
  successOriginal.call(this, response, status);
}


})(jQuery, Drupal);
