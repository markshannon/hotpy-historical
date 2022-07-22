
@c_class
class array:
    pass

@c_function
def _long_size():
    pass

class _type_code:
    __slots__ = [ 'name', 'signed', 'type', 'size', 'index']
    
    def __init__(self, name, signed, type, size, index):
        self.name = name
        self.signed = signed
        self.type = type
        self.size = size
        self.index = index
    
_typecodes = {         
    'b' : _type_code('b', True, int, 1, 0),
    'B' : _type_code('B', False, int, 1, 1),
    'u' : _type_code('u', True, str, 2, 2),
    'h' : _type_code('h', True, int, 2, 3),
    'H' : _type_code('H', False, int, 2, 4),
    'i' : _type_code('i', True, int, 4, 5),
    'I' : _type_code('I', False, int, 4, 6),
    'l' : _type_code('l', True, int, _long_size(), 7),
    'L' : _type_code('L', False, int, _long_size(), 8),
    'f' : _type_code('f', True, float, 4, 9),
    'd' : _type_code('d', True, float, 8, 10)
}

typecodes = ''.join([ t in _typecodes])

@method(array, '__init__')
@_pure
@c_function
def array___init__(self:array, typecode:str, seq=()):
    pass
del array___init__

@method(array, 'append')
@_pure
@c_function
def array_append(self:array, val):
    pass
del array_append

@method(array, 'buffer_info')
@_pure
@c_function
def array_buffer_info(self:array):
    pass
del array_buffer_info

@method(array, 'byteswap')
@_pure
@c_function
def array_byteswap(self:array):
    pass
del array_byteswap

@method(array, 'count')
@_pure
@c_function
def array_count(self:array, val):
    pass
del array_count

@method(array, 'extend')
def array_extend(self:array, val):
    for x in val:
        array_append(self, x)
del array_extend

@method(array, 'index')
@c_function
def array_index(self:array, val):
    pass
del array_index

@method(array, 'insert')
@_pure
@c_function
def array_insert(self:array, index, val):
    pass
del array_insert

@method(array, 'pop')
@_pure
@c_function
def array_pop(self:array, index=-1):
    pass
del array_pop

@method(array, '__getitem__')
@_pure
@c_function
def array_getitem(self:array, index):
    pass
del array_getitem

@method(array, '__setitem__')
@_pure
@c_function
def array_setitem(self:array, index, val):
    pass
del array_getitem



