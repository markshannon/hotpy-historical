
#
#for size in range(60000, 1000, -1000):
#    print('echo "%d"; bash -c "ulimit -v %d; hotpy hello.py"' % (size, size))
#
#for size in range(30000, 1000, -1000):
#    print('echo "%d"; bash -c "ulimit -v %d; hotpy -Hno_comp hello.py"' % (size, size))
#
#for size in range(20000, 1000, -1000):
#    print('echo "%d"; bash -c "ulimit -v %d; python3 hello.py"' % (size, size))
#

pypy = '/home/mark/pypy-1.3/bin/pypy'
unladen = '/home/mark/unladen/opt/python -Xjit=always'


vm = 'python3'
for size in range(10000, 30000, 1000):
    print('echo "%d"; bash -c "ulimit -v %d; %s gcbench.py"' % (size, size, vm))

