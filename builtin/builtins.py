__doc__ = '''Built-in functions, exceptions, and other objects.

Noteworthy: None is the `nil' object; Ellipsis represents `...' in slices.'''

__name__ = 'builtins'
__package__ = None

copyright = 'Copyright (c) 2009-2011 Mark Shannon'
credits = ''


class ArithmeticError(Exception):
    pass

class AssertionError(Exception):
    pass

class AttributeError(Exception):
    pass

class BufferError(Exception):
    pass

class Warning(Exception):
    pass

class BytesWarning(Warning):
    pass

class DeprecationWarning(Warning):
    pass

class EOFError(Exception):
    pass

class EnvironmentError(Exception):
    pass

class TypeError(Exception):
    pass

class NameError(Exception):
    pass

class ValueError(Exception):
    pass

class IndexError(Exception):
    pass

class ImportError(Exception):
    pass

class FloatingPointError(Exception):
    pass

class FutureWarning(Warning):
    pass

class GeneratorExit(Exception):
    pass

class InternalError(Exception):
    pass

class ImportError(Exception):
    pass

class ImportWarning(Warning):
    pass

class IndentationError(Exception):
    pass

class IndexError(Exception):
    pass

class IOError(Exception):
    pass

class KeyError(Exception):
    pass

class KeyboardInterrupt(BaseException):
    pass

    def __str__(self):
        return "Keyboard Interrupt"

class LookupError(Exception):
    pass

class MemoryError(Exception):
    pass

class NameError(Exception):
    pass

class NotImplementedError(Exception):
    pass

class OSError(Exception):
    pass

class OverflowError(Exception):
    pass

class PendingDeprecationWarning(Warning):
    pass

class ReferenceError(Exception):
    pass

class RuntimeError(Exception):
    pass

class RuntimeWarning(Warning):
    pass

class SyntaxWarning(Warning):
    pass

class SystemError(Exception):
    pass

class SystemExit(BaseException):
    pass

class TabError(Exception):
    pass

class UnboundLocalError(Exception):
    pass

class ValueError(Exception):
    pass

class UnicodeError(ValueError):
    pass

class UnicodeDecodeError(UnicodeError):
    pass

class UnicodeEncodeError(UnicodeError):
    pass

class UnicodeWarning(Warning):
    pass

class UserWarning(Warning):
    pass

class ZeroDivisionError(ArithmeticError):
    pass

def divmod(u,v):
    return u/v, u%v

def filter(function, iterable):
    if function is None:
        for item in iterable:
            if item:
                yield item
    else:
        for item in iterable:
            if function(item):
                yield item

#Things to add to builtins
#-------------------------

#__build_class__
#__import__
#ascii
#compile
#dir
#eval
#exec
#format
#help
#input
#issubclass
#memoryview
#open
#pow
#quit
#super

#def issubclass(derived, base):
#    if isinstance(base, tuple):
#        for b in base:
#            if issubclass(derived, b):
#                return True
#        return False
#    elif hasattr(base, "__subclasscheck__"):
#        return base.__subclasscheck__(derived)
#    elif isinstance(base, type):
#        return _issubtype(derived, base)
#    else:
#        return abstract_issubclass(derived, base)
#

@c_function
def globals():
    pass

@c_function
def lib_import(name:str):
    pass

#Should do static optimisations for library functions.
def static(f):
    return f

@c_function
def c_name(func:static$type):
    pass

@c_function
def _pure(f:c_name$type):
    ''' Not properly pure, but demands the following:
        Cannot call back into python code
        Cannot alter module or class attributes
        Does not alter VM or thread state
    '''
    pass

@c_function
def setattr(obj, name:str, value):
    pass

def method(cls, name):
    def _method(func):
        setattr(cls, name, func)
        func.__name__ = name
        return func
    return _method

@c_function
def _no_trace(f:method$type):
    pass

@c_function
def _globals_to_consts(f:method$type):
    pass

@_pure
@c_function
def object_allocate(cls:type):
    pass

@method(object, '__new__')
@c_name
@_no_trace
def object_new(cls, *args, **kws):
    return object_allocate(cls)

@method(object, '__init__')
@c_name
def object_init(cls, *args, **kws):
    pass

_engine = lib_import('_engine')
_no_raise = _engine._no_raise
range = _engine.range
iter = _engine.iter
_print_string = _engine.print_string
_no_overflow = _engine._no_overflow

class irange:
    'Full python implementation of range'

    __slots__ = [ '_start', '_stop', '_step']

    #def __init__(self, *args):
    #    l = len(t)
    #    if l == 1:
    #        self._start = 0
    #        self._stop = args[0]
    #        self._step = 1
    #    elif l == 2:
    #        self._start = args[0]
    #        self._stop = args[1]
    #        self._step = 1
    #    elif l == 3:
    #        self._start = args[0]
    #        self._stop = args[1]
    #        self._step = args[2]
    #        if self._step == 0:
    #            raise ValueError("range() arg 3 must not be zero")
    #    else:
    #        if l == 0:
    #            msg = "range expected 1 arguments, got 0"
    #        else:
    #            msg = "range expected at most 3 arguments, got %d" % l
    #        raise TypeError(msg)

    def __init__(self, _start, _stop = None, _step = 1):
        if _stop is None:
            self._start = 0
            self._stop = _start
        else:
            self._start = _start
            self._stop = _stop
        if _step == 0:
            raise ValueError("range() arg 3 must not be zero")
        else:
            self._step = _step

    def __len__(self):
        if self._step == 1:
            return self._stop - self._start
        else:
            return (self._stop - self._start) // self._step

    def __iter__(self):
        if self._step == 1:
            return irange_iterator(__irange_1(self._start, self._stop))
        elif self._step < 0:
            return irange_iterator(__irange_dec(self._start, self._stop, self._step))
        else:
            return irange_iterator(__irange_inc(self._start, self._stop, self._step))

    def __getitem__(self, index):
        if index < 0:
            raise IndexError("range object index out of range")
        result = self._start + index * self._step
        if self._step < 0:
            if result > self._stop:
                return result
        else:
            if result < self._stop:
                return result
        raise IndexError("range object index out of range")

    def __repr__(self):
        if self._step == 1:
            return 'range(%r, %r)' % (self._start, self._stop)
        else:
            return 'range(%r, %r, %r)' % (self._start, self._stop,
                                          self._step)

    def __reduce__(self):
        return (range, (self._start, self._stop, self._step))

    def __reversed__(self):
        "Returns a reverse iterator."
        new_start = self._start + (len(self) -1) * self._step
        new_stop = self._start - self._step
        if self._step == -1:
            return irange_iterator(__irange_1(new_start, new_stop))
        elif self._step > 0:
            return irange_iterator(__irange_dec(new_start, new_stop, -self._step))
        else:
            return irange_iterator(__irange_inc(new_start, new_stop, -self._step))

def __irange_1(start, stop):
    while start < stop:
        yield start
        start += 1

def __irange_inc(start, stop, step):
    while start < stop:
        yield start
        start += step

def __irange_dec(start, stop, step):
    while start > stop:
        yield start
        start += step

class irange_iterator:
   __slots__ = [ '__gen' ]

   def __init__(self, gen):
       self.___gen = gen

   def __iter__(self):
       return self

   def __next__(self):
       return self.__gen$__next__()

@c_name
@_no_trace
def getattr(obj, name:str):
    return obj$__getattribute__(name)

@c_function
def delattr(obj, name:str):
    pass

@c_function
def make_immutable(cls:type):
    pass

@c_function
def surrogate_new_function(cls:type, f:method$type):
    pass

def surrogate_new(cls):
    def _surrogate_new(func):
        surrogate_new_function(cls, func)
        return func
    return _surrogate_new

@c_name
def new_object_no_init(cls):
    return object_allocate(cls)

@c_name
@_globals_to_consts
@_no_trace
def type_call(cls, *args, **kws):
    obj = $get_class_attr(cls, '__new__')(cls, *args, **kws)
    if $subtype(obj$type, cls):
        obj$__init__(*args, **kws)
    return obj

@c_name
@_no_trace
@_no_trace
def alloc_and_init(cls, *args, **kws):
    obj = object_allocate(cls)
    cls.__init__(obj, *args, **kws)
    return obj

#def clean_up(glbls):
#    for name in glbls:
#       val = glbls[name]
#       if val is None:
#           del glbls[name]

@_pure
@c_function
def object_repr(x):
    pass

object.__repr__ = object_repr
object_repr.__name__ = "__repr__"
del object_repr

@c_function
def repr(obj):
    pass

@c_function
def _bool(cls, arg):
    pass

bool.__new__ = _bool
_bool.__name__ = "bool"
del _bool

@c_name
def print(*args, **kw):
    sep = ' '
    end = '\n'
    if kw:
        if 'file' in kw:
            raise InternalError("Not implemented")
        if 'sep' in kw:
            sep = str(kw['sep'])
        if 'end' in kw:
            end = str(kw['end'])
    items = len(args)
    if items > 0:
        _print_string(str(args[0]))
        i = 1
        while i < items:
            _print_string(sep)
            _print_string(str(args[i]))
            i += 1
    _print_string(end)

@_no_trace
def hash(obj):
    return obj$__hash__()

@_pure
@c_function
def object_hash(self):
    pass
object.__hash__ = object_hash
object_hash.__name__ = '__hash__'
del object_hash

@method(int, '__hash__')
@_pure
@c_function
def int_hash(self):
    pass

@_no_raise
@_pure
@c_function
def object_type(cls, arg):
    pass
type.__new__ = object_type
object_type.__name__ = 'type'
del object_type

@c_function
def _new_str(cls, arg='')->str:
    pass

str.__new__ = _new_str
_new_str.__name__ = 'str'
del _new_str

@c_function
def object_str(self)->str:
    pass
object.__str__ = object_str
object_str.__name__ = '__str__'
del object_str

@_pure
@c_function
def type_str(cls:type)->str:
    pass

type.__str__ = type_str
type_str.__name__ = '__str__'
del type_str

@_no_trace
def type_check(obj, cls):
    if not $subtype(obj$type, cls):
       raise TypeError("Expecting instance of '%s' found instance of '%s'"
                       % (cls.__name__, obj$type.__name__))

@_no_trace
def function_str(self):
    type_check(self, type_check$type)
    return '<function %s id=%s>' % (self.__name__, hex(id(self)))
type_check$type.__str__ = function_str
function_str.__name__ = '__str__'
del function_str

@surrogate_new(str)
@_no_trace
@c_name
@static
def to_str(cls, obj, encoding=None, errors=None):
    if encoding is None:
        return obj$__str__()
    else:
        decode_object(x, encoding, errors)

@_pure
@c_function
def id(obj)->int:
    pass

@_no_trace
@static
@surrogate_new(int)
def to_int(cls, number):
    return number.__int__()
int.__new__ = to_int
to_int.__name__ = "int"
del to_int

@_no_trace
def find_in_str(self:str, s:str):
    type_check(self, str)
    type_check(s, str)
    l = s$__len__()
    for i in range(0, self$__len__() - l):
        if substring(self, i, i+l) == s:
            return i
    return -1
str.find = find_in_str
find_in_str.__name__ = "find"
del find_in_str

@_pure
@c_function
def substring(s:str, low:int, high:int):
    pass

@method(str, '__int__')
@_pure
@c_function
def str_to_int(self:str)->int:
    pass
del str_to_int

@method(float, '__new__')
@surrogate_new(float)
@static
@c_name
def to_float(cls, arg)->float:
    return arg.__float__()
del to_float

@method(int, '__int__')
def _(self):
    return self

@method(int, '__hash__')
def _(self):
    return self

@method(float, '__float__')
def _(self):
    return self

@method(bool, '__bool__')
def _(self):
    return self

@method(str, '__str__')
def _(self):
    return self

@method(generator, '__iter__')
def _(self):
    return self

@method(int_iterator, '__iter__')
def _(self):
    return self

@method(int, '__float__')
@_pure
@c_function
def int_to_float(i:int):
    pass
del int_to_float

@method(float, '__int__')
@_pure
@c_function
def float_to_int(f:float):
    pass
del float_to_int

@method(BaseException, '__init__')
@_no_trace
def init_Exception(self, *args):
    self.args = args
del init_Exception

@method(BaseException, '__str__')
@_no_trace
def str_Exception(self:BaseException):
    type_check(self, BaseException)
    s = ''
#    assert self.args$type is tuple
    try:
        t = self.__traceback__
        while t:
            s += " in %s, line %d\n" % (t.file(), t.line())
            t = t.previous
    except:
        pass
    s += self$type.__name__
    if len(self.args) == 0:
        pass
    elif len(self.args) == 1:
        s += ": " + str(self.args[0])
    else:
        s += ": " + str(self.args)
    return s
del str_Exception

#@c_function
#def frame_line(self):
#    pass
#frame.line = frame_line
#frame_line.__name__ = "line"
#del frame_line
#
#@c_function
#def frame_file(self):
#    pass
#frame.file = frame_file
#frame_file.__name__ = "file"
#del frame_file

@method(bool, '__str__')
@_no_trace
def bool_str(self:bool):
    if self is True:
        return "True"
    elif self is False:
        return "False"
    else:
        type_check(self, bool)
        assert False and "Cannot reach here.  must be True, False or Non-bool"
del bool_str

@method(int, '__bool__')
@_no_raise
@_pure
@c_function
def __bool___int(self:int)->bool:
    pass
del __bool___int

@method(float, '__bool__')
@_no_raise
@_pure
@c_function
def __bool___float(self:float)->bool:
    pass
del __bool___float

@method(int, '__str__')
@method(int, '__repr__')
@_no_raise
@_pure
@c_function
def int_to_str(self:int)->str:
    pass
del int_to_str

@method(float, '__str__')
@method(float, '__repr__')
@_no_raise
@_pure
@c_function
def __str___float(self:float)->str:
    pass
del __str___float

@_no_trace
def none_str(self:NoneType)->str:
    return "None"
NoneType.__str__ = none_str
none_str.__name__ = "__str__"
del none_str

@method(str, '__hash__')
@_no_raise
@_pure
@c_function
def __hash___str(self:str):
    pass
del __hash___str

@_pure
@c_function
def make_slice(args):
    pass

@method(slice, '__new__')
@_no_trace
def slice_new(cls:type, *args, **kws):
    if len(kws):
        raise TypeError('slice() does not take keyword arguments')
    return make_slice(args)
del slice_new

@method(slice, '__str__')
@_no_trace
def __str___slice(self:slice):
    return 'slice(%s, %s, %s)' % (self.start, self. stop, self.step)
del __str___slice

@method(str, '__len__')
@_no_raise
@_pure
@c_function
def __len__str(self:str):
    pass
del __len__str

@_pure
@c_function
def new_list(cls:type):
    pass

@method(list, '__new__')
def _(cls, *args, **kws):
    return new_list(cls)

@method(list, '__init__')
@c_name
@_no_trace
def list_init(self, seq = None):
    if seq is not None:
        for x in seq:
            list.append(self, x)
del list_init

@surrogate_new(list)
@_no_trace
def make_new_list(cls, seq):
    return [x for x in seq]

@method(dict, '__new__')
@_pure
@c_function
def make_dict(cls):
    pass

@method(dict, 'copy')
@c_function
def dict_copy(d:dict):
    pass

@surrogate_new(dict)
def make_new_dict(cls, other=None, **d):
    if other$type == dict:
        result = other.copy()
    else:
        result = {}
        for k in other:
            result[k] = other[k]
    for k in d:
        result[k] = d[k]

#Need a version of dict() using dict comprehension.

@method(dict, '__init__')
@_no_trace
def init_dict(self, other = {}, **d):
    for i in other:
        self[i] = other[i]
    for i in d:
        self[i] = d[i]
del init_dict

@method(dict, 'update')
@_no_trace
def update_dict(self, d):
    self.clear()
    for i in d:
        self[i] = d[i]
del update_dict

@c_function
def _repr_enter(obj):
    pass

@c_function
def _repr_leave(obj):
    pass

@method(list, '__repr__')
@_no_trace
def list_repr(self:list):
    size = len(self)
    if size == 0:
        return "[]"
    if _repr_enter(self):
        return '[...]'
    buf = str_buf()
    it = iter(self)
    str_buf.append(buf, '[')
    str_buf.append(buf, repr(next(it)))
    for x in it:
        str_buf.append(buf, ', ')
        str_buf.append(buf, repr(x))
    str_buf.append(buf, ']')
    _repr_leave(self)
    return buf$__str__()
del list_repr

@method(list, '__str__')
@_no_trace
def list_str(self:list):
    return self$__repr__()
del list_str

@method(list, '__len__')
@_no_raise
@_pure
@c_function
def list_len(self:list):
    pass
del list_len

@method(list, 'pop')
@_pure
@c_function
def list_pop(self, i:int = -1):
    pass
del list_pop

@method(tuple, '__contains__')
@c_function
def tuple_contains(self:tuple, item):
    pass
del tuple_contains

@method(list, '__contains__')
@c_function
def list_contains(self:list, item):
    pass

#@method(list, '__contains__')
#def list_contains(self:list, item):
#    size = len(self)
#    i = 0
#    while i < size:
#        if item == self[i]:
#            return True
#        i += 1
#    return False

del list_contains

@method(list, 'append')
@_pure
@c_function
def list_append(self:list, item):
    pass
del list_append

@method(list, 'reverse')
@_no_raise
@_pure
@c_function
def list_reverse(self:list):
    pass

@method(list, 'insert')
@_pure
@c_function
def list_insert(self, index:int, item):
    pass
del list_insert

@method(list, '__iter__')
@_pure
@c_function
def list_iter(self:list):
    pass
del list_iter

@method(listiterator, '__next__')
@_pure
@c_function
def listiterator_next(self:listiterator):
    pass
del listiterator_next


@method(listiterator, '__iter__')
def _(self):
    return self

@_pure
@c_function
def tuple_from_list(cls:type, l:list)->tuple:
    pass

@method(tuple, '__new__')
@static
def new_tuple(cls, seq):
    if type(seq) is list:
        return tuple_from_list(cls, seq)
    elif type(seq) is tuple:
        return seq
    else:
        l = [x for x in seq]
        return tuple_from_list(cls, l)
del new_tuple

@c_function
def make_set(cls:type):
    pass

@method(set, '__new__')
@_no_trace
@static
def set_new(cls, seq=None):
    return make_set(cls)
del set_new

@method(set, '__init__')
@_no_trace
@static
def set_init(self, seq=None):
    if seq is not None:
        for x in seq:
            self.add(x)

@method(tuple, '__iter__')
@_pure
@c_function
def tuple_iter(self:tuple):
    pass
del tuple_iter

@method(tupleiterator, '__next__')
@_pure
@c_function
def tupleiterator_next(self:tupleiterator):
    pass
del tupleiterator_next

@method(tupleiterator, '__iter__')
def _(self):
    return self

@method(tuple, '__len__')
@_no_raise
@_pure
@c_function
def __len__tuple(self:tuple):
    pass
del __len__tuple

@method(bytes, '__len__')
@_no_raise
@_pure
@c_function
def bytes_length(b:bytes):
    pass
del bytes_length

@method(bytes, '__getitem__')
@_no_raise
@_pure
@c_function
def bytes_getitem(b:bytes, index):
    pass
del bytes_getitem

@method(dict_iterator, '__init__')
@_pure
@c_function
def dict_iter_init(self:dict_iterator, t:dict):
    pass
del dict_iter_init

@method(dict_iterator, '__next__')
@_pure
@c_function
def dict_iter_next(self:dict_iterator):
    pass
del dict_iter_next

@method(dict, '__iter__')
@_no_trace
def dict_iter(self:dict):
    return dict_iterator(self)
del dict_iter

@method(dict_iterator, '__iter__')
@_no_trace
def dict_iterator_iter(self):
    return self
del dict_iterator_iter

@method(type.__dict__$type, '__iter__')
@_pure
@c_function
def dict_proxy_iter(self:type.__dict__$type):
    pass
del dict_proxy_iter

dict_proxy_iter_type = type.__dict__.__iter__()$type

@method(dict_proxy_iter_type, '__next__')
@_pure
@c_function
def dict_proxy_iter_next(self:dict_proxy_iter_type):
    pass
del dict_proxy_iter_next
del dict_proxy_iter_type

@method(dict, 'values')
@_pure
@c_function
def dict_values(self:dict):
    pass
del dict_values

dict_values_iterator_class = {}.values()$type

@method(dict_values_iterator_class, '__iter__')
@_pure
@c_function
def dict_values_iterator_iter(self:dict_values_iterator_class):
    pass
del dict_values_iterator_iter

@method(dict_values_iterator_class, '__next__')
@_pure
@c_function
def dict_values_iterator_next(self:dict_values_iterator_class):
    pass
del dict_values_iterator_next

del dict_values_iterator_class

@method(type.__dict__$type, '__repr__')
@method(dict, '__repr__')
def dict_repr(self):
    size = len(self)
    if size == 0:
        return "{}"
    if _repr_enter(self):
        return '{...}'
    buf = str_buf()
    it = iter(self)
    str_buf.append(buf, '{')
    k = next(it)
    str_buf.append(buf, repr(k))
    str_buf.append(buf, ': ')
    str_buf.append(buf, repr(self[k]))
    for k in it:
        str_buf.append(buf, ', ')
        str_buf.append(buf, repr(k))
        str_buf.append(buf, ': ')
        str_buf.append(buf, repr(self[k]))
    str_buf.append(buf, '}')
    _repr_leave(self)
    return str_buf.__str__(buf)
del dict_repr

@method(type.__dict__$type, '__str__')
@method(dict, '__str__')
@static
def dict_str(self):
    return self$__repr__()

@method(str_buf, 'append')
@_pure
@c_function
def str_buf_append(b:str_buf, s:str):
    pass
del str_buf_append

@method(str_buf, '__str__')
@_pure
@c_function
def str_buf___str__(b:str_buf):
    pass
del str_buf___str__

@method(str_buf, '__len__')
@_pure
@_no_raise
@c_function
def str_buf___len__(b:str_buf):
    pass
del str_buf___len__

@method(str_buf, '__init__')
@_pure
@c_function
def str_buf_init(b:str_buf):
    pass
del str_buf_init

@_pure
@c_function
def str_join_list(s:str, l:list):
    pass

@method(str, 'join')
@_no_trace
def str_join(self:str, l):
    if l$type is list:
        return str_join_list(self, l)
    buf = str_buf()
    it = iter(l)
    try:
        #Use this form rather than buf.append(next(it))
        #as it's a tad quicker in unoptimised code.
        str_buf.append(buf, next(it))
    except StopIteration:
        return ''
    for x in it:
        str_buf.append(buf, self)
        str_buf.append(buf, x)
    return str_buf.__str__(buf)
del str_join

@method(tuple, '__repr__')
def tuple_repr(self:tuple):
    size = len(self)
    if size == 0:
        return "()"
    if _repr_enter(self):
        return '(...)'
    buf = str_buf()
    it = iter(self)
    str_buf.append(buf, '(')
    str_buf.append(buf, repr(next(it)))
    for x in it:
        str_buf.append(buf, ', ')
        str_buf.append(buf, repr(x))
    str_buf.append(buf, ')')
    _repr_leave(self)
    return buf$__str__()

del tuple_repr

@method(type_state, '__eq__')
@c_function
def type_state_eq(l:type_state, r):
    pass
del type_state_eq

@method(type_info, '__eq__')
@c_function
def type_info_eq(l:type_info, r):
    pass
del type_info_eq

@method(tuple, '__eq__')
@c_function
def tuple_eq(l:tuple, r):
    pass
del tuple_eq

@method(tuple, '__ne__')
@c_function
def tuple_ne(l:tuple, r):
    pass
del tuple_ne

@method(list, '__eq__')
@c_function
def list_eq(l:list, r):
    pass
del list_eq

@method(list, '__ne__')
@c_function
def list_ne(l:list, r):
    pass
del list_ne

@method(str, '__iter__')
@_pure
@c_function
def str_iter(self:str):
    pass

str_iterator = str_iter("")$type

@method(str_iterator, '__next__')
@_pure
@c_function
def str_iter_next(self:str_iterator):
    pass

@method(str_iterator, '__iter__')
def str_iter_iter(self:str_iterator):
    return self

del str_iter
del str_iterator
del str_iter_iter
del str_iter_next

@method(str, 'startswith')
def str_startswith(self: str, s:str):
    if len(self) < len(s):
        return False
    for i in range(len(s)):
        if self[i] != s[i]:
            return False
    return True
del str_startswith

@method(set, '__contains__')
@c_function
def set_contains(self:set, item):
    pass
del set_contains

@method(set, 'add')
@c_function
def set_add(self:set, item):
    pass
del set_add

def map1(f, seq):
    for s in seq:
        yield f(s)

@_no_trace
def map(f, *seq):
    if len(seq) == 1:
        return map1(f, seq[0])
    else:
        return mmap(f, seq)

#class entry:
#    __slots__ =  ["key", "value", "next"]
#
#    def __init__(self, key, value):
#        self.key = key
#        self.value = value
#        self.next = None
#
#    def __repr__(self):
#        return self.key.__repr__() + ":" + self.value.__repr__()

@c_function
def isinstance(obj, cls:type):
    pass

@method(int, '__abs__')
@_no_raise
@_pure
@c_function
def abs_int(i:int):
    pass
del abs_int

@method(float, '__abs__')
@_no_raise
@_pure
@c_function
def abs_float(f:float):
    pass
del abs_float

#def abs_complex(c:complex) in abs:
#    return math.sqrt(c.re * c.re + c.im * c.im)

def all(iterable):
    for element in iterable:
        if not element:
            return False
    return True

def any(iterable):
    for element in iterable:
        if element:
            return True
    return False


@_pure
@c_function
def chr(i:int)->str:
    pass

@_pure
@c_function
def ord(c:str)->int:
    pass

@_no_raise
@_pure
@c_function
def callable(x):
    pass

def enumerate(collection):
    'Generates an indexed series:  (0,coll[0]), (1,coll[1]) ...'
    i = 0
    it = iter(collection)
    while True:
        yield (i, it$__next__())
        i += 1

#class enumerate:
#   __slots__ = [ '__gen' ]
#
#   def __init__(self, collection):
#       self.__gen = _enumerate(collection)
#
#   def __iter__(self):
#       return self
#
#   def __next__(self):
#       return next(self.__gen)
#

def zip(*iterables):
    # zip('ABCD', 'xy') --> Ax By
    iterables = tuple(map1(iter, iterables))
    t = tuple(map(next, iterables))
    while t:
        yield t
        t= tuple(map(next, iterables))

#class zip:
#   __slots__ = [ '__gen' ]
#
#   def __init__(self, iterables):
#       self.__gen = _zip(iterables)
#
#   def __iter__(self):
#       return self
#
#   def __next__(self):
#       return next(self.__gen)
#

def filter(function, seq):
    for x in seq:
        if function(x):
            yield x

@c_function
def open(name:str, mode:str = 'r', bufsize:int = -1):
    pass

file.__new__ = open

@method(dict, '__len__')
@_pure
@_no_raise
@c_function
def dict_len(d:dict):
    pass
del dict_len

#@c_function
#def read_file(self:file):
#    pass
#file.read = read_file
#read_file.__name__ = "read"
#del read_file

@c_name
def __import__(filename):
    return _engine.load_module(filename)

def next(x):
    return x$__next__()

@_no_trace
def min2(a, b):
    if b < a:
        return b
    else:
        return a

@_globals_to_consts
@_no_trace
def len(x):
    if x?__len__:
        return x$__len__()
    else:
        raise TypeError("object of type '%s' has no len()" % x$type.__name__)

@_globals_to_consts
@_no_trace
def min(*args, **key):
    if len(args) == 1:
        args = args[0]
    if not args:
        raise ValueError('min() arg is an empty sequence')
    if key and 'key' in key:
        key_func = key['key']
        args = iter(args)
        result = next(args)
        comp = key_func(result)
        for a in args:
            cmp_a = key_func(a)
            if cmp_a < comp:
                result = a
                comp = cmp_a
    else:
        #Common case of two args
        # Tail call so that D.O.C can eliminate this frame
        if len(args) == 2:
            return min2(args[0], args[1])
        else:
            args = iter(args)
            result = next(args)
            for a in args:
                result = min2(result, a)
    return result

@_no_trace
def max2(a, b):
    if b > a:
        return b
    else:
        return a

@_globals_to_consts
@_no_trace
def max(*args, **key):
    if len(args) == 1:
        args = args[0]
    if not args:
        raise ValueError('min() arg is an empty sequence')
    if key and 'key' in key:
        key_func = key['key']
        args = iter(args)
        result = next(args)
        comp = key_func(result)
        for a in args:
            cmp_a = key_func(a)
            if cmp_a > comp:
                result = a
                comp = cmp_a
    else:
        #Common case of two args
        if len(args) == 2:
            #Tail call, see min() for reason.
            return max2(args[0], args[1])
        else:
            args = iter(args)
            result = next(args)
            for a in args:
                result = max2(result, a)
    return result

@c_function
def hasattr(obj, name:str)->bool:
    pass

@method(int_iterator, '__next__')
@_pure
@c_function
def next_int_iterator(self):
    pass
del next_int_iterator

@method(dict, 'clear')
@c_function
def dict_clear(d:dict):
    pass
del dict_clear

def abs(x):
    if hasattr(type(x), '__abs__'):
        return type(x).__abs__(x)
    else:
        raise TypeError("bad operand type for abs(): '%s'" % type(x).__name__)

operator = lib_import('operator')

hex = operator.hex
del operator.hex

oct = operator.oct
del operator.oct

bin = operator.bin
del operator.bin

_index = operator.index

#This function returns either a  bool or an int.
@c_name
@_globals_to_consts
@_no_trace
def truth_surrogate(o):
    # This function must ensure that it returns either a bool or an int
    if o is None:
        return False
    elif o?__bool__:
        t = o$__bool__()
        if t$type is bool:
            return t
        else:
            raise TypeError("__bool__ should return bool, returned %s" %
                            t$type.__name__)
    elif o?__len__:
        l = o$__len__()
        if l$type is int:
            return l
        else:
            return _index(l)
    else:
        return True

del _index

GeneratorType = filter(None, (0,1))$type

@method(GeneratorType, '__next__')
@_globals_to_consts
@_no_trace
def gen_next(gen):
    if gen$type is GeneratorType:
        return $gen_enter(gen)
    else:
        format = "descriptor '__next__' requires a 'generator' object but received a '%s'"
        raise TypeError(format % gen$type.__name__)

del GeneratorType
del gen_next

_NoValue = _engine._NoValue

@c_function
def locals():
    pass

@_globals_to_consts
def vars(obj = _NoValue):
    if obj is _NoValue:
        return locals()
    else:
        try:
            return obj.__dict__
        except:
            raise TypeError('vars() argument must have __dict__ attribute')

iter = _engine.iter

class property(object):
    '''property(fget=None, fset=None, fdel=None, doc=None) -> property attribute

fget is a function to be used for getting an attribute value, and likewise
fset is a function for setting, and fdel a function for del\'ing, an
attribute.  Typical use is to define a managed attribute x:
class C(object):
    def getx(self): return self.__x
    def setx(self, value): self.__x = value
    def delx(self): del self.__x
    x = property(getx, setx, delx, "I\'m the \'x\' property.")'''

    __slots__ = [ '__fget', '__fset', '__fdel', '__doc__' ]

    def __init__(self, fget=None, fset=None, fdel=None, doc=None):
        self.__fget = fget
        self.__fset = fset
        self.__fdel = fdel
        self.__doc__ = doc

    @_globals_to_consts
    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        f = self.__fget
        if f is None:
            raise AttributeError("unreadable attribute")
        return f(obj)

    @_globals_to_consts
    def __set__(self, obj, value):
        f = self.__fset
        if f is None:
            raise AttributeError("can't set attribute")
        f(obj, value)

    @_globals_to_consts
    def __delete__(self, obj):
        f = self.__fdel
        if f is None:
            raise AttributeError("can't delete attribute")
        f(obj)

#_engine.read_only_slot(property, 'fget')
#_engine.read_only_slot(property, 'fset')
#_engine.read_only_slot(property, 'fdel')
#_engine.read_only_slot(property, '__doc__')

sys = lib_import('sys')
sys.modules['operator'] = operator
sys.modules['sys'] = sys
sys.modules['time'] = lib_import('time')
sys.modules['math'] = lib_import('math')
sys.modules['_thread'] = lib_import('_thread')
sys.modules['threading'] = lib_import('threading')
sys.modules['__unsafe'] = lib_import('__unsafe')

_engine.sys = sys
_engine.operator = operator

def reversed(x):
    t = type(x)
    if hasattr(t, '__reversed__'):
        return t.__reversed__(x)
    try:
        i = t.__len__(x)
    except AttributeError:
        raise TypeError('Argument to reversed() must be a sequence')
    while i > 0:
        i -= 1
        yield x[i]

@method(str, 'ljust')
def ljust(self, width, fillchar=' '):
    diff = width - len(self)
    if diff <= 0:
        return self
    else:
        return self + fillchar*diff
del ljust

@method(str, 'rjust')
def rjust(self, width, fillchar=' '):
    diff = width - len(self)
    if diff <= 0:
        return self
    else:
        return fillchar*diff + self
del rjust

@_no_trace
def round(number, ndigits = 0):
    return type(number).__round__(number, ndigits)

@_no_trace
def sum(seq, start = 0):
    # Reject strs & bytes, cause that what it says in the specs.
    t = start$type
    if $subtype(t, str):
        raise TypeError("sum() can't sum strings [use ''.join(seq) instead]")
    if $subtype(t, bytes):
        raise TypeError("sum() can't sum bytes [use b''.join(seq) instead]")
    total = start
    for x in seq:
        total = total + x
    return total

def _dir_type(obj):
    s = set()
    for cls in obj.__mro__:
        for n in cls.__dict__:
            s.add(n)
    return s

def _dir_module(obj):
    d = obj.__dict__
    if $subtype(d$type, dict):
        return list(obj.__dict__)
    else:
        name = obj.__name__
        raise TypeError(name + ".__dict__ is not a dictionary")
    return list(d)

def _generic_dir(obj):
    d = obj.__dict__
    if $subtype(d$type, dict):
        k = d.keys()
    else:
        k = []
    s = _dir_type(type(obj))
    for i in k:
        s.add(i)
    return list(s)

ModuleType = _engine$type

@_globals_to_consts
def dir(obj = _NoValue):
    if obj is _NoValue:
        result = list(locals().keys())
    else:
        t = obj$type
        if t?__dir__:
            result = t$__dir__(obj)
            if not $subtype(result$type, list):
                raise TypeError("__dir__() must return a list, not " +
                                type(result).__name__)
        if $subtype(obj$type, ModuleType):
            result = _dir_module(obj)
        elif $subtype(obj$type, type):
            result = list(_dir_type(obj))
        else:
            result = _generic_dir(obj)
    result.sort()
    return result

del ModuleType

@c_function
def staticmethod(f):
    pass

@c_function
def classmethod(f):
    pass

def sorted(iterable, key=None, reverse=False):
    "sorted(iterable, key=None, reverse=False) --> new sorted list"
    return list(iterable).sort(key, reverse)

FunctionType = print$type
BuiltinFunctionType = locals$type

@c_name
@_globals_to_consts
@_no_trace
def load_special_surrogate(obj, name:str):
    cls = obj$type
    if $has_class_attr(cls, name): # Has class attr
        descriptor = $get_class_attr(cls, name) # Get class attr
        desc_type = descriptor$type
        if desc_type is BuiltinFunctionType:
            return $bind(obj, descriptor) # bind
        elif desc_type is FunctionType:
            return $bind(obj, descriptor) # bind
        elif $has_class_attr(desc_type, '__get__'):
            return $get_class_attr(desc_type, '__get__')(descriptor, obj, cls)
        else:
            return descriptor
    else:
        msg = "'%s' object has no attribute '%s'" % (cls.__name__, name)
        raise AttributeError(msg)


# This is to be used when it is known that it will succeed.
@_globals_to_consts
@_no_trace
def load_special_known(obj, name):
    cls = obj$type
    descriptor = $get_class_attr(cls, name) # Get class attr
    desc_type = descriptor$type
    if desc_type is BuiltinFunctionType:
        return $bind(obj, descriptor) # bind
    elif desc_type is FunctionType:
        return $bind(obj, descriptor) # bind
    elif $has_class_attr(desc_type, '__get__'):
        return $get_class_attr(desc_type, '__get__')(descriptor, obj, cls)
    else:
        return descriptor

@_globals_to_consts
@_no_trace
def generic_getattr(obj, name):
    cls = obj$type
    if $has_class_attr(cls, name): # Has class attr
        descriptor = $get_class_attr(cls, name) # Get class attr
        desc_type = descriptor$type
        if $has_class_attr(desc_type, '__set__'):
            if $has_class_attr(desc_type, '__get__'):
                return $get_class_attr(desc_type, '__get__')(descriptor, obj, cls)
            return $object_dict(obj, name) # Return value if in dict
            return descriptor
        else:
            return $object_dict(obj, name) # Return value if in dict
            desc_type = descriptor$type
            if desc_type is BuiltinFunctionType:
                return $bind(obj, descriptor) # bind
            elif desc_type is FunctionType:
                return $bind(obj, descriptor) # bind
            elif $has_class_attr(desc_type, '__get__'):
                return $get_class_attr(desc_type, '__get__')(descriptor, obj, cls)
            else:
                return descriptor
    else:
        return $object_dict(obj, name) # Return value if in dict
        if obj?__getattr__:
            return obj$__getattr__(name)
        else:
            raise AttributeError("'%s' object has no attribute '%s" %
                                 (cls.__name__, name))

_object_getattr = object.__getattribute__
_type_getattr = type.__getattribute__

@_globals_to_consts
@_no_trace
def type_getattr(cls:type, name):
    if not $subtype(cls$type, type):
        raise TypeError("type.__getattribute__' requires a 'type' object but received a '%s'"
                       % cls$type.__name__)
    meta_cls = cls$type
    if $has_class_attr(meta_cls, name):
        meta_descriptor = $get_class_attr(meta_cls, name)
        meta_desc_type = meta_descriptor$type
        #Overriding meta-descriptor
        if $has_class_attr(meta_desc_type, '__set__'):
            if $has_class_attr(meta_desc_type, '__get__'):
                return $get_class_attr(meta_desc_type, '__get__')(meta_descriptor, cls, meta_cls)
        #Class attribute (i.e. in class dict)
        if $has_class_attr(cls, name):
            descriptor = $get_class_attr(cls, name)
            desc_type = descriptor$type
            if $has_class_attr(desc_type, '__get__'):
                return $get_class_attr(desc_type, '__get__')(descriptor, None, cls)
            return descriptor
        #Non-overriding meta-descriptor
        if $has_class_attr(meta_desc_type, '__get__'):
            return $get_class_attr(meta_desc_type, '__get__')(meta_descriptor, cls, meta_cls)
    else:
        #Class attribute (i.e. in class dict)
        if $has_class_attr(cls, name):
            descriptor = $get_class_attr(cls, name)
            desc_type = descriptor$type
            if $has_class_attr(desc_type, '__get__'):
                return $get_class_attr(desc_type, '__get__')(descriptor, None, cls)
            return descriptor
    raise AttributeError("type '%s' object has no attribute '%s" %
                                 (cls.__name__, name))

@method(FunctionType, '__get__')
@method(BuiltinFunctionType, '__get__')
def _(func, obj, cls):
    if obj is None:
        return func
    else:
        return $bind(obj, func)

@c_name
@_globals_to_consts
@_no_trace
def load_attr_surrogate(obj, name):
    getter = $get_class_attr(obj$type, '__getattribute__') # Get class attr
    if getter is _object_getattr:
        return generic_getattr(obj, name)
    elif getter is _type_getattr:
        return type_getattr(obj, name)
    else:
        return obj$__getattribute__(name)

del FunctionType
del BuiltinFunctionType
del _object_getattr

@_globals_to_consts
@_no_trace
def generic_setattr(obj, name, value):
    cls = obj$type
    if $has_class_attr(cls, name):
        descriptor = $get_class_attr(cls, name) # Get class attr
        desc_type = descriptor$type
        if $has_class_attr(desc_type, '__set__'):
            $get_class_attr(desc_type, '__set__')(descriptor, obj, cls)
        if obj?__dict__:
            $object_dict(obj, name) = value
            # if obj?__dict__: obj$__dict__[name] = value else: raise Error
        else:
            raise AttributeError("'%s' object attribute '%s' is read-only" %
                             (cls.__name__, name))
    else:
        if obj?__dict__:
            $object_dict(obj, name) = value # if obj?__dict__: obj$__dict__[name] = value else: raise Error
        else:
            raise AttributeError("'%s' object has no attribute '%s" %
                                (cls.__name__, name))

_object_setattr = object.__setattr__

@c_name
@_globals_to_consts
@_no_trace
def store_attr_surrogate(value, obj, name):
    setter = $get_class_attr(obj$type, '__setattr__')
    if setter is _object_setattr:
        generic_setattr(obj, name, value)
    else:
        obj$__setattr__(name, value)
    return pass


@method(slice.step$type, '__get__')
@c_name
@_no_trace
def slot_descriptor_get(desc, obj, cls):
    return $slot(desc, obj)

@method(slice.step$type, '__set__')
@c_name
@_no_trace
def slot_descriptor_set(desc, obj, value):
    $slot(desc, obj) = value

del _object_setattr

def print_bytecodes(f):
    print (f.__name__)
    code = f.__code__.code
    for i in range(len(code)):
        print ("%d, " % code[i])

#print_bytecodes(generic_getattr)
#print_bytecodes(generic_setattr)
#print_bytecodes(load_attr_surrogate)
#print_bytecodes(store_attr_surrogate)
del generic_getattr
del generic_setattr


make_immutable(int)
make_immutable(float)
make_immutable(sys$type)
make_immutable(BaseException)
make_immutable(Exception)
make_immutable(type)
make_immutable(str)
make_immutable(dict)
make_immutable(make_immutable$type) # Builtin(C) function
make_immutable(reversed(())$type) # Generator
make_immutable(print$type) # Python function
make_immutable(slice)
make_immutable(str_buf)
#TO DO -- More types to make immutable?

del operator
del sys
del dict_proxy
del int_iterator
del listiterator
del _pure
del _no_raise
del type_state
del type_info



