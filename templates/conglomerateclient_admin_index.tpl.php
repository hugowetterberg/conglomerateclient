<?php
// $Id$
?>
<div class="conglomerateclient-index">
  <ul class="conglomerate-server-list">
    <?php if (empty($servers)): ?>
      <li class="no-servers"><?php print t('No servers added'); ?></li>
    <?php endif ?>
    <?php foreach ($servers as $sid => $info): ?>
      <li class="server"><?php print l($info['name'], 'admin/conglomerateclient/servers/' . $sid) ?></li>
    <?php endforeach ?>
  </ul>
  <div class="conglomerateclient-messages">
  </div>
</div>