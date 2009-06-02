<?php
    
    require_once('multipost.php');

    class ImageShackUploader
    {
        /**
         * API endpoints
         */
        const IMAGESHACK_API_URL       = 'http://imageshack.us/upload_api.php';
        const IMAGESHACK_VIDEO_API_URL = 'http://render.imageshack.us/upload_api.php';

        /**
         * Default connection and response timeout
         */
        const IMAGESHACK_API_TIMEOUT = 10;

        protected $params = array();
        protected $timeout = self::IMAGESHACK_API_TIMEOUT;

        /**
         * Constructs new uploader using specified developer key
         */
        function __construct($developer_key)
        {
            $this->setDeveloperKey($developer_key);
        }

        /**
         * Sets developer key to use
         * @param string developer_key key to use
         */
        function setDeveloperKey($developer_key)
        {
            $this->addParameter('key', $developer_key);
        }

        /**
         * Sets user cookies. By default there are no cookies passed
         * @param string cookie ImageShack cookies
         */
        function setCookie($cookie)
        {
            $this->addParameter('cookie', $cookie);
        }

        /**
         * Sets if image should be optimized or resized. By default no image transformation is performed
         * @param string optsize format: WIDTHxHEIGHT or 'resample'
         */
        function setOptSize($optsize)
        {
            if ($optsize)
            {
                $this->addParameter('optimage', 1);
                $this->addParameter('optsize', $optsize);
            }
            else
            {
                $this->removeParameter('optimage');
                $this->removeParameter('optsize');
            }
        }

        /**
         * Sets if image thumbnail should have no information bar. By default thumbnail generated contains information bar
         * @param boolean removeBar true to remove information bar from thumbnail. 
         */
        function setRemoveBar($removeBar)
        {
            $this->addParameter('rembar', $removeBar ? 'yes' : 'no');
        }

        /**
         * Sets if image should be public. By default all images are public if user defined this in preferences. 
         * Have no impact if user's cookies are not set.
         * @param boolean public true to make image public, false otherwise
         * @see setCookie()
         */
        function setPublic($public)
        {
            $this->addParameter('public', $public ? 'yes' : 'no');
        }

        /**
         * Adds tags to image upload/transload. 
         * @param string tags tags - comma separated string
         */
        function setTags($tags)
        {
            $this->addParameter('tags', $tags);
        }

        /**
         * Sets API timeout
         * @param int timeout API timeout (seconds)
         */
        function setTimeout($timeout)
        {
            $this->timeout = $timeout;
        }

        /**
         * Defines expected destination file name for uploaded file.
         * Note: It's not  mandatory that resulting file will have exactly the same name
         * @param string file destination filename, e.g. destination.jpg
         */
        function setDestFile($file)
        {
            $this->addParameter('dest_file', $file);
        }

        /**
         * Sets IP address from what request was made. By default IP address of caller is used
         * @param string ip remote IP
         */
        function setIp($ip)
        {
            $this->addParameter('remote_ip', $ip);
        }

        /**
         * If this flag is set to true, resulting files will have short names, yfrog-style
         * @param boolean 
         */
        function setMangleFileName($mangle)
        {
            $this->addParameter('mangle_filename', $mangle ? 1 : 0);
        }

        /**
         * Adds new parameter
         * @param name parameter name
         * @param value parameter value
         */
        function addParameter($name, $value)
        {
            $this->params[$name] = $value;
        }

        /**
         * Removes parameter with specified name
         * @param name parameter name
         */
        function removeParameter($name)
        {
            unset($this->params[$name]);
        }

        /**
         * Uploads specified file
         * @param file file to upload
         * @param contentType file content type
         */
        function upload($file, $contentType = null)
        {
            if (!$contentType)
                $contentType = $this->detectFileContentType($file);
            if (strpos($contentType, 'image/') === 0)
                $endpoint = self::IMAGESHACK_API_URL;
            else
                $endpoint = self::IMAGESHACK_VIDEO_API_URL;
            $params = $this->makeParams();
            array_unshift($params, new filepart('fileupload', $file, basename($file), $contentType, 'iso-8859-1'));
            return $this->exec($endpoint, $params);
        }

        /**
         * Transloads URL
         * @param url URL to transload
         */
        function transload($url)
        {
            $contentType = $this->detectUrlContentType($url);
            if (!$contentType)
                return false;
            if (strpos($contentType, 'image/') === 0)
                $endpoint = self::IMAGESHACK_API_URL;
            else
                $endpoint = self::IMAGESHACK_VIDEO_API_URL;
            $params = $this->makeParams();
            $params[] = new stringpart('url', $url);
            return $this->exec($endpoint, $params);
        }

        private function detectFileContentType($name)
        {
            $ext = strtolower(substr($name, strrpos($name, '.') + 1));
            switch ($ext)
            {
                case 'jpg':
                case 'jpeg':
                    return 'image/jpeg';
                case 'png':
                case 'bmp':
                case 'gif':
                    return 'image/' . $ext;
                case 'tif':
                case 'tiff':
                    return 'image/tiff';
                case 'mp4':
                    return 'video/mp4';
                case 'mov':
                    return 'video/quicktime';
                case '3gp':
                    return 'video/3gpp';
                case 'avi':
                    return 'video/avi';
                case 'wmv':
                    return 'video/x-ms-wmv';
                case 'mkv':
                    return 'video/x-matroska';

            }
            return 'application/octet-stream';
        }

        private function makeParams()
        {
            $ret = array();
            foreach ($this->params as $name => $value)
            {
                $ret[] = new stringpart($name, $value);
            }
            return $ret;
        }

        private function detectUrlContentType($url)
        {
            $ch = @curl_init($url);
            
            if ($ch) 
            {
                curl_setopt($ch, CURLOPT_HEADER, 1);
                curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
                curl_setopt($ch, CURLOPT_TIMEOUT, 10);
                curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
                curl_setopt($ch, CURLOPT_NOBODY, 1);
                curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
                curl_setopt($ch, CURLOPT_USERAGENT, 'ImageShack Image Fetcher 1.1');
                
                $response = curl_exec($ch);

                $is_err = curl_errno($ch);
                if ($is_err)
                {             
                    @curl_close($ch);
                    return false;
                }
                $code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
                if ($code != 200)
                {
                    @curl_close($ch);
                    return false;
                }
                $ret = curl_getinfo($ch, CURLINFO_CONTENT_TYPE);
                @curl_close($ch);
                return $ret;
            }
            else
                return false;
        }

        private function exec($endpoint,
                              $request)
        {
            $response = multipost($endpoint, 
                                  $request, 
                                  $errno, 
                                  $errormessage,
                                  $this->timeout);

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
        

    }
?>