#from operator import list_getitem_int
#from operator import list_setitem_int
#from operator import list_sub_list
#
from __unsafe import list_array as _list_array
from __unsafe import array_getitem_int as _getitem
from __unsafe import array_setitem_int as _setitem
from __unsafe import array_sub_array as _subarray
from __unsafe import make_array as _make_array
from __unsafe import array_copy as _array_copy
from __unsafe import array_reverse as _array_reverse
from __unsafe import lock_list as _lock_list
from __unsafe import unlock_list as _unlock_list

def _tail_call(f):
    return f

class _MergeState:
    __slots__ = [ 'pending', 'n', 'temp', 'array', 'length', 'pa', 'na', 'pb', 'nb', 'lo', 'hi' ]

    def __init__(self, l):
        length = len(l)
        self.array = _list_array(l)
        self.pending = _make_array(85)
        self.n = 0
        self.temp = _make_array(length >> 1)
        self.length = length
        self.lo = __merge_lo(self)
        self.hi = __merge_hi(self)
        
    @_no_overflow
    def pop(self):
        #assert self.n > 0
        i = self.n - 1
        self.n = i
        return _getitem(self.pending, i)
        
    def __getitem__(self, index):
        #assert index >= 0 and index < self.n
        return _getitem(self.pending, index)
        
    def __setitem__(self, index, val):
        #assert index >= 0 and index < self.n
        return _setitem(self.pending, index, val)
        
    @_no_overflow   
    def append(self, val):
        i = self.n
        _setitem(self.pending, i, val)
        self.n = i + 1

@_no_overflow
@_no_trace
def __insertion_sort(a, a_offset, a_limit):
    i = a_offset + 1
    while i < a_limit:
        save = _getitem(a, i)
        j = i 
        while j > a_offset:
            item = _getitem(a, j-1)
            if not save < item:
                break;
            _setitem(a, j, item)
            j -= 1
        _setitem(a, j, save)
        i += 1

@_no_overflow        
@_no_trace
def __count_run(a, low, high):
    'Count length of run, reverse inplace if required'
    i = low + 1
    if i == high:
        return 1
    l0 = _getitem(a, i)
    lm1 = _getitem(a, low)
    if l0 < lm1:
        i += 1
        while i < high:
            lm1 = l0
            l0 = _getitem(a, i)
            if not l0 < lm1:
                break
            i += 1
        _array_reverse(a, low, i)
    else:  
        i += 1
        while i < high:
            lm1 = l0
            l0 = _getitem(a, i)
            if l0 < lm1:
                break
            i += 1
    return i - low
    
@_no_overflow
@_no_trace
def __merge_compute_minrun(n):
    r = 0
    #assert n >= 0
    while n >= 32:
        r |= (n & 1)
        n >>= 1
    return n + r
    
@_no_overflow
@_no_trace
def __merge_lo(merge_state):
    a = merge_state.array
    temp = merge_state.temp
    while True:
        #assert pa + na == pb
        pa = merge_state.pa
        na = merge_state.na
        pb = merge_state.pb
        nb = merge_state.nb
        right = _getitem(a, pb)
        left = _getitem(a, pa)
        while not right < left:
            pa += 1
            na -= 1
            left = _getitem(a, pa)
            if na == 0:
                break
        if na:
            i = 0
            j = pb
            r = pa
            _array_copy(a, pa, temp, 0, na)
            end = pb + nb
            while True:
                if not right < left:
                    _setitem(a, r, left)
                    i += 1
                    r += 1
                    if i == na:
                        #assert j == r
                        break
                    left = _getitem(temp, i)
                else:
                    _setitem(a, r, right)
                    j += 1
                    r += 1
                    if j == end:
                        #assert i < half
                        #assert half - i == end - r
                        _array_copy(temp, i, a, r, na-i)
                        break
                    right = _getitem(a, j)
        yield None
    assert False
        
@_no_overflow
@_no_trace
def __merge_hi(merge_state):
    a = merge_state.array
    temp = merge_state.temp
    while True:
        #assert pa + na == pb
        pa = merge_state.pa
        na = merge_state.na
        pb = merge_state.pb
        nb = merge_state.nb
        left = _getitem(a, pa + na - 1)
        right = _getitem(a, pb + nb - 1)
        while left < right:
            nb -= 1
            right = _getitem(a, pb + nb - 1)
            if nb == 0:
                break
        if nb:
            i = pa + na -1
            j = nb-1
            r = pb+nb-1
            _array_copy(a, pb, temp, 0, nb)
            while True:
                if right < left:
                    _setitem(a, r, left)
                    if i == pa:
                        _array_copy(temp, 0, a, pa, j+1)
                        break
                    i -= 1
                    r -= 1
                    left = _getitem(a, i)
                else:
                    _setitem(a, r, right)
                    if j == 0:
                        break
                    j -= 1
                    r -= 1
                    right = _getitem(temp, j)
        yield None
    assert False
    
@_no_overflow
@_no_trace
def __merge_at(merge_state, i):
    t = merge_state[i]
    merge_state.pa = _getitem(t, 0)
    merge_state.na = _getitem(t, 1)
    t = merge_state[i+1]
    merge_state.pb = _getitem(t, 0)
    merge_state.nb = _getitem(t, 1)
    merge_state[i] = merge_state.pa, merge_state.na+merge_state.nb
    last = merge_state.pop()
    if i == merge_state.n-2:
        merge_state[i+1] = last
    #else:
    #    assert i == len(merge_list)-1
    if merge_state.na < merge_state.nb:
        return next(merge_state.lo)
    else:                     
        return next(merge_state.hi)

@_no_overflow
@_no_trace
def __merge_collapse(merge_state):
    while True:
        while merge_state.n > 1:
            n = merge_state.n - 2
            ln = _getitem(merge_state[n], 1)
            ln1 = _getitem(merge_state[n+1], 1)
            if n > 0 and _getitem(merge_state[n-1], 1) <= ln +ln1:
                if _getitem(merge_state[n-1], 1) < ln1:
                    n -= 1
            elif ln > ln1:
                break
            t = merge_state[n]
            merge_state.pa = _getitem(t, 0)
            merge_state.na = _getitem(t, 1)
            t = merge_state[n+1]
            merge_state.pb = _getitem(t, 0)
            merge_state.nb = _getitem(t, 1)
            merge_state[n] = merge_state.pa, merge_state.na+merge_state.nb
            last = merge_state.pop()
            if n == merge_state.n-2:
                merge_state[n+1] = last
            if merge_state.na < merge_state.nb:
                next(merge_state.lo)
            else:                     
                next(merge_state.hi)
        yield None
    assert False

@_no_overflow
@_no_trace
def __merge_force_collapse(merge_state):
    while merge_state.n > 1:
        n = merge_state.n - 2
        ln = _getitem(merge_state[n], 1)
        ln1 = _getitem(merge_state[n+1], 1)
        if n > 0:
            lnm1 = _getitem(merge_state[n-1], 1)
            if lnm1 <= ln +ln1:
                n -= 1
        __merge_at(merge_state, n)

@_no_overflow
@_no_trace
def __adaptivesort(merge_state):
    collapse = __merge_collapse(merge_state)
    lo, hi = 0, merge_state.length
    minrun = __merge_compute_minrun(merge_state.length)
    while True:
        a = merge_state.array
        n = __count_run(a, lo, hi)
        if n < minrun:
            if minrun < hi-lo:
                force = minrun
            else:
                force = hi-lo
            __insertion_sort(a, lo, lo+force)
            n = force
        merge_state.append((lo, n))
        next(collapse)
        lo += n
        if lo == hi:
            break
    return __merge_force_collapse(merge_state)
        
def mergesort(l:list):
    if len(l) < 2:
        return
    #_lock_list(l)
    #try:
    merge_state = _MergeState(l)
    __adaptivesort(merge_state)
    #finally:
    #    _unlock_list(l)

def genRandom(lim, ia = 3877, ic = 29573, im = 139968):
    seed = 42
    imf = im
    while 1:
        seed = (seed * ia + ic) % im
        yield lim / imf * seed

def fill(n):
    r = genRandom(n)
    return [next(r) for i in range(n)]
    
from time import clock as now

def timeit(values):
    t1 = now()
    mergesort(values)
    t2 = now()
    assert is_sorted(values)
    return t2-t1
    
def is_sorted(values):
    for i in range (1, len(values)):
        if values[i-1] > values[i]:
            return False
    return True

if __name__ == "__main__":
    #import random
    tt = 0
    for count in range(20):
        n = 100000
        l = fill(n)
        #l.reverse()
        #random.shuffle(l)
        #l = [3, 4, 8, 0, 6, 7, 4, 2, 1, 9, 4, 5]
        #insertion_sort(l, 0, len(l))
        t = timeit(l)
        tt += t
        #print (l)
        print(t)
    print("Total = %f" % tt)
