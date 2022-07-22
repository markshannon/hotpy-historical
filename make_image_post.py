import re, sys


PY_FUN = re.compile(r'''address interpret_start *// *PY_(\w+)
''')

NATIVE = r'''address PY_\1
'''

HAVE_REF_COUNT = True

if HAVE_REF_COUNT:
    EMPTY_TUPLE_RE = re.compile('''\.public empty_tuple_object
\.object empty_tuple_object
address _tuple
uint32 -?\d+
uint32 0
\.end
''')
    
    EMPTY_BYTES_RE = re.compile('''\.public empty_bytes
\.object empty_bytes
address _bytes
uint32 -?\d+
uint32 0
\.end
''')    
    
    EMPTY_STRING_RE = re.compile('''\.public empty_string_object
\.object empty_string_object
address _str
uint32 -?\d+
uint32 0
uint32 0
\.end
''')
      
else:
    EMPTY_TUPLE_RE = re.compile('''\.public empty_tuple_object
\.object empty_tuple_object
address _tuple
uint32 0
\.end
''')
    
    EMPTY_BYTES_RE = re.compile('''\.public empty_bytes
\.object empty_bytes
address _bytes
uint32 0
\.end
''')

    EMPTY_STRING_RE = re.compile('''\.public empty_string_object
\.object empty_string_object
address _str
uint32 0
uint32 0
\.end
''')

EMPTY_DICT_TABLE_RE = re.compile('''\.public empty_dict_table_object
.object empty_dict_table_object
address _dict_table
int32 0
uint32 8
int32 0
int32 0
address 0
address 0
address 0
address 0
address 0
address 0
address 0
address 0
\.end
''')

if sys.argv[1] == '-compile':
    fname =  sys.argv[2]
else:
    fname =  sys.argv[1]
f = open(fname, 'r')
whole = f.read()
f.close()
f = open(fname, 'w')
if sys.argv[1] == '-compile':
    whole = PY_FUN.sub(NATIVE, whole)

whole = EMPTY_BYTES_RE.sub('', whole)
whole = EMPTY_TUPLE_RE.sub('', whole)
whole = EMPTY_STRING_RE.sub('', whole)
whole = EMPTY_DICT_TABLE_RE.sub('', whole)

print >> f, whole,
f.close()
