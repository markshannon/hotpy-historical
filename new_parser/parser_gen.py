#Vistor template class

import token_gen
import elements
import parse_grammar

#2.5 compatability
try:
    next
except:
    def next(it):
        return it.next()

all_sets = set()

def set_name(s):
    l = [ token_gen.all_names[i] for i in s]
    l.sort()
    return 'SET_%s' % '_'.join(l)

def set_contains(s):
    if len(s) == 1:
        return 'LA_KIND == kind_%s' % token_gen.all_names[next(iter(s))]
    elif (len(s) == 2):
        it = iter(s)
        t = token_gen.all_names[next(it)], token_gen.all_names[next(it)]
        return 'LA_KIND == kind_%s || LA_KIND == kind_%s' % t
    else:
#        assert len(s)
        all_sets.add(frozenset(s))
        return '(%s[LA_KIND >> 5] >> (LA_KIND & 31)) & 1'  % set_name(s)
        
IGNORE_TOKENS = set([ 
        'AS', 'COLON', 'COMMA', 'LBRACK', 'RBRACK',
        'ARROW', 'LPAREN', 'RPAREN', 'ASSIGN', 'LAMBDA', 
        'SEMI', 'CLASS', 'ASSERT', 'ELSE', 'AT', 'IF', 'ELIF',
        'ELSE', 'WITH', 'FOR', 'DOT', 'CONTINUE', 'BREAK',
        'RAISE', 'YIELD', 'NONLOCAL', 'TRY', 'EXCEPT', 'DEF', 
        'LCURLY', 'RCURLY', 'NONE', 'TRUE', 'FALSE', 'NOT', 'NEWLINE',
        'IS', 'RETURN', 'NEWLINE', 'IMPORT', 'FROM',
        'ENDMARKER', 'GLOBAL', 'DEL', 'WHILE'
])

KIND_ONLY_TOKENS = (set(token_gen.operator_tokens.keys()) |
                    set(token_gen.keyword_tokens.keys())) - IGNORE_TOKENS
KIND_ONLY_TOKENS.add('INDENT')
KIND_ONLY_TOKENS.add('DEDENT')

def ignore_token(node):
    if isinstance(node, elements.Token):
        return node.name in IGNORE_TOKENS
    else:
        return False
 
def kind_only(node):
    if isinstance(node, elements.Token):
        return node.name in KIND_ONLY_TOKENS
    else:
        return False
        
def write_set(s, out, header):
    print >> header, 'extern uint32_t %s[];' % set_name(s)
    bits = 0
    for i in s:
        bits |= (1 << i)
    print >> out, '/*',
    for i in s:
        print  >> out, i,
    print >> out, '*/'
    print >> out, 'uint32_t %s[] = {' % set_name(s)
    print >> out, '0x%x,' % (bits & ((1 << 32) - 1))
    print >> out, '0x%x,' % ((bits >> 32) & ((1 << 32) - 1))
    print >> out, '0x%x,' % ((bits >> 64) & ((1 << 32) - 1))
    print >> out, '0x%x' % ((bits >> 96) & ((1 << 32) - 1)), 
    print >> out, '};'
    
_types = {}
        
def read_types(f):
    for l in f.readlines():
        if l.strip() and l[0] != '#':
            if ':' not in l:
                raise Exception("Illegal type declaration '%s'" % l)
            name, tipe = l.split(':')
            _types[name.strip()] = tipe.strip()
        
def get_type(node, default = None):
    name = node.name
    if name in _types:
        return _types[name]
    else:
        if isinstance(node, parse_grammar.Rule):
            node = node.element
        if isinstance(node, elements.Or):
            t = get_type(node.options[0])
            for n in node.options:
                if get_type(n) != t:
                    return 'R_object'
            return t
        elif isinstance(node, elements.ZeroOrOne):
            return get_type(node.inst, default)
        elif isinstance(node, elements.ZeroOrMany):
            return 'R_list'
        elif isinstance(node, elements.OneOrMany):
            return 'R_list'
        elif isinstance(node, elements.SkipList):
            return 'R_list'
        elif isinstance(node, elements.OptionalEnd):
            return 'R_list'
        elif isinstance(node, elements.Token):
            if kind_only(node) or ignore_token(node):
                return 'int'
            else:
                return 'R_str'
        elif isinstance(node, elements.Binary):
            return get_type(node.left, default)
        elif isinstance(node, elements.List):
            used = 0
            for i in node.l:
                if not ignore_token(i):
                    used += 1
            if used == 1:
                for i in node.l:
                    if not ignore_token(i):
                       return get_type(i, default)
            else:
                return 'R_tuple'
        elif default:
            return default
        else:
            if name:
                raise Exception("No type defined for '%s'" % (name))
            else:
                raise Exception("No type defined for unnamed element '%s'" % type(node).__name__)

class ParserGen:            
    
    def __init__(self, out, header, prototypes):
        self.out = out
        self.header = header
        self.prototypes = prototypes
        self.value_id = 0
        self.value = None
        
    def end_block(self):
        print >> self.out, '}'     
        
    def start_block(self, t):
        print >> self.out, '{'
        if str(t) == 'R_AST_stmt' or str(t) == 'R_AST_expr':
            print >> self.out, 'int line = lexer_getline(lexer);'
            print >> self.out, 'int column = lexer_getcolumn(lexer);'
        
    def print_ast(self, t, val, name, targs, args):
        t = str(t)
        if t == targs:
            print >> self.out, '%s = %s;' % (val, args)
            return
        if t == 'R_tuple':
            if isinstance(args, list):
                print >> self.out, '%s = make_tuple(%d);' % (val, len(args))
                for i, (a, at) in enumerate(zip(args, targs)):
                    if str(at) == 'int':
                        print >> self.out, 'ITEM(%s, %d) = (R_object)PY_SMALL_INT(%s);' % (val, i, a)
                    else:
                        print >> self.out, 'ITEM(%s, %d) = (R_object)%s;' % (val, i, a)
                return
        if isinstance(args, list):
            args = ', '.join(args)
        assert isinstance(args, str)
        if isinstance(targs, list):
            targs = ', '.join(targs)
        assert isinstance(targs, str)
        if t == 'R_AST_stmt' or t == 'R_AST_expr':
            print >> self.out, '%s = %s_for_rule_%s(%s, line, column);' % (val, t[6:], name, args)
            print >> self.prototypes, '%s %s_for_rule_%s(%s, int line, int column);' % (t, t[6:], name, targs)
        elif t == targs:
            print >> self.out, '%s = %s;' % (val, args)
        elif t.startswith('R_AST'):
            print >> self.out, '%s = %s_for_rule_%s(%s);' % (val, t[6:], name, args)
            print >> self.prototypes, '%s %s_for_rule_%s(%s);' % (t, t[6:], name, targs)
        else:
            print >> self.out, '%s = %s_for_rule_%s(%s);' % (val, t[2:], name, args)
            print >> self.prototypes, '%s %s_for_rule_%s(%s);' % (t, t[2:], name, targs)
        
        
    def next_value(self):
        self.value_id += 1
        return '_val%d' % self.value_id
    
    def zero_or_one(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'if (%s) {' % set_contains(x.la1())
        x.inst.visit(self)
        print >> self.out, '} else {'
        print >> self.out, '    %s = (%s)NULL;' % (self.value, self.tipe)
        print >> self.out, '}'
        
    
    def zero_or_many(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        t = self.tipe
        val = self.value
        l = self.next_value()
        self.tipe = get_type(x.inst, 'R_object')
        self.value = self.next_value()
        self.start_block(t)
        print >> self.out, 'R_list %s = make_list(4);' % l
        print >> self.out, '%s %s;' % (self.tipe, self.value)
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'while (%s) {' % set_contains(x.la1())
        x.inst.visit(self)
        if self.tipe == 'int':
            print >> self.out, '    list_append(%s, (R_object)PY_SMALL_INT(%s));' % (l, self.value)
        else:
            print >> self.out, '    list_append(%s, (R_object)%s);' % (l, self.value)
        print >> self.out, '    LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, '}'
        if x.name:
            self.print_ast(t, val, x.name, 'R_list', l)
        elif t == 'R_object':
            print >> self.out, '   %s = (R_object)%s;' % (val, l)
        elif t == 'R_list':
            print >> self.out, '   %s = %s;' % (val, l)
        else:
            raise Exception("Cannot convert R_list to %s" % t)
        self.end_block()
        self.value = val
        self.tipe = t
    
    def one_or_many(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        t = self.tipe
        val = self.value
        l = self.next_value()
        self.value = self.next_value()
        self.tipe = get_type(x.inst, 'R_object')
        self.start_block(t)
        print >> self.out, '%s %s;' % (self.tipe, self.value)
        print >> self.out, 'R_list %s = make_list(4);' % l
        x.inst.visit(self)
        print >> self.out, 'list_append(%s, (R_object)%s);' % (l, self.value)
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'while (%s) {' % set_contains(x.la1())
        x.inst.visit(self)
        print >> self.out, '    list_append(%s, (R_object)%s);' % (l, self.value)
        print >> self.out, '    LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, '}'
        if x.name:
            self.print_ast(t, val, x.name, 'R_list', l)
        elif t == 'R_object':
            print >> self.out, '   %s = (R_object)%s;' % (val, l)
        elif t == 'R_list':
            print >> self.out, '   %s = %s;' % (val, l)
        else:
            raise Exception("Cannot convert R_list to %s" % t)
        self.end_block()
        self.value = val
        self.tipe = t
        
    def optional_end(self, x):
        named = x.name and x.name in _types
        print >> self.out, '/* %s */' % type(x).__name__
        val = self.value
        t = self.tipe
        l = self.next_value()
        v = self.next_value()
        self.start_block(t)
        print >> self.out, 'R_list %s = make_list(4);' % l
        print >> self.out, 'R_object %s;' % v
        print >> self.out, 'int dummy;'
        if named:
            print >> self.out, 'int trailing = 0;'
        if x.start is not None:
            self.tipe = 'R_object'
            self.value = v
            x.start.visit(self)
            print >> self.out, 'list_append(%s, (R_object)%s);' % (l, self.value)
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'while (%s) {' % set_contains(x.token.la1())
        self.tipe = 'int'
        self.value = 'dummy'
        x.token.visit(self)
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'if (%s) {' % set_contains(x.repeats.la1())
        self.tipe = 'R_object'
        self.value = v
        x.repeats.visit(self)
        print >> self.out, '    list_append(%s, (R_object)%s);' % (l, self.value)
        print >> self.out, '    LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, '} else {'
        if named:
            print >> self.out, '    trailing = 1;'
        print >> self.out, '    break;'
        print >> self.out, '}'
        print >> self.out, '}'
        if named:
            self.print_ast(t, val, x.name, ['R_list', 'int'], [l, 'trailing'])
        elif t == 'R_object':
            print >> self.out, '   %s = (R_object)%s;' % (val, l)
        elif t == 'R_list':
            print >> self.out, '   %s = %s;' % (val, l)
        else:
            raise Exception("Cannot convert R_list to %s" % self.tipe)
        self.end_block()
        self.value = val
        self.tipe = t

    def list_(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        val = self.value
        t = self.tipe
        if x.name:
            items = []
            types = []
            self.start_block(t)
            for i in x.l:
                if ignore_token(i):
                    tipe, value = 'int', self.next_value()
                else:
                    tipe, value = get_type(i, 'R_object'), self.next_value()
                print >> self.out, '%s %s;' % (tipe, value)
                types.append(tipe)
                items.append(value)
            vals_it = iter(items)
            types_it = iter(types)
            items = []
            types = []
            for i in x.l:
                self.value = next(vals_it)
                self.tipe = next(types_it)
                i.visit(self)
                if not ignore_token(i):
                    items.append(self.value)
                    types.append(self.tipe)
                else:
                    print >> self.out, '/* Ignoring %s */' % i.name
            self.print_ast(t, val, x.name, types, items)
            self.end_block()
        elif t == 'R_tuple' or t == 'R_object':
            length = 0
            for i in x.l:
                if not ignore_token(i):
                    length += 1
            assert length
            if length == 1:
                self.value = self.next_value()
                self.tipe = t
                print >> self.out, '%s %s;' % (t, self.value)
                for i in x.l:
                    i.visit(self)
                    if not ignore_token(i):
                        print >> self.out, '    %s = %s;' % (val, self.value)
            else:
                l = self.next_value()
                v = self.next_value()
                print >> self.out, 'R_object %s;' % v
                if length < len(x.l):
                    print >> self.out, 'int dummy;'
                size = 0
                for i in x.l:
                    if not ignore_token(i):
                        size += 1
                print >> self.out, 'R_tuple %s = make_tuple(%s);' % (l, size)
                index = 0
                for i in x.l:
                    if ignore_token(i):
                        self.tipe = 'int'
                        self.value = 'dummy'
                        i.visit(self)
                    else:
                        self.tipe = 'R_object'
                        self.value = v
                        i.visit(self)
                        print >> self.out, '    ITEM(%s, %d) = %s;' % (l, index, self.value)
                        index += 1
                if t == 'R_tuple':
                    print >> self.out, '%s = %s;' % (val, l)    
                elif t == 'R_object':
                    print >> self.out, '%s = (R_object)%s;' % (val, l)   
        else:
            length = 0
            for i in x.l:
                if not ignore_token(i):
                    length += 1
            assert length
            if length == 1:
                if len(x.l) > 1:
                    ival = self.next_value()
                    print >> self.out, ' int %s;' % ival
                for i in x.l:
                    if ignore_token(i):
                        self.tipe = 'int'
                        self.value = ival
                        i.visit(self)
                    else:
                        self.value = val
                        self.tipe = t
                        i.visit(self)
                        print >> self.out, '    %s = %s;' % (val, self.value)                
            else:
                raise Exception("List with more than one non-ignored token must be of type 'tuple' or 'object' or have name")
        self.value = val
        self.tipe = t
            
    def binary(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        val = self.value
        t = self.tipe
        left = self.next_value()
        op = self.next_value()
        right = self.next_value()
        
        left_t = get_type(x.left)
        op_t = get_type(x.op)
        right_t = get_type(x.right)
        self.start_block(t)
        
        print >> self.out, '%s %s;' % (left_t, left)
        print >> self.out, '%s %s;' % (op_t, op)
        print >> self.out, '%s %s;' % (right_t, right)
        
        self.value = left
        self.tipe = left_t
        x.left.visit(self)
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'while (%s) {' % set_contains(x.op.la1())
        self.value = op
        self.tipe = op_t
        x.op.visit(self)
        self.value = right
        self.tipe = right_t
        x.right.visit(self)
        if x.name:
            self.print_ast(left_t, left, x.name, [left_t, op_t, right_t], [left, op, right])
        else:
            print >> self.out, '   %s = make_triple(%s, %s, %s);' % (left, left, op, right)
        print >> self.out, '     LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, '}'
        print >> self.out, '   %s = %s;' % (val, left)
        self.end_block()
        self.value = val
        self.tipe = t
        
    def skip_list(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        val = self.value
        t = self.tipe
        self.tipe = 'R_object'
        l = self.next_value()
        item = self.next_value()
        comma = self.next_value()
        self.value = item
        self.start_block(t)
        print >> self.out, 'R_list %s = make_list(4);' % l
        self.tipe = get_type(x.left, 'R_object')
        print >> self.out, '%s %s;' % (self.tipe, self.value)
        print >> self.out, 'int %s;' % comma
        x.left.visit(self)
        print >> self.out, '    list_append(%s, (R_object)%s);' % (l, self.value)
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'while (%s) {' % set_contains(x.repeat.la1())
        self.tipe = 'int'
        self.value = comma
        x.repeat.visit(self)
        self.tipe = get_type(x.left, 'R_object')
        self.value = item
        x.left.visit(self)
        print >> self.out, '    list_append(%s, (R_object)%s);' % (l, item)
        print >> self.out, '    LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, '}'
        if x.name and x.name in _types:                                    
            self.print_ast(t, val, x.name, 'R_list', l)
        elif t == 'R_object':
            print >> self.out, '   %s = (R_object)%s;' % (val, l)
        elif t == 'R_list':
            print >> self.out, '   %s = %s;' % (val, l)
        else:
            raise Exception("Cannot convert R_list to %s" % self.tipe)
        self.end_block()
        self.value = val
        self.tipe = t
        
    def or_(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        if x.optional():
            print >> self.out, 'if (%s) {' % set_contains(x.la1())
        print >> self.out, 'if (%s) {' % set_contains(x.options[0].la1())
        x.options[0].visit(self)
        for o in x.options[1:-1]:
            print >> self.out, '} else if (%s) {' % set_contains(o.la1())
            o.visit(self)
        print >> self.out, '} else {'
        x.options[-1].visit(self)
        print >> self.out, '}'
        if x.optional():
            print >> self.out, '} else {'
            print >> self.out, '    %s = (%s)NULL;' % (self.value, self.tipe)
            print >> self.out, '}'
    
    def token(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        print >> self.out, 'LA_KIND = lexer_token_kind(lexer);'
        print >> self.out, 'if (LA_KIND != kind_%s) expecting_error(kind_%s, lexer);' % (x.name, x.name)
        if self.tipe == 'R_str':
            print >> self.out, '%s = lexer_token_text(lexer);' % self.value
        elif self.tipe == 'R_object':
            print >> self.out, '%s = (R_object)PY_SMALL_INT(LA_KIND);' % self.value
        elif self.tipe == 'int':
            print >> self.out, '%s = LA_KIND;' % self.value
        elif x.name:
            self.start_block(self.tipe)
            if kind_only(x) or ignore_token(x):
                self.print_ast(self.tipe, self.value, x.name, 'int', 'LA_KIND')
            else:
                self.print_ast(self.tipe, self.value, x.name, 'R_str', 'lexer_token_text(lexer)')
            self.end_block()
        else:
            raise Exception("Type must be Token or object")
        print >> self.out, 'lexer_advance(lexer);'
        
    def rule(self, x):
        print >> self.out, '/* %s */' % type(x).__name__
        if self.tipe == get_type(x):
            print >> self.out, '%s = parse_%s(lexer);' % (self.value, x.name)
        elif self.tipe == 'R_object':
            if get_type(x) == 'int':
                print >> self.out, '%s = (R_object)PY_SMALL_INT(parse_%s(lexer));' % (self.value, x.name)
            else:
                print >> self.out, '%s = (R_object)parse_%s(lexer);' % (self.value, x.name)
        else:
            print >> self.out, '%s = convert_%s_to_%s(parse_%s(lexer));' % (self.value, x.name, self.tipe, x.name)
            print >> self.prototypes, '%s convert_%s_to_%s(%s x);' % (self.tipe, x.name, self.tipe, get_type(x))
        
    def parser(self, r):
        t = self.tipe = get_type(r)
        self.value = self.next_value()
        print >> self.header, '%s parse_%s(R_Lexer lexer);' % (t, r.name)
        print >> self.out, '%s parse_%s(R_Lexer lexer) {' % (t, r.name)
        print >> self.out, 'int LA_KIND;'
        print >> self.out, '%s %s;' % (t, self.value)
        if not r.element.name:
            r.element.name = r.name
        r.element.visit(self)
        print >> self.out, 'return %s;' % self.value
        print >> self.out, '}\n'
       
if __name__ == '__main__':
    import sys
    rules = parse_grammar.read_rules(open(sys.argv[1]))
    read_types(open(sys.argv[2]))
    out = open(sys.argv[3] + '.c', 'w')
    header = open(sys.argv[3] + '.h', 'w')
    prototypes = open(sys.argv[3] + '-prototypes.h', 'w')
    print >> out, "/* This file autogenerated by %s from %s */" % tuple(sys.argv[:2])
    print >> out, '#include "hotpy.h"\n'
    print >> out, '#include "ast.h"'
    print >> out, '#include "c_lexer.h"'
    print >> out, '#include "tokens.h"'
    print >> out, '#include "%s.h"\n' % sys.argv[3]
    print >> header, '#ifndef _HOTPY_PARSER_AUTOGENERATED_%s_H' % sys.argv[3].upper()
    print >> header, '#define _HOTPY_PARSER_AUTOGENERATED_%s_H' % sys.argv[3].upper()
    print >> out, '#define lookahead(p) ((p)->lookahead ? (p)->lookahead : ((p)->lookahead = next_token((p)->lexer)))' 
    print >> out, '#ifdef NDEBUG'
    print >> out, '#define consume(p) (p)->lookahead = NULL'
    print >> out, '#else'
    print >> out, '#define consume(p) do { assert((p)->lookahead); (p)->lookahead = NULL; } while (0)'
    print >> out, '#endif'
    p = ParserGen(out, header, prototypes)
    for r in rules.values():
        p.parser(r)
    for s in all_sets:
        write_set(s, out, header)
    out.close()
    print >> header, '#endif'
    header.close()
    print >> prototypes
    prototypes.close()
    
