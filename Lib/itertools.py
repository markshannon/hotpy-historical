
def chain(*iterables):
    # chain('ABC', 'DEF') --> A B C D E F
    for it in iterables:
        for element in it:
            yield element
            
def from_iterable(iterables):
    # chain.from_iterable(['ABC', 'DEF']) --> A B C D E F
    for it in iterables:
        for element in it:
            yield element

chain.from_iterable = from_iterable
del from_iterable

# Implementation from py docs, but need to implement generator expressions & for-else in parser first.
#def combinations(iterable, r):
#    # combinations('ABCD', 2) --> AB AC AD BC BD CD
#    # combinations(range(4), 3) --> 012 013 023 123
#    pool = tuple(iterable)
#    n = len(pool)
#    if r > n:
#        return
#    indices = range(r)
#    yield tuple(pool[i] for i in indices)
#    while True:
#        for i in reversed(range(r)):
#            if indices[i] != i + n - r:
#                break
#        else:
#            return
#        indices[i] += 1
#        for j in range(i+1, r):
#            indices[j] = indices[j-1] + 1
#        yield tuple(pool[i] for i in indices)

def combinations(iterable, r):
    if r == 1:
        return [(i,) for i in iterable]
    else:
        l = []
        for i in iterable:
            reduced = [x for x in iterable  if x is not i]
            c = combinations(reduced, r-1)
            for x in c:
                l.append((i,)+x)
        return l
   
def count(n=0):
    # count(10) --> 10 11 12 13 14 ...
    while True:
        yield n
        n += 1

def cycle(iterable):
    # cycle('ABCD') --> A B C D A B C D A B C D ...
    saved = []
    for element in iterable:
        yield element
        saved.append(element)
    while saved:
        for element in saved:
              yield element
              
def dropwhile(predicate, iterable):
    # dropwhile(lambda x: x<5, [1,4,6,4,1]) --> 6 4 1
    iterable = iter(iterable)
    for x in iterable:
        if not predicate(x):
            yield x
            break
    for x in iterable:
        yield x
        
class groupby(object):
    # [k for k, g in groupby('AAAABBBCCDAABBB')] --> A B C D A B
    # [list(g) for k, g in groupby('AAAABBBCCD')] --> AAAA BBB CC D
    def __init__(self, iterable, key=None):
        if key is None:
            key = lambda x: x
        self.keyfunc = key
        self.it = iter(iterable)
        self.tgtkey = self.currkey = self.currvalue = object()
    def __iter__(self):
        return self
    def __next__(self):
        while self.currkey == self.tgtkey:
            self.currvalue = next(self.it)    # Exit on StopIteration
            self.currkey = self.keyfunc(self.currvalue)
        self.tgtkey = self.currkey
        return (self.currkey, self._grouper(self.tgtkey))
    def _grouper(self, tgtkey):
        while self.currkey == tgtkey:
            yield self.currvalue
            self.currvalue = next(self.it)    # Exit on StopIteration
            self.currkey = self.keyfunc(self.currvalue)
            
def ifilter(predicate, iterable):
    # ifilter(lambda x: x%2, range(10)) --> 1 3 5 7 9
    if predicate is None:
        predicate = bool
    for x in iterable:
        if predicate(x):
            yield x
     
def ifilterfalse(predicate, iterable):
    # ifilterfalse(lambda x: x%2, range(10)) --> 0 2 4 6 8
    if predicate is None:
        predicate = bool
    for x in iterable:
        if not predicate(x):
            yield x
            
def imap(function, *iterables):
    # imap(pow, (2,3,10), (5,2,3)) --> 32 9 1000
    iterables = map(iter, iterables)
    while True:
        args = [next(it) for it in iterables]
        if function is None:
            yield tuple(args)
        else:
            yield function(*args)
    
def islice(iterable, *args):
    # islice('ABCDEFG', 2) --> A B
    # islice('ABCDEFG', 2, 4) --> C D
    # islice('ABCDEFG', 2, None) --> C D E F G
    # islice('ABCDEFG', 0, None, 2) --> A C E G
    s = slice(*args)
    it = iter(range(s.start or 0, s.stop or sys.maxint, s.step or 1))
    nexti = next(it)
    for i, element in enumerate(iterable):
        if i == nexti:
            yield element
            nexti = next(it)
            
def izip(*iterables):
    # izip('ABCD', 'xy') --> Ax By
    iterables = map(iter, iterables)
    while iterables:
        yield tuple(map(next, iterables))
        
def izip_longest(*args, **kwds):
    # izip_longest('ABCD', 'xy', fillvalue='-') --> Ax By C- D-
    fillvalue = kwds.get('fillvalue')
    def sentinel(counter = ([fillvalue]*(len(args)-1)).pop):
        yield counter()         # yields the fillvalue, or raises IndexError
    fillers = repeat(fillvalue)
    iters = [chain(it, sentinel(), fillers) for it in args]
    try:
        for tup in izip(*iters):
            yield tup
    except IndexError:
        pass

#def permutations(iterable, r=None):
#    # permutations('ABCD', 2) --> AB AC AD BA BC BD CA CB CD DA DB DC
#    # permutations(range(3)) --> 012 021 102 120 201 210
#    pool = tuple(iterable)
#    n = len(pool)
#    r = n if r is None else r
#    if r > n:
#        return
#    indices = range(n)
#    cycles = range(n, n-r, -1)
#    yield tuple(pool[i] for i in indices[:r])
#    while n:
#        for i in reversed(range(r)):
#            cycles[i] -= 1
#            if cycles[i] == 0:
#                indices[i:] = indices[i+1:] + indices[i:i+1]
#                cycles[i] = n - i
#            else:
#                j = cycles[i]
#                indices[i], indices[-j] = indices[-j], indices[i]
#                yield tuple(pool[i] for i in indices[:r])
#                break
#        else:
#            return

def product(*args, **kwds):
    # product('ABCD', 'xy') --> Ax Ay Bx By Cx Cy Dx Dy
    # product(range(2), repeat=3) --> 000 001 010 011 100 101 110 111
    pools = map(tuple, args) * kwds.get('repeat', 1)
    result = [[]]
    for pool in pools:
        result = [x+[y] for x in result for y in pool]
    for prod in result:
        yield tuple(prod)

def repeat(object, times=None):
    # repeat(10, 3) --> 10 10 10
    if times is None:
        while True:
            yield object
    else:
        for i in xrange(times):
            yield object

def starmap(function, iterable):
    # starmap(pow, [(2,5), (3,2), (10,3)]) --> 32 9 1000
    for args in iterable:
        yield function(*args)

def takewhile(predicate, iterable):
    # takewhile(lambda x: x<5, [1,4,6,4,1]) --> 1 4
    for x in iterable:
        if predicate(x):
            yield x
        else:
            break

#def tee(iterable, n=2):
#    it = iter(iterable)
#    deques = [collections.deque() for i in range(n)]
#    def gen(mydeque):
#        while True:
#            if not mydeque:             # when the local deque is empty
#                newval = next(it)       # fetch a new value and
#                for d in deques:        # load it to all the deques
#                    d.append(newval)
#            yield mydeque.popleft()
#    return tuple(gen(d) for d in deques)


if __name__ == '__main__':
    print(combinations(range(4), 3))
