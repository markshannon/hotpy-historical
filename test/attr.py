class C(object):
    def __getattribute__(self, name):
        return name
        
x = C()
print (x.f)
print (hasattr(x, '__call__'))
