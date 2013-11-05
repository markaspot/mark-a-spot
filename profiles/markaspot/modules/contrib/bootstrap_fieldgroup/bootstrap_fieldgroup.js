
(function($) {

/**
 * Behaviors.
 */
Drupal.behaviors.BootstrapFieldgroup = {
  attach: function (context, settings) {
    
    var mutateForm = function($nav_type) {
      
      switch ($nav_type.val()) {
        case 'tabs':
          $('.bootstrap-fieldgroup-orientation option:odd').show();
          break;
        default:
          $orientation = $('.bootstrap-fieldgroup-orientation');
          if (1 == $orientation.val() % 2) {
            $orientation.val(0);
          }
          $('.bootstrap-fieldgroup-orientation option:odd').hide();
          break;
      }
    };
    
    $('.bootstrap-fieldgroup-nav-type', context).on('change', function() {
      mutateForm($(this));
    });
    
    mutateForm($('.bootstrap-fieldgroup-nav-type'));
  }
};

})(jQuery);