class C:
   def __call__(*args):
       return 17

class D:
   __get__ = C()
   __set__ = print

class E:
    x = D()

print (E.x, E().x)
