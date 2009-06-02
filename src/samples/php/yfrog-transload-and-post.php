<?php
    require_once('commandline.php');
    require_once('yfrog.php');
    
    function usage()
    {
echo <<<EOT
Usage:
    yfrog-transload-and-post.php --username USERNAME --password PASSWORD --url URL [--message MESSAGE] [--tags TAGS] [--public yes | --public no]
EOT;
    }

    $username = param('username');
    $password = param('password');
    $url      = param('url');
    $tags     = param('tags');
    $public   = param('public');
    $message  = param('message');

    if (!$username || !$password || !$url)
    {
        usage();
        die();
    }

    $response = yfrog_transload_and_post($url, $message, $username, $password, $tags, $public == 'yes');
    if ($response['stat'])
        echo 'http://twitter.com/' . $username . '/status/' . $response['statusid'];
    else
        error_log('[' . $response['code'] . '] ' . $response['msg']);


?>