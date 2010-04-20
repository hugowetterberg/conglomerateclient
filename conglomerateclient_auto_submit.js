(function($) {
  $(document).ready(function() {
    if (!$('#conglomerateclient-login-form').hasClass('error')) {
      $('#conglomerateclient-login-form')
        .addClass('conglomerateclient-login-submitted')
        .append($('<div class="progress-message"></div>').text(Drupal.t('Logging in')))
        .submit();
    }
  });
})(jQuery);