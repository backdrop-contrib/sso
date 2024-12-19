Single Sign-On (SSO) Module
===================
The Single Sign-On (SSO) module enables seamless Single Sign-On functionality for Backdrop CMS installations across multiple subdomains of the same main domain. It leverages Backdrop's native session management to authenticate users across shared environments.

Requirements
------------
To ensure proper functionality, the following requirements must be met:

1. To ensure the userbase and sessions are synchronized across all subdomains, configure each site to share specific database tables related to users and sessions. Below are example configurations that can be added to the `settings.php` file of each subdomain to point these tables to the main domain's database:

<details>
<summary>Backdrop CMS < 1.30.0 - Database settings example</summary>

```php
$databases['default']['default'] = array(
  'database' => 'subdomain_db',
  'username' => 'username',
  'password' => 'password',
  'host' => 'localhost',
  'driver' => 'mysql',
  'prefix' => array(
    'default' => '', // Local tables for this subdomain.
    'users' => 'main_domain_db.', // Shared user table.
    'sessions' => 'main_domain_db.', // Shared sessions table.
    'users_roles' => 'main_domain_db.', // Shared user roles table.
    'realname' => 'main_domain_db.', // Shared real name data.
    'field_data_field_first_name' => 'main_domain_db.', // Shared first name field.
    'field_revision_field_first_name' => 'main_domain_db.', // Shared first name field revisions.
    'field_data_field_last_name' => 'main_domain_db.', // Shared last name field.
    'field_revision_field_last_name' => 'main_domain_db.', // Shared last name field revisions.
  ),
);

```
</details>

<details>
<summary>Backdrop CMS >= 1.30.0 - Database settings example</summary>

```php
$database = array(
  'database' => 'subdomain_db',
  'username' => 'username',
  'password' => 'password',
  'host' => 'localhost',
  'prefix' => array(
    'default' => '', // Local tables for this subdomain.
    'users' => 'main_domain_db.', // Shared user table.
    'sessions' => 'main_domain_db.', // Shared sessions table.
    'users_roles' => 'main_domain_db.', // Shared user roles table.
    'realname' => 'main_domain_db.', // Shared real name data.
    'field_data_field_first_name' => 'main_domain_db.', // Shared first name field.
    'field_revision_field_first_name' => 'main_domain_db.', // Shared first name field revisions.
    'field_data_field_last_name' => 'main_domain_db.', // Shared last name field.
    'field_revision_field_last_name' => 'main_domain_db.', // Shared last name field revisions.
  ),
);
```
</details>

2. Cookie Domain Configuration:
   - In the settings.php file for each subdomain, set the $cookie_domain variable to allow cookies to be shared across subdomains.
   - Example: `$cookie_domain = '.example.com';`
   - Replace `example.com` with your actual main domain.

Installation
------------
1. Place the SSO Module in the modules directory of your Backdrop CMS installation.
2. Enable the module in the Backdrop CMS admin interface under Admin > Modules.
3. Ensure the `settings.php` file for each subdomain is properly configured as described above.

How It Works
------------

1. When a user logs in on one subdomain, Backdrop CMS creates a session entry in the shared `sessions` table and sets a session cookie.
2. When the user accesses another subdomain:
   - The session cookie is read and validated against the shared sessions table.
   - If the session is valid, the user is automatically logged in.

Known Limitations
-----------------

- This module does not support cross-domain SSO (e.g., between example.com and example.net). It works only across subdomains of the same main domain (e.g., sub1.example.com and sub2.example.com).

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
