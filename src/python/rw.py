#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os

stdin = sys.stdin.fileno()
stdout = sys.stdout.fileno()
stderr = sys.stderr.fileno()

def open_fd(max_count):
    L = []
    try:
        for i in xrange(max_count):
            newfd = os.dup(stdout)
            L.append(newfd)
    except Exception:
        return L


def main():
    max_count = 4096
    fd_list = open_fd(max_count)
    print len(fd_list)
    print fd_list

if __name__ == '__main__':
    main()

