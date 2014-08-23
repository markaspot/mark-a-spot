(function ($) {
  $(document).ready(function () {

    // Add some bootstrap stuff to elements
    $('.field-label').addClass('label');
    $('.geolocation-address-geocode, .geolocation-client-location, .geolocation-remove').addClass('btn');

    // Hide form input's address on focus.
    $('.geolocation-address input').focus(function () {
      this.value = '';
    });

    $('li.group-location').addClass('active');

    // In case the main form is visible
    function showTab2() {

      $('.group-report a').tab('show');
      var hash = $('.group-report a').attr('href');
      // animate
      $('html, body').animate({
        scrollTop: $(hash).offset().top - 30
      }, 600, function () {
        window.location.hash = hash;
      });
      $('#edit-photo').show();
    }

    // In case we suffer invalid forms:
    if ($('.alert.alert-block.alert-danger').length) {
      $('#edit-submit').html(Drupal.t('Save'));
      showTab2();
    } else {
      $('.node-report-form #edit-submit').html(Drupal.t('Next'));

    }


    $('#edit-photo').hide();

    $('#edit-photo').on('click', function (e) {
      $('.nav-tabs a[href=#---media]').tab('show');
      $('#edit-submit').html(Drupal.t('Save'));
      $('#edit-photo').hide();
    });


    $('.nav-tabs > li > a').on('click', function (e) {

      hash = e.target.hash;

      $('html, body').animate({
        scrollTop: $(hash).offset().top
      }, 600, function () {
        window.location.hash = hash;
      });

      if (hash.match('location')) {
        $('.node-report-form #edit-submit').html(Drupal.t('Next'));
        $('#edit-photo').hide();
      }

      if (hash.match('report')) {
        $('.node-report-form #edit-submit').html(Drupal.t('Save'));
        $('#edit-photo').show();
      }

      if (hash.match('media')) {
        $('.node-report-form #edit-submit').html(Drupal.t('Save'));
        $('#edit-photo').hide();
      }
    });


    // Submit changes
    $('.node-report-form #edit-submit').click(function (e) {

      var url = document.URL.toString();
      e.preventDefault();
      if (!url.split('#')[1] || url.match('location')) {
        $('#edit-submit').html(Drupal.t('Save'));

        showTab2();
      } else if (url.match('media') || url.match('report')) {
        $('form').unbind('submit').submit();
      }
    });
  });

})(jQuery);
