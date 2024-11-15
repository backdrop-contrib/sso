SSO Module
===================
The SSO Module provides seamless Single Sign-On (SSO) functionality between two Backdrop CMS sites, enabling users to log in and out across sites with a single action. It also supports cross-site user impersonation with the Masquerade module, allowing administrators to impersonate users on both sites simultaneously if enabled.

Requirements
------------
This module requires that the userbase (user accounts) be identical on both sites for SSO to function correctly.

To ensure the userbase is synchronized, you can configure both sites to share specific database tables related to users and sessions. Here’s an example configuration that can be added to your `settings.php` file to point certain tables on Site B to Site A's database:

```php
$databases['default']['default'] = array(
  'database' => 'site_B_database',
  'username' => 'site_B_username',
  'password' => 'site_B_database_password_here',
  'host' => 'localhost',
  'driver' => 'mysql',
  'charset' => 'utf8mb4',
  'collation' => 'utf8mb4_general_ci',
  'prefix' => array(
    'default' => '',
    'users' => 'site_A_database.',
    'sessions' => 'site_A_database.',
    'users_roles' => 'site_A_database.',
    'realname' => 'site_A_database.',
    'field_data_field_first_name' => 'site_A_database.',
    'field_revision_field_first_name' => 'site_A_database.',
    'field_data_field_last_name' => 'site_A_database.',
    'field_revision_field_last_name' => 'site_A_database.',
  ),
);
```

Installation
------------
- Install this module following Backdrop CMS’s standard instructions: https://docs.backdropcms.org/documentation/extend-with-modules.
- Go to the configuration page at Administration > Configuration > People > SSO (admin/config/people/sso) and configure:
  - **Shared Secret Key**: a secure key shared across sites to verify SSO tokens.
  - **Other Site URL**: the URL of the linked site for SSO (e.g., `https://site-b.com`).
- If the **Masquerade Module** is enabled, impersonating a user on one site will automatically impersonate the user on the second site.
- Once configured, logging in or impersonating a user on one site will initiate a login on the other site.

Issues
------
Report bugs and feature requests in the Issue Queue:
https://github.com/backdrop-contrib/sso/issues.

Current Maintainers
-------------------
- [Alan Mels](https://github.com/alanmels)
- Seeking additional maintainers

Credits
-------
- Created for Backdrop CMS by [Alan Mels](https://github.com/alanmels).
- Sponsored by [AltaGrade](https://www.altagrade.com).
- This module is not a successor to the abandoned and deprecated [Drupal module](https://www.drupal.org/project/sso) with the same namespace.

We needed a simple solution to synchronize user sessions between just two websites

License
-------
This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
