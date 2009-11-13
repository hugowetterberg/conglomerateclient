<?php
// $Id$
?>
<div class="conglomerateclient-index">
  <ul class="conglomerate-server-list">
    <?php if (empty($servers)): ?>
      <li class="no-servers"><?php print t('No servers added'); ?></li>
    <?php endif ?>
    <?php foreach ($servers as $sid => $info): ?>
      <li class="server">
        <div class="server-name"><?php print l($info['name'], 'admin/conglomerateclient/servers/' . $sid) ?></div>
        <a class="address"><?php print l($info['url'], $info['url']); ?></a>
        <?php if (!empty($info['oauth_token_key'])): ?>
          <span class="authorization authorized"><?php print t('Authorized'); ?></span>
        <?php else: ?>
          <?php print l(t('Authorize'), sprintf('admin/settings/conglomerateclient/authorize/%d', $info['sid'])); ?>
        <?php endif ?>
      </li>
    <?php endforeach ?>
  </ul>
  <div class="conglomerateclient-messages">
  </div>
</div>