#!/usr/bin/env python

import yfrog
import sys

if __name__ == "__main__":

    if len(sys.argv)<4:
        print "Usage upload.py <username> <password> <filename/url> [<text>]"
        sys.exit(1)
    u = yfrog.Uploader()
    if len(sys.argv)==5:
        msg = sys.argv[4]
    else:
        msg = None

    try:
        if sys.argv[3].startswith("http://"):
            print u.uploadURL(sys.argv[3],sys.argv[1],sys.argv[2],message=msg)
        else:
            print u.uploadFile(sys.argv[3],sys.argv[1],sys.argv[2],message=msg)
    except yfrog.ServerException, e:
        print str(e)
