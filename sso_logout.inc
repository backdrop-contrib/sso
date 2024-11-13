<?php

function sso_user_logout($account) {
  // Ensure the account is valid before proceeding.
  if (empty($account->uid) || $account->uid == 0) {
    watchdog('sso', 'Invalid user account; SSO logout aborted.', [], WATCHDOG_ERROR);
    return;
  }

  // Detect if this logout action is triggered by masquerade or switch-back.
  $current_path = current_path();
  $referrer = isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '';
  $is_masquerade = strpos($current_path, 'masquerade/switch/') === 0 || strpos($referrer, '/masquerade/switch/') !== false;
  $is_unswitch = strpos($current_path, 'masquerade/unswitch') === 0 || strpos($referrer, '/masquerade/unswitch') !== false;

  // Skip cross-site logout if this is a masquerade session switch.
  if ($is_masquerade) {
    watchdog('sso', 'Masquerade logout detected; skipping cross-site logout for UID: ' . $account->uid, [], WATCHDOG_NOTICE);
    return;
  }

  // Skip cross-site logout if we’re switching back (unswitch) to the original user.
  if ($is_unswitch) {
    watchdog('sso', 'Masquerade unswitch detected; skipping cross-site logout for masqueraded UID: ' . $account->uid, [], WATCHDOG_NOTICE);

    // Generate a cross-site login for the original user after unswitch.
    $original_user_uid = isset($_SESSION['masquerading']) ? $_SESSION['masquerading'] : NULL;
    if ($original_user_uid) {
      $original_account = user_load($original_user_uid);
      if ($original_account) {
        // Trigger cross-site login for the original user.
        sso_initiate_cross_site_login($original_account);
      }
    }
    return;
  }

  // Proceed with cross-site logout if it's a direct logout.
  $other_site_url = rtrim(config_get('sso.settings', 'other_site_url'), '/');
  if (empty($other_site_url)) {
    watchdog('sso', 'Other site URL is not configured for SSO logout.', [], WATCHDOG_ERROR);
    return;
  }

  $logout_token = sso_generate_logout_token($account);
  if ($logout_token === false) {
    watchdog('sso', 'Failed to generate logout token for UID: ' . $account->uid, [], WATCHDOG_ERROR);
    return;
  }

  $current_site_url = rtrim(url('<front>', ['absolute' => TRUE]), '/');
  $logout_url = $other_site_url . '/user/logout?sso_token=' . $logout_token . '&return_url=' . urlencode($current_site_url);

  watchdog('sso', 'Logging user out on both sites for UID: ' . $account->uid . ' with cross-site logout URL: ' . $logout_url, [], WATCHDOG_NOTICE);

  // Redirect to log out on the other site.
  register_shutdown_function(function() use ($logout_url) {
    header("Location: $logout_url");
    exit();
  });
}



/**
 * Generates a secure token specifically for cross-site logout requests.
 */
function sso_generate_logout_token($account) {
  $key = config_get('sso.settings', 'shared_secret_key');

  // Create the payload with UID and expiration time.
  $payload = json_encode([
    'uid' => $account->uid,
    'exp' => REQUEST_TIME + 600, // Token expires in 10 minutes.
  ]);

  // Encode the payload.
  $base64UrlPayload = str_replace(['+', '/', '='], ['-', '_', ''], base64_encode($payload));

  // Sign the token.
  $signature = hash_hmac('sha256', $base64UrlPayload, $key, true);
  $base64UrlSignature = str_replace(['+', '/', '='], ['-', '_', ''], base64_encode($signature));

  return $base64UrlPayload . "." . $base64UrlSignature;
}

/**
 * Decodes and verifies a logout token.
 */
function sso_decode_logout_token($token, $key) {
  $parts = explode('.', $token);
  if (count($parts) !== 2) {
    watchdog('sso', 'Token format is invalid.', [], WATCHDOG_ERROR);
    return false;
  }

  list($base64UrlPayload, $base64UrlSignature) = $parts;
  $payload = json_decode(base64_decode(strtr($base64UrlPayload, '-_', '+/')), true);

  // Verify the signature.
  $signatureProvided = base64_decode(strtr($base64UrlSignature, '-_', '+/'));
  $signatureValid = hash_hmac('sha256', $base64UrlPayload, $key, true);

  if (!hash_equals($signatureValid, $signatureProvided)) {
    watchdog('sso', 'Token signature mismatch.', [], WATCHDOG_ERROR);
    return false;
  }

  // Check for expiration.
  if (isset($payload['exp']) && $payload['exp'] < REQUEST_TIME) {
    watchdog('sso', 'Token expired at ' . date('Y-m-d H:i:s', $payload['exp']) . '.', [], WATCHDOG_ERROR);
    return false;
  }

  return (object) $payload;
}
