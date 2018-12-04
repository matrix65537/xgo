#!/usr/bin/env python

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
    thelog.set_log_level(5)

    s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    for name in sys.path:
        thelog.develop(name)


    thelog.debug("before LoadLibrary")
    lib = ctypes.cdll.LoadLibrary("./libc1.so")
    thelog.debug("after LoadLibrary")

    thelog.debug("before native call")
    lib.c1_f1()
    lib.c1_f2()
    thelog.debug("after native call")

    return a + b

def foo(a):
    print "foo"
    return

def update():
    print "update"

if __name__ == '__main__':
    add(1, 2)
    foo(1)
    update()
