#!/usr/bin/env python
#coding:utf8

import sys
import socket
import struct
import argparse
import time
from scplib import *

TAG_SUB     = 0x0001
TAG_SUB_ACK = 0x0002

TAG_PUB     = 0x0003
TAG_PUB_ACK = 0x0004

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

def send_all(sock, data):
    while True:
        if data == '':
            break
        print "before send.."
        send_len = sock.send(data)
        print "afeter send len = %d" %(send_len)
        print get_dump_string("send:", data[:send_len])
        data = data[send_len:]

def recv_pkg(sock):
    head = sock.recv(0x04)
    print get_dump_string("recv:", head)
    body_len, = struct.unpack(">H", head[2:4])
    body = b''
    if body_len > 0:
        body = sock.recv(body_len)
        print get_dump_string("recv:", body)
    return head + body

class MqttClient(object):
    def __init__(self, host, port):
        self._host = host
        self._port = port
        self._sock = socket.socket(2, 1, 0)

    def connect(self):
        addr = (self._host, self._port)
        self._sock.connect(addr)

    def disconnect(self):
        self._sock.close()
        pass

    def sub(self, topic):
        data_pkg = DataPackage(TAG_SUB, topic)
        data = data_pkg.to_bytes()
        send_all(self._sock, data)
        while True:
            recv_pkg(self._sock)

    def pub(self, topic, data):
        datas = []
        datas.append(struct.pack(">H", len(topic)))
        datas.append(topic)
        datas.append(struct.pack(">H", len(data)))
        datas.append(data)
        data = b''.join(datas)
        data_pkg = DataPackage(TAG_PUB, data)

        data = data_pkg.to_bytes()
        send_all(self._sock, data)
        recv_pkg(self._sock)

def process_cmd(ns):
    host = "127.0.0.1"
    port = 8000
    mqtt_client = MqttClient(host, port)
    mqtt_client.connect()
    topic = ns.topic
    if ns.sub:
        mqtt_client.sub(topic)
    elif ns.pub:
        data = ns.data
        mqtt_client.pub(topic, data)
    mqtt_client.disconnect()

def main():
    parse = argparse.ArgumentParser(prog = "client", description = "python client", version = "1.0")
    parse.add_argument("-t", "--topic",  action = "store", default = "topic01", help = "set topic")
    parse.add_argument("-s", "--sub",  action = "store_true", help = "set sub type")
    parse.add_argument("-p", "--pub",  action = "store_true", help = "set pub type")
    parse.add_argument("-d", "--data",  action = "store", default = "hello mqtt", help = "set pub data")
    ns = parse.parse_args()
    process_cmd(ns)

if __name__ == '__main__':
    main()

