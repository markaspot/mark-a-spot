/**
 * jquery.purr.js
 * Licensed under the MIT License (http://www.opensource.org/licenses/mit-license.php)
 * 
 * @author R.A. Ray
 * @projectDescription  jQuery plugin for dynamically displaying unobtrusive
 * messages in the browser. Mimics the behavior of the MacOS program "Growl."
 * @version 0.1.0
 * 
 * @requires jquery.js (tested with 1.2.6)
 * 
 * @param fadeInSpeed           int - Duration of fade in animation in miliseconds
 *                           default: 500
 *  @param fadeOutSpeed          int - Duration of fade out animationin miliseconds
                             default: 500
 *  @param removeTimer          int - Timeout, in miliseconds, before notice is
 *                                    removed once it is the top non-sticky notice in the list
                             default: 4000
 *  @param isSticky             bool - Whether the notice should fade out on its
 *                                     own or wait to be manually closed
                             default: false
 *  @param pauseOnHover         bool - Whether hovering over the notice should
 *                                     stop it fading out.
                             default: true
 *  @param usingTransparentPNG   bool - Whether or not the notice is using
 *                                      transparent .png images in its styling
                             default: false
 *
 * This version has been modified from the original by tanc for use in a Drupal module.
 * The modified code is under a dual MIT and GPL licence to conform with drupal.org's
 * licensing policy, with the permission of the original authors.
 */

(function($) {
  $.purr = function(notice, options) {
    // Convert notice to a jQuery object
    notice = $(notice);

    // Add a class to denote the notice as not sticky
    if (!options.isSticky) {
      notice.addClass('not-sticky');
    };
    
    // Get the container element from the page
    var cont = window.top.document.getElementById('purr-container');
    
    // If the container doesn't yet exist, we need to create it
    if (!cont) {
      cont = '<div id="purr-container"></div>';
    }
    
    // Convert cont to a jQuery object
    cont = $(cont);
    
    // Add the container to the page
    $(options.attachTo, window.top.document).append(cont);
      
    notify();

    function notify() {
      // Set up the close button
      var close = window.top.document.createElement('a');
      $(close).attr({
        className: 'close',
        href: '#close',
        innerHTML: 'Close'
      })
      .appendTo(notice);
      
      // Add the notice to the page and keep it hidden initially
      notice.appendTo(cont).hide();
      
      $('.notice a.close', window.top.document).bind('click', function(event) {
        removeNotice($(event.target).parent());
        return false;
      });

      if (!jQuery.support.opacity && options.usingTransparentPNG) {
        // IE7 and earlier can't handle the combination of opacity and
        // transparent pngs, so if we're using transparent pngs in our
        // notice style, we'll just skip the fading in.
        notice.show();
      }
      else {
        //Fade in the notice we just added
        notice.fadeIn(options.fadeInSpeed);
      }

      // Set up the timer
      var noticeTimer = $.timer(function() {
        removeNotice($('#purr-container .notice:first', window.top.document));
        this.stop();
      });

      // Set up the removal interval for the added notice if that notice is not a sticky
      if (!options.isSticky) {
        var topSpotInt = setInterval(function() {
          // Check to see if our notice is the first non-sticky notice in the list
          if (notice.prevAll('.not-sticky').length == 0) {
            // Stop checking once the condition is met
            clearInterval(topSpotInt);
            processNotice(noticeTimer);
          }
          else {
            // Check if a timer is running (on the top item)
            if (noticeTimer.active == false) {
              processNotice(noticeTimer);
            }
          }
        }, 1000);
      }
    }

    function processNotice(noticeTimer) {
      // Call the close action after the timeout set in options
      noticeTimer.set({ time : options.removeTimer, autostart : true });
      if (options.pauseOnHover == true) {
        $('#purr-container .notice:first', window.top.document).mouseout(function() {
          noticeTimer.play();
        });
        $('#purr-container .notice:first', window.top.document).mouseover(function() {
          noticeTimer.pause();
        });
      }
    }

    function removeNotice(notice) {
      // IE7 and earlier can't handle the combination of opacity and
      // transparent pngs, so if we're using transparent pngs in our
      // notice style, we'll just skip the fading out.
      if (!jQuery.support.opacity && options.usingTransparentPNG) {
        notice.css({opacity: 0}).animate({ 
          height: '0px'
        },
        {
	        duration: options.fadeOutSpeed, 
          complete: function() {
            notice.remove();
          } 
        });
      }
      else {
        // Fade the object out before reducing its height to produce
        // the sliding effect.
        notice.animate({ 
          opacity: '0'
        }, 
        { 
          duration: options.fadeOutSpeed, 
          complete: function() {
            notice.animate({
              height: '0px'
            },
            {
              duration: options.fadeOutSpeed,
              complete: function() {
                notice.remove();
              }
            });
          }
        });
      }
    };
  };
  
  $.fn.purr = function(options) {
    options = options || {};
    options.fadeInSpeed = Drupal.settings.purr_messages.fadeInSpeed || 500;
    options.fadeOutSpeed = Drupal.settings.purr_messages.fadeOutSpeed || 500;
    options.removeTimer = Drupal.settings.purr_messages.removeTimer || 4000;
    options.isSticky = Drupal.settings.purr_messages.isSticky || false;
    options.pauseOnHover = Drupal.settings.purr_messages.pauseOnHover || false;
    options.usingTransparentPNG = Drupal.settings.purr_messages.usingTransparentPNG || false;
    options.attachTo = Drupal.settings.purr_messages.attachTo || 'body';
    this.each(function() {
      new $.purr(this, options);
    });
    
    return this;
  };
})(jQuery);