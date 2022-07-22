def f(a,b,c):
    assert a == 1 and b == 2 and c == 3
    
def ft(a, *t):
    b, c = t
    assert a == 1 and b == 2 and c == 3
    
def fd(a, **d):
    b = d['b']
    c = d['c']
    assert a == 1 and b == 2 and c == 3
    
def ftd(a, *t, **d):
    b, = t
    c = d['c']
    assert a == 1 and b == 2 and c == 3

f(1,2,3)
f(1, *(2,3))
f(1, 2, c=3)
f(1, b=2, c=3)
f(1, **{'b':2, 'c': 3})
f(1, *(2,), **{'c': 3})

ft(1,2,3)
ft(1, *(2,3))

fd(1,b=2,c=3)
fd(1, **{'b':2, 'c': 3})

ftd(1, 2, c=3)
ftd(1, *(2,), **{'c': 3})

