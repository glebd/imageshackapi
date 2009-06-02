<?php
    
    require_once('multipost.php');

    /**
     * API endpoints
     */
    define('IMAGESHACK_API_URL',       'http://imageshack.us/upload_api.php');
    define('IMAGESHACK_VIDEO_API_URL', 'http://render1.imageshack.us/upload_api.php');

    /**
     * Default connection and response timeout
     */
    define('IMAGESHACK_API_TIMEOUT', 10);


    function imageshack_upload($developer_key,
                               $filename,
                               $content_type,
                               $user_cookie = null,
                               $timeout = IMAGESHACK_API_TIMEOUT,
                               $optsize = null,                    
                               $remove_bar = true,
                               $tags = null,
                               $public = true,
                               $more_params = null)
    {
        $request = array();
        $request[] = new stringpart('key', $developer_key);
        $request[] = new filepart('fileupload', $filename, basename($filename), $content_type, 'iso-8859-1');
        if ($optsize)
            $request[] = new stringpart('optsize', $optsize);
        if ($user_cookie)
            $request[] = new stringpart('cookie', $user_cookie);
        if ($remove_bar)
            $request[] = new stringpart('rembar', 'yes');
        $request[] = new stringpart('public', $public ? 'yes' : 'no');
        if ($tags)
            $request[] = new stringpart('tags', $tags);
        if ($more_params)
        {
            foreach ($more_params as $name => $value)
            {
                $request[] = new stringpart($name, $value);
            }
        }
        return __imageshack_exec($content_type, $request, $timeout);
    }

    function imageshack_transload($developer_key,
                                  $url,
                                  $content_type,
                                  $user_cookie = null,
                                  $timeout = IMAGESHACK_API_TIMEOUT,
                                  $optsize = null,                    
                                  $remove_bar = true,
                                  $tags = null,
                                  $public = true,
                                  $more_params = null)
    {
        $request = array();
        $request[] = new stringpart('key', $developer_key);
        $request[] = new stringpart('url', $url);
        if ($optsize)
            $request[] = new stringpart('optsize', $optsize);
        if ($user_cookie)
            $request[] = new stringpart('cookie', $user_cookie);
        if ($remove_bar)
            $request[] = new stringpart('rembar', 'yes');
        $request[] = new stringpart('public', $public ? 'yes' : 'no');
        if ($tags)
            $request[] = new stringpart('tags', $tags);
        if ($more_params)
        {
            foreach ($more_params as $name => $value)
            {
                $request[] = new stringpart($name, $value);
            }
        }

        return __imageshack_exec($content_type, $request, $timeout);
    }


    function __imageshack_exec($content_type,
                               $request,
                               $timeout)
    {
        if (strpos($content_type, 'image/') === 0)
            $url = IMAGESHACK_API_URL;
        else 
            $url = IMAGESHACK_VIDEO_API_URL;

        $response = multipost($url, 
                              $request, 
                              $errno, 
                              $errormessage,
                              $timeout);

        $xml = @simplexml_load_string($response);
        if (!$xml)
        {
            //error_log('Failed to parse XML: ' . $response);
            return false;
        }

        if (!@$xml->error && !@$xml->files)
        {
            //error_log('Unexpected XML');
            return false;
        }

        return $xml;
    }
    

?>