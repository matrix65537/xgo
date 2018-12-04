import sys
import os
import os.path
import socket
import argparse
import ctypes
import scplib
from scplib import *
from scplib.core import log as thelog
from numpy import *

def add(a, b):
    print "add"
    s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s1.bind(("127.0.0.1", 8000))
    s1.listen(10)
    thelog.set_log_level(5)
    while True:
        try:
            print "accept..."
            s2, addr = s1.accept()
            print addr
            while True:
                data = s2.recv(0x01)
                if data == 'q':
                    print "bye"
                    s2.close()
                    continue
                thelog.debug(data)
        except Exception as e:
            print e
            continue

    for name in sys.path:
        print name
    data = b'\x01' * 0x10
    msg = s(data)
    print msg

    thelog.debug(msg)
    thelog.develop(msg)
    thelog.info(msg)
    thelog.info(msg)

    return a + b

def foo(a):
    print "foo"
    return

def update():
    print "update"
