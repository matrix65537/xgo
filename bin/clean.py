#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import os
import os.path

def main():
    for root, dirs, files in os.walk(sys.argv[1]):
        for filename in files:
            if filename == "a.out" or filename.endswith(".o") or filename.endswith(".a"):
                full_filename = os.path.join(root, filename)
                print full_filename
                os.remove(full_filename)

if __name__ == '__main__':
    main()

