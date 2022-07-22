#Next with no exception:

def next_surrogate(iter):
    $push(iter)
    return iter$__next__()

#With normal exception:

#Trace next as:
#load_constant offset
#Trace enter next_surrogate

def next_surrogate(iter):
    try:
        $copy(iter)
        return iter$__next__()
    except StopIteration:
        $drop() # Discard iter
        $pop_frame()
        $jump_to()

#jump_to: Jumps frame_pointer->next_ip + TOS.

