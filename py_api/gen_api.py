#This module creates the API bindings for HotPy.

#All functions are of the form:
# XXX PyYYY(args)
#args can be vanilla or ref-stealing. 
# All args can be string (C type) or tuple (C type, ref-behaviour).
# The default behaviour for parameters is "borrowed". 
# For return types it is "unstable". "Unstable" is not allowed and the function 
# is converted to a _REF variant.

import re, sys

native = open('native.c', 'w')
print >> native, '/** This file is auto-generated */\n'
print >> native, '#include "py_api/Python.h"'
print >> native, '#include "gvmt/native.h"'

proxies = open('proxies.c', 'w')
print >> proxies, '/** This file is auto-generated */'
print >> proxies, '#include "hotpy.h"'
print >> proxies, '#include "operators.h"'
header = open('api.h', 'w')
print >> header, '/** This file is auto-generated */'
print >> header, '#include <stdarg.h>'
print >> header, '#include <wchar.h>'
prototypes = open('prototypes.h', 'w')
print >> prototypes, '/** This file is auto-generated */'
not_impl = open('not_impl.h', 'w')
print >> not_impl, '/** This file is auto-generated */'
print >> not_impl, '#include "hotpy.h"'

TYPE =  r'((?:const +)?\w+(?:\*\*? *| +\*?\*? *))'  
PARAM = ' *%s(\w*) *| *... *' % TYPE
ARG = re.compile(PARAM)
R_TYPE = re.compile(TYPE)
ID = re.compile(r'(\w+)')

arg_id = 0

class Arg:
    def __init__(self, x):
        global arg_id
        if x.strip() == '...':
            self.type, self.name = '...', ''
        else:
            self.type, self.name = ARG.match(x).groups()
            if self.type.strip() == 'unsigned':
                if self.name == 'int' or self.name == 'long':
                    self.type = '%s %s' % (self.type.strip(), self.name)
                    self.name = ''
            self.type = self.type.replace(' *', '*')
            self.type = self.type.replace(' *', '*')
            if not self.name:
                self.name = 'p%d' % arg_id
                arg_id += 1
            
    def decl(self):
        return '%s %s' % (self.type, self.name)
        
    def __str__(self):
        return '%s %s' % (self.type, self.name)
        
    def __repr__(self):
        return "Arg('%s %s')" % (self.type, self.name)

suffix = { 'PyObject*' : 'p', 'int' : 'i', 'long' : 'j', 
           'Py_ssize_t' : 'j', 'PyObject**' : 'p',
           'PyVarObject*' : 'p' , 'PyTypeObject*' : 'p',
           'void': '*VOID*', 'void*': 'p', 'char*': 'p', 'const char*': 'p'}

def as_callee(args):
    last = args[-1]
    if last.name == '':
        args = args[:-1] + (Arg('va_list _ap_list'),)
    return args    
    
ref_types = {'PyObject*' : 'R_object', 
             'PyTypeObject*' : 'R_type',
             'PyFrameObject*' : 'R_frame',
             'PyVarObject*' : 'R_object',
             'PyCodeObject*' : 'R_code_object' }
    
def native_type(t):
    return t not in ref_types
    
def is_native(ret, *args):
    return native_type(ret) and all(native_type(arg.type) for arg in args)
    
def is_test(name):
    return 'Check' in name or 'Is' in name or 'Has' in name
    
def hotname(name):
    return 'Hot%s' % name
    
def make_function(ret, name, *args):
    caller_args = args
    callee_args = as_callee(args)
    if name in macros:
        macro = '%s(%s)' % (name, ', '.join([a.name for a in caller_args]))
        func = '%s(%s)' % (macros[name], ', '.join([a.name for a in caller_args]))
        print >> header, '#define %s %s' % (macro, func)
    else:
        signature = '%s %s(%s)' % (ret, name, ', '.join([a.decl() for a in caller_args]))
        print >> header, signature, ';'
        print >> header
        for a in callee_args:
            if a.type in ref_types:
                a.type = ref_types[a.type]
        if ret in ref_types:
            print >> native, 'extern %s _hotpy_proxy_%s(void);' % (ret, name)
            print >> native
            print >> native, signature, '{'
            print >> proxies, 'R_object _hotpy_proxy_%s(%s) {' % (name, ', '.join([a.decl() for a in callee_args]))
            print >> proxies, '    R_object result;'
            print >> proxies, '    PyObject* pinned;'
            print >> proxies, '    result = (R_object)%s(%s);' % (hotname(name), ', '.join([a.name for a in callee_args]))
            print >> proxies, '    if (!gvmt_is_tagged((GVMT_Object)result)) {'
            print >> proxies, '        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);'
            print >> proxies, '        Py_INCREF(pinned);'
            print >> proxies, '    }'
            print >> proxies, '    return result;'
            print >> proxies, '}'
            print >> proxies
            call = 'gvmt_call((gvmt_func_ptr)_hotpy_proxy_%s, %d, %s)'  % (name, len(args), ', '.join([a.name for a in callee_args]))
        else:
            print >> native, 'extern %s %s(void);' % (ret, hotname(name))
            print >> native, signature, '{'
            call = 'gvmt_call((gvmt_func_ptr)%s, %d, %s)'  % (hotname(name), len(args), ', '.join([a.name for a in callee_args]))
        if callee_args is caller_args:
            if ret == 'void':
                print >> native, '    %s;' % call
            else:
                print >> native, '    return (%s)%s.%s;' % (ret, call, suffix[ret])
        else:
            print >> native, '    va_list _ap_list;'
            print >> native, '    va_start(_ap_list, %s);' % callee_args[-2].name
            print >> native, '    %s _result = (%s)%s.%s;' % (ret, ret, call, suffix[ret])
            print >> native, '    va_end(_ap_list);'
            print >> native, '    return _result;'
        print >> native, '}'
        print >> native
        if name in return_types: 
            ret = return_types[name] 
        elif ret == 'int' and not is_test(name) and name != 'Py_ReprEnter': 
            ret = 'void' 
        elif ret in ref_types:
            ret = ref_types[ret]
        print >> prototypes, '%s %s(%s);' % (ret, hotname(name), ', '.join([a.decl() for a in callee_args]))
        print >> not_impl, '%s %s(%s) {' % (ret, hotname(name), ', '.join([a.decl() for a in callee_args]))
        print >> not_impl, '    raise_char(type_InternalError, "Not implemented");'
        print >> not_impl, '}\n'
    
SIGNATURE = re.compile(r'%s(\w+)\((%s(,%s)*)?\)')
    
def parse_function_line(line):
    assert 'c:function::' in line
    line = line[line.find('c:function::') + len('c:function::'):]
    line = line.strip()
    ret = R_TYPE.match(line).group(0)
    line = line[len(ret):]
    #print line
    name = ID.match(line).group(0)
    line = line[len(name):]
    line = line.strip(' ()')
    args = line.split(',')
    if args[-1].strip() == 'NULL':
        args = args[:-1]
    args = [Arg(x) for x in args]
    #Ensure ret type is known.
    ret = ret.strip()
    suffix[ret]
    return ret, name, args
    
return_types = { 'PyObject_Not': 'int',
                 'PyObject_RichCompareBool': 'int',
                 'PyObject_Repr' : 'R_str'
}
    
py_funcs = set([ 'PyObject_Dir',  
                 ])
    
blacklist = set([ 'PySequence_Fast_ITEMS', 'PySequence_Fast_GET_ITEM',
                  'PySequence_Fast_GET_SIZE', 'PyByteArray_GET_SIZE',
                  'PyBytes_GET_SIZE', 'PyTuple_GET_SIZE',
                  'PyUnicode_GET_SIZE', 'Py_INCREF' , 'Py_XINCREF',
                  'Py_DECREF', 'Py_XDECREF', 'Py_CLEAR', 
                  'PyObject_ASCII', 'PyObject_Bytes', 'PyObject_Type',
                  'PyObject_TypeCheck', 'PyComplex_FromCComplex',
                  'PyObject_Dir', 'PyObject_HashNotImplemented',
                  'PyErr_Format', 
                  ]) 

macros = { }

f = open(sys.argv[1])
file_list = f.readlines()
f.close()
    
for filename in file_list:
    filename = filename.strip()
    if not filename or filename.startswith('#'):
        continue
    in_file = open(filename)
    lines = in_file.readlines()
    print >> native, '\n\n/* %s */' % filename
    print >> proxies, '\n\n/* %s */' % filename
    print >> header, '\n\n/* %s */' % filename
    print >> prototypes, '\n\n/* %s */' % filename
    print >> not_impl, '\n\n/* %s */' % filename
    for line in lines:
        if 'c:function::' in line and 'Decode' not in line:
            arg_id = 0
            try:
                ret, name, args = parse_function_line(line)
            except:
                print "Cannot parse '%s'" % line
                continue
            if name not in blacklist and name[0] != '_':
                if is_native(ret, *args):
                    print "Native function:", ret, name, args
                    # What to do... 
                    # Add name to a file for copying from CPython?
                else:
                    make_function(ret, name, *args)
    in_file.close()

native.close()
proxies.close()
header.close()
prototypes.close()
not_impl.close()



