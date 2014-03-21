(function($) {
  $(document).ready(function() {

    $('.field-label').addClass('label');

    $('.geolocation-address-geocode, .geolocation-client-location, .geolocation-remove').addClass('btn');

    // Hide form input's address on focus.
    $('.geolocation-address input').focus(function(){
      this.value = '';
    });

    $('.nav-tabs > li > a').on('click', function(e) {

      hash = e.target.hash;

      $('html, body').animate({
         scrollTop: $(hash).offset().top
       }, 600, function(){
         window.location.hash = hash;
      });

      if (hash.match('3--'))  {
        $('.node-report-form #edit-submit').html(Drupal.t('Save'));
      } else {
        $('.node-report-form #edit-submit').html(Drupal.t('Next'));
      }
    });

    var url = document.location.toString();
    if (url.match('#')) {
      $('.nav-tabs a[href=#' + url.split('#')[1] + ']').tab('show');
    }

    $('.node-report-form #edit-submit').html(Drupal.t('Next'));
    // Submit changes
    $('.node-report-form #edit-submit').click(function(e) {

      var url = document.URL.toString();
      e.preventDefault();
      if (!url.split('#')[1] || url.match('1--')) {

        $('a:contains(2.)').tab('show');
        var hash = $('a:contains(2.)').attr('href');
         // animate
        $('html, body').animate({
           scrollTop: $(hash).offset().top - 30
         }, 600, function(){
           window.location.hash = hash;
        });
      } else if (url.match('2--')) {
        $('a:contains(3.)').tab('show');

        var hash = $('a:contains(3.)').attr('href');
         // animate
        $('html, body').animate({
           scrollTop: $(hash).offset().top - 30
         }, 600, function(){
           window.location.hash = hash;
         });
        $('#edit-submit').html(Drupal.t('Save'));
      } else if (url.match('3--')) {
        $('form').unbind('submit').submit();
      }
    });
  });

})(jQuery);
