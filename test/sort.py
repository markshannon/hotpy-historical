import random
from time import clock as now

def fill(n):
    from random import random
    return [random() for i in range(n)]

def timeit(values):
    X = values[:]
    t1 = now()
    values.sort()
    t2 = now()
    return t2-t1

format = "%5s  %9.2f"
f2     = "%5s  %9.2f  %11.2f"

def drive():
    count = sst = sscmp = mst = mscmp = nelts = 0
    for count in range(20):
        n = 100000
        nelts += n
        x = fill(n)
        t  = timeit(x)
        sst += t
        print (t)

    print ("count", count, "nelts", nelts )
    print (format % ("sort",  sst))

drive()

