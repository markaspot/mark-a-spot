/**
 * @file mark_a_spot_admin.js
 *
 * Provides some more leaflet layers to check reports
 */

(function ($) {
  Drupal.behaviors.markaspotAdmin = {

    attach: function (context, settings) {
      // Disable User notify
      $('#edit-field-notify-user .form-checkbox').attr('checked', false);
    }
  };
})(jQuery);

