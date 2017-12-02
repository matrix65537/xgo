#!/usr/bin/env python
#codign:utf-8

import sys
import os

def main():
    while True:
        try:
            cmd = raw_input("% ")
            cmd = cmd.strip().lower()
            print cmd
            if cmd == 'q':
                print "Bye!"
                sys.exit()
            argv = cmd.split()
            #print argv
            if len(argv) == 0:
                print "command error."
                continue
            pid = os.fork()
            if pid == 0:
                try:
                    os.execvp(argv[0], argv)
                except Exception as e:
                    print str(e)
            else:
                os.wait()
        except KeyboardInterrupt:
            pass

if __name__ == '__main__':
    main()

