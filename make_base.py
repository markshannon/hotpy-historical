import objects, sys, common, gtypes, getopt
import ast

FINALIZE_FLAG = 1

class array(object):

    def __init__(self, data):
        self.data = data

class frame(object):
    pass

def make_annotations(func, types):
    a = {}
    try:
        i = 0
        for n in func.func_code.co_varnames:
            a[n] = types[i]
            i += 1
    except __builtins__.IndexError:
        pass
    return a

def gen(func):
    yield 0

class builtin_function(object):

    def __init__(self, name, format, p, t):
        self.types = t
        self.params = p
        self.__name__ = name
        self.format = format

c_function = builtin_function('c_function', 0, ('function',), (make_annotations.__class__,))

object_allocate = builtin_function('object_allocate', 0, ('cls',), (type,))
object_init = builtin_function('object_init', 0, ('self',), (object,))

class code_object(object):

    def __init__(self, data):
        self.func = data

class set(object):
    pass

class frozenset(object):
    pass

class complex(object):
    pass

class class_method(object):
    pass

class static_method(object):
    pass

class specialiser(object):
    pass

class operator(object):
    pass

class binary_operator(operator):
    pass

class comparison_operator(binary_operator):
    pass

class unary_operator(operator):
    pass

class partial_operator(object):
    pass

class traceback(object):
    pass

class range_iterator(object):
    pass

class char_array(object):
    pass

class getset_wrapper(object):
    pass

class binary_api(object):
    pass

class binary_pred_api(object):
    pass

class binary_r_api(object):
    pass

class objint_api(object):
    pass

class objintobj_api(object):
    pass

class setitem_api(object):
    pass

class deferred(object):
    pass

class deferred_frame(object):
    pass

class deferred_state(object):
    pass

class unary_api(object):
    pass

class unary_pred_api(object):
    pass

class len_api(object):
    pass

class new_api(object):
    pass

class method_api(object):
    pass

class compare_api(object):
    pass

class hash_api(object):
    pass

class dict_iterator(object):
    pass

class scopes(object):
    pass

class dict_proxy_iterator(object):
    pass

class dict_values_iterator(object):
    pass

class tupleiterator(object):
    pass

class exception_handler(object):
    pass

class thread_state(object):
    pass

class listiterator(object):
    pass

class trace_record(object):
    pass

class int_iterator(object):
    pass

class dict_proxy(object):

    def __init__(self, cls):
        self.type = cls

class code_builder(object):
    pass

class dict(object):

    def __init__(self, data):
        self.values = dict_values(data)

canonicals = {}

class generator(object):
    pass

class guard(object):
    pass

class ellipsis(object):
    pass

class symtable(object):
    pass

class symtable_scope(object):
    pass

class str_iterator(object):
    pass

class dict_keys(object):

    def __init__(self, data):
        self.data = data

class dict_values(object):

    def __init__(self, data):
       self.keys = dict_keys(data)
       self.data = data

class module_dict_values(object):

    def __init__(self, data):
        self.keys = dict_keys(data)
        self.data = data

class hashtable(object):
    pass

class bound_method(object):
    pass

class range(object):
    pass

class slice(object):
    pass

class Thread(object):
    pass

class Lock(object):
    pass

class str_buf(object):
    pass

class Condition(object):
    pass

class int_vector(object):

    def __init__(self, data):
        self.data = data

class Constants(object):
    pass

class slot_descriptor(object):

    def __init__(self, owner, name, ro, slot = None):
        self.name = name
        self.owner = owner
        if ro:
            self.writeable = 0
        else:
            self.writeable = 1
        if slot is None:
            self.slot = name
        else:
            self.slot = slot
        self.type = None

class class_descriptor(object):
    pass

CLASS_DESCRIPTOR = class_descriptor()

class int_slot_descriptor(object):

    def __init__(self, owner, name, ro, slot = None):
        self.name = name
        self.owner = owner
        if ro:
            self.writeable = 0
        else:
            self.writeable = 1
        if slot is None:
            self.slot = name
        else:
            self.slot = slot

class call_descriptor(object):
    pass

class dict_descriptor(object):
    pass

class Lexer(object):
    pass

class Token(object):
    pass

class bytes(object):

    def __init__(self, data):
        self.data = data

class bytearray(bytes):
    pass

class bytelist(object):
    pass

class BaseException(object):
    pass

class Exception(BaseException):
    pass

class StopIteration(Exception):
    pass

class TypeError(Exception):
    pass

class NameError(Exception):
    pass

class KeyError(Exception):
    pass

class SyntaxError(Exception):
    pass

class ValueError(Exception):
    pass

class MemoryError(Exception):
    pass

class GeneratorExit(Exception):
    pass

class ImportError(Exception):
    pass

class IOError(Exception):
    pass

class AttributeError(Exception):
    pass

class UnboundLocalError(Exception):
    pass

class InternalError(Exception):
    pass

class IndexError(Exception):
    pass

class KeyboardInterrupt(Exception):
    pass

class RuntimeError(Exception):
    pass

class OverflowError(Exception):
    pass

class SystemExit(BaseException):
    pass

class ZeroDivisionError(Exception):
    pass

class info_item0(object):
    pass

class info_item1(object):
    pass

class info_item2(object):
    pass

class info_item3(object):
    pass

class type_info(object):
    pass

class frozen_type_info(type_info):
    pass

class type_state(object):
    pass

class exec_link(object):
    pass

class mono_exec_link(exec_link):
    pass

class poly_exec_link(exec_link):
    pass

class trace(object):
    pass

IMMUTABLES = (int, float, tuple, str)

def are_the_same(item, other):
    if type(item) is type(other):
        if type(item) in IMMUTABLES:
            return item == other
        else:
            return item is other
    else:
        return False

class identity_dict:

    def __init__(self):
        self.list = []

    def __setitem__(self, item, value):
        for i in self.list:
            if are_the_same(item, i[0]):
                i[1] = value
                return
        self.list.append([item, value])

    def __contains__(self, item):
        for i in self.list:
            if are_the_same(item, i[0]):
                return True
        return False

    def __getitem__(self, item):
        for i in self.list:
            if are_the_same(item, i[0]):
                return i[1]
        raise KeyError, item

class identity_set:

    def __init__(self):
        self.map = identity_dict()

    def add(self, item):
        self.map[item] = None

    def __contains__(self, item):
        return item in self.map

scanned = identity_set()
scanned.add(type.__dict__.__class__)
scanned.add(str.__mro__[1])

next_id = 0;

ids = identity_dict()

import re
ID = re.compile(r'^[a-zA-Z_]\w*$')

EXTERNAL_OBJECTS = __builtins__.set([ '', () ])

def external_defn(obj):
    'Return True if object is defined externally (in C code)'
    try:
        return obj == {} or obj in EXTERNAL_OBJECTS
    except:
        return False

def obj_name(obj):
    global next_id
    if hasattr(obj, '__name'):
        return obj.__name
    if obj is None:
        return 'NONE'
    if obj is NotImplemented:
        return 'NOT_IMPLEMENTED'
    if obj is False:
        return '_false'
    if obj is True:
        return '_true'
    if obj == {}:
        return 'empty_dict_table_object'
    if obj == ():
        return 'empty_tuple_object'
    if obj is object_allocate:
        return 'func_object_allocate'
    if obj is object_init:
        return 'func_object_init'
    if type(obj) is str:
        if ID.match(obj):
            return 'str_%s' % obj
        elif obj == '':
            return 'empty_string_object'
    elif type(obj) is type:
        return '_%s' % class_name(obj)
    elif type(obj) is __builtins__.__class__:
        if obj.__name__ == '__builtin__':
            return 'module_builtins'
        else:
            return 'module_%s' % obj.__name__
#    elif type (obj) is int_vector and len(obj.data) == 0:
#        return 'empty_int_vector'
    if obj not in ids:
        next_id += 1
        ids[obj] = '%s_%s' % (obj_name(type(obj)), next_id)
    return ids[obj]

def type_dict(t):
    return {}

def skip(t):
    return None

base_classes = { str : (object,), dict : (object,) }

def bases(t):
    if t in base_classes:
        return base_classes[t]
    else:
        return t.__bases__

# Copied from http://www.python.org/download/releases/2.3/mro/
def merge(seqs):
    #print '\n\nCPL[%s]=%s' % (seqs[0][0],seqs),
    res = []; i=0
    while 1:
      nonemptyseqs=[seq for seq in seqs if seq]
      if not nonemptyseqs: return res
      i+=1; #print '\n',i,'round: candidates...',
      for seq in nonemptyseqs: # find merge candidates among seq heads
          cand = seq[0]; #print ' ',cand,
          nothead=[s for s in nonemptyseqs if cand in s[1:]]
          if nothead: cand=None #reject candidate
          else: break
      if not cand: raise "Inconsistent hierarchy"
      res.append(cand)
      for seq in nonemptyseqs: # remove cand
          if seq[0] == cand: del seq[0]

def _mro(C):
    "Compute the class precedence list (mro) according to C3"
    return merge([[C]]+map(_mro,bases(C))+[list(bases(C))])

def mro(c):
    return tuple(_mro(c))

def roundup(x, r):
    while x % r:
        x += 1
    return x

def get_offset_of(cls, name):
    entries = get_struct(cls).entries
    offset = 0
    for e in entries:
        if e.name == name:
            return offset
        else:
            offset = roundup(offset, e.tipe.size)
            if e.count == 0:
                return 0
            if e.count:
                offset += e.tipe.size * int(e.count)
            else:
                offset += e.tipe.size
    return 0

def class_name(t):
    if t in class_names:
        return class_names[t]
    elif hasattr(t, '__name'):
        return t.__name[1:]
    else:
        return t.__name__

def get_struct(t):
    name = class_name(t)
    if name not in structs:
        name = class_name(t.__bases__[0])
    if name not in structs:
        name = class_name(t.__bases__[0].__bases__[0])
    if name in structs:
        return structs[name]
    else:
        raise __builtins__.Exception("No structure definition for %s" % name)

type_flags = {
    int : 1 << 8,
    list: 1 << 9,
    tuple: 1 << 10,
    bytes: 1<<11,
    bytearray: 1<<11,
    str: 1 << 12,
    dict: 1 << 13,
    BaseException: 1 << 14,
    type: 1 << 15
}

REF_COUNT_FLAG = 2

# Mapping c field-name -> python descriptor name
descriptor_names = {
    'ob_type' : '__class__'
}

def write_type(t):
    st = get_struct(t)
    shape = st.shape
    s = []
    for v in shape:
        s.append('%d' % v)
    assert len(shape) < 7
    if len(shape) == 6:
        if shape[-1]:
            print t.__name__
        assert shape[-1] == 0
        val('shape', s[:-1])
    else:
        s += [ '0'] * (5 - len(s))
        val('shape', s)
    if shape[-1] == 0:
        if st.entries[-1].tipe == gtypes.r:
            val('variable', st.entries[-1].tipe.size)
        else:
            val('variable', -st.entries[-1].tipe.size)
    else:
        val('variable', 0)
    length = int(s[0]) - int(s[1]) + int(s[2]) - int(s[3]) + int(s[4])
    length *= gtypes.r.size
    val('length', length)
    index = 0
    if t in descriptors:
        desc_dict = descriptors[t]
    else:
        desc_dict = {}
    for item in st.entries:
        if item.comment and ('read-only' in item.comment or 'read-write' in item.comment):
            if item.name in descriptor_names:
                desc_name = descriptor_names[item.name]
            else:
                desc_name = item.name
            if desc_name == '__class__':
                d = CLASS_DESCRIPTOR
            elif item.tipe == gtypes.r:
                d = slot_descriptor(t, desc_name, 'read-only' in item.comment, item.name)
            elif item.tipe == gtypes.i4 or item.tipe == gtypes.u4:
                d = int_slot_descriptor(t, desc_name, 'read-only' in item.comment, item.name)
            else:
                raise __builtins__.Exception(
                    "Don't know what to do with slot of type %s" % item.tipe)
            desc_dict[desc_name] = d
            others.append(d)
    val('dict_offset', get_offset_of(t, 'dictionary'))
    val('unique_index', uid(t))
    if BaseException in t.__mro__:
        val('marshaller', 'gvmt_marshal_BaseException')
    elif st.qualifiers and 'special' in st.qualifiers:
        val('marshaller', 0)
    else:
        val('marshaller', 'gvmt_marshal%s' % obj_name(t))
    flag = 0
    for super_type in t.__mro__:
        if super_type in type_flags:
            flag = type_flags[super_type]
    if has_ref_count(t):
        flag |= REF_COUNT_FLAG
    val('flags', flag)
    val('tp_name', 0)
    if t in actions['call'] or  t.__name__.endswith('_api'):
        val('call', 'desc_call_%s' % (class_name(t)))
    elif t is object:
        val('call', 'not_callable')
    else:
        val('call', 0)
    for action in ['get', 'set']:
        if t in actions[action]:
            val(action, 'desc_%s_%s' % (action, class_name(t)))
        else:
            val(action, 0)
    if t in unhashable:
        val('hash', 'HotPyObject_HashNotImplemented')
    elif t in hash_functions:
        val('hash', 'hotpy_%s_hash' % class_name(t))
    else:
        val('hash', 0)
    val('lock', 1)
    if t is type:
        val('__getattribute__', 'type_getattribute')
    elif t is object:
        val('__getattribute__', '_HotPyObject_GenericGetAttr')
    else:
        val('__getattribute__', 0)
    if t is type:
        val('__setattr__', 'type_setattr')
    elif t is object:
        val('__setattr__', '_HotPyObject_GenericSetAttr')
    else:
        val('__setattr__', 0)
    val('api_getattro', 0)
    val('api_setattro', 0)
    val('api_call', 0)
    proxy = dict_proxy(t)
    val('__dict__', obj_name(proxy))
    val('__name__',  obj_name(class_name(t)))
    others.append(class_name(t))
    val('attributes', obj_name(desc_dict))
    others.append(desc_dict)
    val('__bases__',  obj_name(bases(t)))
    others.append(bases(t))
    m = mro(t)
    val('__mro__',  obj_name(m))
    guards = dict({})
    val('guards', obj_name(guards))
    val('override_getattr', 0)
    val('keys_cache', 'empty_dict_keys_object')
    val('surrogate_new', 0)
    others.append(m)
    others.append(guards)
    others.append(proxy)

def write_list(l):
    a = array(l)
    val('lock', 1)
    val('array',  obj_name(a))
    others.append(a)

def write_tuple(t):
    val('length', len(t))
    s = []
    for i in t:
        s.append(obj_name(i))
    val('array', s)

def write_array(a):
    length = 2
    while length < len(a.data):
        length *= 2
    val('length', length)
    val('a_size', len(a.data))
    s = []
    for i in a.data:
        s.append(obj_name(i))
    for i in __builtins__.range(len(a.data), length):
        s.append(0)
    val('array', s)

def write_slots(obj):
    items = slots[type(obj)]
    for name in items:
        val(name, obj_name(getattr(obj, items[name])))

def write_code(c):
    val('kind', str(1 << 16))
    val('params', len(c.func.func_code.co_varnames))
    val('locals', len(c.func.func_code.co_varnames))
    val('code', 'empty_bytes')
    val('name', 'str_%s' % c.func.__name__)
    val('constants', 'empty_constants');
    val('local_names', 'empty_tuple_object')
    val('execution_count', 0)
    val('bytecodes', 0)

def write_builtin_func(f):
    val('parameter_format', f.format)
    val('executable', 'PY_%s' % f.__name__)
    val('__name__', 'str_%s' % f.__name__)
    val('source_name', 'str_%s' % f.__name__)
    val('paramNames', obj_name(f.params))
    val('param_types', obj_name(f.types))
    val('defaults', 'empty_tuple_object')
    val('__doc__', 'empty_string_object')
    val('return_type', 0)
    others.append(f.types)
    others.append(f.params)
    others.append(f.__name__)

def write_func(f):
    flags = f.func_code.co_flags & 12
    val('parameter_format', flags)
    val('executable', 'PY_%s' % f.__name__)
    c = code_object(f)
    val('__code__', obj_name(c))
    val('__globals__', 'module_builtins_dict')
    val('__builtins__', 'module_builtins_dict')
    val('__name__', 'str_%s' % f.__name__)
    val('paramNames', obj_name(f.func_code.co_varnames))
    if f in func_types:
        types = func_types[f]
        annotations = dict(make_annotations(f, types))
    else:
        annotations = dict({})
    val('__annotations__',  obj_name(annotations))
    val('defaults', 'empty_tuple_object')
    val('optimised', 0)
    val('byte_code_length', 0)
    val('bytecodes', 0)
    val('instrumentation', 0)
    val('guard_ring', 0)
    others.append(c)
    others.append(annotations)
    others.append(f.func_code.co_varnames)
    others.append(f.__name__)

def write_int(i):
    top_bits = (i >> 30) & 3
    assert top_bits == 0 or top_bits == 3
    val('length', 1)
    val('digits', [i])

def write_float(i):
    val('value', '%f' % i)

def write_bool(b):
    val('always_one', 1)
    val('value', int(b))

def hotpy_hash(s):
    h = 0
    for c in s:
        h = (((h << 5) | ((h >> 27) & 0x1f)) + ord(c)) & 0xffffffff ;
    return h

def nextPower2(x):
    r = 1
    while r < x:
        r *= 2
    return r

def make_module_table(m):
    d = {}
    for k in m.__dict__:
        v = m.__dict__[k]
        if type(v) is str:
            d[k] = v
        for t in (type, str, int, float, bool, object, bytes, dict_proxy,
                  None.__class__, slice,
                  dict_iterator, array, Thread, bytearray,
                  tupleiterator, range, KeyboardInterrupt,
                  Exception, int_iterator, frame, list, tuple, str_buf,
                  dict, file, generator, listiterator, complex, set,
                  frozenset,
                  BaseException, StopIteration, type_state, type_info):
            d[class_name(t)] = t
        d['c_function'] = c_function
        d['NotImplemented'] = NotImplemented
        d['None'] = None
        d['True'] = True
        d['False'] = False
    others.append(d)
    return d

def write_module(m):
    table = make_module_table(m)
    proxy = dict(table)
    proxy.values = module_dict_values(table)
    proxy.__name =  'module_builtins_dict'
    others.append(proxy)
    val('dictionary', obj_name(proxy))
    val('filename', obj_name('builtin/builtins.py'))

def write_dict(d):
    val('lock', 1)
    val('values', obj_name(d.values))

def insert(table, s, v):
    perturb = hotpy_hash(s)
    i = perturb & (len(table) - 1)
    while table[i]:
        i = (5 * i + perturb + 1) & (len(table) - 1)
        perturb //= 32
    table[i] = (s, v)

def make_table(d, min_size):
    entries = nextPower2(max((len(d) * 3)//2, min_size))
    s = []
    table = [None for x in xrange(entries)]
    for k in d:
        insert(table, k, d[k])
    assert len(table) == entries
    assert len(table) > 2
    return table

def write_dict_keys(k):
    d = k.data
    table = make_table(d, 4)
    entries = len(table)
    val('length', entries)
    if len(d):
        val('load', (entries+1) * 2//3)
    else:
        # Empty dict.
        val('load', 0)
    val('used', len(d))
    assert len(d) <= (entries+1) * 2//3
    s = []
    for e in table:
        if not e:
            s.append(0)
        else:
            s.append(obj_name(e[0]))
    val('array', s)

def write_dict_values(v):
    d = v.data
    table = make_table(d, 4)
    entries = len(table)
    val('length', entries)
    val('size', len(d))
    if len(d):
        val('keys', obj_name(v.keys))
    else:
        val('keys', 'empty_dict_keys_object')
    s = []
    for e in table:
        if not e:
            s.append(0)
        else:
            s.append(obj_name(e[1]))
    val('array', s)

def write_module_dict_values(v):
    d = v.data
    table = make_table(d, 16)
    entries = len(table)
    val('length', entries)
    val('size', len(d))
    val('keys', obj_name(v.keys))
    if table[0]:
        val('extra1', obj_name(table[0][1]))
    else:
        val('extra1', 0)
    if table[1]:
        val('extra2', obj_name(table[1][1]))
    else:
        val('extra2', 0)
    s = []
    for e in table[2:]:
        if not e:
            s.append(0)
        else:
            s.append(obj_name(e[1]))
    s.append(0)
    s.append(0)
    val('array', s)


def write_dict_proxy(d):
    val('type', obj_name(d.type))

def write_dict_table(d):
    table = make_table(d, 8)
    entries = len(table)
    val('length', entries * 2)
    val('size', len(d))
    if len(d):
        val('load', entries * 2//3)
    else:
        # Empty dict.
        val('load', 0)
    assert entries >= len(d) * 3//2
    val('used', len(d))
    s = []
    for e in table:
        if not e:
            s.append(0)
            s.append(0)
        else:
            s.append(obj_name(e[0]))
            s.append(obj_name(e[1]))
    val('array', s)

def write_str(s):
    canonicals[s] = None
    val('hashcode',  '%d' % hotpy_hash(s))
    val('length', '%d' % len(s))
    text = []
    for i in s:
        text.append('%d' % ord(i))
    val('text', text)

def write_bytes(b):
    x = b.data >> 8
    l = 1
    while x:
        l += 1
        x >>= 8
    val('length', '%d' % l)
    bytes = []
    for i in xrange(0, l):
        bytes.append('%d' % ((b.data >> (i * 8)) & 0xff))
    val('bytes', bytes)

def write_descriptor(d):
    val('offset', str(get_offset_of(d.owner, d.slot)))
    val('__doc__', obj_name(''))
    val('name', 'str_%s\n' % d.name)
    val('flags', d.writeable)
    if d.type:
        val('type', obj_name(d.type))
    else:
        val('type', 0)

def write_int_descriptor(d):
    val('offset', str(get_offset_of(d.owner, d.slot)))
    val('__doc__', obj_name(''))
    val('name', 'str_%s\n' % d.name)
    val('flags', d.writeable)

def write_exception(e):
    val('__traceback__', obj_name(None))
    val('args', obj_name(None))
    d = dict({})
    val('dictionary', obj_name(d))
    others.append(d)

functions = {
    tuple : write_tuple,
    object : skip,
    type : write_type,
    dict : write_dict,
    type.__dict__.__class__ : write_dict_table,
    Exception : write_slots,
    int : write_int,
    float : write_float,
    list: write_list,
    array : write_array,
    code_object : write_code,
    obj_name.__class__ : write_func, # function
    "".__class__ : write_str,
    bool : write_bool,
    __builtins__.dict : write_dict_table,
    None.__class__ : skip,
    bytes: write_bytes,
    bytearray: write_bytes,
    class_descriptor : skip,
    slot_descriptor : write_descriptor,
    int_slot_descriptor : write_int_descriptor,
    __builtins__.__class__ : write_module,
    call_descriptor: skip,
    dict_descriptor: skip,
    NotImplemented.__class__ : skip,
    builtin_function: write_builtin_func,
    dict_keys: write_dict_keys,
    dict_values: write_dict_values,
    module_dict_values: write_module_dict_values,
    dict_proxy: write_dict_proxy,
    StopIteration: write_exception
}

class_names = {
    type.__dict__.__class__ : 'dict_table',
    __builtins__.dict : 'dict_table',
}

def scan_array(a):
    scan_tuple(a.data)

def scan_tuple(t):
    for i in t:
        scan(i)

def scan_dict_table(d):
    for n in d:
        scan(n)
        scan(d[n])

def scan_dict(d):
    scan(d.values)

def scan_dict_values(d):
    scan(d.keys)


overrides = {
    type.__dict__.__class__ : scan_dict_table,
    __builtins__.dict : scan_dict_table,
    tuple : scan_tuple,
    array : scan_array,
    dict : scan_dict,
    __builtins__.__class__ : skip,
    scan_dict.__class__ : skip,
    code_object : skip,
    type : skip,
    list : skip,
    str: skip,
    int: skip,
    bytes: skip,
    bytearray: skip,
    builtin_function: skip,
    dict_values: scan_dict_values,
    module_dict_values: scan_dict_values,
    dict_keys: skip,
    StopIteration: skip
}

def is_immortal(x):
    if x in (None, True, False):
        return True
    if isinstance(x, int) and -128 <= x < 128:
        return True
    if isinstance(x, type):
        return True
    return False


def has_ref_count(cls):
    st = get_struct_for_class(cls)
    for item in st.entries:
        if item.name == 'ref_count':
            return True
    return False

no_ref_count = __builtins__.set(
    [info_item0, info_item2, info_item2, info_item3,
     dict_values, module_dict_values, dict_values,
     type.__dict__.__class__, exception_handler,
     __builtins__.dict, dict_keys ])

def scan(obj, name = None):
    if external_defn(obj) or obj in scanned:
        return
    scanned.add(obj)
    gsc_file << '// ' << str(obj).replace('\n', '\n//') << ' \n'
    if name is None:
        assert not external_defn(obj)
        gsc_file<< '.public %s\n' % obj_name(obj)
        gsc_file<< '.object %s\n' % obj_name(obj)
    else:
        assert not external_defn(obj)
        gsc_file<< '.public %s\n' % name
        gsc_file<< '.object %s\n' % name
    val('ob_type', obj_name(obj.__class__))
    if has_ref_count(obj.__class__):
        if is_immortal(obj):
            val('ref_count', 1 << 31)
        else:
            val('ref_count', 0)
    if type(obj) in functions:
        functions[type(obj)](obj)
    else:
        raise __builtins__.TypeError, "Don't know how to handle %r" %  obj
    flush(obj.__class__)
    gsc_file<< '.end\n'
    scan(obj.__class__)
    struct = get_struct_for_class(obj.__class__)
    if type(obj) in overrides:
        overrides[type(obj)](obj)
        return
    for item in struct.entries:
        if item.tipe == gtypes.r and item.name != 'ob_type':
            scan(getattr(obj, item.name))

type_slots  = { '__mro__' : '__mro__',  '__bases__' : '__bases__',  }

current = {}

def ref(obj, name, pname= None):
    if not pname:
        pname = name
    val(name, obj_name(getattr(obj, pname)))

def val(name, v):
    global current
    current[name] = v
#    gsc_file<< '    .%s = %s,\n' % (name, v)

def get_struct_for_class(cls):
    for st in cls.__mro__:
        if class_name(st) in structs:
            return structs[class_name(st)]
    raise __builtins__.Exception("There is no type named %s" %  cls_name)

def flush(cls):
    cls_name = class_name(cls)
    global current
#    print cls_name, current
    s = get_struct_for_class(cls)
    # Need to get list of names for this type and emit in correct order...
    for e in s.entries:
        if e.name in current:
            if e.tipe == gtypes.r or e.tipe == gtypes.p:
                tname = 'address'
            else:
                tname = e.tipe.name
            if e.count is not None:
                gsc_file<< '//%s {\n' % e.name
                lst = current[e.name]
                if e.count != 0:
                    if len(lst) != e.count:
                        print ("Incorrect number of entries for %s: %d," +
                               "should be %d") % (e.name, len(lst), e.count)
                        sys.exit(1)
                for i in lst:
                    gsc_file<< '    %s %s\n' % (tname, i)
                gsc_file<< '//} %s\n' % e.name
            else:
                gsc_file<< '// %s\n'% e.name
                gsc_file<< '%s %s\n' % (tname, current[e.name])
            del current[e.name]
        else:
            print "No '%s' in class '%s'" % (e.name, cls_name)
            sys.exit(1)
    if current:
        for name in current:
            print "%s defined, but not in cls %s" % (name, cls_name)
        sys.exit(1)
    current = {}

NONE_TYPE_ID = 1
next_uid = NONE_TYPE_ID + 1
uids = {}

def uid(t):
    global next_uid
    if t == None.__class__:
        return NONE_TYPE_ID
    if t not in uids:
        uids[t] = next_uid
        next_uid += 1
    return uids[t]

opts, args = getopt.getopt(sys.argv[1:], 'o:')
gsc_file = None
for opt, value in opts:
    if opt == '-o':
        gsc_file = common.Out(value)
if not gsc_file:
    print "No output file defined"
    sys.exit(1)

structs = {}
slist = []
for f in args:
    slist += objects.read_file(common.In(f))
for s in slist:
    structs[s.name] = s

cd = call_descriptor()

descriptors = {
    type:  {'__call__': cd,
             '__getattribute__': builtin_function('type_getattribute', 0, ('object', 'name'), (object,object)),
             '__setattr__': builtin_function('type_setattr', 0, ('object', 'name', 'value'), (object,object,object)),
             '__delattr__': builtin_function('type_delattr', 0, ('object', 'name'), (object,object))},
    scan.__class__: {'__call__': cd},
    builtin_function: {'__call__': cd},
    bound_method: {'__call__': cd},
    generator: {'__call__': cd},
    object: {'__dict__': dict_descriptor(),
             '__getattribute__': builtin_function('object_getattribute', 0, ('object', 'name'), (object,object)),
             '__setattr__': builtin_function('object_setattr', 0, ('object', 'name', 'value'), (object,object,object)),
             '__delattr__': builtin_function('object_delattr', 0, ('object', 'name'), (object,object))},
    bound_method: {'__func__': slot_descriptor(bound_method, '__func__', True, 'callable')},
    static_method: {'__func__': slot_descriptor(static_method, '__func__', True, 'callable')},
    class_method: {'__func__': slot_descriptor(class_method, '__func__', True, 'callable')},
    file: { 'read': builtin_function('read', 0, ('self',), (file,)),
            '_read_as_utf8': builtin_function('_read_as_utf8', 0, ('self',), (file,))},
    #This is required for annotations & dict consts.
    dict: { '__setitem__': builtin_function('setitem_dict', 0, ('self', 'key', 'value'), (dict,)) }
}

slots = {
          object : {},
          type : type_slots,
          Exception : {},
          int : {},
          float : {},
          list: {},
          array : {},
          code_object : {},
          builtin_function : {}, # builtin_function
          scan.__class__ : {}, # function
          "".__class__ : {},
          bool : {}
}

others = []

f = open('standard_names.txt')
standard_names = []
index = 0
for name in f:
    n = name.strip('\n')
    if n and n[0] != '#':
        standard_names.append(n)
f.close()

f = open('operator_names.txt')
operator_names = []
index = 0
for name in f:
    n = name.strip('\n')
    if n and n[0] != '#':
        operator_names.append(n)
f.close()

gsc_file<< '.heap\n'

actions = {}
actions['get'] = [ scan.__class__, bound_method, builtin_function,
                   call_descriptor, slot_descriptor, int_slot_descriptor,
                   method_api, class_method, static_method, class_descriptor,
                   dict_descriptor, getset_wrapper ]
actions['call'] = [ scan.__class__, type, bound_method, builtin_function,
                   call_descriptor, unary_operator,
                   binary_operator, operator ]
actions['set'] = [ call_descriptor, slot_descriptor, getset_wrapper,
                   dict_descriptor, int_slot_descriptor, class_descriptor ]

hash_functions = [ object, int, float, tuple, bool, None.__class__, bytes, str, type_state, frozen_type_info ]

unhashable = [ list, array, set, BaseException, dict, bytearray ]

objects = [object, type, Exception, int, float, tuple, list, bool, array, set,
           True, False, scopes, str_iterator, frozenset, GeneratorExit,
           code_object, frame, scan.__class__, 0, {}, (), '', partial_operator,
           int_vector, Constants, None, dict, dict_proxy_iterator,
           generator, hashtable, '__main__',
           bound_method, int_slot_descriptor, traceback,
           slot_descriptor, call_descriptor, dict_descriptor, NotImplemented,
           __builtins__, file, c_function, char_array, int_iterator, unary_api,
           dict_iterator, StopIteration, TypeError, NameError, KeyError,
           ValueError, MemoryError, ImportError, KeyboardInterrupt, guard,
           IOError, AttributeError, UnboundLocalError, InternalError, bytes,
           OverflowError, ZeroDivisionError, 'builtins', '__metaclass__',
           '__index__', range, 'raise', 'call', 'trace', 'run', deferred,
           'profile', 'return', '__del__', exception_handler, Lexer, Token,
           bytearray, thread_state, getset_wrapper, ellipsis,
           slice, IndexError, listiterator, tupleiterator, len_api,
           operator, binary_operator, unary_operator, RuntimeError, Thread,
           Lock, Condition, str_buf, SyntaxError, dict_keys, dict_values,
           range_iterator, dict_values_iterator, builtin_function, complex,
           trace_record, 'builtin/builtins.py', '__doc__', '__package__',
           info_item0, info_item1, info_item2, info_item3, binary_api,
           setitem_api, binary_r_api, objint_api, objintobj_api, new_api,
           binary_pred_api, unary_pred_api, hash_api, compare_api, SystemExit,
           '__ixor__', '__ior__', '__iand__', '__rfloordiv__', '__ifloordiv__',
           'kws', 'p0', method_api, class_method, static_method, bytelist,
           type_info, type_state, comparison_operator, specialiser, trace,
           exec_link, mono_exec_link, poly_exec_link, module_dict_values,
           frozen_type_info, deferred_frame, deferred_state, symtable,
           symtable_scope, code_builder ]

roots = {'canonical_strings': 'canonical_string_dict'}
scan(object_allocate, 'func_object_allocate')
scan(object_init, 'func_object_init')
scan(tuple(standard_names), 'standard_names_table')
scan(tuple(operator_names), 'operator_names_table')
scan([], 'sys_argv_list')
_stop = StopIteration()
scan(_stop, "stop_iteration_object")
for name in dir(ast):
    o = ast.__dict__[name]
    if type(o) == type:
        o.__name = '_AST_' + o.__name__
        scan(o)
for o in objects:
    scan(o)
while others:
    scan(others.pop())

scan(dict(canonicals), 'canonical_string_dict')
scan(1<<27)
scan(dict_keys({}), 'empty_dict_keys_object')
scan(dict_values({}), 'empty_dict_values_object')
scan(module_dict_values({}), 'empty_module_dict_values_object')
gsc_file<< '.roots\n'
for label in roots:
    gsc_file<< '.public %s\n' % label
    gsc_file<< '.label %s\n' % label
    gsc_file<< 'address %s\n' % roots[label]
gsc_file<< '.opaque\n'
gsc_file<< '.public global_class_count\n'
gsc_file<< '.label global_class_count\n'
gsc_file<< 'uint32 %d\n' % next_uid
gsc_file.close()

