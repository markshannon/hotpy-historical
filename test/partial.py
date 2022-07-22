iadd = int.__add__
print(iadd.__class__)
print(iadd(1,2))

ineg = int.__neg__
print(ineg.__class__)
print(ineg(1))

oneadd = (1).__add__
print(oneadd.__class__)
print(oneadd(2))

oneneg = (1).__neg__
print(oneneg.__class__)
print(oneneg())

