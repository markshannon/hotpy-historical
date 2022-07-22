class X:
    __slots__ = ['next']

n= 1000000

def make_linked_list(n):
    a = None
    i = 0
    while i < n:
        b = X()
        b.next = a
        a = b
        i += 1

make_linked_list(n)

