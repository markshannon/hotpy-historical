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
from __unsafe import lock_list as _lock_list
from __unsafe import unlock_list as _unlock_list

@_no_trace
def __insertion_sort(a, a_offset, a_limit):
    i = a_offset + 1
    while i < a_limit:
        save = _getitem(a, i)
        j = i 
        while j > a_offset:
            item = _getitem(a, j-1)
            if item <= save:
                break;
            _setitem(a, j, item)
            j -= 1
        _setitem(a, j, save)
        i += 1

@_no_trace
def __merge(a, offset, end, temp):
    split = (offset + end + 1) >> 1
    half = split - offset
    #temp = _subarray(a, offset, split)
    _array_copy(a, offset, temp, 0, half)
    #assert len(temp) == half
    i, j, r = 0, split, offset
    #assert i < half and j < end
    left = _getitem(temp, i)
    right = _getitem(a, j)
    while True:
        if left <= right:
            _setitem(a, r, left)
            i += 1
            r += 1
            if i == half:
                #assert j == r
                return
            left = _getitem(temp, i)
        else:
            _setitem(a, r, right)
            j += 1
            r += 1
            if j == end:
                #assert i < half
                #assert half - i == end - r
                _array_copy(temp, i, a, r, half-i)
                return
            right = _getitem(a, j)
        
@_no_trace
def __mergesort_r(a, a_offset, a_limit, temp):
    if a_limit < a_offset + 16:
        __insertion_sort(a, a_offset, a_limit)
    else:
        half = (a_offset + a_limit+1) >> 1
        __mergesort_r(a, a_offset, half, temp)
        __mergesort_r(a, half, a_limit, temp)
        __merge(a, a_offset, a_limit, temp)
        
def mergesort(l:list):
    #_lock_list(l)
    #try:
        a = _list_array(l)
        length = len(l)
        if length < 2:
            #_unlock_list(l)
            return
        temp = _make_array((length+1)>>1)
        __mergesort_r(a, 0, length, temp)
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
    return t2-t1

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
