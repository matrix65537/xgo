#! /usr/bin/python
#coding: utf-8

import sys
import os
import argparse
import socket
from scplib import *

def main():
    s = socket.socket()
    s.bind(("0.0.0.0", 8080))
    s.listen(5)

    print "listen sock name: ", s.getsockname()
    while True:
        print "accept..."
        local_con, local_addr = s.accept()
        print "conn sock name: ", local_con.getsockname()
        print "conn peer name: ", local_con.getpeername()
        local_con.close()

def process_cmd(ns):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("127.0.0.1", ns.port))
    sock.listen(128)
    while True:
        try:
            print "accept..."
            connection, addr = sock.accept()
            print addr
            while True:
                data = connection.recv(0x100)
                if(len(data) == 0):
                    connection.close()
                    break
                else:
                    print ds(data, True, True)
        except Exception as e:
            print e

def main():
    parse = argparse.ArgumentParser(prog = "server", description = "simple tcp server", version = "1.0")
    parse.add_argument("--port", type = int, default = 8080, help = "set port")
    ns = parse.parse_args()
    print ns
    process_cmd(ns)

if __name__ == '__main__':
    main()


if __name__ == '__main__':
    main()
