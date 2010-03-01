<?php
  $i = 0;
?>
<ul class="conglomerate-nearby-items">
  <?php foreach ($items as $item): ?>
    <?php $i++; ?>
    <li class="item item-<?php print $i%2 ? 'odd' : 'even' ?>">
      <?php if (!empty($item['image'])): ?>
        <a href="<?php print $item['href']; ?>" class="image"><img src="<?php print $item['image']; ?>" /></a>
      <?php endif ?>
      <a href="<?php print $item['href']; ?>" class="title"><?php print $item['title']; ?></a>
    </li>
  <?php endforeach ?>
</ul>