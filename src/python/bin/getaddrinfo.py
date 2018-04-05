#!/usr/bin/env python
#-*- coding:utf-8 -*-

import sys
import os
import argparse
import socket

def process_cmd(ns):
    ip, port = ns.ip, ns.port
    res_list = socket.getaddrinfo(ip, port)
    for i in range(len(res_list)):
        print "%02d: " %i, res_list[i]

def main():
    parse = argparse.ArgumentParser(prog = "getaddrinfo", description = "get address information", version = "1.0")
    parse.add_argument("--ip", default = "localhost", help = "set IP address")
    parse.add_argument("--port", type = int, default = 8080, help = "set IP address")
    ns = parse.parse_args()
    process_cmd(ns)

if __name__ == '__main__':
    main()

