#!/usr/bin/env python

from imageshack import upload
import sys

if __name__ == "__main__":
    # Sample 
    if len(sys.argv)!=3:
        print "Usage upload.py <DEVELOPER KEY> <filename>"
        sys.exit(1)
   
    print upload.upload_file(sys.argv[1], sys.argv[2])
