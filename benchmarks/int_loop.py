import sys

def loop(n):
    i = n
    while i:
        i -= 1
    assert i == 0
    
n = int(sys.argv[1])
loop(n)
