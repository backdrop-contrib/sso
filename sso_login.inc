<?php

/**
 * Implements hook_user_login().
 * Handles the login flow by generating a token and redirecting to the other site,
 * including for masquerade actions to switch users cross-site.
 */
function sso_user_login($edit, $account) {
  // Ensure the account is valid.
  if (empty($account->uid) || $account->uid == 0) {
    watchdog('sso', 'Invalid user account; SSO login aborted.', [], WATCHDOG_ERROR);
    return;
  }

  // Detect if this login action is triggered by a masquerade.
  $current_path = current_path();
  $referrer = isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '';
  $is_masquerade = strpos($current_path, 'masquerade/switch/') === 0 || strpos($referrer, '/masquerade/switch/') !== false;

  // Retrieve the current site URL and the configured other site URL.
  $current_site_url = rtrim(url('<front>', ['absolute' => TRUE]), '/');
  $other_site_url = rtrim(config_get('sso.settings', 'other_site_url') ?? '', '/');

  // If no other site URL is configured, abort.
  if (empty($other_site_url)) {
    watchdog('sso', 'Other site URL is not configured.', [], WATCHDOG_ERROR);
    backdrop_set_message(t('Other site URL is not configured.'), 'error');
    return;
  }

  // Generate a fresh SSO token for this login.
  $token = sso_generate_token($account);
  if ($token === false) {
    watchdog('sso', 'Failed to generate token for user with UID: ' . $account->uid, [], WATCHDOG_ERROR);
    return;
  }

  // Set the return URL to redirect back to the current site after SSO completes.
  $return_url = $current_site_url;

  // Construct the redirect URL for SSO login on the other site.
  $redirect_url = $other_site_url . '/sso-login?token=' . $token . '&return_url=' . urlencode($return_url);

  // Log the intended redirection.
  if ($is_masquerade) {
    watchdog('sso', 'Masquerade switch detected; initiating cross-site login for impersonated user UID: ' . $account->uid, [], WATCHDOG_NOTICE);
  } else {
    watchdog('sso', 'Preparing to redirect user UID ' . $account->uid . ' to SSO login at ' . $redirect_url, [], WATCHDOG_NOTICE);
  }

  // Register a shutdown function to perform the redirect after all other operations.
  register_shutdown_function(function() use ($redirect_url) {
    header("Location: $redirect_url");
    exit();
  });
}


/**
 * Processes SSO login by verifying the token and logging the user in.
 */
function sso_process_login() {
  global $user;

  // Check if the user is already logged in. If so, we should destroy their current session.
  if ($user->uid != 0) {
    // Clear the current session.
    _backdrop_session_destroy(session_id());
  }

  // Ensure a token is present and valid.
  if (isset($_GET['token']) && !empty($_GET['token'])) {
    $key = config_get('sso.settings', 'shared_secret_key');
    $token = $_GET['token'];
    $payload = sso_decode_token($token, $key);

    if ($payload && isset($payload->uid) && $payload->uid != 0) {
      // Load the user account from the payload.
      $account = user_load($payload->uid);

      if ($account) {
        // Set the user account and regenerate the session to secure it.
        $user = $account;
        backdrop_session_regenerate();
        watchdog('sso', 'User session finalized for UID: ' . $user->uid . '. Username: ' . $user->name, [], WATCHDOG_NOTICE);

        // Redirect to the return URL or the homepage if not specified.
        $redirect_to = isset($_GET['return_url']) ? $_GET['return_url'] : url('<front>', ['absolute' => TRUE]);
        backdrop_goto($redirect_to);
      } else {
        watchdog('sso', 'User account not found for UID: ' . $payload->uid . '.', [], WATCHDOG_ERROR);
      }
    } else {
      watchdog('sso', 'Invalid or expired SSO token.', [], WATCHDOG_ERROR);
    }
  } else {
    watchdog('sso', 'SSO token missing or empty.', [], WATCHDOG_ERROR);
  }
}

/**
 * Initiates cross-site login for a given account.
 */
function sso_initiate_cross_site_login($account) {
  // Retrieve other site URL and check if configured.
  $other_site_url = rtrim(config_get('sso.settings', 'other_site_url') ?? '', '/');

  // If no other site URL is configured, abort.
  if (empty($other_site_url)) {
    watchdog('sso', 'Other site URL is not configured.', [], WATCHDOG_ERROR);
    backdrop_set_message(t('Other site URL is not configured.'), 'error');
    return;
  }

  // Generate the SSO login token.
  $token = sso_generate_token($account);
  if ($token === false) {
    watchdog('sso', 'Failed to generate login token for user with UID: ' . $account->uid, [], WATCHDOG_ERROR);
    return;
  }

  $current_site_url = rtrim(url('<front>', ['absolute' => TRUE]), '/');
  $login_url = $other_site_url . '/sso-login?token=' . $token . '&return_url=' . urlencode($current_site_url);

  watchdog('sso', 'Initiating cross-site login for UID: ' . $account->uid . ' with login URL: ' . $login_url, [], WATCHDOG_NOTICE);

  // Register shutdown function to perform the redirect after other actions.
  register_shutdown_function(function() use ($login_url) {
    header("Location: $login_url");
    exit();
  });
}

