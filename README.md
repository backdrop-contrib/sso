Single Sign-On (SSO) Module
===================
The Single Sign-On (SSO) module enables seamless Single Sign-On functionality for Backdrop CMS installations across multiple subdomains of the same main domain. It leverages Backdrop's native session management to authenticate users across shared environments.

Requirements
------------
To ensure proper functionality, the following requirements must be met:

1. Shared User Tables:
   - The users and sessions tables must be shared between the subdomains.
   - This allows all subdomains to access the same user and session data.

2. Cookie Domain Configuration:
   - In the settings.php file for each subdomain, set the $cookie_domain variable to allow cookies to be shared across subdomains.
   - Example: `$cookie_domain = '.example.com';`
   - Replace `example.com` with your actual main domain.

To ensure the userbase is synchronized, configure both sites to share specific database tables related to users and sessions. Here’s an example configuration that can be added to your `settings.php` file to point certain tables on a subdomain website to main domain website's database:

```php
$databases['default']['default'] = array(
  'database' => 'subdomain_db',
  'username' => 'username',
  'password' => 'password',
  'host' => 'localhost',
  'driver' => 'mysql',
  'prefix' => array(
    'default' => '',
    'users' => 'main_domain_db.',
    'sessions' => 'main_domain_db.',
    'users_roles' => 'main_domain_db.',
    'realname' => 'main_domain_db.',
    'field_data_field_first_name' => 'main_domain_db.',
    'field_revision_field_first_name' => 'main_domain_db.',
    'field_data_field_last_name' => 'main_domain_db.',
    'field_revision_field_last_name' => 'main_domain_db.',
  ),
);
```

Installation
------------
- Install this module following Backdrop CMS’s standard instructions: https://docs.backdropcms.org/documentation/extend-with-modules.
- If the **Masquerade Module** is enabled, impersonating a user on one site will automatically impersonate the user on the second site.
- Once configured, logging in or impersonating a user on one site will initiate a login on the other site.

Issues
------
Report bugs and feature requests in the Issue Queue:
https://github.com/backdrop-contrib/sso/issues.

Current Maintainers
-------------------
- [Alan Mels](https://github.com/alanmels)

Credits
-------
- Created for Backdrop CMS by [Alan Mels](https://github.com/alanmels).
- Sponsored by [AltaGrade](https://www.altagrade.com).

License
-------
This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
