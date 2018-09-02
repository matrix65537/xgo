#! /usr/bin/python

import socket
import random
import time
from scplib import *

def b2s(bytes):
    s = ' '.join(["%02X" %ord(x) for x in bytes])
    return s

ip = '127.0.0.1'
ip = '47.94.106.99'
port = 8080

COUNT = 1
SOCK_COUNT = 10

def recvall(sock, length):
    data = ''
    while len(data) < length:
        r = sock.recv(length - len(data))
        print s(r)
        data += r
    return data

def main():
    s_list = []
    for i in range(SOCK_COUNT):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0);
        sock.connect((ip, port))
        s_list.append(sock)

    data =  b("010203040506") * 10
    for i in range(COUNT):
        for sock in s_list:
            print "<< ", s(data)
            sock.send(data)
            r = recvall(sock, len(data))
            print ">> ", s(r)
            #print r
            #time.sleep(1)

main()
