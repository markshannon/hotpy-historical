
def array_copy(src, s_offset, length, dst, d_offset):
    dst[d_offset:d_offset+length] = src[s_offset:s_offset+length]

def insertion_sort(a, a_offset, a_limit):
    i = a_offset + 1
    while i < a_limit:
        save = a[i]
        j = i
        while j > a_offset and a[j - 1] > save:
            a[j] = a[j - 1]
            j -= 1
        a[j] = save
        i += 1

def merge(a, split, end, temp):
    array_copy(a, 0, split, temp, 0)
    i, j, r = 0, split, 0
    while i < split and j < end:
        if temp[i] <= a[j]:
            a[r] = temp[i]
            i += 1
        else:
            a[r] = a[j]
            j += 1
        r += 1
    if i < split:
        array_copy(temp, i, split - i, a, r)
    else: 
        array_copy(a, j, end - j, a, r)
        
def mergesort_r(a, a_offset, a_limit, temp):
    if a_limit < a_offset + 8:
        insertion_sort(a, a_offset, a_limit)
    else:
        half = (a_offset + a_limit) >> 1
        mergesort_r(a, 0, half, temp)
        mergesort_r(a, half, a_limit, temp)
        merge(a, half, a_limit, temp)
        
def mergesort(l:list):
    #l.lock()
    a = l #a = l.__array
    length = len(l)
    #temp = make_array(half)
    temp = [ None ] * ((length+1) >> 1)
    mergesort_r(a, 0, length, temp)
    #l.unlock()

if __name__ == "__main__":
    l = list(range(100000))
    #l = [3, 4, 8, 0, 6, 7, 4, 2, 1, 9, 4, 5]
    #insertion_sort(l, 0, len(l))
    mergesort(l)
    #print (l)

