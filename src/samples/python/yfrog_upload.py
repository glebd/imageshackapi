#!/usr/bin/env python

import yfrog
import sys

if __name__ == "__main__":

    if len(sys.argv)!=4:
        print "Usage upload.py <username> <password> <filename/url>"
        sys.exit(1)
    u = yfrog.YfrogUploader()
    if sys.argv[3].startswith("http://"):
        print u.uploadURL(sys.argv[3],sys.argv[1],sys.argv[2])
    else:
        print u.uploadFile(sys.argv[3],sys.argv[1],sys.argv[2])
