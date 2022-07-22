    
import token_gen

class Element(object):
    pass
    
class AmbiguityError(Exception):
    
    def __init__(self, el, amb):
        self.el = el
        self.tokens = [ token_gen.all_names[t] for t in amb]
     
    def __str__(self):
        return "Ambiguity in %s for the following tokens: %s" % (self.el.nice_name(), self.tokens)
    
class ZeroOrOne(Element):
    
    def __init__(self, inst):
        
        self.inst = inst
        self.name = None
        
    def optional(self):
        return True
        
    def la1(self):
        return self.inst.la1()
        
    def __eq__(self, other):
        return self.__class__ is other.__class__ and self.inst == other.inst
        
    def nice_name(self):
        return self.inst.nice_name()
        
    def verify(self):
        self.inst.verify()
        
    def visit(self, visitor):
        visitor.zero_or_one(self)
    
class ZeroOrMany(Element):
    
    def __init__(self, inst):
        self.inst = inst
        self.name = None
        
    def optional(self):
        return True
        
    def la1(self):
        return self.inst.la1()
        
    def nice_name(self):
        n = self.inst.nice_name()
        return n + '_list'
        
    def verify(self):
        self.inst.verify()
        
    def visit(self, visitor):
        visitor.zero_or_many(self)
        
class OptionalEnd(Element):
    
    def __init__(self, start, repeats, end):
        self.start = start
        assert repeats.__class__ == List
        if len(repeats.l) == 1:
            self.repeats = repeats.l[1]
        else: 
            self.repeats = List(repeats.l[1:])
        self.token = end
        self.name = None
            
    def optional(self):
        return self.start == None or self.start.optional()
        
    def la1(self):
        if self.start == None:
            return self.token.la1()
        elif self.start.optional():
            return self.start.la1().union(self.token.la1())
        else:
            return self.start.la1()
                    
    def verify(self):
        self.token.verify()
        self.repeats.verify()
        if self.start:
            self.start.verify()
            if self.start.optional():
                if self.start.la1().intersection(self.token.la1()):
                    raise AmbiguityError(self, self.start.la1().intersection(self.token.la1()))

    def nice_name(self):
        if self.start == None:
            return self.token.nice_name() + '_' + self.repeats.nice_name()
        else:
            return self.start.nice_name() + '_' + self.token.nice_name() + '_' + self.repeats.nice_name()
        
    def visit(self, visitor):
        visitor.optional_end(self)
        
class OneOrMany(Element):
    
    def __init__(self, inst):
        self.inst = inst
        self.name = None
        
    def optional(self):
        return self.inst.optional()
        
    def la1(self):
        return self.inst.la1()
        
    def verify(self):
        self.inst.verify()

    def nice_name(self):
        n = self.inst.nice_name()
        return n + '_list'
        
    def visit(self, visitor):
        visitor.one_or_many(self)

def ignore_part(t):
    if isinstance(t, ZeroOrOne):
        return ignore_part(t.inst)
    else:
        return (t.name in token_gen.all_tokens or 
                t.name == 'NEWLINE' or
                t.name == 'ENDMARKER')

class List(Element):
    
    def __init__(self, l):
        self.l = l
        self.name = None
        assert len(l)
        self.la1_set = set()
        
    def optional(self):
        return all(x.optional() for x in self.l)
        
    def la1(self):
        if not self.la1_set:
            for i in self.l:
                self.la1_set = self.la1_set.union(i.la1())
                if not i.optional():
                    break
        return self.la1_set
        
    def verify(self):
        for i in self.l:
            i.verify()
        s = set()
        for i in self.l:
            if s.intersection(i.la1()):
                raise AmbiguityError(self, s.intersection(i.la1()))
            if i.optional():
                s = s.union(i.la1())
            else:
                s = set()
        
    def nice_name(self):
        l =[]
        for t in self.l:
            if not ignore_part(t):
                l.append(t.nice_name())
        if len(l) == 1:
            return l[0]
        else:
            return '_'.join(l)
        
    def visit(self, visitor):
        visitor.list_(self)
        
class Binary(Element):
    
    def __init__(self, left, op, right):
        self.left = left
        self.op = op
        self.right = right
        self.name = None
            
    def optional(self):
        return False
        
    def la1(self):
        if self.left.optional():
            return self.left.la1().union(self.op.la1())
        else:
            return self.left.la1()
        
    def verify(self):
        self.left.verify()
        self.op.verify()
        self.right.verify()
        if self.left.optional():
            if self.left.la1().intersection(self.op.la1()):
                raise AmbiguityError(self, self.left.la1().intersection(self.op.la1()))
        
    def visit(self, visitor):
        visitor.binary(self)
        
class SkipList(Element):
    
    def __init__(self, left, repeat):
        self.left = left
        assert isinstance(repeat, Element)
        self.repeat = repeat
        self.name = None
            
    def optional(self):
        return self.left.optional()
        
    def la1(self):
        if self.left.optional():
            return self.left.la1().union(self.repeat.la1())
        else:
            return self.left.la1()
        
    def verify(self):
        self.left.verify()
        self.repeat.verify()
        if self.left.optional():
            if self.left.la1().intersection(self.repeat.la1()):
                raise AmbiguityError(self, self.left.la1().intersection(self.repeat.la1()))
        
    def visit(self, visitor):
        visitor.skip_list(self)
        
class Or(Element):
    
    def __init__(self, opts, name = None):
        self.name = name
        self.options = opts
        self.start_set = set()
        
    def optional(self):
        return any(x.optional() for x in self.options)
        
    def nice_name(self):
        for o in self.options:
            if not o.nice_name():
                return None
        return '_or_'.join([o.nice_name() for o in self.options])
        
    def init(self):
        self.options = [insts[n] for n in self.options]
                            
    def la1(self):
        if not self.start_set:
#            self.options = [insts[n] for n in self.options]
            for o in self.options:
                self.start_set = self.start_set.union(o.la1())
        return self.start_set
                            
    def verify(self):
        ambiguity = set()
        possibles = set()
        for o in self.options:
            ambig = possibles.intersection(o.la1())
            possibles = possibles.union(o.la1())
            ambiguity = ambiguity.union(ambig)
        if ambiguity:
            raise AmbiguityError(self, ambiguity)

    def visit(self, visitor):
        visitor.or_(self)
                
class Token(Element):
    
    def __init__(self, name):
        self.name = name
        self.id = token_gen.all_names.index(name)
        self.start_set = set([self.id])
    
    def optional(self):
        return False
        
    def __eq__(self, other):
        return self.__class__ is other.__class__ and self.name == other.name
        
    def la1(self):
        return self.start_set
        
    def verify(self):
        pass        
        
    def nice_name(self):
        return self.name.lower()
        
        
    def visit(self, visitor):
        visitor.token(self)

