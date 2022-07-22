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

def _no_overflow(f):
    return f

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
def __merge_lo(a, pa, na, pb, nb):
    #assert pa + na == pb
    right = _getitem(a, pb)
    left = _getitem(a, pa)
    while not right < left:
        pa += 1
        na -= 1
        left = _getitem(a, pa)
        if na == 0:
            return
    i, j, r = 0, pb, pa
    temp = _subarray(a, pa, pa+na)
    end = pb + nb
    while True:
        if not right < left:
            _setitem(a, r, left)
            i += 1
            r += 1
            if i == na:
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
                _array_copy(temp, i, a, r, na-i)
                return
            right = _getitem(a, j)

@_no_overflow
@_no_trace
def __merge_hi(a, pa, na, pb, nb):
    #assert pa + na == pb
    left = _getitem(a, pa + na - 1)
    right = _getitem(a, pb + nb - 1)
    while left < right:
        nb -= 1
        right = _getitem(a, pb + nb - 1)
        if nb == 0:
            return
    i, j, r = pa + na -1, nb-1, pb+nb-1
    temp = _subarray(a, pb, pb+nb)
    while True:
        if right < left:
            _setitem(a, r, left)
            if i == pa:
                _array_copy(temp, 0, a, pa, j+1)
                return
            i -= 1
            r -= 1
            left = _getitem(a, i)
        else:
            _setitem(a, r, right)
            if j == 0:
                return
            j -= 1
            r -= 1
            right = _getitem(temp, j)

@_no_overflow
@_no_trace
def __merge_at(merge_list, a, n):
    pa, na = merge_list[n]
    pb, nb = merge_list[n+1]
    merge_list[n] = pa, na+nb
    last = merge_list.pop()
    if n == len(merge_list)-2:
        merge_list[n+1] = last
    #else:
    #    assert i == len(merge_list)-1
    if na < nb:
        #__merge_lo(a, pa, na, pb, nb)
        right = _getitem(a, pb)
        left = _getitem(a, pa)
        while not right < left:
            pa += 1
            na -= 1
            left = _getitem(a, pa)
            if na == 0:
                return
        i, j, r = 0, pb, pa
        temp = _subarray(a, pa, pa+na)
        end = pb + nb
        while True:
            if not right < left:
                _setitem(a, r, left)
                i += 1
                r += 1
                if i == na:
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
                    _array_copy(temp, i, a, r, na-i)
                    return
                right = _getitem(a, j)
        #end merge_lo
    else:
        #__merge_hi(a, pa, na, pb, nb)
        left = _getitem(a, pa + na - 1)
        right = _getitem(a, pb + nb - 1)
        while left < right:
            nb -= 1
            right = _getitem(a, pb + nb - 1)
            if nb == 0:
                return
        i, j, r = pa + na -1, nb-1, pb+nb-1
        temp = _subarray(a, pb, pb+nb)
        while True:
            if right < left:
                _setitem(a, r, left)
                if i == pa:
                    _array_copy(temp, 0, a, pa, j+1)
                    return
                i -= 1
                r -= 1
                left = _getitem(a, i)
            else:
                _setitem(a, r, right)
                if j == 0:
                    return
                j -= 1
                r -= 1
                right = _getitem(temp, j)
        #end merge_hi

@_no_overflow
@_no_trace
def __merge_collapse(merge_list, a):
    while len(merge_list) > 1:
        n = len(merge_list) - 2
        ln = merge_list[n][1]
        ln1 = merge_list[n+1][1]
        if n > 0 and merge_list[n-1][1] <= ln +ln1:
            if merge_list[n-1][1] < ln1:
                n -= 1
        elif ln > ln1:
            break
        __merge_at(merge_list, a, n)

@_no_overflow
@_no_trace
def __merge_force_collapse(merge_list, a):
    while len(merge_list) > 1:
        n = len(merge_list) - 2
        ln = merge_list[n][1]
        ln1 = merge_list[n+1][1]
        if n > 0:
            lnm1 = merge_list[n-1][1]
            if lnm1 <= ln +ln1:
                n -= 1
        __merge_at(merge_list, a, n)

def __adaptivesort(a, length):
    merge_list = []
    if length < 2:
        return
    minrun = __merge_compute_minrun(length)
    lo, hi = 0, length
    while True:
        n = __count_run(a, lo, hi)
        if n < minrun:
            if minrun < hi-lo:
                force = minrun
            else:
                force = hi-lo
            __insertion_sort(a, lo, lo+force)
            n = force
        merge_list.append((lo, n))
        __merge_collapse(merge_list, a)
        lo += n
        if lo == hi:
            break
    __merge_force_collapse(merge_list, a)

def mergesort(l:list):
    #_lock_list(l)
    #try:
        a = _list_array(l)
        length = len(l)
        if length < 2:
            #_unlock_list(l)
            return
        __adaptivesort(a, length)
    #finally:
    #    _unlock_list(l)

def genRandom(lim, ia = 3877, ic = 29573, im = 139968):
    seed = 42
    imf = im
    while 1:
        seed = (seed * ia + ic) % im
        yield lim * seed // imf

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
    if len(values) < 2:
        return True
    vals = iter(values)
    prev = next(vals)
    for item in vals:
        if prev > item
            return False
        prev = item
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
