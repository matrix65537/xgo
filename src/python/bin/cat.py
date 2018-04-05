#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os
import argparse

stdin = sys.stdin.fileno()
stdout = sys.stdout.fileno()
stderr = sys.stderr.fileno()

def process_cmd(ns):
    files = ns.files
    fd_list = []
    if len(files) == 0:
        fd_list.append(stdin)
    else:
        for filename in files:
            if filename == '-':
                fd_list.append(stdin)
            else:
                fobj = open(filename, "rb")
                fd_list.append(fobj.fileno())

    for fd in fd_list:
        num = 1
        write_last_flag = False
        while True:
            data = os.read(fd, 0x1000)
            if(len(data) == 0):
                break

            if write_last_flag:
                os.write(stdout, "%6d  " %(num))
                num += 1
                write_last_flag = False

            if ns.number:
                os.write(stdout, "%6d  " %(num))
                num += 1
                length = len(data)
                for i in range(length):
                    x = data[i]
                    if x == '\n':
                        if ns.show_ends:
                            os.write(stdout, '$')
                        os.write(stdout, x)
                        if i < length - 1:
                            os.write(stdout, "%6d  " %(num))
                            num += 1
                        else:
                            write_last_flag = True
                    else:
                        os.write(stdout, x)
            else:
                os.write(stdout, data)

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

