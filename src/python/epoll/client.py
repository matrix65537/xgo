#!/usr/bin/env python
#coding:utf8

import sys
import socket
import argparse
import time

def send_all(sock, data):
    while True:
        if data == '':
            break
        print "before send.."
        send_len = sock.send(data)
        print "afeter send len = %d" %(send_len)
        data = data[send_len:]

def process_cmd(ns):
    addr = ("127.0.0.1", 8000)
    sock_list = []
    data_len = ns.length
    data = b'A' * data_len

    for i in range(ns.count):
        sock = socket.socket(2, 1, 0)
        sock.connect(addr)
        sock_list.append(sock)

    for sock in sock_list:
        send_all(sock, data)

    for sock in sock_list:
        sock.recv(data_len)

    return
    time.sleep(2)
    for sock in sock_list:
        sock.close()

def main():
    parse = argparse.ArgumentParser(prog = "client", description = "python client", version = "1.0")
    parse.add_argument("-l", "--length",  action = "store", type = int, default = 0x10, help = "set data length")
    parse.add_argument("-c", "--count",  action = "store", type = int, default = 0x01, help = "set connection count")
    ns = parse.parse_args()
    process_cmd(ns)

if __name__ == '__main__':
    main()

