#! /usr/bin/python
#coding: utf-8

'''
1. 直接下来
2. 如果想把第2个弄下来， 则先把第一个上去， 然后第2个下来，第一个在下来
3. 最邻近的一个在上边，其他都在下面

x. 如果想把第n个弄下来, 要保证n-1在上面， 其它全在下面
'''

'''
当前初始状态:
第n个在上面，其它n-1全在下面， 如何把n个全弄下去
'''

counter = 0

'''
0: 上
1: 下
'''
states = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

# <=n 的环全弄下去
def D(n):
    global counter
    if n == 1:
        if states[n] == 0:
            counter += 1
            states[n] = 1
            print u"% 3d:    1下" %(counter)
        return
    elif n == 0:
        return
    else:
        D(n - 2)
        U(n - 1)
        if states[n] == 0:
            counter += 1
            states[n] = 1
            print u"% 3d:    %d下" %(counter, n)
            D(n)
        print "%d OK ================" %(n)

# 仅把第n个环弄上去
def U(n):
    global counter
    if n == 1:
        if states[n] == 1:
            counter += 1
            states[n] = 0
            print u"% 3d:    1上" %(counter)
        return
    elif n == 0:
        return
    else:
        D(n - 2)
        U(n - 1)
        if states[n] == 1:
            counter += 1
            states[n] = 0
            print u"% 3d:    %d上" %(counter, n)
            U(n - 1)

def main():
    D(5)

if __name__ == '__main__':
    main()

