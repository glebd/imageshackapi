#!/usr/bin/env python

'''
Upload image file to imageshack and returns XML document
'''

import urllib2_file
import urllib2
import socket

API_URL = 'http://www.imageshack.us/upload_api.php'
HTTP_UPLOAD_TIMEOUT = 300

def yesno(x):
    if x:
        return 'yes'
    else:
        return 'no'

def upload_file(developer_key,
                filename,
                optsize=None,
                user_cookie=None,
                remove_bar=True,
                tags=None,
                public=True,
                dest_file=None,
                content_type=None,
                thumb_filename=None,
                more_params = None
                ):

    fd = open(filename,'rb')
    try:
        data = {'fileupload' : urllib2_file.FileUpload(fd,content_type),
                'key' : developer_key,
                'rembar' : yesno(remove_bar),
                'public' : yesno(public)
                }
        tfd = None
        if thumb_filename!=None:
            tfd = open(thumb_filename,'rb')
        try:
            if tfd!=None:
                data['frmupload'] = urllib2_file.FileUpload(tfd,"image/jpeg")

            if more_params!=None:
                data.update(more_params)
            # Some optional parameters
            if dest_file!=None:
                data['dest_file']=dest_file
            if optsize:
                data['optsize'] = optsize
            if user_cookie:
                data['cookie'] = user_cookie
            if tags:
                data['tags'] = tags

            req = urllib2.Request(API_URL, data, {})
            socket.setdefaulttimeout(HTTP_UPLOAD_TIMEOUT)
            u = urllib2.urlopen(req)
            return u.read()
        finally:
            if tfd!=None:
                tfd.close()
    finally:
        fd.close()

    
    
    
