#Vistor template class
class Vistor: 
    
    def zero_or_one(self, x):
        pass
    
    def zero_or_many(self, x):
        pass
    
    def one_or_many(self, x):
        pass
        
    def optional_end(self, x):
        pass
        
    def list_(self, x):
        pass
            
    def binary(self, x):
        pass

    def skip_list(self, x):
        pass
        
    def or_(self, x):
        pass
    
    def token(self, x):
        pass
        
    def rule(self, x):
        pass
        
#Prints grammar
class PrettyPrinter:
    
    def zero_or_one(self, x):
        print '(',
        x.inst.visit(self)
        print ')?',
    
    def zero_or_many(self, x):
        print '(',
        x.inst.visit(self)
        print ')*',
    
    def one_or_many(self, x):
        print '(',
        x.inst.visit(self)
        print ')+',
        
    def optional_end(self, x):
        if x.start:
            x.start.visit(self)
        print '(',
        x.token.visit(self)
        x.repeats.visit(self)
        print ')*',
        print ' trailing',
        x.token.visit(self)
        
    def list_(self, x):
        for i in x.l:
            i.visit(self)
            
    def binary(self, x):
        x.left.visit(self)        
        print ' (',
        x.op.visit(self)
        x.right.visit(self)
        print ')*',

    def skip_list(self, x):
        x.left.visit(self)        
        print ' (',
        x.repeat.visit(self)
        print ')*',
        
    def or_(self, x):
        print ' (',
        it = iter(x.options)
        next(it).visit(self)
        for i in it:
            print '|', 
            i.visit(self)
        print ')',
    
    def token(self, x):
        print x.name,
        
    def rule(self, x):
        print x.name, 
 
