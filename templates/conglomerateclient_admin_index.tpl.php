<?php
// $Id$
?>
<div class="conglomerateclient-index">
  <ul class="conglomerate-server-list">
    <?php foreach ($servers as $sid => $info): ?>
      <li class="server"><?php print l($info['name'], 'admin/conglomerateclient/servers/' . $sid) ?></li>
    <?php endforeach ?>
  </ul>
  <div class="conglomerateclient-messages">
  </div>
</div>