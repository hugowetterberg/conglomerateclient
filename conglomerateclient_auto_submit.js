(function($) {
  $(document).ready(function() {
    $('#conglomerateclient-login-form')
      .addClass('conglomerateclient-login-submitted')
      .append($('<div class="progress-message"></div>').text(Drupal.t('Logging in')))
      .submit();
  });
})(jQuery);