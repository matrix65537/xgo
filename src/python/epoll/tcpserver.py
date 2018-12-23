#!/usr/bin/env python
#coding:utf8

import socket
import errno
import functools
from ioloop import IOLoop
from scplib import *

################################################################################

class TCPServer(object):
    def __init__(self, port = 9999):
        self.__port = port
        self.__instance = IOLoop.instance()
        self.init()

    def init(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.setblocking(0)
        sock.bind(("", self.__port))
        sock.listen(128)
        info("listening on port: [%d]" %(self.__port))
        self.__instance.add_handler(sock, self.callback_connect, IOLoop.READ)

    ################################################################################

    def on_connect(self, sock, fd):
        pass

    def on_input(self, sock, fd, data):
        pass

    def on_output(self, sock, fd):
        pass

    def on_close(self, sock, fd):
        pass

    ################################################################################

    def set_sock_read(self, sock, fd):
        self.__instance.update_handler(fd, IOLoop.READ)

    def set_sock_write(self, sock, fd):
        self.__instance.update_handler(fd, IOLoop.WRITE)

    def set_sock_read_write(self, sock, fd):
        self.__instance.update_handler(fd, IOLoop.READ | IOLoop.WRITE)
    ################################################################################

    def release_sock(self, sock, fd):
        info("close sock [%d]" %(fd))
        self.__instance.remove_handler(fd)
        sock.close()
        self.on_close(sock, fd)

    def callback_connect(self, sock, fd, events):
        while True:
            try:
                connection, address = sock.accept()
            except socket.error, e:
                if e.args[0] in (errno.EWOULDBLOCK, errno.EAGAIN):
                    break
                else:
                    raise
            info("new connection comming: [%s][%d]" %(str(address), connection.fileno()))
            connection.setblocking(0)
            self.__instance.add_handler(connection, self.callback_read_write, IOLoop.READ)
            self.on_connect(connection, connection.fileno())

    def callback_read_write(self, sock, fd, events):
        if events & IOLoop.READ:
            counter = 0
            while True:
                try:
                    data = sock.recv(0x1000)
                    counter += 1
                    info("[%d] recv data, counter = %d" %(fd, counter))
                    if counter == 0x08:
                        pass
                        #break
                    if len(data) == 0:
                        self.release_sock(sock, fd)
                        break
                    else:
                        info("sock [%d] recv data, len = [%04d]" %(fd, len(data)))
                        #debug(ds(data, True, True))
                        self.on_input(sock, fd, data)
                except socket.error, e:
                    if e.args[0] in (errno.EWOULDBLOCK, errno.EAGAIN):
                        break
                    else:
                        log.error(str(e))
                        self.release_sock(sock, fd)
                        break
        elif events & IOLoop.WRITE:
            while True:
                try:
                    finished = self.on_output(sock, fd)
                    if finished:
                        break
                except socket.error, e:
                    if e.args[0] in (errno.EWOULDBLOCK, errno.EAGAIN):
                        break
                    else:
                        self.release_sock(sock, fd)
        else:
            assert(False)

    def start(self):
        self.__instance.start()

if __name__ == '__main__':
    log.set_log_level(5)
    server = TCPServer()
    server.start()
