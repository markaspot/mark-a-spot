<?php
/**
 * @file
 * Displays a Bootstrap-style nav for tabs/pills.
 *
 * Available variables:
 * - $is_empty: Boolean: any content to render at all?
 * - $wrapper_classes: The classes set on the wrapping div.
 * - $nav_classes: The classes set on the nav ul.
 * - $navs: An array of nav elements (tabs, pills)
 * - $pane_classes: The classes set on the panes containing content.
 * - $panes: An array of panes containing content.
 * - $index: The index of the active tab/pane.
 *
 * @ingroup themeable
 */
?>

<?php if (!$is_empty) : ?>

  <div class="<?php print $wrapper_classes; ?>">

    <?php if ($flip) : ?>
      <div class="tab-content <?php print $pane_classes; ?>">
        <?php foreach ($panes as $index => $pane) : ?>
          <div id="<?php print $pane['id']; ?>" class="tab-pane <?php if ($index === $active) print 'active'; ?>">
            <?php print $pane['content']; ?>
          </div>
        <?php endforeach; ?>
      </div>
    <?php endif; ?>

    <ul class="nav <?php print $nav_classes; ?>">
      <?php foreach ($navs as $index => $nav) : ?>
        <li class="<?php print $nav['classes']; ?><?php if ($index === $active) print 'active'; ?>">
          <?php print $nav['content']; ?>
        </li>
      <?php endforeach; ?>
    </ul>

    <?php if (!$flip) : ?>
      <div class="tab-content <?php print $pane_classes; ?>">
        <?php foreach ($panes as $index => $pane) : ?>
          <div id="<?php print $pane['id']; ?>" class="tab-pane <?php if ($index === $active) print 'active'; ?>">
            <?php print $pane['content']; ?>
          </div>
        <?php endforeach; ?>
      </div>
    <?php endif; ?>

  </div>

<?php endif; ?>
