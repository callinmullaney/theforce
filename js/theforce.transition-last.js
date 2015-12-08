// setup allTransitionEnd event (see http://benalman.com/news/2010/03/jquery-special-events/)
(function ($) {

  var transitionEnd = (function () {
    var transitionEndEvents = [
      'webkitTransitionEnd',
      'oTransitionEnd',
      'otransitionend',
      'transitionend'
    ];
    return $.map(transitionEndEvents, function (event) {
      return event + '.allTransitionEnd';
    }).join(' ');
  })();

  var TransitionProperties = function ($element) {
    this.$element = $element;
    this.properties = null;
  };

  TransitionProperties.prototype = {

    initialize: function () {
      if (this.properties !== null) {
        return;
      }

      this.properties = {};
      var style = window.getComputedStyle(this.$element.get(0), null);
      var properties = style.getPropertyValue('transition-property').split(', ');
      var obj = this;
      $.each(properties, function (index, property) {
        obj.properties[property] = false;
      });
    },

    markTransitionEnd: function (property) {
      this.initialize();
      if (this.properties[property] !== undefined) {
        this.properties[property] = true;
      }
    },

    allTransitionEnd: function () {
      for (var key in this.properties) {
        if (this.properties.hasOwnProperty(key) && this.properties[key] === false) {
          return false;
        }
      }
      return true;
    },

    reset: function () {
      this.properties = null;
    }

  };

  $.event.special.allTransitionEnd = {

    setup: function (data, namespaces, eventHandle) {
      var $element = $(this);
      var properties = new TransitionProperties($element);

      $element.on(transitionEnd, function (event) {
        properties.markTransitionEnd(
          event.originalEvent.propertyName
        );
        if (properties.allTransitionEnd()) {
          properties.reset();
          $element.trigger('allTransitionEnd');
        }
      });
    },

    teardown: function (namespaces) {
      $(this).off(transitionEnd);
    }

  };
})(jQuery);
