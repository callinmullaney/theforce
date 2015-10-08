(function ($, Drupal) {

var theForce = {
  $dropdown: null,
  forceUses: 0
}

theForce.attach = function(context, settings) {
  var self = this;
  self.$body = $('body');

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
      element_settings.url = $(this).attr('href') + '/ajax';
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
  self.menu(context, settings);
  self.ink(context, settings);
};


  /**
   * Dropdown handler.
   */
theForce.side = function(context, settings){
  var self = this;
  if(!settings.theforce.isManagement){
    var $side = $('#theforce-side:not(.theforce-side-processed)').addClass('theforce-side-processed');
    var timeout;
    if($side.length){
      $side.on('mouseenter', function(){
        timeout = setTimeout(function(){
          self.$body.addClass('theforce-side-hover').trigger('theforce-side:show');
        }, 300);
      }).on('mouseleave', function(){
        clearTimeout(timeout);
        self.$body.removeClass('theforce-side-hover').trigger('theforce-side:hide');
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
      console.log($dropdown);
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
  console.log($menuExpand);
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

Drupal.behaviors.theforce = theForce;

/**
 * Overlay
 */

/**
 * Command to insert new content into shelf.
 */
Drupal.ajax.prototype.commands.theforceOverlay = function (ajax, response, status) {
  var settings = response.settings || ajax.settings || Drupal.settings;
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

    $(window).on('click' + '.offcanvas', function(e){
      if(!$(e.target).closest('#theforce-overlay-inner').length){
        theforceOverlay.close();
      }
    });

    setTimeout(function(){
      $('body').addClass('has-theforce-overlay');
      self.$overlay.addClass('animate');
    }, 20);
  }
  self.$overlay.html('<div id="theforce-overlay-inner">' + html + '</div>');
  Drupal.attachBehaviors(self.$shelf, settings);
}

theforceOverlay.close = function(html) {
  var self = this;
  if(self.$overlay){
    self.$overlay.removeClass('animate')
    setTimeout(function(){
      self.$overlay.remove();
      self.$overlay = null;
    }, 300);
    $('body').removeClass('has-theforce-overlay');
    $(window).off('click' + '.offcanvas');
  }
}

/**
 * Shelf
 */

/**
 * Command to insert new content into shelf.
 */
Drupal.ajax.prototype.commands.theforceShelf = function (ajax, response, status) {
  var settings = response.settings || ajax.settings || Drupal.settings;
  theforceShelf.close();
  theforceShelf.open(response.data, settings);
};

Drupal.ajax.prototype.commands.theforceShelfClose = function (ajax, response, status) {
  theforceShelf.close();
};

var theforceShelf = {
  $shelf: null,
};

theforceShelf.open = function(html, settings) {
  var self = this;
  self.$shelf = $('<div id="theforce-shelf"></div>').html(html).appendTo($('#theforce-top'));
  Drupal.attachBehaviors(self.$shelf, settings);
}

theforceShelf.close = function(html) {
  var self = this;
  if(self.$shelf){
    self.$shelf.remove();
  }
}

/**
 * Extend the default Drupal.ajax.prototype.beforeSend;
 */
var successOriginal = Drupal.ajax.prototype.success;
Drupal.ajax.prototype.success = function (response, status){
  // Unbind events if once is set.
  if (this.theforce && this.once) {
    delete Drupal.ajax[this.base];
    $(this.element).unbind('click.theforce');
  }
  successOriginal.call(this, response, status);
}


})(jQuery, Drupal);
