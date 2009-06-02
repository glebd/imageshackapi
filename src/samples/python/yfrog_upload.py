#!/usr/bin/env python

import yfrog
import sys

if __name__ == "__main__":
    # Sample 
    if len(sys.argv)!=4:
        print "Usage upload.py <username> <password> <filename>"
        sys.exit(1)
    u = yfrog.YfrogUploader()
    print u.uploadFile(sys.argv[3],sys.argv[1],sys.argv[2])
