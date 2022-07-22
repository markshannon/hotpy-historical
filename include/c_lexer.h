#ifndef C_LEXER_H
#define C_LEXER_H
#include <wchar.h>
#include <wctype.h>

void syntax_error(R_str msg, int line, int column);

int _lexer_token_kind(R_Lexer);
R_str lexer_token_text(R_Lexer);
#define lexer_getline(l) ((l)->line)
#define lexer_getcolumn(l) ((l)->token_start-(l)->line_start)
void lexer_advance(R_Lexer);

#define lexer_token_kind(l) ((l)->token_kind ? (l)->token_kind : _lexer_token_kind(l))

#define MAX_INDENTS 32

GVMT_OBJECT(Lexer) {   // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint32_t pending_dedents;
    uint32_t new_line;
    uint32_t open_brackets;
    uint32_t indent_level;
    uint32_t line;
    uint32_t indents[32];
    uint16_t* next_char;   /* pointer */
    uint16_t* limit;    /* pointer */
    uint16_t* buffer;
    uint16_t* token_start;
    uint16_t* line_start;
    int token_kind;
    GVMT_OBJECT(str)* token_text;
    GVMT_OBJECT(object)* source;
    GVMT_OBJECT(str)* source_name;
    GVMT_OBJECT(str_buf)* part;
};

int next_string_and_fetch(R_Lexer l, const char* msg);
int next_string_and_peek(R_Lexer l);
void push_back(R_Lexer l);

int next_token(R_Lexer l);

void print_token(int t);

int string(int start, R_Lexer l);
int operator(int start, R_Lexer l);
int new_line(int start, R_Lexer l);
int line_continuation(int start, R_Lexer l);
int dot(int start, R_Lexer l);
int number(int start, R_Lexer l);
int raw_string(int start, R_Lexer l);

int identifier(int start, R_Lexer l);
int comment(int start, R_Lexer l);

R_Lexer new_lexer(R_object source, R_str source_name);
typedef int (*token_func)(int start, R_Lexer l);
extern token_func specials[256];
extern int token_table[256];
extern int operator1[256];
extern int operator_assign[256];
extern int operator2[256];
extern int operator2_assign[256];
extern int bracket_table[256];
extern int escape_chars['z'];
extern int digit_characters[256];
extern char* token_kind_names[];

//#define UNICODE_ALPHANUMERIC_CHARACTER(c) iswalnum(c)
//#define UNICODE_WORD_CHARACTER(c) iswalpha(c)

#define at_end(l) ((l)->next_char == (l)->limit)

#define fetch_character(l, msg) \
(at_end(l) ? next_string_and_fetch(l, msg) : *(l->next_char++))

#define peek_character(l) \
(at_end(l) ? next_string_and_peek(l) : *(l->next_char))

#define advance(l) \
(at_end(l) ? ((void)next_string_and_fetch(l, "Internal Error")) : ((void)(l->next_char++)))

extern int8_t ascii_identifier[129];

#define IS_ID(c) (((c)<128) ? ascii_identifier[c+1] : Py_UNICODE_ISALNUM(c))

#endif
