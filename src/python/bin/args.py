#!/usr/bin/env python
#codign:utf-8

import sys
import os

def main():
    print sys.argv
    data = sys.stdin.read()
    length = len(data)
    if length > 0:
        print "length = %d" %(length)

if __name__ == '__main__':
    main()

