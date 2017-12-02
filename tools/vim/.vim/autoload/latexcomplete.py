#!/usr/bin/env python
# -*- coding:cp936 -*-
ur'''
@brief 
@file latexcomplete.py
@author Hulei
@version 1.0
@date 2013-12-07
@copyright (C) 2013 Feitian Technologies Co., Ltd. All rights reserved.
'''
# Changes:
#   ** version 1.0 2013-12-07 Hulei **
#       1. first version

import vim
import re

import ctypes

def debug(msg):
    ctypes.windll.kernel32.OutputDebugStringA(msg)

command_dict = {}
environ_dict = {}

def latex_complete():
    base = vim.eval("g:latex_base_text")
    vim.command("let g:latex_complete_dict = []")
    if base.startswith("begin{") or base.startswith("end{"):
        contents = "\n".join(vim.current.buffer)
        envs = re.findall(r"\\begin\{(\w+)\}", contents)
        front = base[:base.index("{") + 1]
        base = base[len(front):]
        for k, v in environ_dict.iteritems():
            if k.startswith(base):
                if isinstance(v, str):
                    cmd = r"""call add(g:latex_complete_dict, {'word':'%s', 'abbr':'%s', 'menu':'%s'})""" % (front + k + "}", k, v)
                else:
                    cmd = r"""call add(g:latex_complete_dict, {'word':'%s', 'abbr':'%s', 'menu':'%s', 'info':'%s'})""" % (front + k + "}", k, v[0], v[1])
                vim.command(cmd)
        keys = environ_dict.keys()
        for env in envs:
            if env not in keys and env.startswith(base):
                cmd = r"""call add(g:latex_complete_dict, {'word':'%s', 'abbr':'%s'})""" % (front + env + "}", env)
                vim.command(cmd)
    else:
        contents = "\n".join(vim.current.buffer)
        cmds = re.findall(r"\\([a-zA-Z]+)", contents)
        for k, v in command_dict.iteritems():
            if k.startswith(base):
                if isinstance(v, str):
                    cmd = r"""call add(g:latex_complete_dict, {'word':'%s', 'menu':'%s'})""" % (k, v)
                else:
                    cmd = r"""call add(g:latex_complete_dict, {'word':'%s', 'menu':'%s', 'info':'%s'})""" % (k, v[0], v[1])
                vim.command(cmd)
        keys = command_dict.keys()
        for cmd in cmds:
            if cmd not in keys and cmd.startswith(base):
                vim.command(r"""call add(g:latex_complete_dict, '%s')""" % cmd)

def latex_init():
    global command_dict, environ_dict
    basedir = vim.eval("$vimfiles") + "/bundle/latex_complete/autoload"
    cmdfile = basedir + "/command"
    envfile = basedir + "/environ"
    glb = {}
    try:
        command_dict = eval(open(cmdfile).read())
        environ_dict = eval(open(envfile).read())
    except Exception:
        pass

latex_init()
del latex_init
