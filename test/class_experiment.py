
def type_call(cls, *args, **kws):
    obj = cls.__new__(cls, *args, **kws)
    if obj.__class__ is cls:
        obj.__init__(*args, **kws)
    return obj

0      :line 3
3      :load_frame 0(cls)
5      :load_attr __new__
8      :load_frame 0(cls)
10     :pack 1
12     :load_frame 1(args)
14     :tuple_concat
15     :load_frame 2(kws)
17     :copy_dict
18     :f_call
19     :store_fast 3(obj)
21     :line 4
24     :load_frame 3(obj)
26     :load_attr __class__
29     :load_frame 0(cls)
31     :is
32     :on_false 50
35     :line 5
38     :load_frame 3(obj)
40     :load_attr __init__
43     :load_frame 1(args)
45     :load_frame 2(kws)
47     :copy_dict
48     :f_call
49     :drop
50     :line 6
53     :load_frame 3(obj)
55     :return
    
for i in range(100):
    x = type_call(dict, a = 1, b = 2)
print(x)

#class C: 
#    pass
#
#def f(self):
#    print(self)
#    
#C.f = classmethod(f)
#C().f()
#C.f()
#print(C.f)
