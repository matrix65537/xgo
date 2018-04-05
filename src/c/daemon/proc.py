#! /usr/bin/python
#coding:utf8

def main():
    for i in range(0x400):
        print "cp ./a.out daemon_%04d" %(i)
    for i in range(0x400):
        print "./daemon_%04d %04d.log" %(i, i)

if __name__ == '__main__':
    main()
