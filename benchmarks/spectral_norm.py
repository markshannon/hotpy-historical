# The Computer Language Benchmarks Game
# http://shootout.alioth.debian.org/
#
# Contributed by Sebastien Loisel
# Fixed by Isaac Gouy
# Sped up by Josh Goldfoot
# Dirtily sped up by Simon Descarpentries
# 2to3

#class enumerate:
#    __slots__ = [ '_count', '_iterator' ]
#
#    def __init__(self, seq):
#        self._count = 0
#        self._iterator = iter(seq)
#
#    def __iter__(self):
#        return self
#
#    def __next__(self):
#        c = self._count
#        self._count = c + 1
#        return (c, self._iterator.__next__())

from math      import sqrt

from sys       import argv

def eval_A (i, j):
    return 1.0 / ((i + j) * (i + j + 1) / 2 + i + 1)

def eval_A_times_u (u):
    resulted_list = []

    for i in range (len (u)):
        partial_sum = 0

        for j, u_j in enumerate(u):
            partial_sum += eval_A (i, j) * u_j

        resulted_list.append (partial_sum)

    return resulted_list

def eval_At_times_u (u):
    resulted_list = []

    for i in range (len (u)):
        partial_sum = 0

        for j, u_j in enumerate(u):
            partial_sum += eval_A (j, i) * u_j

        resulted_list.append (partial_sum)

    return resulted_list

def eval_AtA_times_u (u):
    return eval_At_times_u (eval_A_times_u (u))

def main():
    n = int (argv [1])
    u = [1] * n

    for dummy in range (10):
        v = eval_AtA_times_u (u)
        u = eval_AtA_times_u (v)

    vBv = vv = 0

    for ue, ve in zip (u, v):
        vBv += ue * ve
        vv  += ve * ve

    print("%g" % (sqrt(vBv/vv)))

main()

