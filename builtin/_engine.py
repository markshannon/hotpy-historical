# Various odds and ends that mostly ought to be elsewhere
# Plus a few things that HAVE to be loaded early, for the VM to
# bootstrap properly.

def method_not_found():
    raise TypeError("Method not found")

@c_function
def _no_overflow(f:method_not_found$type):
    pass

@c_function
def _no_raise(f:c_name$type):
    ''' Cannot raise an exception or, by implication
        call back into python code'''
    pass

@_pure
@c_function
def print_string(s:str):
    pass

def _new_exception(cls, msg):
    return cls(str(msg))

def _raise(cls, msg):
    raise cls(str(msg))

c_name(_raise)

SOURCE = 0
COMPILED = 1

@c_function
def path_exists(path:str):
    pass

@c_function
def path_getmtime(path:str):
    pass

@c_function
def antlr_parse(pathname:str):
    pass

@c_function
def build_module(bytecodes:bytes, name:str, filename:str):
    pass

_TAG = '.hotpy'

def parse(pathname):
    antlr_parse(pathname)
    compiled = pathname + _TAG + '.pyc'
    return open(compiled, 'rb').read()

@_no_trace
def run_script(name):
    file, pathname, description = imp_find_module(name, sys.path)
    imp_load_module('__main__', file, pathname, description)

c_name(run_script)

@_no_trace
def imp_find_module(name, path):
    for p in path:
        source = p + name
        if path_exists(source):
            compiled = source + _TAG + '.pyc'
            if path_exists(compiled) and path_getmtime(source) < path_getmtime(compiled):
                return open(compiled, 'rb'), compiled, COMPILED
            else:
                return open(source, 'r'), source, SOURCE
    raise ImportError('Cannot find %s\n' % name)

@_no_trace
def imp_load_module(name, file, pathname, description):
    if description == SOURCE:
        bytes = parse(pathname)
    else:
        bytes = file.read()
    return build_module(bytes, name, pathname)

def load_module(name):
    try:
        return sys.modules[name]
    except KeyError:
        pass
    file, pathname, description = imp_find_module(name + '.py', sys.path)
    module = imp_load_module(name + '.py', file, pathname, description)
    sys.modules[name] = module
    return module

@_no_raise
@_pure
@c_function
def range_iter(self):
    pass

range.__iter__ = range_iter
del range_iter

@_pure
@c_function
def range_getitem(self, other):
    pass

range.__getitem__ = range_getitem
del range_getitem

def __len__(self):
  if self._step == 1:
      return self._end - self._begin
  if self._step < 0:
      return (self._begin - self._end) / -self._step
  else:
      return (self._end - self._begin) / self._step

range.__len__ = __len__
del __len__

int_iterator = int_iterator # Copy from builtins

@_pure
@c_function
def range_iterator_next(self):
    pass

range_iterator_next.__name__ = '__next__'

@_pure
@c_function
def range(low, high = None, step = 1)->range:
    pass

range(0, 0, 2)$__iter__()$type.__next__ = range_iterator_next
del range_iterator_next

@c_function
def tuple_hash(t):
    pass

tuple.__hash__ = tuple_hash
tuple_hash.__name__ = '__hash__'
del tuple_hash

# Copied from http://www.python.org/download/releases/2.3/mro/
def mro_merge(seqs):
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
      if not cand: raise TypeError("Inconsistent hierarchy")
      res.append(cand)
      for seq in nonemptyseqs: # remove cand
          if seq[0] == cand: del seq[0]

def c3_mro(C:type, bases:tuple):
    return tuple(mro_merge([[C]]+[list(c.__mro__) for c in bases]+[list(bases)]))

c_name(c3_mro)

def getitem_fallback(seq):
    raise TypeError("'%s' object is unsubscriptable" % seq$type.__name__)

@c_name
@_no_trace
def getitem_surrogate(seq, index):
    return seq$__getitem__(index)

def setitem_fallback(seq):
    raise TypeError("'%s' object does not support item assignment" %
                    seq$type.__name__)

@c_name
@_no_trace
def setitem_surrogate(value, seq, index):
    seq$__setitem__(index, value)
    return pass

@c_name
@_no_trace
def next_fallback(it):
    raise TypeError("'%s' object is not an iterator", it$type.__name__)

#Trace recording should copy iter before calling this.
@c_name
@_no_trace
def next_surrogate(iter):
    return $pop_caller_handler(iter$__next__())

@c_name
@_no_trace
def contains_surrogate(item, container):
    return container$__contains__(item)

@c_name
@_no_trace
def bool_fallback(o):
   #if o$type?__len__()
   #    i =  operator.index(o$__len__())
   #    return i != 0  # Special casing of small integers avoids circularity
   return True

@c_name
@_no_trace
def generic_getattr(obj, name):
    cls = obj$type
    if has_class_attribute(cls, name):
        cls_attr = get_class_attribute(cls, name)
        if cls_attr?__set__:
            return cls_attr$__get__(obj, cls)
        if obj?dict:
            d = obj$dict
            if d$__contains__(name):
                return d[name]
        if cls_attr?__get__:
            return cls_attr$__get__(obj, cls)
        return cls_attr
    else:
        if obj?dict:
            d = obj$dict
            if d$__contains__(name):
                return d[name]
        if obj?__getattr__:
            return obj$__getattr__(name)
        else:
            raise _attribute_error(cls, name)

@c_name
@_no_trace
def generic_setattr(obj, name, value):
    cls = obj$type
    if has_class_attribute(cls, name):
        cls_attr = get_class_attribute(cls, name)
        if cls_attr?__set__:
            return cls_attr$__set__(obj, cls, value)
    if obj?dict:
        obj$dict[name] = value
        return
    raise _attribute_error(cls, name)


#@_no_trace
#def type_getattr(cls, name):
#    meta = cls$type
#    if has_class_attribute(meta, name):
#        meta_desc = get_class_attribute(meta, name)
#        if meta_desc?__set__:
#            return meta_desc$__get__(cls, meta)
#        if has_class_attribute(cls, name):
#            desc = get_class_attribute(cls, name)
#            if desc?__get__:
#                return desc$__get__(None, cls)
#            return desc
#        if meta_desc?__get__:
#            return meta_desc$__get__(cls, meta)
#    else:
#        if has_class_attribute(cls, name):
#            desc = get_class_attribute(cls, name)
#            if desc?__get__:
#                return desc$__get__(None, cls)
#            return desc
#    raise _attribute_error(cls, name)

# Special value for designating missing parameter.

@_no_trace
def _default_iterator(getitem_func):
    index = 0
    try:
        while True:
            yield getitem_func(index)
            index += 1
    except IndexError:
        return

_NoValue = object()

@_globals_to_consts
@c_name
@_no_trace
def iter_surrogate(seq):
    if seq?__iter__:
        return seq$__iter__()
    elif seq?__getitem__:
        return _default_iterator(seq$__getitem__)
    else:
        raise TypeError("'%s' object is not iterable", seq$type.__name__)

@_no_trace
def _sentinel_iterator(func, sentinel):
    val = func()
    while val != sentinel:
        yield val
        val = func()

@_globals_to_consts
@_no_trace
def iter(seq, sentinel = _NoValue):
    if sentinel is _NoValue:
        return iter_surrogate(seq)
    else:
        return _sentinel_iterator(seq, sentinel)

del _sentinel_iterator
del iter_surrogate

