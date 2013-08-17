<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="row-fluid">
    <div class="span6">
      <?php
        // Hide comments, tags, and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        hide($content['field_tags']);
        
         $nodeid = (arg(0) == 'node' && is_numeric(arg(1)))?arg(1):FALSE; 

        if(arg(1) == $nodeid){
          print views_embed_view('markers', 'report_detail', arg(1));
        } else { 
          print render($content);
        }
      ?>
      <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
        <footer>
          <?php print render($content['field_tags']); ?>
          <?php print render($content['links']); ?>
        </footer>
      <?php endif; ?>
    </div>

    <div class="span5 offset1">
      <div id="map" class="detail"></div>
    </div>
  </div>

  <div class="row-fluid">
    <div class="span6">
     <?php print render($content['comments']); ?>
    </div>
    <div class="thumbnail span5 offset1">
        <?php print render($content['field_image'])?>
    </div>
  </div>
</article> <!-- /.node -->
