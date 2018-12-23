#!/usr/bin/env python
#coding:utf8

import errno
import socket
import select

_POLL_TIMEOUT = 5.0
_POLL_TIMEOUT = 3600.0

class IOLoop(object):

    _EPOLLIN        = 0x0001
    _EPOLLPRI       = 0x0002
    _EPOLLOUT       = 0x0004
    _EPOLLERR       = 0x0008
    _EPOLLHUP       = 0x0010
    _EPOLLRDHUP     = 0x2000
    _EPOLLONESHOT   = (1 << 30)
    _EPOLLET        = (1 << 31)


    NONE            = 0
    READ            = _EPOLLIN
    WRITE           = _EPOLLOUT
    ERROR           = _EPOLLERR | _EPOLLHUP

    @staticmethod
    def instance():
        if not hasattr(IOLoop, "_instance"):
            if hasattr(select, "epoll"):
                cls = EPollIOLoop
            else:
                cls = SelectIOLoop
            instance = cls()
            instance.initialize()
            IOLoop._instance = instance
        return IOLoop._instance

    def split_fd(self, fd):
        try:
            return fd.fileno(), fd
        except AttributeError:
            return fd, fd

    def close_fd(self, fd):
        try:
            try:
                fd.close()
            except AttributeError:
                os.close(fd)
        except OSError:
            pass


class PollIOLoop(IOLoop):

    def initialize(self, impl):
        self._impl = impl

        self._handlers = {}
        self._events = {}

    def add_handler(self, sock, handler, events):
        fd, obj = self.split_fd(sock)
        self._handlers[fd] = (obj, handler)
        self._impl.register(fd, events | self.ERROR)

    def update_handler(self, sock, events):
        fd, obj = self.split_fd(sock)
        self._impl.modify(fd, events | self.ERROR)

    def remove_handler(self, sock):
        fd, obj = self.split_fd(sock)
        self._handlers.pop(fd, None)
        self._events.pop(fd, None)
        self._impl.unregister(fd)

    def start(self):
        while True:
            print "poll..."
            event_pairs = self._impl.poll(_POLL_TIMEOUT)
            self._events.update(event_pairs)
            while self._events:
                fd, events = self._events.popitem()
                fd_obj, handler_func = self._handlers[fd]
                print "handler function: %d" %(fd)
                handler_func(fd_obj, fd, events)

################################################################################

class EPollIOLoop(PollIOLoop):
    def initialize(self):
        super(EPollIOLoop, self).initialize(impl = select.epoll())

################################################################################

class _Select(object):
    def __init__(self):
        self.read_fds = set()
        self.write_fds = set()
        self.error_fds = set()
        self.fd_sets = (self.read_fds, self.write_fds, self.error_fds)

    def close(self):
        pass

    def register(self, fd, events):
        if fd in self.read_fds or fd in self.write_fds or fd in self.error_fds:
            raise IOError("fd %s already registered" % fd)
        if events & IOLoop.READ:
            self.read_fds.add(fd)
        if events & IOLoop.WRITE:
            self.write_fds.add(fd)
        if events & IOLoop.ERROR:
            self.error_fds.add(fd)

    def modify(self, fd, events):
        self.unregister(fd)
        self.register(fd, events)

    def unregister(self, fd):
        self.read_fds.discard(fd)
        self.write_fds.discard(fd)
        self.error_fds.discard(fd)

    def poll(self, timeout):
        readable, writeable, errors = select.select(self.read_fds, self.write_fds, self.error_fds, timeout)
        events = {}
        for fd in readable:
            events[fd] = events.get(fd, 0) | IOLoop.READ
        for fd in writeable:
            events[fd] = events.get(fd, 0) | IOLoop.WRITE
        for fd in errors:
            events[fd] = events.get(fd, 0) | IOLoop.ERROR
        return events.items()


class SelectIOLoop(PollIOLoop):
    def initialize(self):
        super(SelectIOLoop, self).initialize(impl=_Select())

################################################################################

if __name__ == '__main__':
    pass

