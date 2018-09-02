#!/usr/bin/env python
# coding:utf-8
import os,sys,time

def daemon_init(stdin='/dev/null',stdout='/dev/null',stderr='/dev/null'):
    sys.stdin = open(stdin,'r')
    sys.stdout = open(stdout,'a+')
    sys.stderr = open(stderr,'a+')
    try:
        pid = os.fork()
        if pid > 0:        #parrent
            os._exit(0)
    except OSError,e:
        sys.stderr.write("first fork failed!!"+e.strerror)
        os._exit(1)

    # 子进程， 由于父进程已经退出，所以子进程变为孤儿进程，由init收养
    os.setsid()
    os.chdir("/")
    os.umask(0)

    try:
        pid = os.fork()     #第二次进行fork,为了防止会话首进程意外获得控制终端
        if pid > 0:
            os._exit(0)     #父进程退出
    except OSError,e:
        sys.stderr.write("second fork failed!!"+e.strerror)
        os._exit(1)

    sys.stdout.write("Daemon has been created! with pid: %d\n" % os.getpid())
    sys.stdout.flush()  #由于这里我们使用的是标准IO，回顾APUE第五章，这里应该是行缓冲或全缓冲，因此要调用flush，从内存中刷入日志文件。

