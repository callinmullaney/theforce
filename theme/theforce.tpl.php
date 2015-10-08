<div<?php print $attributes; ?> class="<?php print $classes; ?>">
  <?php foreach($content as $location_name => $location): ?>
    <div<?php print $location_attributes[$location_name]; ?>>
      <?php print render($location); ?>
    </div>
  <?php endforeach; ?>
</div>
