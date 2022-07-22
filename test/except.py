print (Exception("a"))
print (AttributeError("a"))
 
try:
    raise Exception("x")
    print ("Shouldn't happen")
except Exception as e:
    print (e)

print (TypeError("t"))

print (IndexError("Out of bounds"))

def f():
    g()
    
def g():
    h()
        
def h():
    print ("5" + 5)
    
i = iter(range(1))
try:
    while True:
        next(i)
except StopIteration as s:
    print (s)
    
f()


