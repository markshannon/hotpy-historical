def special_call(f):
    'Fix Me!'
    return f

@_no_trace
@special_call
def xxx(a, b):
    if type(a) != type(b) and issubclass(type(b), type(a)):
        c = b.__rxxx__(a)
        if c is NotImplemented:
            c = a.__xxx__(b)
            if c is NotImplemented:
                no_xxx(a, b)
    else:
        c = a.__xxx__(b)
        if c is NotImplemented:
            c = b.__rxxx__(a)
            if c is NotImplemented:
                no_xxx(a, b)
    return c        

