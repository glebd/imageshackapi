#!/usr/bin/env python

'''
Upload image file to imageshack and returns XML document
(simple version for image/PDF files, no video support)
'''

import urllib2_file
import urllib2

API_URL = 'http://www.imageshack.us/upload_api.php'

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
                ):

    data = {'fileupload' : open(filename,'rb'),
            'key' : developer_key,
            'rembar' : yesno(remove_bar),
            'public' : yesno(public)
            }

    # Some optional parameters
    if optsize:
        data['optsize'] = optsize
    if user_cookie:
        data['cookie'] = user_cookie
    if tags:
        data['tags'] = tags
    
    req = urllib2.Request(API_URL, data, {})
    u = urllib2.urlopen(req)
    return u.read()
    


    
    
    
