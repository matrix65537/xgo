#!/usr/bin/env python
#codign:utf-8

import sys
import os

def main():
    os.execl("/bin/ls", "ls", "-l")

if __name__ == '__main__':
    main()

