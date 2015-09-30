(function ($, Drupal) {

Drupal.behaviors.theforce = {
  $dropdown: null,

  attach: function(context, settings) {
    var self = this;

    self.dropdown(context, settings);
    self.ink(context, settings);
  },


  /**
   * Dropdown handler.
   */
  dropdown: function(context, settings){
    var self = this;

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

    $('.theforce-dropdown-trigger', context).once('theforce').on('click', function(e){
      e.preventDefault();
      var $dropdown = $(this).closest('.theforce-dropdown');
      if(self.$dropdown && $dropdown[0] == self.$dropdown[0]){
        close();
      }
      else{
        open($dropdown);
      }
    });
  },

  /**
   * Ink handler.
   */
  ink: function(context, settings) {
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
};

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

/**
 * Update a type/region.
 */

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


})(jQuery, Drupal);
