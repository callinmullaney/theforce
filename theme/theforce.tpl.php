<div<?php print $attributes; ?> class="<?php print $classes; ?>">
  <?php foreach($content as $region_name => $region): ?>
    <div<?php print $region_attributes[$region_name]; ?>>
      <?php print render($region); ?>
    </div>
  <?php endforeach; ?>
</div>
