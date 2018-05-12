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

STATE_UP            = 0x00
STATE_DOWN          = 0x01
STATE_XOR_MASK      = 0x01

INIT_STATE          = STATE_UP

states = [INIT_STATE] * 10
L = []

def check_all(n, state):
    '''
    检查是否已经全部处于正确状态
    '''
    for i in range(1, n + 1):
        if states[i] != state:
            return False
    return True

def GO(n, state):
    '''
    使得前n个环全部处于state状态
    '''
    if check_all(n, state):
        return
    global counter
    if n == 1:
        #状态不同，说明要转换
        if states[n] != state:
            counter += 1
            states[n] ^= STATE_XOR_MASK
            L.append((n, state))
            #print u"% 3d:    %d => %d" %(counter, n, state)
        return
    elif n == 0:
        return
    else:
        #n - 1个全部上去
        GO(n - 1, STATE_UP)
        #n - 2个全部下去
        GO(n - 2, STATE_DOWN)
        # 调整第n个状态
        if states[n] != state:
            counter += 1
            states[n] ^= STATE_XOR_MASK
            L.append((n, state))
            #print u"% 3d:    %d => %d" %(counter, n, state)
        GO(n - 2, STATE_UP)
        if state == STATE_DOWN:
            GO(n - 1, STATE_DOWN)
        #print "%d OK ================" %(n)

def dump(L):
    for i in range(len(L)):
        n, state = L[i]
        info = u"上" if state == STATE_UP else u"下"
        print "step%03d: %d %s" %(i + 1, n, info)

def main():
    global L
    n = 9
    L = []
    GO(n, INIT_STATE ^ STATE_XOR_MASK)
    print "[%d, %3d]" %(n, len(L))
    dump(L)
    #print '-' * 80

if __name__ == '__main__':
    main()

