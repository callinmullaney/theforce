(function ($) {

/**
 * Attach the machine-readable name form element behavior.
 */
Drupal.behaviors.machineName.transliterate = function (source, settings) {
  var rx = new RegExp(settings.replace_pattern, 'g');
  return this.camelize(source.toLowerCase().replace(rx, ' ').substr(0, settings.maxlength));
}

Drupal.behaviors.machineName.camelize = function (str) {
  return str.replace(/(?:^\w|[A-Z]|\b\w)/g, function(letter, index) {
    return index == 0 ? letter.toLowerCase() : letter.toUpperCase();
  }).replace(/\s+/g, '');
}

})(jQuery);
