class C: pass

x = C()

print (x.__dict__)
x.__dict__[1] = 1
print (x.__dict__)

import sys
print (sys.__dict__)
sys.__dict__[1] = 1
print (sys.__dict__[1])
print (sys.__dict__)

d = {}
d['self'] = d
print (d)

