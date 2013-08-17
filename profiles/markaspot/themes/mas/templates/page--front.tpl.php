<div class="navbar-wrapper">
  <div class="container">
    <!-- header id="navbar" role="banner" class="navbar nav-collapse collapse navbar-inverse" -->
    <header id="navbar" role="banner" class="navbar navbar-inverse">
      <div class="navbar-inner">
        <div class="container">
          <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
          <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </a>

          <?php if (!empty($logo)): ?>
            <a class="logo pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>

          <?php if (!empty($site_name)): ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="brand"><?php print $site_name; ?></a>
            </h1>
          <?php endif; ?>

          <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
            <div class="nav-collapse collapse">
              <nav role="navigation">
                <?php if (!empty($primary_nav)): ?>
                  <?php print render($primary_nav); ?>
                <?php endif; ?>
                <?php if (!empty($page['navigation'])): ?>
                  <?php print render($page['navigation']); ?>
                <?php endif; ?>
                <?php if (!empty($secondary_nav)): ?>
                  <?php print render($secondary_nav); ?>
                <?php endif; ?>
              </nav>
            </div>
          <?php endif; ?>
        </div>
      </div>
    </header>
  </div>
</div>
<div id="map_wrapper_splash">
  <a id="start" title="<?php t('Click to get the map view')?>" href="/map"></a>
  <div id="map" class="front"></div>
</div>
<div class="mapheader masthead">
  <div class="container">
    <h1>Fix MaS-City</h1>
    <p>Help to make your city a better place</p>
    <p>
      <a href="node/add/report" class="btn btn-primary btn-success btn-large"><i class="icon-bullhorn"> </i> Report Issue</a>
       <a href="list" class="btn btn-primary btn-action btn-large"><i class="icon-eye-open"> </i>Track Issues</a>

    </p>
    <ul class="masthead-links">
      <li>
        <a href="imprint" onclick="">Imprint</a>
      </li>
      <li>
        <a href="open311" onclick="">Open311</a>
      </li>
      <li>
        <a href="about" onclick="">About us</a>
      </li>
      <li>
        Mark-a-Spot Version 2.2
      </li>
    </ul>
  </div>
</div>


<div class="main-container container">
  
  <header role="banner" id="page-header">
    <?php if (!empty($site_slogan)): ?>
      <p class="lead"><?php print $site_slogan; ?></p>
    <?php endif; ?>

    <?php print render($page['header']); ?>
  </header> <!-- /#header -->

  <div class="row">

    <?php if (!empty($page['sidebar_first'])): ?>
      <aside class="span3" role="complementary">
        <?php print render($page['sidebar_first']); ?>
      </aside>  <!-- /#sidebar-first -->
    <?php endif; ?>  

    <section class="<?php print _bootstrap_content_span($columns); ?>">  
      <?php
      $viewName = 'Gallery';
      print views_embed_view($viewName);
      ?>
      <?php if (!empty($page['highlighted'])): ?>
        <div class="highlighted hero-unit"><?php print render($page['highlighted']); ?></div>
      <?php endif; ?>
      <?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if (!empty($title)): ?>
        <h1 class="page-header"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php if (!empty($tabs)): ?>
        <?php print render($tabs); ?>
      <?php endif; ?>
      <?php if (!empty($page['help'])): ?>
        <div class="well"><?php print render($page['help']); ?></div>
      <?php endif; ?>
      <?php if (!empty($action_links)): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </section>

    <?php if (!empty($page['sidebar_second'])): ?>
      <aside class="span3" role="complementary">     
        <?php print render($page['sidebar_second']); ?>
      </aside>  <!-- /#sidebar-second -->
    <?php endif; ?>

  </div>
</div>
<footer class="footer">
  <?php if ($page['footer_firstcolumn'] || $page['footer_secondcolumn'] || $page['footer_thirdcolumn'] || $page['footer_fourthcolumn']): ?>
    <div id="footer-columns" class="container">
      <?php print render($page['footer_firstcolumn']); ?>
      <?php print render($page['footer_secondcolumn']); ?>
      <?php print render($page['footer_thirdcolumn']); ?>
      <?php print render($page['footer_fourthcolumn']); ?>
    </div> <!-- /#footer-columns -->
  <?php endif; ?>
  <?php print render($page['footer']); ?>
</footer>
