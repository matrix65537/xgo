#!/usr/bin/env python

for i in range(65536):
    a = unichr(i)
    print i, a.encode('utf8')

