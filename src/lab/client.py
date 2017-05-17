#! /usr/bin/python

import socket
import random
import time

def b2s(bytes):
    s = ' '.join(["%02X" %ord(x) for x in bytes])
    return s

ip = '127.0.0.1'
port = 9999

COUNT = 0x100000
SOCK_COUNT = 1000

def main():
    try:
        s_list = []
        for i in range(SOCK_COUNT):
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0);
            s.connect((ip, port))
            s_list.append(s)

        for i in range(COUNT):
            for s in s_list:
                s.send(b"ABCD\n")
                r = s.recv(0x80)
                #print r
                #time.sleep(1)
    except Exception as e:
        print e

    time.sleep(100)

main()
