
template = r"""
\subsubsection*{%s (%s $\Rightarrow$ %s)} 
%s
%s
"""

import re
VERB = re.compile(r"\{([^}]+)\}")

def escape_chars(s):
    s = s.replace('_', r'\_').replace('#', r'\#')
    return s.replace('&', r'\&')

def fix_operators(s):
    s = s.replace('<<', '$\ll$').replace('>>', '$\gg$')
    s = s.replace('<=', '$\leq$').replace('>=', '$\geq$')
    s = s.replace('<', '$<$').replace('>', '$>$')
    s = s.replace('==', '$=$').replace('!=', '$\neq$')
    s = s.replace('^', '$\oplus$')
    return s.replace('|', r'$|$').replace('*', r'$\times$')
    
def texify(s):
    s = escape_chars(s)
    s = fix_operators(s)
    # Convert 0 instruction parameters to ns in documentation.
    s = s.replace('(0', '(n').replace('#0', '#n').replace('[0', '[n').replace('#@', r'#$\uparrow$')
    s = re.sub(VERB, '\\\\texttt{\g<1>}', s)
    # Make "nth" pretty.
    s = s.replace('0th', 'nth').replace('nth', 'n\\textsuperscript{th}')
    return s

def stack(items):
    if items:
        s = ', '.join([to_str(x) for x in items])
        return texify(s)
    else:
        return '---'

def to_str(x):
    if isinstance(x, tuple):
        assert len(x) == 2
        n, t = x
        return '%s %s' % (t,n)
    else:
        return str(x)
        
def parse_instruction(line):
    line = line.replace('--', '---')
    return "\\subsubsection{" + texify(line) + "}\n"
    
def extract_key(line):
    assert line[0] == '$'
    return line[1:line.index('(')].strip()
    
def title(stars, text):
    if stars == 1:
        t = "\\section{"
    elif stars == 2:
        t = "\\subsection"
    else:
        raise Exception("Wrong number of *")
    return t + text + "}\n"
    
def print_section(name):
        print("\\section{" + texify(name) + "}")
    
def print_subsection(name): 
        print("\\subsection{" + texify(name) + "}")

def print_instructions(names, all_instructions):
    names = sorted(names)
    for name in names:
        print(all_instructions[name])
            
            
if __name__ == '__main__':
    import sys
    f = open(sys.argv[1])
    lines = f.readlines()
    f.close()
    key = None 
    instructions = {}
    compacted = False
    for line in lines:
        if line and line[0] == '$':
            if key:
                text += "\\vspace{-1em}"
                instructions[key] = text
            key = extract_key(line)
            text = parse_instruction(line[1:-1])
            text += "\\vspace{-1em}"
            compacted = True
        elif line and line[0] == '%':
            if not compacted:
                text += "\\vspace{-1em}"
            text += "\\begin{verbatim}\n  "
            text += line[1:-1].lstrip() + '\n'
            text += "\\end{verbatim}\n"
            text += "\\vspace{-1em}"
            compacted = True
        elif line and line[0] == '*':
            pass
            # Ignore
            #stars = 1
            #while line[stars] == '*':
            #    stars += 1
            #text += title(stars, line[stars:-1]) 
        elif line.strip():
            text += texify(line[:-1]) + " "
            compacted = False
    instructions[key] = text
    f = open(sys.argv[2])
    lines = f.readlines()
    f.close()
    base_subs = dict(simple=[], compound=[], replace=[], no=[])
    sections = dict(base=base_subs, trace=[], spec=[], doc=[], super=[])  
    for line in lines:
        if not line.strip() or line[0] == '#':
            continue
        name, rest = line.split(':')
        name = name.strip()
        if ',' in rest:
            category, sub = rest.split(',')
        else:
            category = rest
            sub = ''
        category = category.strip()
        sub = sub.strip()
        if sub:
            sections[category][sub].append(name)
        else:
            sections[category].append(name)
    print(r"""
        All instructions are shown in the GVMT interpreter description format of
        name followed by stack effect and instruction effect. 
        Values on the left of the --- divider are inputs, those on the right are 
        outputs. All outputs go the stack. Inputs come from the stack unless 
        marked with a \#, in which case they are fetched from the instruction
        stream. \#x is a one byte value, \#\#x is a two byte value. \#$\uparrow$ is a
        pointer sized value.
        
    For example: \vspace{-1em}
        \subsubsection{truth(R\_object o  --- R\_bool b)}\vspace{-1em}
        Explanatory text follows the stack effect.
        
        
        """)
            
    print_section("Base Instructions")
    print("""
    The instructions listed in this section are those required to express 
    unoptimised Python programs. The output of the source-to-bytecode compiler
    consists entirely of these bytecodes.""")
    print_subsection("Atomic Instructions")
    print("""
    These instructions are treated as atomic by the optimisers. 
    They are recorded directly by tracing and either left intact or 
    removed entirely by subsequent optimisations.""")
    print_instructions(sections['base']['simple'], instructions)
    print_subsection("Compound Instructions")
    print("""
    These instructions can be defined in terms of other instructions.
    For example the \\verb|binary| bytecode can be defined in Python, 
    as shown in Appendix \\ref{app:surrogates}.
    These bytecodes can be replaced by a call to a function that 
    implements the same functionality. However, this only done during tracing.
""")
    print_instructions(sections['base']['compound'], instructions)
    print_subsection("Instructions Replaced During Tracing")
    print("""
        These instructions are replaced during tracing with a single alternative.
        Jumps are eliminated and conditional branches are replaced with 
        condiitonal exits. 
    """)
    print_instructions(sections['base']['replace'], instructions)
    print_subsection("Instructions Not Allowed in a Trace")
    print("""
        The following instructions have complex semantics and are expected 
        to occur only in start-up code. If any of thme are encountered during 
        tracing the trace is abandoned and normal interpretation continues.""") 
    print_instructions(sections['base']['no'], instructions)
    print_section("Instructions Required for Tracing")
    print("""
    The instructions required for tracing are mainly equivalents of 
    branch instructions that exit the trace instead.
    For example the \\verb|on_true| bytecode which branhces if the TOS evaluates 
    as true will be replaced with \\verb|exit_on_false| if the branch was taken
    or \\verb|exit_on_true| if it was not.
    """)
    print_instructions(sections['trace'], instructions)
    print_section("Specialised Instructions")
    print("""
    Specialised instructions are used when the type of the operands are known.
    Many are of the form \\verb|i_xxx| or \\verb|f_xxx| which are operations
    specialised for integers and floats respectively.
    The \\verb|native_call| instruction allows C functions to be called directly 
    inplace of the \\verb|f_call| or \\verb|binary| bytecodes, when the tyes are known.
        """)
    print_instructions(sections['spec'], instructions)
    print_section("D.O.C. Instructions")
    print("""
        These instructions are those required by the Deferred Object Creation pass.
        They are either related to unboxing floating point operations, 
        or to storing values in the (thread-local) cache, in order to avoid creating frames.
        """)
    print_instructions(sections['doc'], instructions)
    print_section("Super Instructions")
    print("""
        Super-instructions are concatenations of other instructions.
        For example, the instruction \\verb|line_none| is the concatenation
        of the instructions \\verb|line| and \\verb|none|.""")
    print_instructions(sections['super'], instructions)


