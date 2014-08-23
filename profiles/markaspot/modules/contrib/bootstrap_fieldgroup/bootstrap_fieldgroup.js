
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

/**
 * Implements Drupal.FieldGroup.processHook().
 */
Drupal.FieldGroup = Drupal.FieldGroup || {};
Drupal.FieldGroup.Effects = Drupal.FieldGroup.Effects || {};
Drupal.FieldGroup.Effects.processBootstrap_Fieldgroup_Nav = {
  execute: function (context, settings, type) {
    
    if (type == 'form') {
      
      // Add required fields mark to any element containing required fields
      $('ul.nav', context).once('fieldgroup-effects', function(i) {
        
        $('li', this).each(function() {
          
          if ($(this).is('.required-fields')) {
            
            var $link = $('a', this);
            var $group = $(this).closest('.bootstrap-nav-wrapper');
            var $pane = $('.tab-content', $group).find($link.attr('href'));
            if ($pane.find('.form-required').length > 0) {
              $link.append(' ').append($('.form-required').eq(0).clone());
            }
          }
        });
      });
    }
  }
};

})(jQuery);