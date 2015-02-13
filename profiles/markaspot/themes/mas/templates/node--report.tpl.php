<?php
/**
 * @file
 * Node template for reports
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php
  // taxonomy_term_load($node->field_category[LANGUAGE_NONE][0]['taxonomy_term']['tid']);
  $cat_tid    = $node->field_category[LANGUAGE_NONE][0]['tid'];
  $status_tid = $node->field_status[LANGUAGE_NONE][0]['tid'];
  $category = taxonomy_term_load($cat_tid);
  $status   = taxonomy_term_load($status_tid);
  ?>
  <div class="row">
    <div class="col-md-6">
      <header>
        <?php print render($title_prefix); ?>
        <?php if (!$page && $title): ?>
          <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($display_submitted): ?>
          <span class="submitted">
            <?php print $user_picture; ?>
            <?php print $submitted; ?>
          </span>
        <?php endif; ?>
        <div class="cat-stat-wrapper">
          <span class="label label-default marker-category col-<?php echo ltrim($category->field_category_hex[LANGUAGE_NONE][0]['rgb'], '#') ?> col-md-6"><?php print drupal_render($variables['icon_category']); ?> <?php echo $category->name?> </span> <span class="label label-default marker-status col-<?php echo ltrim($status->field_status_hex[LANGUAGE_NONE][0]['rgb'],'#') ?> col-md-6"><?php print drupal_render($variables['icon_status']); ?> <?php echo $status->name ?></span>
        </div>
      </header>
      <?php
        // Hide comments, tags, and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        hide($content['field_tags']);
        hide($content['field_address']);

        print render($content['body']);
        print render($content['field_statement']);

      ?>
      <?php if (!empty($node->field_address)): ?>
        <div class="marker-address"><p><i class="icon-li icon-location "></i><?php print $node->field_address[LANGUAGE_NONE][0]['value'];?></p></div>
      <?php endif; ?>
    </div>
    <div class="col-md-5 col-md-offset-1">
      <?php print render($content['field_geo']); ?>
    </div>
  </div>


  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
    <footer>
      <?php print render($content['field_tags']); ?>
      <?php print render($content['links']); ?>
    </footer>
  <?php endif; ?>
  <div class="row">
    <div class="col-md-6">
      <?php print render($content['comments']); ?>
    </div>
    <div class="col-md-5 col-md-offset-1 ">
      <?php if (!empty($content['field_image'])): ?>
      <div class="node-gallery">
        <?php print render($content['field_image']); ?>
      </div>
      <?php endif; ?>
    </div>
  </div>
</article> <!-- /.node -->
