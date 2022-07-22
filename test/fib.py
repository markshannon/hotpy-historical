def fib(n):
    last, result = 0 , 1
    for i in range(n):
        last, result = result, result + last
    return result

import sys
#print(fib(int(sys.argv[1])))
print(fib(int(sys.argv[1])))

