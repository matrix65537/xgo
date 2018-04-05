#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os
import argparse

stdin = sys.stdin.fileno()
stdout = sys.stdout.fileno()
stderr = sys.stderr.fileno()

def main():
    while True:
        data = os.read(stdin, 0x1000)
        if(len(data) == 0):
            break
        os.write(stdout, data)

    parse = argparse.ArgumentParser(prog = "cat", description = "cat stdin and files to output", version = "1.0")
    parse.add_argument("files", nargs = "*")

if __name__ == '__main__':
    main()

