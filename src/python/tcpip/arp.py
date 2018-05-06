#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os
import stat
import argparse
from scapy.all import(
    Ether,
    ARP,
    sendp,
    hexdump,
)

def process_cmd(ns):
    eth = Ether()
    arp = ARP(
        op = "is-at",
        hwsrc="12:34:56:78:9A:BC",
        psrc="192.168.12.34",
        )
    print((eth/arp).show())
    hexdump(eth/arp)
    sendp(eth/arp, inter=2, loop=1)

def main():
    parse = argparse.ArgumentParser(prog = "cat", description = "cat stdin and files to output", version = "1.0")
    parse.add_argument("-T", "--show-tabs",  action = "store_true", help = "display TAB characters as ^I")
    parse.add_argument("-E", "--show-ends",  action = "store_true", help = "display $ at end of each line")
    parse.add_argument("-n", "--number",  action = "store_true", help = "number all output lines")
    parse.add_argument("files", nargs = "*")
    ns = parse.parse_args()
    print ns
    process_cmd(ns)

if __name__ == '__main__':
    main()

