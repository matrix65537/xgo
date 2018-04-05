#! /usr/bin/python
#coding: utf-8

import socket

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

if __name__ == '__main__':
    main()
