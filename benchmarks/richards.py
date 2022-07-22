'''Python implementation of Martin Richard's bench program

$Id: bench.py 5 2004-04-23 21:34:54Z phansen $
'''

import sys

# test parameters
test_params = {
    '1': (10000, 23246, 9297),
    '2': (10000 * 2, 46712, 18684),
    '5': (10000 * 5,116391, 46556),
    '10': (10000 * 10, 232625, 93050),
    '100': (10000 * 100, 2326410, 930563),
    }

# globals
tasktab = {}
tasklist = []
tcb = None
taskid = 0
v1 = 0
v2 = 0
qpktcount = 0
holdcount = 0
_tracing = False
layout = 0

# constants
BUFSIZE = 3

PKTBIT  = 1             # packet is waiting
WAITBIT = 2
HOLDBIT = 4             # task is held

S_RUN           = 0
S_RUNPKT        = 1
S_WAIT          = 2
S_WAITPKT       = 3
S_HOLD          = 4
S_HOLDPKT       = 5
S_HOLDWAIT      = 6
S_HOLDWAITPKT   = 7

alphabet = '0ABCDEFGHIJKLMNOPQRSTUVWXYZ'

# task ids
I_IDLE      = 1
I_WORK      = 2
I_HANDLERA  = 3
I_HANDLERB  = 4
I_DEVA      = 5
I_DEVB      = 6

# packet types
K_DEV   = 1000
K_WORK  = 1001



class packet:
    def __init__(self, id, kind):
        self.id = id
        self.kind = kind
        self.a2 = ''
        self.a1 = 0


class task:
    def __init__(self, id=0, pri=0, wkq=None, state=0, fn=None, v1=0, v2=0):
        self.id = id
        self.pri = pri
        self.wkq = wkq
        self.state = state
        self.fn = fn
        self.v1 = v1
        self.v2 = v2

    def __repr__(self):
        return '<task %s at 0x%08X>' % (self.id, id(self))


def createtask(
    id,
    pri=0,
    wkq=None,
    state=0,
    fn=None,
    v1=0,
    v2=0,
    ):
    t = task(id, pri, wkq, state, fn, v1, v2)
    tasktab[id] = t
    t.link = tasklist and tasklist[0] or None
    tasklist.insert(0, t)
    return t


def trace(a):
    global layout

    layout -= 1
    if layout <= 0:
        sys.stdout.write('\n')
        layout = 50

    sys.stdout.write(a)


def schedule():
    global tcb, v1, v2, taskid

    while tcb:
        pkt = None

        if tcb.state == S_WAITPKT:
            #~ if _tracing:
                #~ trace('%s: %s\n' % (tcb, tcb.state))
            if tcb.wkq:
                pkt = tcb.wkq.pop(0)
            if not tcb.wkq:
                tcb.state = S_RUN
            else:
                tcb.state = S_RUNPKT

        if tcb.state in [S_RUN, S_RUNPKT]:
            #~ if _tracing:
                #~ trace('%s: %s\n' % (tcb, tcb.state))
            taskid = tcb.id
            v1 = tcb.v1
            v2 = tcb.v2
            if _tracing:
                trace(str(taskid))
                #~ trace('\n')
                #~ trace(str(tcb))
                #~ trace('\n')
            newtcb = tcb.fn(pkt)
            tcb.v1 = v1
            tcb.v2 = v2
            tcb = newtcb
            continue

        if tcb.state in [S_WAIT, S_HOLD, S_HOLDPKT, S_HOLDWAIT, S_HOLDWAITPKT]:
            #~ if _tracing:
                #~ trace('%s: %s\n' % (tcb, tcb.state))
            tcb = tcb.link

def wait():
    tcb.state |= WAITBIT
    return tcb


def holdself():
    global holdcount
    holdcount += 1
    tcb.state |= HOLDBIT
    return tcb.link


def findtcb(id):
    return tasktab[id]


def release(id):
    task = findtcb(id)
    task.state &= ~HOLDBIT
    if task.pri > tcb.pri:
        return task
    else:
        return tcb


def qpkt(pkt):
    global qpktcount

    t = findtcb(pkt.id)

    qpktcount += 1

    pkt.id = taskid

    if not t.wkq:
        t.wkq = [pkt]
        t.state |= PKTBIT
        if t.pri > tcb.pri:
            return t
    else:
        t.wkq.append(pkt)

    return tcb


def idlefn(pkt=None):
    '''releases other tasks based on pseudo-random sequence and sometimes holds self'''
    global v1, v2
    v2 -= 1
    if v2 == 0:
        return holdself()

    if (v1 & 1) == 0:
        v1 = v1 >> 1
        return release(I_DEVA)
    else:
        v1 = (v1 >> 1) ^ 0xD008
        return release(I_DEVB)


def workfn(pkt):
    global v1, v2

    if not pkt:
        return wait()

    v1 = I_HANDLERA + I_HANDLERB - v1
    pkt.id = v1

    pkt.a1 = 0
    pkt.a2 = ''
    for i in range(4):
        v2 += 1
        if v2 > 26:
            v2 = 1
        pkt.a2 += alphabet[v2]

    return qpkt(pkt)


def handlerfn(pkt):
    if pkt:
        if pkt.kind == K_WORK:
            v1.append(pkt)
        else:
            v2.append(pkt)

    if v1:
        workpkt = v1[0]
        count = workpkt.a1
        if count > BUFSIZE:
            v1.pop(0)
            return qpkt(workpkt)

        if v2:
            devpkt = v2.pop(0)
            devpkt.a1 = workpkt.a2[count]
            workpkt.a1 = count + 1
            return qpkt(devpkt)

    return wait()


def devfn(pkt):
    global v1

    if not pkt:
        if not v1:
            return wait()
        pkt = v1
        v1 = None
        return qpkt(pkt)
    else:
        v1 = pkt
        if _tracing:
            trace(pkt.a1)
        return holdself()


def append(pkt, ptr):
    pkt.link = None
    while ptr.link:
        ptr = ptr.link
    ptr.link = pkt


def main(tracing=False, count='1'):
    global _tracing
    _tracing = tracing

    Count, Qpktcountval, Holdcountval = test_params[count]
    print('bench mark starting, Count=%s\n' % Count)

    createtask(I_IDLE, 0, None, S_RUN, idlefn, 1, Count)

    wkq = [
        packet(0, K_WORK),
        packet(0, K_WORK),
        ]
    createtask(I_WORK, 1000, wkq, S_WAITPKT, workfn, I_HANDLERA, 0)

    wkq = [
        packet(I_DEVA, K_DEV),
        packet(I_DEVA, K_DEV),
        packet(I_DEVA, K_DEV),
        ]
    createtask(I_HANDLERA, 2000, wkq, S_WAITPKT, handlerfn, [], [])

    wkq = [
        packet(I_DEVB, K_DEV),
        packet(I_DEVB, K_DEV),
        packet(I_DEVB, K_DEV),
        ]
    createtask(I_HANDLERB, 3000, wkq, S_WAITPKT, handlerfn, [], [])

    createtask(I_DEVA, 4000, None, S_WAIT, devfn, 0, 0)
    createtask(I_DEVB, 5000, None, S_WAIT, devfn, 0, 0)

    global tcb, qpktcount, holdcount
    tcb = tasklist[0]

    qpktcount = holdcount = 0;

    print ('starting')

    layout = 0

    schedule()

    print ('\nfinished')

    print ('qpkt count = %d  holdcount = %d' % (qpktcount, holdcount))

    print ('these results are')
    if qpktcount == Qpktcountval and holdcount == Holdcountval:
        print ('correct')
    else:
        print ('incorrect')

    print ('end of run')


if __name__ == '__main__':
    import getopt
    opts, args = getopt.getopt(sys.argv[1:], 'c:t')

    tracing = False
    count = '1'
    for opt, val in opts:
        if opt == '-t':
            tracing = True
        elif opt == '-c':
            count = val

    main(tracing, count)

