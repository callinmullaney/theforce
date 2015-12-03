(function($) {

Drupal.behaviors.theforceAdmin = {
  once: 0,
  attach: function(context, settings) {
    var self = this;
    if(!this.once){
      this.once = 1;
    }
  }
}






var theforceAdminUI = {
  once: 0,
  $items: {}
};

theforceAdminUI.attach = function(context, settings){
  var self = this;

  if(!self.once){
    self.once = 1;
  }

  self.$items = $('.theforce .theforce-ui-item:not(.theforce-processed)');

  self.itemsInit(settings);
  self.itemsSort();
  self.themeSelect();
}

theforceAdminUI.itemsInit = function(settings){
  var self = this, $item, $ops, $drag, $remove, $edit, $trash, url, id, config, trash, base, vars;

  self.$items.addClass('theforce-processed').each(function(){
    $item = $(this);
    $ops = $('<div class="theforce-ui-ops"></div>');
    config = $item.data('config');
    trash = $item.data('trash');
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

    // Delete
    if(trash){
      base = 'theforce-ui-delete-' + id;
      url = settings.basePath + settings.pathPrefix + settings.theforce.basePath + '/plugin/delete/' + id;
      $trash = $('<a href="'+url+'" class="theforce-ui-delete theforce-ui-op"><i class="fa fa-trash"></i></a>').appendTo($ops);
      vars = {url: url + '/nojs',event: 'click',progress: {type: 'theforce'}};
      Drupal.ajax[base] = new Drupal.ajax(base, $trash, vars);
    }

  });
}

theforceAdminUI.itemsSort = function(){
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
                Drupal.behaviors.theforce.loading();
              },
              success: function(response, status) {
                Drupal.behaviors.theforce.success();
              }
            });
          }
        }
      });
    });
  }
}

theforceAdminUI.themeSelect = function(){
  $('#theforce-theme-select a').once('theforce').click(function(e){
    var $this, value, items = ['top-bg','top-primary','side-bg','side-primary'];
    e.preventDefault();
    $this = $(this);

    $('#theforce-theme-select a').removeClass('active');
    $this.addClass('active');

    $.each(items, function( index, key ) {
      value = $this.attr('data-' + key);
      $('#edit-theforce-' + key).val(value);
      $('.form-item-theforce-' + key + ' i').css({color:value});
    });
  });
}

Drupal.behaviors.theforceAdminUI = theforceAdminUI;

})(jQuery);
