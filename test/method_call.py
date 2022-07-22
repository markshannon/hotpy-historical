class C:
    
    def __init__(self, val):
        self.val = val
    
    def m(self, other):
        return self.val + other
        
        
x = C(7)
print (x.m(4.0))

