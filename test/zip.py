#
#def zip(*iterables):
#    # zip('ABCD', 'xy') --> Ax By
#    iterables = tuple(map(iter, iterables))
#    t = tuple(map(next, iterables))
#    while t:
#        yield t 
#        t= tuple(map(next, iterables))

def zip(*iterables):
    # zip('ABCD', 'xy') --> Ax By
    iterables = tuple(map(iter, iterables))
    iter_len = len(iterables)
    while iterables:
        t = tuple(map(next, iterables))
        if len(t) < iter_len: return
        yield t


z = zip('ABCD', 'xy', 'abc')
print (z)
for i in z:
    print (i)

