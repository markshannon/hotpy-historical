
@c_function
def array_type():
    pass

@_pure
@_no_raise
@c_function
def list_array(l:list)->array_type():
    pass

@_pure
@_no_raise
@c_function
def array_getitem_int(a, i:int):
    "Don't specifiy type of a, as its OK for a to be an array or a tuple"
    pass

@_pure
@_no_raise
@c_function
def array_setitem_int(a:array_type(), i:int, value):
    pass

@_pure
@_no_raise
@c_function
def lock_list(l:list):
    pass

@_pure
@_no_raise
@c_function
def unlock_list(l:list):
    pass

@_pure
@_no_raise
@c_function
def lock_dict(d:dict):
    pass

@_pure
@_no_raise
@c_function
def unlock_dict(d:dict):
    pass

@_pure
@c_function
def array_copy(src:array_type(), s_offset:int, dst:array_type(), d_offset:int, size:int):
    pass

@_pure
@c_function
def array_reverse(src:array_type(), s_offset:int, size:int):
    pass

@_pure
@c_function
def make_array(size:int)->array_type():
    pass

@_pure
@c_function
def array_sub_array(a:array_type(), start:int, stop:int)->array_type():
    pass

del array_type

@_pure
@c_function
def tagged_floordiv(a, b):
    pass

@_pure
@c_function
def tagged_mod(a, b):
    pass
