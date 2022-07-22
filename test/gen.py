
from itertools import islice

def f(n):
    for i in range(n):
        yield i
    yield 17
    
for i in f(10):
    print(i)
    
for i in reversed(range(10)):
    print (i)
 
print('------')
for x in islice(range(10), 4):
    print (x)

x = ' '.join(map(str, islice(range(10), 8)))
    
print(x)
