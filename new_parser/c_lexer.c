#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <alloca.h>
#include <assert.h>
#include <malloc.h>
#include "hotpy.h"
#include "tokens.h"
#include "c_lexer.h"
#include "operators.h"
#include "unicodeobject.h"

extern R_type type_Token;
extern R_type type_Stream;
extern R_type type_Lexer;
extern R_type type_SyntaxError;
extern R_type type_InternalError;
extern R_type type_ValueError;

int make_float_token(R_Lexer l);
int big_string(int start, R_Lexer l, int raw);

token_func specials[256];
int token_table[256];
int operator1[256];
int operator_assign[256];
int operator2[256];
int operator2_assign[256];
int bracket_table[256];

#define BUFFER_SIZE 1024

extern int hex_digit(Py_UNICODE c);

static Py_UNICODE unexpected_end_of_file(R_Lexer l, const char* what) {
    const char* fmt = "Unexpected end-of-file in %s";
    R_str msg = py_sprintf(fmt, string_from_c_string(what));
    raise_str(type_SyntaxError, msg);
    return 0;   
}

void syntax_error(R_str msg, int line, int column) {
    msg = py_sprintf("%s at line %i, column %i", msg, PY_SMALL_INT(line),
                     PY_SMALL_INT(column));
    raise_str(type_SyntaxError, msg);
}

R_str token_text(R_Lexer l, uint16_t* end) {
    R_str s;
    if (l->part == NULL) {
        s = string_from_chars(l->token_start, end - l->token_start);
    } else {        
        str_buf_append_chars(l->part, l->token_start, end - l->token_start);
        s = str_buf_to_str(l->part);
        l->part = NULL;
    }
    return s;
}

int next_string_and_peek(R_Lexer l) {
    call_func next = CALLABLE(l->source);
    R_str s;
    R_BaseException ex = NULL;
    int len;
    s = (R_str)next(l->source, empty_tuple, NULL);
    if (!isinstance(s, type_str)) {
        R_str msg = py_sprintf("Expected string not '%s'", TYPE_NAME(s));
        raise_str(type_TypeError, msg);
    }
    if (l->token_start != l->next_char) {
        if (l->part == NULL) {
            l->part = make_str_buf();
        }
        str_buf_append_chars(l->part, l->token_start, l->limit - l->token_start);
    }
    len = LEN(s);
    if (len == 0) {
        l->next_char = l->limit = l->token_start = l->line_start = NULL;
        return -1;
    } else if (len < BUFFER_SIZE) {
        // Copy   
        int i;
        for (i = 0, len = LEN(s); i < len; i++) {
            l->buffer[i] = s->text[i];
        }
        l->line_start = l->buffer + (l->line_start - l->next_char);
        l->next_char = l->token_start = l->buffer;
        l->limit = l->next_char + len;
    } else {
        // Pin
        void *ptr = gvmt_pin((GVMT_Object)s);
        uint16_t* start = (Py_UNICODE*)(((char*)ptr) + offsetof(GVMT_OBJECT(str), text[0]));
        l->line_start = start + (l->line_start - l->next_char);
        l->next_char = l->token_start = start;
        l->limit = l->next_char + len;
    }
    return *(l->next_char);
}

int next_string_and_fetch(R_Lexer l, const char* msg) {
    int c = next_string_and_peek(l);
    if (c < 0) {
        unexpected_end_of_file(l, msg);
    }
    advance(l);
    return c;
}

void push_back(R_Lexer l) {
    if (l->next_char == l->limit) {
        // At EOF, so no change of state.
    } else {
        l->next_char--; 
    }
}

R_Lexer new_lexer(R_object source, R_str source_name) {
    void* ptr;
    R_Lexer l = gc_allocate(Lexer);
    SET_TYPE(l, type_Lexer);
    l->line = 1;
    l->buffer = (Py_UNICODE*)malloc(sizeof(Py_UNICODE)*BUFFER_SIZE);
    l->next_char = l->limit = NULL;
    l->source = source;
    l->source_name = source_name;
    l->token_kind = 0;
    l->token_text = NULL;
    return l;
}

int new_line(int c, R_Lexer l) {
    if (l->open_brackets) {
        l->line++;
        return next_token(l);
    } else {
        l->new_line = 1;
        return kind_NEWLINE;
    }
}

int dot(int start, R_Lexer l) {
    int c;
    c = peek_character(l);
    if  (c == '.') {
        advance(l);
        c = peek_character(l);
        if  (c == '.') {
            advance(l);
            return kind_ELLIPSIS;
        } else {
            push_back(l);
            return kind_DOT;
        }
    } else if (c >= '0' && c <= '9') {
        return make_float_token(l);
    } else {
        return kind_DOT;   
    }
}

void lexer_advance(R_Lexer l) {
    if (l->token_kind == 0) {  
        next_token(l);
        l->token_text = NULL;
    } else {
        l->token_kind = 0;
        l->token_text = NULL;
    }
}

int _lexer_token_kind(R_Lexer l) {
    if (l->token_kind == 0) {  
        l->token_text = NULL;
        l->token_kind = next_token(l);
    }
    return l->token_kind;
}

R_str lexer_token_text(R_Lexer l) {
    if (l->token_kind == 0) {  
        l->token_text = NULL;
        l->token_kind = next_token(l);
    }
    if (l->token_text == 0) {
        l->token_text = token_text(l, l->next_char);
    }
    return l->token_text;
}

int next_token(R_Lexer l) {
    int t;
    int c, c2;
    start:
    if (l->pending_dedents) {
        l->pending_dedents--;
        return kind_DEDENT;
    }
    l->token_start = l->next_char;
    c = peek_character(l);
    if (c < 0) {
        if (l->new_line) {
            l->line += 1;
            l->line_start = l->next_char;
            l->new_line = 0;
        }
        if (l->indent_level) {
            l->indent_level--;
            return kind_DEDENT;
        } else {
            return kind_ENDMARKER;
        }
    }
    if (l->new_line && l->open_brackets == 0) {
        int spaces;
        do {
            spaces = 0;
            do {
                if (c == ' ') {
                    spaces ++;
                } else if (c == '\t') {
                    spaces += 8;                
                } else {
                    break;   
                }
                advance(l);
                c = peek_character(l);
            } while (1);
            if (c != '\n')
                break;
            l->line++;
            advance(l);
            c = peek_character(l);
        } while (1);
        if (spaces > l->indents[l->indent_level]) {
            l->indent_level++;
            if (l->indent_level == MAX_INDENTS) {
                R_str msg = py_sprintf("Too many levels of indentation at line %i", R_int_from_ssize_t(l->line));
                raise_str(type_SyntaxError, msg);
            }
            l->indents[l->indent_level] = spaces;
            l->new_line = 0;
            l->line += 1;
            l->line_start = l->next_char;
            return kind_INDENT;
        } else if (spaces < l->indents[l->indent_level]) {
            l->indent_level--;
            while(spaces < l->indents[l->indent_level]) {
                l->indent_level--;
                l->pending_dedents++;
            }
            if (spaces != l->indents[l->indent_level]) {
                R_str msg = py_sprintf("Illegal indent at line %i", R_int_from_ssize_t(l->line));
                raise_str(type_SyntaxError, msg);
            }
            l->new_line = 0;
            l->line += 1;
            l->line_start = l->next_char;
            return kind_DEDENT;   
        }
        if (l->new_line) {
            l->line += 1;
            l->line_start = l->next_char;
            l->new_line = 0;
        }
    } else {
        if (l->new_line) {
            l->line += 1;
            l->line_start = l->next_char;
            l->new_line = 0;
        }
        while ((c == ' ') | (c == '\t')) {
            advance(l);
            c = peek_character(l); 
        }
    }
    l->token_start = l->next_char;
    c = peek_character(l);
    assert(l->new_line == 0);
    if (c < 256) {
        if (c < 0) {
            if (l->indent_level) {
                l->indent_level--;
                return kind_DEDENT;
            } else {
                return kind_ENDMARKER;
            }
        }
        advance(l);
        t = token_table[c];
        if (t) {
            l->open_brackets += bracket_table[c];
            return t;
        } else {
            token_func f = specials[c];
            if (f) {
                t = f(c, l);
                if (t)
                    return t;
                else
                    goto start;
            } else {
                R_str ch = single_character(c);
                R_str msg = py_sprintf("Illegal character \'%s\' at line %i", ch, R_int_from_ssize_t(l->line));
                raise_str(type_SyntaxError, msg);
                return 0;
            }
        }
    } else if (Py_UNICODE_ISALPHA((Py_UNICODE)c)) {
        advance(l);
        return identifier(c, l);
    } else {
        R_str msg = py_sprintf("Illegal character U%i at line %i", R_int_from_ssize_t(c), R_int_from_ssize_t(l->line)); 
        raise_str(type_SyntaxError, msg);
        return 0;
    }
}

static int _string(int start, R_Lexer l, int raw) {
    int len = 0;
    Py_UNICODE buffer[256];
    int c = fetch_character(l, "string literal");
    int lsh, msh;
    if (c == start) {
        int c3 = peek_character(l);
        if (c3 == c) {
            advance(l);
            return big_string(start, l, raw);
        } else {
            return kind_STRING;
        }
    }
    do {
        if (c == start)
            goto done;
        if (c =='\\' && !raw) {
            c = fetch_character(l, "string literal");
            if (c == 'x') {
                c = fetch_character(l, "string literal");
                msh = hex_digit(c);
                if (msh < 0) {
                    R_str msg = py_sprintf("Illegal hexidecimal digit \'%s\' at line %i", single_character(c), R_int_from_ssize_t(l->line));
                    raise_str(type_SyntaxError, msg);
                }
                c = fetch_character(l, "string literal");
                lsh = hex_digit(c);
                if (lsh < 0) {
                    R_str msg = py_sprintf("Illegal hexidecimal digit \'%s\' at line %i", single_character(c), R_int_from_ssize_t(l->line));
                    raise_str(type_SyntaxError, msg);
                }
                c = msh << 4 || lsh;
            } else {
                if (c < 'z') {
                    int e = escape_chars[c];
                    if (e < 0) {
                        R_str msg = py_sprintf("Illegal sequence \'\\%s\' at line %i", single_character(c), R_int_from_ssize_t(l->line));
                        raise_str(type_SyntaxError, msg);
                    }
                    c = e;
                } else {
                    R_str msg = py_sprintf("Illegal sequence \'\\%s\' at line %i", single_character(c), R_int_from_ssize_t(l->line));
                    raise_str(type_SyntaxError, msg);   
                }
            }
        } else if (c == -1) {
            raise_char(type_SyntaxError, "Reached end of file in string literal");
        } else if (c == '\n') {
            raise_char(type_SyntaxError, "Reached end of line in single quoted string literal");
        } else {
            buffer[len] = c;
            len++;
            if (len >= 256) {
                raise_char(type_SyntaxError, "Too long line in single quoted string literal");  
            }
        }
        c = fetch_character(l, "string literal");
    } while (1); 
    done:
    return kind_STRING;
}

int string(int start, R_Lexer l) {
    return  _string(start, l, 0);
}
    
int raw_string(int start, R_Lexer l) {
    return  _string(start, l, 0);
}

int big_string(int start, R_Lexer l, int raw) {
    Py_UNICODE fixed_buffer[256];
    Py_UNICODE *buffer = fixed_buffer;
    int len = 0;
    int limit = 256;
    int quotes = 0;
    int c1, c2, c3;
    c1 = fetch_character(l, "string literal");
    if (c1 == start) 
        quotes++;
    if (c1 == '\n') 
        l->line++;
    c2 = fetch_character(l, "string literal");
    if (c2 == start) 
            quotes++;
    if (c2 == '\n') 
        l->line++;
    c3 = fetch_character(l, "string literal");
    do {
        if (c3 == '\n') 
            l->line++;
        if (c3 == start) {
            if (quotes == 2) {
                goto done;
            } else {
                quotes++;
            }
        } else {
            quotes = 0;   
        }
        buffer[len] = c1;
        len++;
        if (len == limit) {
            Py_UNICODE* new_buffer = malloc(limit * 2 * sizeof(Py_UNICODE));
            memcpy(new_buffer, buffer, limit * sizeof(Py_UNICODE));
            if (limit > 256) {
                free(buffer);
            }
            limit *= 2;
            buffer = new_buffer;
        }
        c1 = c2;
        c2 = c3;
        c3 = fetch_character(l, "string literal");
    } while (1);
    done:
    return kind_STRING;
}

int operator(int c, R_Lexer l) {
    R_str msg;
    int c2;
    c2 = peek_character(l);
    if (c2 == '=') {
        advance(l);
        return operator_assign[c];
    } else if (c2 == c) {
        advance(l);
        c2 = peek_character(l);
        if (c2 == '=') {
            advance(l);
            return operator2_assign[c];
        } else {
            return operator2[c];
        }
    } else {
        if (c2 == '>' && c == '-') {
            advance(l);
            return kind_ARROW;   
        } else {
            return operator1[c];
        }
    }
}

// Is this correct?
int comment(int start, R_Lexer l) {
    int c;
    int lines = 0;
    do {
        lines++;
        do {
            if (at_end(l)) {
                return 0;
            }
            c = peek_character(l);
            advance(l);
        } while (c != '\n');
        if (at_end(l)) {
            return 0;
        }
        c = peek_character(l);
        if (c != '#') {
            l->line += lines;
            return 0;
        }
    } while (1);
    // Keep compiler happy
    return 0;
}


int identifier(int start, R_Lexer l) {
    int c;
    assert(IS_ID('_'));
    c = peek_character(l);
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

int line_continuation(int start, R_Lexer l) {
    R_str msg;
    int c;
    if (at_end(l))
        return 0;
    c = peek_character(l);
    advance(l);
    do {
        if (c == '\n') {
            l->line++; 
            return 0;
        } else if (c == ' ' || c == '\t') {
            advance(l);
        } else {
            msg = py_sprintf("Illegal characters after \\ at line %i", R_int_from_ssize_t(l->line));
            raise_str(type_SyntaxError, msg);   
        } 
    } while (1);
    return 0;
}

extern R_float parse_float(R_str s, R_type overflow);

int make_float_token(R_Lexer l) {
    int c = peek_character(l);
    while (c >= '0' && c <= '9') {
        advance(l);
        c = peek_character(l);
    }
    if (c == 'e' || c == 'E') {
        advance(l);
        c = fetch_character(l, "float literal");
        if (c == '-' || c == '+') {
            c = fetch_character(l, "float literal");
        }
        if (c < '0' || c > '9') {
            R_str msg = py_sprintf("Illegal floating point constant '%s'",
                                   lexer_token_text(l));
            syntax_error(msg ,lexer_getline(l), lexer_getcolumn(l));
        }
        c = peek_character(l);
        while (c >= '0' && c <= '9') {
            advance(l);
            c = peek_character(l);
        }
    }
    return kind_FLOAT;
}

extern R_object parse_int(R_str s, int base);

#define DIGIT(c) (((c) < 128) ? ascii_digits[c] : Py_UNICODE_TODIGIT(c))

static int non_decimal(R_Lexer l, int base) {
    int c = fetch_character(l, "integer literal");
    int digit, value;
    value = Py_UNICODE_TODIGIT((Py_UNICODE)c);
    if (value < 0)
        goto error;
    c = peek_character(l);
    digit = Py_UNICODE_TODIGIT((Py_UNICODE)c);
    while (c >= 0 && digit < base) {
        advance(l);
        if (at_end(l))
            break;
        c = peek_character(l);
        digit = Py_UNICODE_TODIGIT((Py_UNICODE)c);
    }
    return kind_INTEGER;
error:
    {
        R_str msg = py_sprintf("Illegal token at line %i", R_int_from_ssize_t(l->line));     
        raise_str(type_SyntaxError, msg);   
    }
    return 0;
}

int number(int start, R_Lexer l) {
    int integral = start - '0';
    int c;
    c = peek_character(l);
    if (integral == 0) {
        if (c == 'x' || c == 'X') {
            advance(l);
            return non_decimal(l, 16); 
        } else if (c == 'o' || c == 'O') {
            advance(l);
            return non_decimal(l, 8); 
        } else if (c == 'b' || c == 'B') {
            advance(l);
            return non_decimal(l, 2); 
        }
    } 
    while (c >= '0' && c <= '9') {
        advance(l);
        c = peek_character(l);
    }
    if (c == '.') {
        advance(l);
        return make_float_token(l);
    } else if (c == 'e' || c == 'E') {
        return make_float_token(l);
    } else {
        return kind_INTEGER;
    }
}

