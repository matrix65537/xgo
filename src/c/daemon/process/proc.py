#! /usr/bin/python
#coding:utf8

def main():
    for i in range(0x2000):
        print "cp ./a.out hello_%04d" %(i)
    for i in range(0x2000):
        print "./hello_%04d hello_%04d.log" %(i, i)

if __name__ == '__main__':
    main()
