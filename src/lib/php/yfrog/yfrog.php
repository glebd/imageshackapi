<?php
    
    /**
     * API endpoint
     */
    define('YFROG_API_URL',     'http://yfrog.com/api');

    /**
     * Default connection and response timeout
     */
    define('YFROG_API_TIMEOUT', 10);


    /**
     * Error codes,
     * 3XXX are client errors
     * 2XXX are ImageShack-specified errors
     * 1XXX are standard errors
     */
    define('YFROG_ERROR_BAD_CREDENTIALS',       1001);
    define('YFROG_ERROR_NO_FILE',               1002);
    define('YFROG_ERROR_SIZE_TOO_BIG',          1004);

    define('YFROG_ERROR_WRONG_ACTION',          2001);
    define('YFROG_ERROR_UPLOAD_FAILED',         2002);
    define('YFROG_ERROR_STATUS_UPDATE_FAILED',  2003);

    define('YFROG_ERROR_IO_ERROR',              3001);
    define('YFROG_ERROR_MALFORMED_XML',         3002);

    /**
     * Uploads file to yfrog.com. 
     * Requirements: simplexml, curl
     * @param filename filename to upload
     * @param username Twitter username
     * @param password Twitter password
     * @param tags comma-separated list of tags
     * @param public is uploaded media should be public or not
     * @param timeout connection and response timeout
     * @return array 
     *  stat: true/false; true indicates success, false - error
     *  code: error code  (only if stat = false)
     *  msg: error message (only if stat = false)
     *  mediaid media identifier (only if stat = true)
     *  mediaurl media URL (only if stat = true)
     */
    function yfrog_upload($filename,
                          $username,
                          $password,
                          $tags = '',
                          $public = true,
                          $timeout = YFROG_API_TIMEOUT)
    {
        $request = array
        (
            'username' => $username,
            'password' => $password,
            'tags'     => $tags,
            'public'   => $public ? 'yes' : 'no',
            'media'    => '@' . $filename
        );

        return __yfrog_exec('upload', $request, $timeout);
    }

    /**
     * Transloads file to yfrog.com. 
     * Requirements: simplexml, curl
     * @param url URL to transload
     * @param username Twitter username
     * @param password Twitter password
     * @param tags comma-separated list of tags
     * @param public is uploaded media should be public or not
     * @param timeout connection and response timeout
     * @return array 
     *  stat: true/false; true indicates success, false - error
     *  code: error code  (only if stat = false)
     *  msg: error message (only if stat = false)
     *  mediaid media identifier (only if stat = true)
     *  mediaurl media URL (only if stat = true)
     */
    function yfrog_transload($url,
                             $username,
                             $password,
                             $tags = '',
                             $public = true,
                             $timeout = YFROG_API_TIMEOUT)
    {
        $request = array
        (
            'username' => $username,
            'password' => $password,
            'tags'     => $tags,
            'public'   => $public ? 'yes' : 'no',
            'url'      => $url
        );

        return __yfrog_exec('upload', $request, $timeout);
    }


    /**
     * @deprecated
     */
    function yfrog_post($filename,
                        $message,
                        $username,
                        $password,
                        $tags = '',
                        $public = true,
                        $timeout = YFROG_API_TIMEOUT)
    {
        return yfrog_upload_and_post($filename,
                                     $message,
                                     $username,
                                     $password,
                                     $tags,
                                     $public,
                                     $timeout);
    }

    /**
     * Uploads file to yfrog.com and posts message to Twitter. 
     * Requirements: simplexml, curl
     * @param filename filename to upload
     * @param message message to send
     * @param username Twitter username
     * @param password Twitter password
     * @param tags comma-separated list of tags
     * @param public is uploaded media should be public or not
     * @param timeout connection and response timeout
     * @return array 
     *  stat: true/false; true indicates success, false - error
     *  code: error code  (only if stat = false)
     *  msg: error message (only if stat = false)
     *  mediaid media identifier (only if stat = true)
     *  mediaurl media URL (only if stat = true)
     */
    function yfrog_upload_and_post($filename,
                                   $message,
                                   $username,
                                   $password,
                                   $tags = '',
                                   $public = true,
                                   $timeout = YFROG_API_TIMEOUT)
    {
        $request = array
        (
            'username' => $username,
            'password' => $password,
            'tags'     => $tags,
            'public'   => $public ? 'yes' : 'no',
            'media'    => '@' . $filename,
            'message'  => $message
        );

        return __yfrog_exec('uploadAndPost', $request, $timeout);
    }


    /**
     * Transloads file to yfrog.com and posts message to Twitter. 
     * Requirements: simplexml, curl
     * @param url URL to upload
     * @param message message to send
     * @param username Twitter username
     * @param password Twitter password
     * @param tags comma-separated list of tags
     * @param public is uploaded media should be public or not
     * @param timeout connection and response timeout
     * @return array 
     *  stat: true/false; true indicates success, false - error
     *  code: error code  (only if stat = false)
     *  msg: error message (only if stat = false)
     *  mediaid media identifier (only if stat = true)
     *  mediaurl media URL (only if stat = true)
     */
    function yfrog_transload_and_post($url,
                                      $message,
                                      $username,
                                      $password,
                                      $tags = '',
                                      $public = true,
                                      $timeout = YFROG_API_TIMEOUT)
    {
        $request = array
        (
            'username' => $username,
            'password' => $password,
            'tags'     => $tags,
            'public'   => $public ? 'yes' : 'no',
            'url'      => $url,
            'message'  => $message
        );

        return __yfrog_exec('uploadAndPost', $request, $timeout);
    }



    function __yfrog_exec($action,
                          $request,
                          $timeout)
    {
        $handle = @curl_init(YFROG_API_URL . '/'. $action);
        if (!$handle)
            return array('stat' => false, 'code' => YFROG_ERROR_IO_ERROR, 'msg' => 'Unable to initialize CURL');

        curl_setopt($handle, CURLOPT_HEADER, 0);
        curl_setopt($handle, CURLOPT_CONNECTTIMEOUT, $timeout);
        curl_setopt($handle, CURLOPT_TIMEOUT, $timeout);
        curl_setopt($handle, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($handle, CURLOPT_POSTFIELDS, $request);

        $response = curl_exec($handle);

        $error = curl_errno($handle);
        if ($error)
        {
            $ret = array('stat' => false, 'code' => YFROG_ERROR_IO_ERROR, 'msg' => curl_error($handle) . ' [' . $error . ']');
            curl_close($handle);
            return $ret;
        }

        curl_close($handle);

        $xml = @simplexml_load_string($response);
        if (!$xml)
            return array('stat' => false, 'code' => YFROG_ERROR_MALFORMED_XML, 'msg' => 'Malformed XML is received as response');

        if (@$xml->attributes()->stat == 'fail')
        {
            return array('stat' => false, 'code' => $xml->err->attributes()->code, 'msg' => $xml->err->attributes()->msg);
        }
        elseif (@$xml->attributes()->stat == 'ok')
        {
            return array
            (
                'stat'      => true,
                'mediaid'   => $xml->mediaid,
                'mediaurl'  => $xml->mediaurl,
                'statusid'  => @$xml->statusid,
                'userid'    => @$xml->userid
            );
        }
        else
            return array('stat' => false, 'code' => YFROG_ERROR_MALFORMED_XML, 'msg' => 'Unexpected XML is received as response');

    }
    

?>