#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os
import stat
import argparse

def process_cmd(ns):
    ip = ns.ip
    ip = ip.strip()
    L = ip.split(".")
    if len(L) == 4:
        L2 = ["%02X" %int(x) for x in L]
        print ip, " => ", " ".join(L2)
    else:
        v = int(ip, 0)
        L = []
        for i in range(4):
            x = v & 0xFF
            v >>= 8
            L.append(str(x))
        L.reverse()
        print ip, " => ", ".".join(L)

def main():
    parse = argparse.ArgumentParser(prog = "ip", description = "convert ip addr", version = "1.0")
    parse.add_argument("ip")
    ns = parse.parse_args()
    process_cmd(ns)

if __name__ == '__main__':
    main()

