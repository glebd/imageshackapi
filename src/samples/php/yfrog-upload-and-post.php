<?php
    require_once('commandline.php');
    require_once('yfrog.php');
    
    function usage()
    {
echo <<<EOT
Usage:
    yfrog-upload-and-post.php --username USERNAME --password PASSWORD --file FILE [--message MESSAGE] [--tags TAGS] [--public yes | --public no]
EOT;
    }

    $username = param('username');
    $password = param('password');
    $file     = param('file');
    $tags     = param('tags');
    $public   = param('public');
    $message  = param('message');

    if (!$username || !$password || !$file)
    {
        usage();
        die();
    }

    $response = yfrog_upload_and_post($file, $message, $username, $password, $tags, $public == 'yes');
    if ($response['stat'])
        echo 'http://twitter.com/' . $username . '/status/' . $response['statusid'];
    else
        error_log('[' . $response['code'] . '] ' . $response['msg']);


?>