# This needs lots of memory: use -m600M or more
# With a generational collector, this should go down.
import time

try:
    xrange
except:
    xrange = range


def Main():
 l = []
 prev_time = time.time()
 for i in xrange(10000000):
   if i % 1000000 == 0:
     cur_time = time.time()
     print (i, cur_time - prev_time)
     prev_time = cur_time
   l.append((i % 2, i % 12))

Main()

