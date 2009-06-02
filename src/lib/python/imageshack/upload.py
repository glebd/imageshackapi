#!/usr/bin/env python

'''
client API library to upload images and videos to imageshack.us
'''

import urllib2_file
import urllib2
import socket

from mimetypes import guess_type
from xml.dom.minidom import parseString
from os.path import exists

IMAGE_API_URL = 'http://www.imageshack.us/upload_api.php'
VIDEO_API_URL = 'http://render.imageshack.us/upload_api.php'
HTTP_UPLOAD_TIMEOUT = 300

class UploadException(Exception):
    ''' Exceptions of this class are raised for various upload based errors '''
    pass

class ServerException(Exception):
    ''' Exceptions of this class are raised for upload errors reported by server '''
    
    def __init__(self, code, message):
        self.code = code
        self.message = message

    def __str__(self):
        return "ServerException:%d:%s" % (self.code, self.message)

class ImageshackUploader:
    
    def __init__(self, dev_key, timeout=HTTP_UPLOAD_TIMEOUT):
        '''Creates uploader object.
        Args:
        dev_key: developer key
        timeout: timeout in seconds for upload operation (optional)
        '''
        self.cookie = None
        self.username = None
        self.password = None
        self.dev_key = dev_key
        self.timeout = timeout

    def __init__(self, dev_key, cookie, timeout=HTTP_UPLOAD_TIMEOUT):
        '''Creates uploader object.
        Args:
        cookie: cookie
        dev_key: developer key
        timeout: timeout in seconds for upload operation (optional)
        '''
        self.cookie = cookie
        self.username = None
        self.password = None
        self.dev_key = dev_key
        self.timeout = timeout

    def __init__(self, dev_key, username, password, timeout=HTTP_UPLOAD_TIMEOUT):
        '''Creates uploader object.
        Args:
        cookie: cookie
        dev_key: developer key
        timeout: timeout in seconds for upload operation (optional)
        '''
        self.cookie = None
        self.username = username
        self.password = password
        self.dev_key = dev_key
        self.timeout = timeout


    def uploadFile(self,
                   filename,
                   optsize = None,
                   remove_bar = True,
                   tags = None,
                   public = True,
                   content_type = None,
                   frame_filename = None):
        ''' upload image or video file

        Args:
        optizie: optional reisizing parameter in format of (widh, height) tuple

        '''
        

        if not exists(filename):
            raise UploadException("File %s does not exist" % filename)

        if frame_filename and not exists(frame_filename):
            raise UploadException("File %s does not exist" % frame_filename)
            
        if content_type == None:
            (content_type, encoding) = guess_type(filename, False)
            if content_type==None:
                raise UploadException("Could not guess content/type for input file %s" % filename)

        if content_type.lower().startswith("image/"):
            u = IMAGE_API_URL
            is_video=False
        elif content_type.lower().startswith("video/"):
            u = VIDEO_API_URL
            is_video=True
        else:
            raise UploadException("Unsupported content type %s", % content_type)

        # some sanity checks
        if is_video:
            if optsize:
                raise UploadException("Resizing is not supported for video files")
        else:
            if frame_filename:
                raise UploadException("Could not specify frame for image files")

        fd = open(filename,'rb')
        try:
            data = {'fileupload' : urllib2_file.FileUpload(fd,content_type),
                    'key' : self.dev_key,
                    'rembar' : self._yesno(remove_bar),
                    'public' : self._yesno(public)
                    }
            tfd = None
            if frame_filename!=None:
                tfd = open(frame_filename,'rb')
            try:
                if tfd!=None:
                    data['frmupload'] = urllib2_file.FileUpload(tfd,"image/jpeg")

                # Some optional parameters
                if optsize:
                    data['optimage'] = '1'
                    data['optsize'] = "%dx%d" % optsize
                if self.cookie:
                    data['cookie'] = self.cookie
                if tags:
                    data['tags'] = tags

                req = urllib2.Request(u, data, {})
                socket.setdefaulttimeout(HTTP_UPLOAD_TIMEOUT)
                u = urllib2.urlopen(req)
                u.read()
                return u.read()
            finally:
                if tfd!=None:
                    tfd.close()
        finally:
            fd.close()

    def _yesno(self, x):
        if x:
            return 'yes'
        else:
            return 'no'

