<?php
    require_once('commandline.php');
    require_once('imageshack.php');
    
    function usage()
    {
echo <<<EOT
Usage:
    imageshack-transload.php --key DEVKEY --url URL --content-type CONTENT-TYPE [--cookie COOKIE] [--remove-bar yes | --remove-bar no] [] [--tags TAGS] [--public yes | --public no] [--optsize SIZE]
EOT;
    }

    $key      = param('key');
    $url      = param('url');
    $ct       = param('content-type');
    $cookie   = param('cookie');
    $tags     = param('tags');
    $public   = param('public') != 'no';
    $rembar   = param('remove-bar') != 'no';
    $optsize  = param('optsize');

    if (!$url || !$key || !$ct)
    {
        usage();
        die();
    }

    $response = imageshack_transload($key, $url, $ct, $cookie, IMAGESHACK_API_TIMEOUT, $optsize, $rembar, $tags, $public);
    print_r($response);

?>