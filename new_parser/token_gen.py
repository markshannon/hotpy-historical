def make_tries(tkns, f):
    letters = {}
    for name in tkns:
        text = tkns[name]
        if text[0] not in letters:
            letters[text[0]] = []
        letters[text[0]].append((text, name))
    for c in letters:
        options = letters[c]
        print_trie(c, make_trie(options), f)
            
def make_trie(options):
    trie = {}
    for text, name in options:
        level = trie
        for c in text[1:-1]:
            if c in level:
                level = level[c]
            else:
                new_level = {}
                level[c] = new_level
                level = new_level
        level[text[-1]] = name
    return trie
    
def print_trie(c, trie, f):
    print >> f, "static int keyword_%s(int start, R_Lexer l) {" % c
    print >> f, "     int i;"
    print >> f, "     int c;"
    if c == 'r':
        print >> f, """  c = peek_character(l);   
     if (c == '\\'') {
         return raw_string('\\'', l);
     } else if (c == '\\"') {
         return raw_string('\\"', l);
     }"""
    print_trie_recursive(trie, f, c)
    print >> f, '    state_id:'     
    print >> f, """    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }"""
    print >> f, "    return kind_NAME;"
    print >> f, "}\n"
    
def print_trie_recursive(trie, f, state):
    pre = ''
    print >> f, "state_%s:" % state
    if len(state) > 1:
        print >> f, "    advance(l);" 
    if isinstance(trie, str):
        print >> f, "    if (at_end(l))"
        print >> f, "        return kind_%s;" % trie.upper()
        print >> f, "    c = peek_character(l);"
        print >> f, "    if (IS_ID(c))"
        print >> f, "        goto state_id;"
        print >> f, "    else"
        print >> f, "        return kind_%s;" % trie.upper()
    else:
        print >> f, "    c = peek_character(l);"
        for c in trie:
            print >> f, "    if (c == '%s') goto state_%s%s;" % (c, state, c)
        print >> f, "    goto state_id;"      
        for c in trie:
            d = trie[c]
            print_trie_recursive(d, f, state + c)
        
kind_names = {
"STRING": "a string",
"NAME" : "an identifier",
"INTEGER": "an integer",
"FLOAT": "a float",
"NEWLINE": "a new line",
"ENDMARKER": "the end of input"
}

operator_tokens = {
"EQUAL" : "==",
"NOTEQUAL" : "!=",
"GREATER" : ">",
"GREATEREQUAL" : ">=",
"LESS" : "<",
"LESSEQUAL" : "<=",
"PLUS" : "+",
"PLUSEQUAL" : "+=",
"MINUS" : "-",
"MINUSEQUAL" : "-=",
"STAR" : "*",
"STAREQUAL" : "*=",
"SLASH" : "/",
"SLASHEQUAL" : "/=",
"PERCENT" : "%",
"PERCENTEQUAL" : "%=",
"AMPER" : "&",
"AMPEREQUAL" : "&=",
"CIRCUMFLEX" : "^",
"CIRCUMFLEXEQUAL" : "^=",
"RIGHTSHIFT" : ">>",
"RIGHTSHIFTEQUAL" : ">>=",
"LEFTSHIFT" : "<<",
"LEFTSHIFTEQUAL" : "<<=",
"DOUBLESTAR" : "**",
"DOUBLESTAREQUAL" : "**=",
"DOUBLESLASH" : "//", 
"DOUBLESLASHEQUAL" : "//=",
"VBAR" : "|", 
"VBAREQUAL" : "|=",
"ASSIGN" : "=",
}

bracket_tokens = {
"LPAREN" : "(",
"RPAREN" : ")",
"LCURLY" : "{",
"RCURLY" : "}", 
"LBRACK" : "[",
"RBRACK" : "]",
}

unique_char_tokens = {
"COMMA" : ",",
"COLON" : ":",
"SEMI" : ";",
"TILDE" : "~",
"AT" : "@",
}

keyword_tokens = {
"AS" : "as",
"AND" : "and",
"ASSERT" : "assert",
"BREAK" : "break",
"CLASS" : "class",
"CONTINUE" : "continue",
"DEF" : "def",
"DEL" : "del",
"ELSE" : "else",
"ELIF" : "elif",
"EXCEPT" : "except",
"EXEC" : "exec",
"FALSE" : "False", 
"FOR" : "for",
"FROM" : "from",
"FINALLY" : "finally",
"GLOBAL" : "global",
"IF" : "if",
"IN" : "in",
"IS" : "is",
"IMPORT" : "import",
"LAMBDA" : "lambda",
"NONLOCAL" : "nonlocal", 
"NOT" : "not",
"OR" : "or",
"PASS" : "pass",
"RAISE" : "raise",
"RETURN" : "return",
"TRUE" : "True", 
"TRY" : "try",
"XOR" : "xor",
"WHILE" : "while",
"WITH" : "with",
"YIELD" : "yield",
"NONE" : "None",
}

other_tokens = {
"DEDENT" : "DEDENT",
"INDENT" : "INDENT",
"ARROW" : "->",
"DOT" : ".",
"ELLIPSIS": "..."
}

all_tokens = {}
all_tokens.update(operator_tokens)
all_tokens.update(bracket_tokens)
all_tokens.update(unique_char_tokens)
all_tokens.update(keyword_tokens)
all_tokens.update(other_tokens)

all_names = [ 'ERROR' ] + sorted(kind_names.keys() + all_tokens.keys())

f = open('tokens.h', 'w')
print >> f, '#ifndef TOKEN_H'
print >> f, '#define TOKEN_H'
print >> f, '''#include "hotpy.h"

'''
i = 0
for name in all_names:
    print >> f, '#define kind_%s %d' % (name.upper(), i)
    i += 1
print >> f
print >> f, "#endif"
f.close()

f = open('tokens.c', 'w')
print >> f, '#include "hotpy.h"'
print >> f, '#include "tokens.h"'
print >> f, '#include "c_lexer.h"'
print >> f, '#include "core_objects.h"'
print >> f, '#include "unicodeobject.h"'
print >> f
print >> f, 'char* token_kind_names[] = { '
for name in all_names:
    print >> f, '"%s",' % name
print >> f, '};'
print >> f
print >> f, 'char* token_kind_display[] = { "Internal Error",'
for name in all_names[1:]:
    if name in all_tokens:
        print >> f, '"\\\'%s\\\'",' % all_tokens[name]
    else:
        print >> f, '"%s",' % kind_names[name]
print >> f, '};'
print >> f

make_tries(keyword_tokens, f)

print >> f, 'R_object init_tokens(void) {'
for name in unique_char_tokens:
    print >> f, "    token_table['%s'] = kind_%s;" % (unique_char_tokens[name], name)
for name in bracket_tokens:
    print >> f, "    token_table['%s'] = kind_%s;" % (bracket_tokens[name], name)
    if bracket_tokens[name] in '({[':
        print >> f, "    bracket_table['%s'] = 1;" % bracket_tokens[name]
    else:
        print >> f, "    bracket_table['%s'] = -1;" % bracket_tokens[name]
for name in operator_tokens:
    text = operator_tokens[name]
    print >> f, "    specials['%s'] = operator;" % text[0]
    if len(text) == 1:
        print >> f, "    operator1['%s'] = kind_%s;" % (text, name)
    elif len(text) == 2:
        if text[1] == '=':
            print >> f, "    operator_assign['%s'] = kind_%s;" % (text[0], name)
        else:
            assert text[0] == text[1]
            print >> f, "    operator2['%s'] = kind_%s;" % (text[0], name)
    else:
        assert len(text) == 3
        print >> f, "    operator2_assign['%s'] = kind_%s;" % (text[0], name)
alpha = "abcdefghijklmnopqrstuvwxyz"   
keyword_starts = set()
for name in keyword_tokens:
    keyword_starts.add(keyword_tokens[name][0])
for c in "_" + alpha + alpha.upper():
    if c in keyword_starts:
        print >> f, "    specials['%s'] = keyword_%s;" % (c, c)
    else:
        print >> f, "    specials['%s'] = identifier;" % c
for c in range(10):  
    print >> f, "    specials['%s'] = number;" % c
print >> f, "    specials['#'] = comment;"
print >> f, "    specials['.'] = dot;"
print >> f, "    specials['\\n'] = new_line;"
print >> f, "    specials['\\\\'] = line_continuation;"
print >> f, "    specials['\\''] = string;"
print >> f, "    specials['\\\"'] = string;"
print >> f, "    return 0;" 
print >> f, '}'

id_chars = [ 0 ] * 128
for c in '0123456789_':
    id_chars[ord(c)] = 1
for c in alpha + alpha.upper():
    id_chars[ord(c)] = 1
print >> f, "int8_t ascii_identifier[] = { 0, /* -1 == EOF */"
for c in range(128): 
    if id_chars[c]:
        print >> f, "    1, /* '%s' */" % chr(c)
    else:
        print >> f, "    0,"
print >> f, "};"
escape_chars = [ -1 ] * ord('z')  
escape_chars[ord('a')] = r"'\a'"
escape_chars[ord('b')] = r"'\b'"
escape_chars[ord('f')] = r"'\f'"
escape_chars[ord('n')] = r"'\n'"
escape_chars[ord('r')] = r"'\r'"
escape_chars[ord('t')] = r"'\t'"
escape_chars[ord('0')] = r"'\0'"
escape_chars[ord('\'')] = r"'\''"
escape_chars[ord('\"')] = r"'\"'"
escape_chars[ord('\\')] = r"'\\'"
print >> f, "int escape_chars[] = {"
for c in range(ord('z')): 
    print >> f, "    %s," % escape_chars[c]
print >> f, "};"

