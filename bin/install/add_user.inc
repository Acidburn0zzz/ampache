<?php
/* vim:set softtabstop=4 shiftwidth=4 expandtab: */
/**
 *
 * LICENSE: GNU Affero General Public License, version 3 (AGPLv3)
 * Copyright 2001 - 2019 Ampache.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

if(php_sapi_name() != 'cli') {
    exit(1);
}

define('NO_SESSION', 1);
define('CLI', 1);

$path = dirname(__FILE__);
$prefix = realpath($path . '/../../');
require_once $prefix . '/lib/init.php';

$options = getopt('e:l:n:p:u:w:');
if (empty($options) || !isset($options['u'])) {
    echo T_("Usage:") . "\n";
    echo "add_user.inc -u <" . T_('Username') .
        "> [ -l <" . T_('Access Level') .
        "> ] [ -p <" . T_('Password') .
        "> ] [ -e <" . T_('E-mail') .
        "> ] [ -w <" . T_('Website') .
        "> ] [ -n <" . T_('Name') . "> ]\n";
    exit(1);
}

$username = $options['u'];
$password = isset($options['p']) ? $options['p'] : mt_rand();
$access = isset($options['l']) ? $options['l'] : AmpConfig::get('auto_user');
$access = isset($access) ? $access : 'guest';
$access = is_numeric($access) ? $access : User::access_name_to_level($access);
$email = isset($options['e']) ? $options['e'] : '';
$website = isset($options['w']) ? $options['w'] : '';
$name = isset($options['n']) ? $options['n'] : '';

if (User::create($username, $name, $email, $website, $password, $access)) {
    /* HINT: %1 Access Level (Administrator/User/etc), %2 Username, %3 password */
    printf(T_('Created %1$s user %2$s with password %3$s'), User::access_level_to_name((string) $access), $username, $password);
    echo "\n";
}
else {
    echo T_('User creation failed'), "\n";
    exit(1);
}

User::fix_preferences('-1');

?>
