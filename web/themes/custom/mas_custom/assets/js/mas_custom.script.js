(function ($, Drupal, drupalSettings, window, document) {

  var path = $('body.path-report');
  if (path.length !== 1) {
    localStorage.setItem("category", false);
  }

  var $bs = $("#edit-field-object-id-wrapper, #edit-field-add-data-wrapper");
  if (localStorage.getItem("category") == "13" || localStorage.getItem("category") == "2") {
    $bs.css('display', 'block');
    $("#edit-field-object-id-wrapper").find("label").text("Papierkorbnummer");
  } else {
    $bs.css('display', 'none');
  }

  $(document).on("change", 'select', function () {
    if (this.value == 13 ||this.value == 2) {
      $("#edit-field-object-id-wrapper").find("label").text("Papierkorbnummer");
      localStorage.setItem("category", this.value);
      $bs.fadeIn(1000);
    } else {
      $("#edit-field-object-id-wrapper").find("label").text("Objektnummer");
      $("#edit-field-object-id-wrapper input").val('');
      // $("#edit-field-add-data-value").prop('checked', false);
      $bs.fadeOut();
      localStorage.setItem("category", false);
    }
    if (this.value == 39) {
      $("textarea, input").prop('disabled', true);
    } else {
      $("textarea, input").prop('disabled', false);
    }
  });

})(jQuery, Drupal, drupalSettings, this, this.document);
