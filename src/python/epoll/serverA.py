#!/usr/bin/env python
#coding:utf8

import socket
import errno
from scplib import *
from tcpserver import *
import time

################################################################################

class DataNode(object):

    def __init__(self):
        self._data = ''

    def get_data(self):
        return self._data

    def set_data(self, data):
        self._data = data

    def append(self, data):
        self._data += data

class ServerA(TCPServer):
    def __init__(self, port):
        super(ServerA, self).__init__(port)
        self._data_nodes = {}

    def on_connect(self, sock, fd):
        log.info("================> Connect")
        data_node = DataNode()
        self._data_nodes[fd] = (sock, data_node)

    def on_input(self, sock, fd, data):
        log.debug(get_dump_string("====> input", data))

        #output for each sock
        fd_list = self._data_nodes.keys()
        fd_list.sort()
        for fd in fd_list:
            sock, node = self._data_nodes[fd]
            node.append(data)
            self.set_sock_read_write(sock, fd)

    def on_output(self, sock, fd):
        csock, node = self._data_nodes[fd]
        data = node.get_data()
        data_len = len(data)
        send_len = sock.send(data)
        log.develop(get_dump_string("<==== [%d] output" %(fd), data[:send_len]))
        node.set_data(data[send_len:])
        if send_len == data_len:
            self.set_sock_read(sock, fd)
            return True
        return False

    def on_close(self, sock, fd):
        log.info("================> close")
        self._data_nodes.pop(fd, None)

if __name__ == '__main__':
    log.set_log_level(6)
    server = ServerA(8000)
    server.start()

