#!/usr/bin/env python

'''
API to upload image and video files to yfrog.omc
'''

import urllib2_file
import urllib2
import socket

from mimetypes import guess_type
from xml.dom.minidom import parseString

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


class YfrogUploader:
    
    def __init__(self, timeout=HTTP_UPLOAD_TIMEOUT):
        '''Creates uploader object.
        Args:
        timeout: timeout in seconds for upload operation (optional)
        '''
        self.timeout = timeout

    def upload(self,
               filename,
               twitter_username,
               twitter_password,
               content_type = None,
               tags = None,
               public = True):
        '''Uploads given file.
        
        Args:
        filename: media file name to be uploaded
        twitter_username: password
        twitter_password: username
        content_type: content type of file. (optional)
        tags: comma-separated list of tags (optional)
        public: whenever image is public or not
        
        returns dictionary with with following keys:
        url: url of uploaded image (this is URL for HTML page)
        '''
        if content_type == None:
            (content_type, encoding) = guess_type(filename, False)
            if content_type==None:
                raise YfrogUploadException("Could not guess content/type for input file %s" % filename)

        fd = open(filename,'rb')
        try:
            data = {'media' : urllib2_file.FileUpload(fd, content_type),
                    'public' : self.yesno(public),
                    'username' : username,
                    'password' : password
                    }
            if tags:
                data['tags'] = tags

            req = urllib2.Request(API_URL % "upload", data, {})
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
        ca = err[0].attributes.get('msg')
        if ma==None:
            raise YfrogUploadException("Cound not decode server XML response (no msg attriubute)")
        raise YfrogServerException(int(ca.value),ma.value)


    def __parseOKResponse(self,d):
        mu = d.getElementsByTagName('mediaurl')
        if mu==None or len(mu)!=1:
            raise YfrogUploadException("Cound not decode server XML response (no mediaurl element)")
        url = self._getText(mu[0])
        return {'url':url}
        
    def _parseResponse(self, xmlres):
        dom = parseString(xmlres)
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
            dom.unlink()
            
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
    
    
    
