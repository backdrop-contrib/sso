Backdrop SSO Module
===================
Backdrop SSO Module provides seamless Single Sign-On (SSO) functionality between multiple Backdrop CMS sites, enabling users to log in and out across sites with a single action. It also supports user masquerading across sites if the Masquerade module is enabled, allowing administrators to impersonate users on both sites simultaneously.

Requirements
------------
This module has no required dependencies. However, it supports cross-site user impersonation when used with the [Masquerade Module](https://github.com/backdrop-contrib/masquerade).

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
https://github.com/backdrop-contrib/backdrop_sso/issues.

Current Maintainers
-------------------
- [Your Name](https://github.com/username)
- Seeking additional maintainers

Credits
-------
- Ported to Backdrop CMS by [Your Name](https://github.com/username).
- Inspired by other SSO implementations and designed to work with the Backdrop Masquerade module.
- Sponsored by [Your Organization](https://example.org).

License
-------
This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
