def f(i):
    def g(j):
        return j - i
    return g
    
sub2 = f(2)
print (sub2(3))

