(function($) {

  // Make evrything local
  var ml = ml || {};
  ml.options = ml.options || {};

  Drupal.behaviors.maxlength = {
    attach: function(context, settings) {

      if (Drupal.wysiwyg != undefined) {
        $.each(Drupal.wysiwyg.editor.init, function(editor) {
          if (typeof ml[editor] == 'function') {
            ml[editor]();
          }
        });
      } else if (Drupal.settings.ckeditor != undefined) {
        ml.ckeditor();
      }

      $('.maxlength', context).once('maxlength', function() {
        var options = {};
        options['counterText'] = $(this).attr('maxlength_js_label');  
        if ($(this).hasClass('maxlength_js_enforce')) {
          options['enforce'] = true;
        }
        if ($(this).hasClass('maxlength_js_truncate_html')) {
          options['truncateHtml'] = true;
        }
        $(this).charCount(options);
      });
    },
    detach: function(context, settings) {
      $('.maxlength', context).removeOnce('maxlength', function() {
        $(this).charCount({
          action: 'detach'
        });
      });
    }
  };

  /**
   * Code below is based on:
   *   Character Count Plugin - jQuery plugin
   *   Dynamic character count for text areas and input fields
   *   written by Alen Grakalic
   *   http://cssglobe.com/post/7161/jquery-plugin-simplest-twitterlike-dynamic-character-count-for-textareas
   *
   *  @param obj
   *    a jQuery object for input elements
   *  @param options
   *    an array of options.
   *  @param count
   *    In case obj.val() wouldn't return the text to count, this should
   *    be passed with the number of characters.
   */
  ml.calculate = function(obj, options, count, wysiwyg, getter, setter) {
    var counter = $('#' + obj.attr('id') + '-' + options.css);
    var limit = parseInt(obj.attr('maxlength'));

    if (count == undefined) {
      if (options.truncateHtml) {
        count = ml.strip_tags(obj.val()).length;
      }
      else {
        count = ml.twochar_lineending(obj.val()).length;
      }
    }

    var available = limit - count;

    if (available <= options.warning) {
      counter.addClass(options.cssWarning);
    }
    else {
      counter.removeClass(options.cssWarning);
    }

    if (available < 0) {
      counter.addClass(options.cssExceeded);
      // Trim text.
      if (options.enforce) {
        if (wysiwyg != undefined) {
          if (typeof ml[getter] == 'function' && typeof ml[setter] == 'function') {
            if (options.truncateHtml) {
              var new_html = ml.truncate_html(ml[getter](wysiwyg), limit)
              ml[setter](wysiwyg, new_html);
              count = ml.strip_tags(new_html).length;
            } else {
              var new_html = ml[getter](wysiwyg).substr(0, limit);
              ml[setter](wysiwyg, new_html);
              count = new_html.length;
            }
          }
        } else {
          if (options.truncateHtml) {
            obj.val(ml.truncate_html(obj.val(), limit));
            // Re calculate text length
            count = ml.strip_tags(obj.val()).length;
          } else {
            obj.val(obj.val().substr(0, limit));
            // Re calculate text length
            count = ml.twochar_lineending(obj.val()).length;
          }
        }
      }
      available = limit - count;
    }
    else {
      counter.removeClass(options.cssExceeded);
    }

    counter.html(options.counterText.replace('@limit', limit).replace('@remaining', available));
  };

  /**
   * Replaces line ending with to chars, because PHP-calculation counts with two chars
   * as two characters.
   *
   * @see http://www.sitepoint.com/blogs/2004/02/16/line-endings-in-javascript/
   */
  ml.twochar_lineending = function(str) {
    return str.replace(/(\r\n|\r|\n)/g, "\r\n");
  };
  
  ml.strip_tags = function(input, allowed) {
    // making the lineendings with two chars
    input = ml.twochar_lineending(input);
    // We do want that the space characters to count as 1, not 6...
    input = input.replace('&nbsp;', ' ');
     //input = input.split(' ').join('');
    // Strips HTML and PHP tags from a string
     allowed = (((allowed || "") + "")
         .toLowerCase()
         .match(/<[a-z][a-z0-9]*>/g) || [])
         .join(''); // making sure the allowed arg is a string containing only tags in lowercase (<a><b><c>)
     var tags = /<\/?([a-z][a-z0-9]*)\b[^>]*>/gi,
         commentsAndPhpTags = /<!--[\s\S]*?-->|<\?(?:php)?[\s\S]*?\?>/gi;
     return input.replace(commentsAndPhpTags, '').replace(tags, function($0, $1){
         return allowed.indexOf('<' + $1.toLowerCase() + '>') > -1 ? $0 : '';
     });
  };
  
  /**
   * Cuts a html text up to limit characters. Still experimental.
   */
  ml.truncate_html = function(text, limit) {
    // The html result after cut.
    var result_html = '';
    // The text result, that will actually used when counting characters.
    var result_text = '';
    // A stack that will keep the tags that are open at a given time.
    var tags_open = new Array();
    // making the lineendings with two chars
    text = ml.twochar_lineending(text);
    // We do want that the space characters to count as 1, not 6...
    text = text.replace('&nbsp;', ' ');
    while (result_text.length < limit && text.length > 0) {
      switch (text.charAt(0)) {
        case '<': {
          if (text.charAt(1) != '/') {
            var tag_name = '';
            var tag_name_completed = false;
            while (text.charAt(0) != '>' && text.length > 0) {
              var first_char = text.charAt(0).toString();
              // Until the tag is closed, we do not append anything
              // to the visible text, only to the html.
              result_html += first_char;
              // Also, check if we have a valid tag name.
              if (!tag_name_completed && first_char == ' ') {
                // We have the tag name, so push it into the stack.
                tag_name_completed = true;
                tags_open.push(tag_name);
              }
              // Check if we are still in the tag name.
              if (!tag_name_completed && first_char != '<') {
                tag_name += first_char;
              }
              //If we have the combination "/>" it means that the tag
              //is closed, so remove it from the open tags stack.
              if (first_char == '/' && text.length > 1 && text.charAt(1) == '>') {
                tags_open.pop();
              }
              //Done with this char, remove it from the original text.
              text = text.substring(1);
            }
            if (!tag_name_completed) {
              // In this case we have a tag like "<strong>some text</strong> so
              // we did not have any attributes in the tag, but still, the tag
              // has to be marked as open.
              tags_open.push(tag_name);
            }
            //We are here, then the tag is closed, so just remove the
            //remaining ">" character.
            if (text.length > 0) {
             result_html += text.charAt(0).toString();
            }
          } else {
            // In this case, we have an ending tag.
            // The name of the ending tag should match the last open tag,
            // otherwise, something is wrong with th html text.
            var tag_name = '';
            while (text.charAt(0) != '>' && text.length > 0) {
              var first_char = text.charAt(0).toString();
              if (first_char != '<' && first_char != '/') {
                tag_name += first_char;
              }
              result_html += first_char;
              text = text.substring(1);
            }
            if (text.length > 0) {
             result_html = result_html + text.charAt(0).toString();
            }
            // Pop the last element from the tags stack and compare it with
            // the tag name.
            var expected_tag_name = tags_open.pop();
            if (expected_tag_name != tag_name) {
              //Should throw an exception, but for the moment just alert.
              alert('Expected end tag: ' + expected_tag_name + '; Found end tag: '+tag_name);
            }
          }
          break;
        }
        default: {
          // In this case, we have a character that should also count for the
          // limit, so append it to both, the html and text result.
          var first_char = text.charAt(0).toString();
          result_html += first_char;
          result_text += first_char;
          break;
        }
      }
      // Remove the first character, it did its job.
      text = text.substring(1);
    }
    // Restore the open tags that were not closed. This happens when the text
    // got truncated in the middle of one or more html tags.
    var tag = '';
    while (tag = tags_open.pop()) {
      result_html += '</' + tag + ">";
    }
    return result_html;
  }

  $.fn.charCount = function(options) {
    // default configuration properties
    var defaults = {
      warning: 10,
      css: 'counter',
      counterElement: 'div',
      cssWarning: 'messages warning',
      cssExceeded: 'error',
      counterText: Drupal.t('Content limited to @limit characters, remaining: <strong>@remaining</strong>'),
      action: 'attach',
      enforce: false,
      truncateHtml: false
    };

    var options = $.extend(defaults, options);
    ml.options[$(this).attr('id')] = options;

    if (options.action == 'detach') {
      $(this).removeClass('maxlength-processed');
      $('#' + $(this).attr('id') + '-' + options.css).remove();
      delete ml.options[$(this).attr('id')];
      return 'removed';
    }

    $(this).after('<' + options.counterElement + ' id="' + $(this).attr('id') + '-' + options.css + '" class="' + options.css + '"></' + options.counterElement + '>');
    ml.calculate($(this), options);
    $(this).keyup(function() {
      ml.calculate($(this), options);
    });
    $(this).change(function() {
      ml.calculate($(this), options);
    });

  };

  /**
   * Integrate with WYSIWYG
   * Detect changes on editors and invoke ml.calculate()
   */
  ml.tinymce = function() {
    // We only run it once
    var onlyOnce = false;
    if (!onlyOnce) {
      onlyOnce = true;
      tinyMCE.onAddEditor.add(function(mgr,ed) {
          // the editor is on a maxlength field
          var editor = $('#' + ed.editorId + '.maxlength');
          if (editor.length == 1) {
            if (editor.hasClass('maxlength_js_enforce')) {
              ml.options[ed.editorId].enforce = true;
            } else {
              ml.options[ed.editorId].enforce = false;
            }
            // Check if we should strip the tags when counting.
            if (editor.hasClass('maxlength_js_truncate_html')) {
              ml.options[ed.editorId].truncateHtml = true;
            } else {
              ml.options[ed.editorId].truncateHtml = false;
            }
            ed.onChange.add(function(ed, l) {
              ml.tinymceChange(ed);
            });
            ed.onKeyUp.add(function(ed, e) {
              ml.tinymceChange(ed);
            });
            ed.onPaste.add(function(ed, e) {
              setTimeout(function(){ml.tinymceChange(ed)}, 500);
            });
          }
      });
    }
  }
  // Invoke ml.calculate() for editor
  ml.tinymceChange = function(ed) {
    // CLone to avoid changing defaults
    var options = $.extend({}, ml.options[ed.editorId]);
    if (options.truncateHtml){
      ml.calculate($(ed.getElement()), options, ml.strip_tags(ml.tinymceGetData(ed)).length, ed, 'tinymceGetData', 'tinymceSetData');
    }
    else {
      ml.calculate($(ed.getElement()), options, ml.twochar_lineending(ml.tinymceGetData(ed)).length, ed, 'tinymceGetData', 'tinymceSetData');
    }
  };
  
  // Gets the data from the tinyMCE. Not tested yet.
  ml.tinymceGetData = function(e) {
    return e.getContent();
  }
  
  // Sets the data into a tinyMCE. Not tested yet.
  ml.tinymceSetData = function(e, data) {
    e.setContent(data);
  }

  /**
   * Integrate with ckEditor
   * Detect changes on editors and invoke ml.calculate()
   */
  ml.ckeditor = function() {
    // We only run it once
    var onlyOnce = false;
    if (!onlyOnce) {
      onlyOnce = true;
      CKEDITOR.on('instanceReady', function(e) {
        var editor = $('#' + e.editor.name + '.maxlength');
        if (editor.length == 1) {
          if (editor.hasClass('maxlength_js_enforce')) {
            ml.options[e.editor.element.getId()].enforce = true;
          } else {
            ml.options[e.editor.element.getId()].enforce = false;
          }
          // Check if we should strip the tags when counting.
          if (editor.hasClass('maxlength_js_truncate_html')) {
            ml.options[e.editor.element.getId()].truncateHtml = true;
          } else {
            ml.options[e.editor.element.getId()].truncateHtml = false;
          }
          // Add the events on the editor.
          e.editor.on('key', function(e) {
            setTimeout(function(){ml.ckeditorChange(e)}, 100);
          });
          e.editor.on('paste', function(e) {
            setTimeout(function(){ml.ckeditorChange(e)}, 500);
          });
          e.editor.on('elementsPathUpdate', function(e) {
            setTimeout(function(){ml.ckeditorChange(e)}, 100);
          });
        }
      });
    }
  }
  // Invoke ml.calculate() for editor
  ml.ckeditorChange = function(e) {
    // Clone to avoid changing defaults
    var options = $.extend({}, ml.options[e.editor.element.getId()]);
    if (options.truncateHtml){
      ml.calculate($('#' + e.editor.element.getId()), options, ml.strip_tags(ml.ckeditorGetData(e)).length, e, 'ckeditorGetData', 'ckeditorSetData');
    }
    else {
      ml.calculate($('#' + e.editor.element.getId()), options, ml.twochar_lineending(ml.ckeditorGetData(e)).length, e, 'ckeditorGetData', 'ckeditorSetData');
    }
  };
  
  // Gets the data from the ckeditor.
  ml.ckeditorGetData = function(e) {
    return e.editor.getData();
  }
  
  // Sets the data into a ckeditor.
  ml.ckeditorSetData = function(e, data) {
    e.editor.setData(data);
  }

})(jQuery);
