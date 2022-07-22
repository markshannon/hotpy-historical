print ({'a' : 1 })
print (object.__dict__)
print (''.__dict__)
# Not sure if its a good idea to allow this.
def hello(self):
    print ("Hello there\n")
object.__dict__['hi'] = hello
print (object.hi)
object.hey = hello
x = object()
x.hey()
x.hi()
