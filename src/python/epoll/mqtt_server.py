#!/usr/bin/env python
#coding:utf8

import socket
import errno
import struct
from scplib import *
from tcpserver import *
import time

################################################################################

TAG_SUB         = 0x0001
TAG_SUB_ACK     = 0x0002

TAG_PUB         = 0x0003
TAG_PUB_ACK     = 0x0004

TAG_SUB_DATA    = 0x0005

class DataPackage(object):
    def __init__(self, tag = 0x0000, data = ''):
        self.__tag = tag
        self.__data = data

    def get_tag(self):
        return self.__tag

    def set_tag(self, tag):
        self.__tag = tag

    def get_data(self):
        return self.__data

    def set_data(self, data):
        self.__data = data

    def to_bytes(self):
        return int2bytes(self.__tag, 0x02) + int2bytes(len(self.__data), 0x02) + self.__data

    def to_string(self):
        return s(self.to_bytes())

    def __str__(self):
        return self.to_string()

class CmdContext(object):

    HEAD_LEN = 0x04

    def __init__(self, max_data_length = (1 << 20)):
        self.__max_data_length = max_data_length
        self.reset()

    def reset(self):
        self.__head = ''
        self.__data = ''

        self.__head_recved = False
        self.__data_recved = False


    def get_tag(self):
        return bytes2int(self.__head[0:2])

    def get_length(self):
        return bytes2int(self.__head[2:4])

    def get_data(self):
        return self.__data

    def push_data(self, data):
        length = len(data)
        if len(self.__data) + length > self.__max_data_length:
            return False
        i = 0
        while True:
            if not self.__head_recved:
                if len(self.__head) + (length - i) > CmdContext.HEAD_LEN:
                    copy_len = CmdContext.HEAD_LEN - len(self.__head)
                else:
                    copy_len = length
                self.__head += data[i:i+copy_len]
                i += copy_len
                if (len(self.__head) == CmdContext.HEAD_LEN):
                    self.__head_recved = True
                else:
                    break

            if self.get_length() > self.__max_data_length:
                return False

            if not self.__data_recved:
                want_eat_len = self.get_length() - len(self.__data)
                remain_len = length - i
                eat_len = remain_len if remain_len <= want_eat_len else want_eat_len
                self.__data += data[i : i+eat_len]
                i += eat_len
                if len(self.__data) == self.get_length():
                    self.__data_recved = True
                else:
                    break
            break
        return True

    def is_finished(self):
        return self.__head_recved and self.__data_recved

class RspContext(object):

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
        self._clients = {}
        self._topics = {}

    def on_connect(self, sock, fd):
        log.info("================> Connect")
        cmd_context = CmdContext()
        rsp_context = RspContext()
        self._clients[fd] = (sock, cmd_context, rsp_context)

    def process_sub(self, sock, fd, cmd_context):
        topic = cmd_context.get_data()
        log.info("SUB Topic: %s" %(topic))
        if topic in self._topics:
            self._topics[topic].append((sock, fd))
        else:
            self._topics[topic] = [(sock, fd)]

    def process_pub(self, sock, fd, cmd_context):
        offset = 0
        data = cmd_context.get_data()
        topic_len, = struct.unpack(">H", data[offset:offset + 2])
        offset += 2
        topic = data[offset:offset + topic_len]
        offset += topic_len
        log.info("PUB Topic: %s" %(topic))

        data_len = struct.unpack(">H", data[offset:offset + 2])
        offset += 2
        pub_data = data[offset:]
        log.info(get_dump_string("pub data", pub_data))

        if topic in self._topics:
            fd_list = self._topics[topic]
            for sock, fd in fd_list:
                data_pkg = DataPackage(TAG_SUB_DATA, pub_data)
                rsp_data = data_pkg.to_bytes()
                _, _, rsp_context = self._clients[fd]
                rsp_context.set_data(rsp_data)
                self.set_sock_write(sock, fd)

    def on_input(self, sock, fd, data):
        log.debug(get_dump_string("====> input", data))
        sock, cmd_context, rsp_context = self._clients[fd]
        cmd_context.push_data(data)
        if cmd_context.is_finished():
            tag = cmd_context.get_tag()
            if tag == TAG_SUB:
                self.process_sub(sock, fd, cmd_context)
                data_pkg = DataPackage(TAG_SUB_ACK)
            elif tag == TAG_PUB:
                self.process_pub(sock, fd, cmd_context)
                data_pkg = DataPackage(TAG_PUB_ACK)
            else:
                raise Exception("tag error")
            #set output data
            rsp_data = data_pkg.to_bytes()
            rsp_context.set_data(rsp_data)
            self.set_sock_write(sock, fd)
            cmd_context.reset()

    def on_output(self, sock, fd):
        csock, cmd_context, rsp_context = self._clients[fd]
        data = rsp_context.get_data()
        data_len = len(data)
        send_len = sock.send(data)
        log.develop(get_dump_string("<==== [%d] output" %(fd), data[:send_len]))
        rsp_context.set_data(data[send_len:])
        if send_len == data_len:
            self.set_sock_read(sock, fd)
            return True
        return False

    def on_close(self, sock, fd):
        log.info("================> close")
        self._clients.pop(fd, None)

if __name__ == '__main__':
    log.set_log_level(6)
    server = ServerA(8000)
    server.start()

