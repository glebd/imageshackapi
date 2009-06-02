<?php
    require_once('commandline.php');
    require_once('imageshack.php');
    
    function usage()
    {
echo <<<EOT
Usage:
    imageshack-upload.php --key DEVKEY --file FILE --content-type CONTENT-TYPE [--cookie COOKIE] [--remove-bar yes | --remove-bar no] [] [--tags TAGS] [--public yes | --public no] [--optsize SIZE]
EOT;
    }

    $key      = param('key');
    $file     = param('file');
    $ct       = param('content-type');
    $cookie   = param('cookie');
    $tags     = param('tags');
    $public   = param('public') != 'no';
    $rembar   = param('remove-bar') != 'no';
    $optsize  = param('optsize');

    if (!$file || !$key || !$ct)
    {
        usage();
        die();
    }

    $response = imageshack_upload($key, $file, $ct, $cookie, IMAGESHACK_API_TIMEOUT, $optsize, $rembar, $tags, $public);
    print_r($response);

?>