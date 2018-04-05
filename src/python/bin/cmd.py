#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os
import stat
import argparse

stdin = sys.stdin.fileno()
stdout = sys.stdout.fileno()
stderr = sys.stderr.fileno()

def process_cmd(ns):
    files = ns.files
    fd_list = []
    if len(files) == 0:
        files.append(".")
    for filename in files:
        info = os.lstat(filename)
        print info
        st_mode = info.st_mode
        if stat.S_ISREG(st_mode):
            msg = "file"
        elif stat.S_ISDIR(st_mode):
            msg = "dir"
        elif stat.S_ISCHR(st_mode):
            msg = "character device"
        elif stat.S_ISBLK(st_mode):
            msg = "block device"
        elif stat.S_ISFIFO(st_mode):
            msg = "fifo"
        elif stat.S_ISLNK(st_mode):
            msg = "link"
        elif stat.S_ISSOCK(st_mode):
            msg = "sock"
        else:
            msg = "unkown"
        print msg

def main():
    parse = argparse.ArgumentParser(prog = "cat", description = "cat stdin and files to output", version = "1.0")
    parse.add_argument("-T", "--show-tabs",  action = "store_true", help = "display TAB characters as ^I")
    parse.add_argument("-E", "--show-ends",  action = "store_true", help = "display $ at end of each line")
    parse.add_argument("-n", "--number",  action = "store_true", help = "number all output lines")
    parse.add_argument("files", nargs = "*")
    ns = parse.parse_args()
    process_cmd(ns)

if __name__ == '__main__':
    main()

