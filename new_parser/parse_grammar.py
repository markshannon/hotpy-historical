# Parse the python grammar file 
import re
import copy
from tokeniser import Tokeniser

from elements import *


import token_gen
    
tokens = {}

for t in token_gen.all_tokens:
    tokens[token_gen.all_tokens[t]] = Token(t)
        
for n in token_gen.kind_names:
    tokens[n] = Token(n)
tokens['INDENT'] = Token('INDENT')
tokens['DEDENT'] = Token('DEDENT')

list_ops = [ tokens[','], tokens['.'], tokens['and'], tokens['or'] 
]
     
def read_rule(tkn, rules):
    try:
        name = tkn.next()
        while name == '\n':
            name = tkn.next()
    except StopIteration:
        return None
    asgn = tkn.next()
    if asgn != ':':
        print "Expecting ':' found '%s' in %s" % (asgn, name)
    assert asgn == ':'
    parts = []
    for t in tkn:
        if t == '\n':
            return Rule(name, parts, rules)
        parts.append(t)
    return Rule(name, parts, rules)

id = 0

def next_id():
    global id
    id += 1
    return id

def part_process(insts, t, rest, rules):
    if t == '?':
        insts[-1] = ZeroOrOne(insts[-1])
        return insts, rest
    elif t == '*':
        insts[-1] = ZeroOrMany(insts[-1])
        return insts, rest
    elif t == '+':
        insts[-1] = OneOrMany(insts[-1])
        return insts, rest
    elif t[0] == '"' or t[0] == "'":
        insts.append(tokens[t[1:-1]])
        return insts, rest
    elif t == '[':
        level = 1
        end = -1
        while level:
            end += 1
            if rest[end] == '[':
                level += 1
            if rest[end] == ']':
                level -= 1
        opts = rest[:end]
        insts.append(ZeroOrOne(process(opts, rules)))
        return insts, rest[end+1:]
    elif t == '(':
        level = 1
        end = -1
        while level:
            end += 1
            if rest[end] == '(':
                level += 1
            if rest[end] == ')':
                level -= 1
        insts.append(process(rest[:end], rules))
        return insts, rest[end+1:]
    elif t == '|':
        return [or_expr( resolve(insts), process(rest, rules))], []
    elif t in rules:
        insts.append(rules[t])
        return insts, rest
    elif t in tokens:
        insts.append(tokens[t])
        return insts, rest
    else:
        raise Exception("Unknown token/rule: %s" % t)
        
def or_expr(a, b):
    if isinstance(a, Or):
        if isinstance(b, Or):
            return Or(a.options + b.options)
        else:
            return Or(a.options + [b])
    else:
        if isinstance(b, Or):
            return Or([a] + b.options)
        else:
            return Or([a,b])
        
def resolve(insts):
    # Test for binary: [ x, [y,x]* ]
    if (len(insts) == 2 and 
        insts[1].__class__ == ZeroOrMany and 
        insts[1].inst.__class__ == List and 
        len(insts[1].inst.l) == 2 and
        insts[0] == insts[1].inst.l[1]):
        op = insts[1].inst.l[0]
        if op in list_ops:
            i = SkipList(insts[0], insts[1].inst.l[0])
        elif op.name and op.name.startswith('comp'):
            i = List(insts)
        else:
            i = Binary(insts[0], insts[1].inst.l[0], insts[1].inst.l[1])
    elif len(insts) == 1:
        i = insts[0]
    elif (insts[-1].__class__ == ZeroOrOne and insts[-1].inst.__class__ == Token
            and len(insts) > 1 and insts[-2].__class__ == ZeroOrMany and
            insts[-2].inst.__class__ == List and insts[-2].inst.l[0] == insts[-1].inst):
        if len(insts) > 2:
            i = OptionalEnd(resolve(insts[:-2]), insts[-2].inst, insts[-1].inst)
        else:
            i = OptionalEnd(None, insts[-2].inst, insts[-1].inst)
    else:
        i = List(insts)
    return i
        
def process(txt, rules):
    insts = []
    while txt:
        insts, txt = part_process(insts, txt[0], txt[1:], rules)
    return resolve(insts)    

class Rule(Element):
    
    def __init__(self, name, tokens, rules):
        self.name = name
        self.tokens = tokens
        self.element = None
        self.recurse = False
        rules[name] = self
        self.rules = rules
        self._la1 = None
        
    def __eq__(self, other):
        return self.__class__ is other.__class__ and self.name == other.name

    def la1(self):
        return self.element.la1()

    def verify(self):
        pass
        
    def resolve(self):
        if self.recurse:
            raise Error("Recursion in rules")
        self.recurse = True
        self.element = process(self.tokens, self.rules)
        self.recurse = False

    def nice_name(self):
        return self.name
        
    def code(self, y):
        print >> y, '%s(lexer)' % self.name
        
    def visit(self, visitor):
        visitor.rule(self)
        
    def optional(self):
        return self.element.optional()
  
lexical = ['identifier', 'letter', 'lowercase', 'uppercase', 'stringliteral',
'stringprefix', 'shortstring', 'longstring', 'shortstringitem', 'digit', 
'longstringitem', 'shortstringchar', 'longstringchar', 'escapeseq', 
 'integer', 'decimalinteger', 'octinteger', 'hexinteger', 
'nonzerodigit', 'octdigit', 'hexdigit', 'floatnumber', 'pointfloat', 
'exponentfloat', 'intpart', 'fraction', 'exponent', 'imagnumber']
    
import sys

def read_rules(grammar_file):
    f = Tokeniser(grammar_file)
    rules = {}
    r = read_rule(f, rules)
    while r:
        r = read_rule(f, rules)   
    for r in rules.values():
        r.resolve()
    for r in rules.values():
        r.la1()
    for r in rules.values():
        try:
            r.element.verify()
        except Exception, e:
            print 'Ambiguity in rule %s for %s' % (r.name, ' '.join(e.tokens))
            import sys
            sys.exit(1)
    return rules


if __name__ == '__main__':
    rules = read_rules(open(sys.argv[1]))
    from visitor import PrettyPrinter
    
    p = PrettyPrinter()
    for r in rules.values():
        print r.name, ': ',
        r.element.visit(p)
        print
        
    sys.exit(0)
    
    def need_rule(r):
        if r.name not in written:
            rules[r.name] = r
        
    def number_duplicates(names):
        numbers = {}
        for n in names:
            if n in numbers:
                numbers[n] = 1
            else:
                numbers[n] = 0
        result = []
        for n in names:
            if numbers[n]:
                result.append(n + str(numbers[n]))
                numbers[n] += 1
            else:
                result.append(n)
        return result
            
    def visit(m, t):
        print >> p, '@behaviour(%s)' % m
        print >> p, "def %s(scope:Scope, node:%s):" % (m, t)
        print >> p, "    pass"
        print >> p
        
    def pyclass(name):
        if 'expr' in name :
            print >> p, 'class %s(object):' % name
            print >> p, '    def write(out):'
            print >> p, '        pass'
            print >> p, '    def branch(onTrue, target, out):'
            print >> p, '        pass'
            print >> p
        
        visit('entry', name)
        visit('exit', name)
        
    def new_object(name, used):
        nice_names = [u.nice_name() for u in used]
        names = []
        i = 0
        for n in nice_names:
            i += 1
            if n:
                names.append(n)
            else:
                names.append('part%d'%i)
        names = number_duplicates(names)
        name_types = ['R_object ' + n for n in names]
        print >> h, 'R_object make_%s(%s);' % (name, ', '.join(name_types))
        print >> c, 'R_object make_%s(%s) {' % (name, ', '.join(name_types))
        print >> c, '    R_%s result = gc_allocate(%s);' % (name, name)
        print >> c, '    result->__class__ = _%s;' % name
        for n in names:
            print >> c, '    result->%s = %s;' % (n, n)
        print >> c, '    return (R_object)result;'
        print >> c, '}'
        print >> c
        print >> shapes, '%s:' % name
        print >> shapes, '    + type __class__; ro'
        for n in names:
            print >> shapes, '    + union %s; ro' % n
        print >> shapes
        print >> c, 'int print_%s(visitor, self) {' % name
        print >> c, '    R_object child;'
        print >> c, '    int s_id = id++;'
        for n in names:
            print >> c, '    child = binary_call(VISIT, visitor, self->%s);' % n
            print >> c, '    printf("node_%d -> node_%d;", s_id, child);'
        print >> c, '    return s_id;'
        print >> c, '}'
        print >> c, 'void walk_%s(self, visitor) {' % name
        print >> c, '    assert(self->__class__ == _%s);' % name
        print >> c, '    binary_call(ENTRY, visitor, self);'
        for n in names:
            print >> c, '    PY_CALL("walk", self->%s, visitor)' % n     
        print >> c, '    binary_call(EXIT, visitor, self);'
        print >> c, '}'
        print >> c
        pyclass(name)
        
    def new_binary(name):
        print >> h, 'R_object make_%s(R_object left, R_object op, R_object right);' % name
        print >> c, 'R_object make_%s(R_object left, R_object op, R_object right) {' % name
        print >> c, '    R_%s result = gc_malloc(sizeof(struct gvmt_%s_t));' % (name, name)
        print >> c, '    result->__class__ = %s;' % name
        print >> c, '    result->left = left;'
        print >> c, '    result->op = op;' 
        print >> c, '    result->right = right;'
        print >> c, '    return result;'
        print >> c, '}'
        print >> shapes, '%s:' % name
        print >> shapes, '    + type __class__; ro'
        print >> shapes, '    + union left; ro'
        print >> shapes, '    + union op; ro'
        print >> shapes, '    + union right; ro'
        print >> shapes
        pyclass(name)
        
        
    y = open(sys.argv[1]+'.y', 'w')
    c = open(sys.argv[1]+'.c', 'w')
    h = open(sys.argv[1]+'.h', 'w')
    p = open(sys.argv[1]+'.py', 'w')
    shapes = open(sys.argv[1]+'.shapes', 'w')
    
    print >> y, '''
    %%{
    #define  YYDEBUG 1
    #define  YYERROR_VERBOSE
    #define YYSTYPE R_object
    #define YYSTYPE_IS_TRIVIAL 1
    #include "%s.h"
    %%}
    ''' % sys.argv[1]
    
    print >> h, '#ifndef %s_H' % sys.argv[1].upper()
    print >> h, '#define %s_H' % sys.argv[1].upper()
    print >> h, 'typedef union gvmt_all_types *R_object;'
    print >> h, 'R_object empty_list(void);'
    print >> h, 'R_object list_append(R_object list, R_object item);'
    print >> h, 'R_object list_of_one(R_object item);'
    print >> h, 'extern R_object None;'
    
    print >> c, '#include "%s.h"' % sys.argv[1]
    
    for name in token_gen.all_names:
        print >> y, '%token', name
    
    print >> y, '%start file_input'
    print >> y, '%%'
        
    written = set()
    rules = {}
        
    start = insts['file_input']
    rules['file_input'] = start
    while rules:
        n, r = rules.popitem()
        written.add(n)
        r.rule(r.name, y)
        r.parser(r.name, y)
    
    print >> h, '#endif'
    y.close()
    c.close()
    h.close()
    shapes.close()
    p.close()
