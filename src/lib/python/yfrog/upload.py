#!/usr/bin/env python

'''
client API library to upload images and videos to yfrog.com
'''

import urllib2_file
import urllib2
import socket

from mimetypes import guess_type
from xml.dom.minidom import parseString
from os.path import exists

API_URL = 'http://yfrog.com/api/%s'
HTTP_UPLOAD_TIMEOUT = 300

class YfrogUploadException(Exception):
    ''' Exceptions of this class are raised for various upload based errors '''
    pass

class YfrogServerException(Exception):
    ''' Exceptions of this class are raised for upload errors reported by server '''
    
    def __init__(self, code, message):
        self.code = code
        self.message = message

    def __str__(self):
        return "YfrogServerException:%d:%s" % (self.code, self.message)

class YfrogUploader:
    
    def __init__(self, timeout=HTTP_UPLOAD_TIMEOUT):
        '''Creates uploader object.
        Args:
        timeout: timeout in seconds for upload operation (optional)
        '''
        self.timeout = timeout

    def uploadURL(self,
                  url,
                  twitter_username,
                  twitter_password,
                  message = None,
                  tags = None,
                  public = True):
        '''Uploads local file.
        
        Args:
        url: url of file to be uploaded
        twitter_username: password
        twitter_password: username
        message: Message to post to twitter. The URL of the image or video is automatically added. (optional)
        tags: comma-separated list of tags (optional)
        public: whenever image is public or not
        
        returns dictionary with with following keys:
        url: url of uploaded image (this is URL for HTML page)
        '''
        data = {'url' : url,
                'public' : self._yesno(public),
                'username' : twitter_username,
                'password' : twitter_password
                }
        if tags:
            data['tags'] = tags
        if message:
            data['message'] = message
            apiurl = API_URL % "uploadAndPost"
        else:
            apiurl = API_URL % "upload"
        
        req = urllib2.Request(apiurl, data, {})
        socket.setdefaulttimeout(self.timeout)
        u = urllib2.urlopen(req)
        xmlres = u.read()
        return self._parseResponse(xmlres)
    
    def uploadFile(self,
                   filename,
                   twitter_username,
                   twitter_password,
                   message = None,
                   content_type = None,
                   tags = None,
                   public = True):
        '''Uploads local file.
        
        Args:
        filename: media file name to be uploaded
        twitter_username: password
        twitter_password: username
        message: Message to post to twitter. The URL of the image or video is automatically added. (optional)
        content_type: content type of file. (optional)
        tags: comma-separated list of tags (optional)
        public: whenever image is public or not
        
        returns dictionary with with following keys:
        url: url of uploaded image (this is URL for HTML page)
        '''

        if not exists(filename):
            raise YfrogUploadException("File %s does not exist" % filename)
            
        if content_type == None:
            (content_type, encoding) = guess_type(filename, False)
            if content_type==None:
                raise YfrogUploadException("Could not guess content/type for input file %s" % filename)

        fd = open(filename,'rb')
        try:
            data = {'media' : urllib2_file.FileUpload(fd, content_type),
                    'public' : self._yesno(public),
                    'username' : twitter_username,
                    'password' : twitter_password
                    }
            if tags:
                data['tags'] = tags
            if message:
                data['message'] = message
                apiurl = API_URL % "uploadAndPost"
            else:
                apirul = API_URL % "upload"

            req = urllib2.Request(apiurl, data, {})
            socket.setdefaulttimeout(self.timeout)
            u = urllib2.urlopen(req)
            xmlres = u.read()
            return self._parseResponse(xmlres)
        finally:
            fd.close()


    def _parseErrorResponse(self, d):
        err = d.getElementsByTagName('err')
        if err==None or len(err)!=1:
            raise YfrogUploadException("Cound not decode server XML response (no err element)")
        ca = err[0].attributes.get('code')
        if ca==None:
            raise YfrogUploadException("Cound not decode server XML response (no code attriubute)")
        ma = err[0].attributes.get('msg')
        if ma==None:
            raise YfrogServerException(int(ca.value), None)
        else:
            raise YfrogServerException(int(ca.value),ma.value)


    def _parseOKResponse(self,d):
        mu = d.getElementsByTagName('mediaurl')
        if mu==None or len(mu)!=1:
            raise YfrogUploadException("Cound not decode server XML response (no mediaurl element)")
        url = self._getText(mu[0].childNodes)
        return {'url':url}
        
    def _parseResponse(self, xmlres):
        d = parseString(xmlres)
        try:
            rsp = d.getElementsByTagName('rsp')
            if rsp==None or len(rsp)!=1:
                raise YfrogUploadException("Cound not decode server XML response (no rsp element)")
            sa =rsp[0].attributes.get('stat')
            if sa==None:
                raise YfrogUploadException("Cound not decode server XML response (no stat attriubute)")
            if sa.value=='fail':
                return self._parseErrorResponse(d)
            elif sa.value=='ok':
                return self._parseOKResponse(d)
            else:
                raise YfrogUploadException("Cound not decode server XML response (unrecognized stat attriubute value)")
        finally:
            d.unlink()
            
    def _yesno(self, x):
        if x:
            return 'yes'
        else:
            return 'no'


    def _getText(self, nodelist):
        rc = ""
        for node in nodelist:
            if node.nodeType == node.TEXT_NODE:
                rc = rc + node.data
        return rc
    
    
    
