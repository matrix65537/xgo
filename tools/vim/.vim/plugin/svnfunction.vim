"coding:utf-8
if has('g:svnfunction_loaded')
    finish
endif
let g:svnfunction_loaded = 1

python << EOF_PYTHON
import os, sys, locale
from subprocess import Popen

def svn_diff_this_file(args = []):
    filepath = vim.eval("expand('%:p')")
    filename = vim.eval("expand('%')")
    if not filename:
        return
    tempdir = os.path.dirname(vim.eval("tempname()"))
    destfile = os.path.join(tempdir, filename)
    iswin = int(vim.eval("g:isWin"))
    if iswin:
        diffbat = destfile + ".bat"
        with open(diffbat, 'w') as f:
            f.write('''@echo off\ncopy %6 "{0}"\ngvim -d "{0}" %7\n'''.format(destfile))
    else:
        diffbat = destfile + ".sh"
        with open(diffbat, 'w') as f:
            f.write('''#!/bin/sh\ncp "$6" '{0}'\ngvim -d '{0}' "$7"\n'''.format(destfile))
        os.system("chmod +x '%s'" % diffbat)
    Popen(["svn", "diff"] + args + ["--diff-cmd", diffbat, filename])


EOF_PYTHON

command SvnDiff      python svn_diff_this_file()
command SvnDiffPrev  python svn_diff_this_file(['-rPREV:BASE'])
