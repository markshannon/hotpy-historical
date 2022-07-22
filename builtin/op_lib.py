
_getitem_in_c = False

@method(dict, '__contains__')
@c_function
def dict_contains(d:dict, i)->bool:
    pass

@method(dict_proxy, '__contains__')
@c_function
def dict_proxy_contains(d:dict_proxy, o)->bool:
    pass

del dict_proxy_contains
del dict_contains

def no_delitem(x, y, z):
    raise TypeError("'%s' object does not support item deletion" % x$type.__name__)

delitem = make_operator('delitem', '__delitem__', no_delitem, 3)
del no_delitem

@method(list, '__delitem__')
@_pure
@c_function
def delitem_list(self:list, i:int):
    pass
del delitem_list

@method(str, '__getitem__')
@_pure
@c_function
def getitem_str(self:str, other)->str:
    pass
del getitem_str

@_pure
@c_function
def iadd_list_list(self:list, other:list)->list:
    pass

@method(list, '__iadd__')
def iadd_list(self:list, other)->list:
    if type(other) is list:
        iadd_list_list(self, other)
    else:
        for x in other:
            self.append(x)
    return self
del iadd_list

@method(tuple, '__add__')
@c_function
def tuple_add(self:tuple, other:tuple):
    pass

behaviour(_add)(tuple_add)

del tuple_add

@behaviour(_mul)
@_pure
@c_function
def mul_list_int(self:list, n:int)->list:
     pass

@behaviour(_mul)
@_pure
@c_function
def mul_str_int(self:str, n:int)->str:
     pass

@behaviour(_add)
@_pure
@c_function
def add_list_list(self:list, other:list)->list:
    pass

@method(dict, '__getitem__')
@_pure
@c_function
def getitem_dict(self:dict, i):
    pass
del getitem_dict

#Cannot be pure, as dict could belong to a module.
@method(dict, '__setitem__')
@c_function
def setitem_dict(self:dict, key, value):
    pass
del setitem_dict

@method(dict_proxy, '__getitem__')
@_pure
@c_function
def getitem_dict_proxy(self:dict_proxy, key):
    pass
del getitem_dict_proxy

@method(dict_proxy, '__setitem__')
@c_function
def setitem_dict_proxy(self:dict_proxy, key, value):
    pass
del setitem_dict_proxy

@method(NoneType, '__bool__')
def NoneType_bool(x):
    return False

NoneType.__bool__ = NoneType_bool
NoneType_bool.__name__ = '__bool__'
del NoneType_bool

@method(tuple, '__bool__')
@method(str, '__bool__')
@_no_raise
@_pure
@c_function
def seq_bool(x)->bool:
    pass
seq_bool.__name__ = '__bool__'

del seq_bool

@method(list, '__bool__')
@_no_raise
@_pure
@c_function
def list_bool(l:list)->bool:
    pass
del list_bool

@behaviour(_add)
@_pure
@c_function
def add_str_str(self:str, other:str)->str:
    pass
del add_str_str

@_pure
@c_function
def tuple_getitem_int(self:tuple, index:int):
    pass

@_pure
@c_function
def list_getitem_int(self:list, index:int):
    pass

@_pure
@c_function
def list_getitem_slice(self:list, index:slice)->list:
    pass

@_pure
@c_function
def list_left_slice(self:list, index:int)->list:
    pass

@_pure
@c_function
def list_right_slice(self:list, index:int)->list:
    pass

@_pure
@c_function
def list_reversed(self:list)->list:
    pass

@_pure
@c_function
def list_copy(self:list)->list:
    pass

if _getitem_in_c:

    @method(list, '__getitem__')
    @c_function
    def list_getitem(self:list, index):
        pass

    @method(tuple, '__getitem__')
    @c_function
    def tuple_getitem(self:tuple, index):
        pass

else:

    @method(list, '__getitem__')
    @_no_trace
    @_globals_to_consts
    def list_getitem(self:list, index):
        if index$type is int:
            return list_getitem_int(self, index)
        elif index$type is slice:
            if index.step is None or index.step == 1:
                if index.start is None:
                    if index.stop is None:
                        return list_copy(self)
                    else:
                        return list_left_slice(self, index.stop)
                elif index.stop is None:
                    return list_right_slice(self, index.start)
            elif index.step == -1:
                if index.start is None:
                    if index.stop is None:
                        return list_reversed(self)
                    else:
                        l = list_right_slice(self, index.start+1)
                        list_reverse(l)
                        return l
                elif index.stop is None:
                    l = list_left_slice(self, index.start+1)
                    list_reverse(l)
                    return l
            return list_getitem_slice(self, index)
        elif index?__index__:
            res = index$__index__()
            if not $subtype(res$type, int):
                raise TypeError("__index__ returned non-int type %s)" %
                                type(res).__name__)
            return list_getitem_int(self, res)
        else:
            raise TypeError('list indices must be integers, not %s' %
                            index$type.__name__)

    @method(tuple, '__getitem__')
    #@_no_trace
    #@_globals_to_consts
    def tuple_getitem(self:tuple, index):
        if index$type is int:
            return tuple_getitem_int(self, index)
        elif index$type is slice:
            if index.step is None:
                if index.start is None:
                    if index.stop is None:
                        return self
                    else:
                        return tuple_left_slice(self, index.stop)
                elif index.stop is None:
                    return tuple_right_slice(self, index.start)
            elif index.step == -1:
                if index.start is None:
                    if index.stop is None:
                        return tuple_reversed(self)
                    else:
                        l = tuple_right_slice(self, index.start+1)
                        tuple_reverse(l)
                        return l
                elif index.stop is None:
                    l = tuple_left_slice(self, index.start+1)
                    tuple_reverse(l)
                    return l
            return tuple_getitem_slice(self, index)
        else:
            return tuple_getitem_index(self, index)

del list_getitem

@_pure
@c_function
def list_update(self:list, value:list):
    pass

@c_function
def list_setitem_slice_list_(self:list, index:slice, value:list):
    pass

@_pure
@c_function
def list_left_assign(self:list, index:int, value:list):
    pass

@_pure
@c_function
def list_right_assign(self:list, index:int, value:list):
    pass

@_pure
@c_function
def list_setitem_int(self:list, index:int, value):
    pass

@_no_trace
@_globals_to_consts
def list_setitem_slice_list(self:list, index:slice, value:list):
    if index.step is None:
        if index.start is None:
            if index.stop is None:
                return list_update(self, value)
            else:
                return list_left_assign(self, index.stop, value)
        elif index.stop is None:
            return list_right_assign(self, index.start, value)
    return list_setitem_slice_list_(self, index, value)

@c_function
def list_setitem_slice(self:list, index:slice, value):
    pass

if _getitem_in_c:

    @method(list, '__setitem__')
    @c_function
    def list_setitem(self, index, value):
        pass

else:

    @method(list, '__setitem__')
    @_no_trace
    @_globals_to_consts
    def list_setitem(self, item, value):
        if item$type is int:
            return list_setitem_int(self, item, value)
        elif item$type is slice:
            if type(value) is list:
                return list_setitem_slice_list(self, item, value)
            return list_setitem_slice(self, item, value)
        else:
            if hasattr(item$type, '__index__'):
                item = item$__index__()
                if not $subtype(item$type, int):
                    raise TypeError("__index__ returned non-int type %s)" %
                        type(item).__name__)
                return list_setitem_int(self, item, value)
            else:
                raise TypeError('list indices must be integers, not %s' %
                        type(item).__name__)

del list_setitem

@behaviour(_mod)
@c_function
def mod_str_tuple(s:str, t:tuple):
    pass
del mod_str_tuple

@behaviour(_mod)
def mod(s:str, right:str) :
    return s % (right,)

@behaviour(_mod)
def mod(s:str, b:bool) :
    return s % (b,)

@behaviour(_mod)
def mod(s:str, i:int) :
    return s % (i,)

@behaviour(_mod)
def mod(s:str, f:float) :
    return s % (f,)

def not_(x):
    return not x

@c_function
def make_unary_operator(name:str, func_name:str, fallback:not_$type):
    pass

def no_pos(x):
    raise TypeError("bad operand type for unary +: '%s'" % x$type.__name__)

pos = make_unary_operator("pos", "__pos__", no_pos)

@_globals_to_consts
@_no_trace
def surrogate_pos(x):
    if x?__pos__:
        return x$__pos__()
    else:
        no_pos(x)
tracing_function(pos, surrogate_pos)

del no_pos
def pos(x):
    return +x

def no_neg(x):
    raise TypeError("bad operand type for unary -: '%s'" % x$type.__name__)

neg = make_unary_operator("neg", "__neg__", no_neg)

@_globals_to_consts
@_no_trace
def surrogate_neg(x):
    if x?__neg__:
        return x$__neg__()
    else:
        no_neg(x)
tracing_function(neg, surrogate_neg)

del no_neg
def neg(x):
    return -x

def no_invert(x):
    raise TypeError("bad operand type for unary ~: '%s'" % x$type.__name__)

invert = make_unary_operator("invert", "__invert__", no_invert)

@_globals_to_consts
@_no_trace
def surrogate_invert(x):
    if x?__invert__:
        return x$__invert__()
    else:
        no_invert(x)
tracing_function(invert, surrogate_invert)

del no_invert
def invert(x):
    return ~x

abs = abs

def contains(seq, item):
    return item in seq

@method(list, 'iadd')
def list_extend(self:list, other):
    for i in other:
        self.append(i)
    return self

@method(int, '__index__')
def int_index(self):
    return self

@_no_trace
def index(item)->int:
    res = item$__index__()
    if not $subtype(res$type, int):
        raise TypeError("__index__ returned non-int type %s)" %
                        type(res).__name__)
    return res

def hex(i):
    return index(i)._format(16)

def oct(i):
    return index(i)._format(8)

def bin(i):
    return index(i)._format(2)

del _getitem_in_c

