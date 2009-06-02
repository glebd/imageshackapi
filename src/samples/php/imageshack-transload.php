<?php
    require_once('commandline.php');
    require_once('imageshack.class.php');
    
    function usage()
    {
echo <<<EOT
Usage:
    imageshack-transload.php --key DEVKEY --url URL [--cookie COOKIE] [--remove-bar yes | --remove-bar no] [] [--tags TAGS] [--public yes | --public no] [--optsize SIZE]
EOT;
    }

    $key = param('key');
    if (!$key)
    {
        usage();
        die();
    }

    $url = param('url');
    if (!$url)
    {        
        usage();
        die();
    }

    $uploader = &new ImageShackUploader($key);
    $cookie = param('cookie');
    if ($cookie)
        $uploader->setCookie($cookie);
    $tags = param('tags');
    if ($tags)
        $uploader->setTags($tags);
    $public = param('public');
    if ($public)
        $public = $public != 'no';
    if ($public !== null)
        $uploader->setPublic($public);
    $rembar = param('remove-bar') != 'no';
    if ($rembar)
        $uploader->setRemoveBar($rembar);
    $optsize = param('optsize');
    if ($optsize)
        $uploader->setOptSize($optsize);

    $response = $uploader->transload($url);
    print_r($response);

?>