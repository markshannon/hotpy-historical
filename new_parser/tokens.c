#include "hotpy.h"
#include "tokens.h"
#include "c_lexer.h"
#include "core_objects.h"
#include "unicodeobject.h"

char* token_kind_names[] = { 
"ERROR",
"AMPER",
"AMPEREQUAL",
"AND",
"ARROW",
"AS",
"ASSERT",
"ASSIGN",
"AT",
"BREAK",
"CIRCUMFLEX",
"CIRCUMFLEXEQUAL",
"CLASS",
"COLON",
"COMMA",
"CONTINUE",
"DEDENT",
"DEF",
"DEL",
"DOT",
"DOUBLESLASH",
"DOUBLESLASHEQUAL",
"DOUBLESTAR",
"DOUBLESTAREQUAL",
"ELIF",
"ELLIPSIS",
"ELSE",
"ENDMARKER",
"EQUAL",
"EXCEPT",
"EXEC",
"FALSE",
"FINALLY",
"FLOAT",
"FOR",
"FROM",
"GLOBAL",
"GREATER",
"GREATEREQUAL",
"IF",
"IMPORT",
"IN",
"INDENT",
"INTEGER",
"IS",
"LAMBDA",
"LBRACK",
"LCURLY",
"LEFTSHIFT",
"LEFTSHIFTEQUAL",
"LESS",
"LESSEQUAL",
"LPAREN",
"MINUS",
"MINUSEQUAL",
"NAME",
"NEWLINE",
"NONE",
"NONLOCAL",
"NOT",
"NOTEQUAL",
"OR",
"PASS",
"PERCENT",
"PERCENTEQUAL",
"PLUS",
"PLUSEQUAL",
"RAISE",
"RBRACK",
"RCURLY",
"RETURN",
"RIGHTSHIFT",
"RIGHTSHIFTEQUAL",
"RPAREN",
"SEMI",
"SLASH",
"SLASHEQUAL",
"STAR",
"STAREQUAL",
"STRING",
"TILDE",
"TRUE",
"TRY",
"VBAR",
"VBAREQUAL",
"WHILE",
"WITH",
"XOR",
"YIELD",
};

char* token_kind_display[] = { "Internal Error",
"\'&\'",
"\'&=\'",
"\'and\'",
"\'->\'",
"\'as\'",
"\'assert\'",
"\'=\'",
"\'@\'",
"\'break\'",
"\'^\'",
"\'^=\'",
"\'class\'",
"\':\'",
"\',\'",
"\'continue\'",
"\'DEDENT\'",
"\'def\'",
"\'del\'",
"\'.\'",
"\'//\'",
"\'//=\'",
"\'**\'",
"\'**=\'",
"\'elif\'",
"\'...\'",
"\'else\'",
"the end of input",
"\'==\'",
"\'except\'",
"\'exec\'",
"\'False\'",
"\'finally\'",
"a float",
"\'for\'",
"\'from\'",
"\'global\'",
"\'>\'",
"\'>=\'",
"\'if\'",
"\'import\'",
"\'in\'",
"\'INDENT\'",
"an integer",
"\'is\'",
"\'lambda\'",
"\'[\'",
"\'{\'",
"\'<<\'",
"\'<<=\'",
"\'<\'",
"\'<=\'",
"\'(\'",
"\'-\'",
"\'-=\'",
"an identifier",
"a new line",
"\'None\'",
"\'nonlocal\'",
"\'not\'",
"\'!=\'",
"\'or\'",
"\'pass\'",
"\'%\'",
"\'%=\'",
"\'+\'",
"\'+=\'",
"\'raise\'",
"\']\'",
"\'}\'",
"\'return\'",
"\'>>\'",
"\'>>=\'",
"\')\'",
"\';\'",
"\'/\'",
"\'/=\'",
"\'*\'",
"\'*=\'",
"a string",
"\'~\'",
"\'True\'",
"\'try\'",
"\'|\'",
"\'|=\'",
"\'while\'",
"\'with\'",
"\'xor\'",
"\'yield\'",
};

static int keyword_a(int start, R_Lexer l) {
     int i;
     int c;
state_a:
    c = peek_character(l);
    if (c == 's') goto state_as;
    if (c == 'n') goto state_an;
    goto state_id;
state_as:
    advance(l);
    if (at_end(l))
        return kind_AS;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_AS;
state_an:
    advance(l);
    c = peek_character(l);
    if (c == 'd') goto state_and;
    goto state_id;
state_and:
    advance(l);
    if (at_end(l))
        return kind_AND;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_AND;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_c(int start, R_Lexer l) {
     int i;
     int c;
state_c:
    c = peek_character(l);
    if (c == 'l') goto state_cl;
    if (c == 'o') goto state_co;
    goto state_id;
state_cl:
    advance(l);
    c = peek_character(l);
    if (c == 'a') goto state_cla;
    goto state_id;
state_cla:
    advance(l);
    c = peek_character(l);
    if (c == 's') goto state_clas;
    goto state_id;
state_clas:
    advance(l);
    c = peek_character(l);
    if (c == 's') goto state_class;
    goto state_id;
state_class:
    advance(l);
    if (at_end(l))
        return kind_CLASS;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_CLASS;
state_co:
    advance(l);
    c = peek_character(l);
    if (c == 'n') goto state_con;
    goto state_id;
state_con:
    advance(l);
    c = peek_character(l);
    if (c == 't') goto state_cont;
    goto state_id;
state_cont:
    advance(l);
    c = peek_character(l);
    if (c == 'i') goto state_conti;
    goto state_id;
state_conti:
    advance(l);
    c = peek_character(l);
    if (c == 'n') goto state_contin;
    goto state_id;
state_contin:
    advance(l);
    c = peek_character(l);
    if (c == 'u') goto state_continu;
    goto state_id;
state_continu:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_continue;
    goto state_id;
state_continue:
    advance(l);
    if (at_end(l))
        return kind_CONTINUE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_CONTINUE;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_b(int start, R_Lexer l) {
     int i;
     int c;
state_b:
    c = peek_character(l);
    if (c == 'r') goto state_br;
    goto state_id;
state_br:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_bre;
    goto state_id;
state_bre:
    advance(l);
    c = peek_character(l);
    if (c == 'a') goto state_brea;
    goto state_id;
state_brea:
    advance(l);
    c = peek_character(l);
    if (c == 'k') goto state_break;
    goto state_id;
state_break:
    advance(l);
    if (at_end(l))
        return kind_BREAK;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_BREAK;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_e(int start, R_Lexer l) {
     int i;
     int c;
state_e:
    c = peek_character(l);
    if (c == 'x') goto state_ex;
    if (c == 'l') goto state_el;
    goto state_id;
state_ex:
    advance(l);
    c = peek_character(l);
    if (c == 'c') goto state_exc;
    if (c == 'e') goto state_exe;
    goto state_id;
state_exc:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_exce;
    goto state_id;
state_exce:
    advance(l);
    c = peek_character(l);
    if (c == 'p') goto state_excep;
    goto state_id;
state_excep:
    advance(l);
    c = peek_character(l);
    if (c == 't') goto state_except;
    goto state_id;
state_except:
    advance(l);
    if (at_end(l))
        return kind_EXCEPT;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_EXCEPT;
state_exe:
    advance(l);
    c = peek_character(l);
    if (c == 'c') goto state_exec;
    goto state_id;
state_exec:
    advance(l);
    if (at_end(l))
        return kind_EXEC;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_EXEC;
state_el:
    advance(l);
    c = peek_character(l);
    if (c == 'i') goto state_eli;
    if (c == 's') goto state_els;
    goto state_id;
state_eli:
    advance(l);
    c = peek_character(l);
    if (c == 'f') goto state_elif;
    goto state_id;
state_elif:
    advance(l);
    if (at_end(l))
        return kind_ELIF;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_ELIF;
state_els:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_else;
    goto state_id;
state_else:
    advance(l);
    if (at_end(l))
        return kind_ELSE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_ELSE;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_d(int start, R_Lexer l) {
     int i;
     int c;
state_d:
    c = peek_character(l);
    if (c == 'e') goto state_de;
    goto state_id;
state_de:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_del;
    if (c == 'f') goto state_def;
    goto state_id;
state_del:
    advance(l);
    if (at_end(l))
        return kind_DEL;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_DEL;
state_def:
    advance(l);
    if (at_end(l))
        return kind_DEF;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_DEF;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_g(int start, R_Lexer l) {
     int i;
     int c;
state_g:
    c = peek_character(l);
    if (c == 'l') goto state_gl;
    goto state_id;
state_gl:
    advance(l);
    c = peek_character(l);
    if (c == 'o') goto state_glo;
    goto state_id;
state_glo:
    advance(l);
    c = peek_character(l);
    if (c == 'b') goto state_glob;
    goto state_id;
state_glob:
    advance(l);
    c = peek_character(l);
    if (c == 'a') goto state_globa;
    goto state_id;
state_globa:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_global;
    goto state_id;
state_global:
    advance(l);
    if (at_end(l))
        return kind_GLOBAL;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_GLOBAL;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_f(int start, R_Lexer l) {
     int i;
     int c;
state_f:
    c = peek_character(l);
    if (c == 'i') goto state_fi;
    if (c == 'r') goto state_fr;
    if (c == 'o') goto state_fo;
    goto state_id;
state_fi:
    advance(l);
    c = peek_character(l);
    if (c == 'n') goto state_fin;
    goto state_id;
state_fin:
    advance(l);
    c = peek_character(l);
    if (c == 'a') goto state_fina;
    goto state_id;
state_fina:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_final;
    goto state_id;
state_final:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_finall;
    goto state_id;
state_finall:
    advance(l);
    c = peek_character(l);
    if (c == 'y') goto state_finally;
    goto state_id;
state_finally:
    advance(l);
    if (at_end(l))
        return kind_FINALLY;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_FINALLY;
state_fr:
    advance(l);
    c = peek_character(l);
    if (c == 'o') goto state_fro;
    goto state_id;
state_fro:
    advance(l);
    c = peek_character(l);
    if (c == 'm') goto state_from;
    goto state_id;
state_from:
    advance(l);
    if (at_end(l))
        return kind_FROM;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_FROM;
state_fo:
    advance(l);
    c = peek_character(l);
    if (c == 'r') goto state_for;
    goto state_id;
state_for:
    advance(l);
    if (at_end(l))
        return kind_FOR;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_FOR;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_i(int start, R_Lexer l) {
     int i;
     int c;
state_i:
    c = peek_character(l);
    if (c == 's') goto state_is;
    if (c == 'f') goto state_if;
    if (c == 'm') goto state_im;
    if (c == 'n') goto state_in;
    goto state_id;
state_is:
    advance(l);
    if (at_end(l))
        return kind_IS;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_IS;
state_if:
    advance(l);
    if (at_end(l))
        return kind_IF;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_IF;
state_im:
    advance(l);
    c = peek_character(l);
    if (c == 'p') goto state_imp;
    goto state_id;
state_imp:
    advance(l);
    c = peek_character(l);
    if (c == 'o') goto state_impo;
    goto state_id;
state_impo:
    advance(l);
    c = peek_character(l);
    if (c == 'r') goto state_impor;
    goto state_id;
state_impor:
    advance(l);
    c = peek_character(l);
    if (c == 't') goto state_import;
    goto state_id;
state_import:
    advance(l);
    if (at_end(l))
        return kind_IMPORT;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_IMPORT;
state_in:
    advance(l);
    if (at_end(l))
        return kind_IN;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_IN;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_F(int start, R_Lexer l) {
     int i;
     int c;
state_F:
    c = peek_character(l);
    if (c == 'a') goto state_Fa;
    goto state_id;
state_Fa:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_Fal;
    goto state_id;
state_Fal:
    advance(l);
    c = peek_character(l);
    if (c == 's') goto state_Fals;
    goto state_id;
state_Fals:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_False;
    goto state_id;
state_False:
    advance(l);
    if (at_end(l))
        return kind_FALSE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_FALSE;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_l(int start, R_Lexer l) {
     int i;
     int c;
state_l:
    c = peek_character(l);
    if (c == 'a') goto state_la;
    goto state_id;
state_la:
    advance(l);
    c = peek_character(l);
    if (c == 'm') goto state_lam;
    goto state_id;
state_lam:
    advance(l);
    c = peek_character(l);
    if (c == 'b') goto state_lamb;
    goto state_id;
state_lamb:
    advance(l);
    c = peek_character(l);
    if (c == 'd') goto state_lambd;
    goto state_id;
state_lambd:
    advance(l);
    c = peek_character(l);
    if (c == 'a') goto state_lambda;
    goto state_id;
state_lambda:
    advance(l);
    if (at_end(l))
        return kind_LAMBDA;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_LAMBDA;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_o(int start, R_Lexer l) {
     int i;
     int c;
state_o:
    c = peek_character(l);
    if (c == 'r') goto state_or;
    goto state_id;
state_or:
    advance(l);
    if (at_end(l))
        return kind_OR;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_OR;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_N(int start, R_Lexer l) {
     int i;
     int c;
state_N:
    c = peek_character(l);
    if (c == 'o') goto state_No;
    goto state_id;
state_No:
    advance(l);
    c = peek_character(l);
    if (c == 'n') goto state_Non;
    goto state_id;
state_Non:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_None;
    goto state_id;
state_None:
    advance(l);
    if (at_end(l))
        return kind_NONE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_NONE;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_p(int start, R_Lexer l) {
     int i;
     int c;
state_p:
    c = peek_character(l);
    if (c == 'a') goto state_pa;
    goto state_id;
state_pa:
    advance(l);
    c = peek_character(l);
    if (c == 's') goto state_pas;
    goto state_id;
state_pas:
    advance(l);
    c = peek_character(l);
    if (c == 's') goto state_pass;
    goto state_id;
state_pass:
    advance(l);
    if (at_end(l))
        return kind_PASS;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_PASS;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_r(int start, R_Lexer l) {
     int i;
     int c;
  c = peek_character(l);   
     if (c == '\'') {
         return raw_string('\'', l);
     } else if (c == '\"') {
         return raw_string('\"', l);
     }
state_r:
    c = peek_character(l);
    if (c == 'a') goto state_ra;
    if (c == 'e') goto state_re;
    goto state_id;
state_ra:
    advance(l);
    c = peek_character(l);
    if (c == 'i') goto state_rai;
    goto state_id;
state_rai:
    advance(l);
    c = peek_character(l);
    if (c == 's') goto state_rais;
    goto state_id;
state_rais:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_raise;
    goto state_id;
state_raise:
    advance(l);
    if (at_end(l))
        return kind_RAISE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_RAISE;
state_re:
    advance(l);
    c = peek_character(l);
    if (c == 't') goto state_ret;
    goto state_id;
state_ret:
    advance(l);
    c = peek_character(l);
    if (c == 'u') goto state_retu;
    goto state_id;
state_retu:
    advance(l);
    c = peek_character(l);
    if (c == 'r') goto state_retur;
    goto state_id;
state_retur:
    advance(l);
    c = peek_character(l);
    if (c == 'n') goto state_return;
    goto state_id;
state_return:
    advance(l);
    if (at_end(l))
        return kind_RETURN;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_RETURN;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_T(int start, R_Lexer l) {
     int i;
     int c;
state_T:
    c = peek_character(l);
    if (c == 'r') goto state_Tr;
    goto state_id;
state_Tr:
    advance(l);
    c = peek_character(l);
    if (c == 'u') goto state_Tru;
    goto state_id;
state_Tru:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_True;
    goto state_id;
state_True:
    advance(l);
    if (at_end(l))
        return kind_TRUE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_TRUE;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_w(int start, R_Lexer l) {
     int i;
     int c;
state_w:
    c = peek_character(l);
    if (c == 'i') goto state_wi;
    if (c == 'h') goto state_wh;
    goto state_id;
state_wi:
    advance(l);
    c = peek_character(l);
    if (c == 't') goto state_wit;
    goto state_id;
state_wit:
    advance(l);
    c = peek_character(l);
    if (c == 'h') goto state_with;
    goto state_id;
state_with:
    advance(l);
    if (at_end(l))
        return kind_WITH;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_WITH;
state_wh:
    advance(l);
    c = peek_character(l);
    if (c == 'i') goto state_whi;
    goto state_id;
state_whi:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_whil;
    goto state_id;
state_whil:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_while;
    goto state_id;
state_while:
    advance(l);
    if (at_end(l))
        return kind_WHILE;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_WHILE;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_y(int start, R_Lexer l) {
     int i;
     int c;
state_y:
    c = peek_character(l);
    if (c == 'i') goto state_yi;
    goto state_id;
state_yi:
    advance(l);
    c = peek_character(l);
    if (c == 'e') goto state_yie;
    goto state_id;
state_yie:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_yiel;
    goto state_id;
state_yiel:
    advance(l);
    c = peek_character(l);
    if (c == 'd') goto state_yield;
    goto state_id;
state_yield:
    advance(l);
    if (at_end(l))
        return kind_YIELD;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_YIELD;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_x(int start, R_Lexer l) {
     int i;
     int c;
state_x:
    c = peek_character(l);
    if (c == 'o') goto state_xo;
    goto state_id;
state_xo:
    advance(l);
    c = peek_character(l);
    if (c == 'r') goto state_xor;
    goto state_id;
state_xor:
    advance(l);
    if (at_end(l))
        return kind_XOR;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_XOR;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_n(int start, R_Lexer l) {
     int i;
     int c;
state_n:
    c = peek_character(l);
    if (c == 'o') goto state_no;
    goto state_id;
state_no:
    advance(l);
    c = peek_character(l);
    if (c == 't') goto state_not;
    if (c == 'n') goto state_non;
    goto state_id;
state_not:
    advance(l);
    if (at_end(l))
        return kind_NOT;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_NOT;
state_non:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_nonl;
    goto state_id;
state_nonl:
    advance(l);
    c = peek_character(l);
    if (c == 'o') goto state_nonlo;
    goto state_id;
state_nonlo:
    advance(l);
    c = peek_character(l);
    if (c == 'c') goto state_nonloc;
    goto state_id;
state_nonloc:
    advance(l);
    c = peek_character(l);
    if (c == 'a') goto state_nonloca;
    goto state_id;
state_nonloca:
    advance(l);
    c = peek_character(l);
    if (c == 'l') goto state_nonlocal;
    goto state_id;
state_nonlocal:
    advance(l);
    if (at_end(l))
        return kind_NONLOCAL;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_NONLOCAL;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

static int keyword_t(int start, R_Lexer l) {
     int i;
     int c;
state_t:
    c = peek_character(l);
    if (c == 'r') goto state_tr;
    goto state_id;
state_tr:
    advance(l);
    c = peek_character(l);
    if (c == 'y') goto state_try;
    goto state_id;
state_try:
    advance(l);
    if (at_end(l))
        return kind_TRY;
    c = peek_character(l);
    if (IS_ID(c))
        goto state_id;
    else
        return kind_TRY;
    state_id:
    while (IS_ID(c)) {
        advance(l);
        c = peek_character(l);
    }
    return kind_NAME;
}

R_object init_tokens(void) {
    token_table[':'] = kind_COLON;
    token_table[','] = kind_COMMA;
    token_table['@'] = kind_AT;
    token_table[';'] = kind_SEMI;
    token_table['~'] = kind_TILDE;
    token_table[')'] = kind_RPAREN;
    bracket_table[')'] = -1;
    token_table['}'] = kind_RCURLY;
    bracket_table['}'] = -1;
    token_table[']'] = kind_RBRACK;
    bracket_table[']'] = -1;
    token_table['['] = kind_LBRACK;
    bracket_table['['] = 1;
    token_table['{'] = kind_LCURLY;
    bracket_table['{'] = 1;
    token_table['('] = kind_LPAREN;
    bracket_table['('] = 1;
    specials['*'] = operator;
    operator_assign['*'] = kind_STAREQUAL;
    specials['*'] = operator;
    operator1['*'] = kind_STAR;
    specials['/'] = operator;
    operator1['/'] = kind_SLASH;
    specials['!'] = operator;
    operator_assign['!'] = kind_NOTEQUAL;
    specials['&'] = operator;
    operator_assign['&'] = kind_AMPEREQUAL;
    specials['='] = operator;
    operator_assign['='] = kind_EQUAL;
    specials['^'] = operator;
    operator1['^'] = kind_CIRCUMFLEX;
    specials['|'] = operator;
    operator1['|'] = kind_VBAR;
    specials['*'] = operator;
    operator2['*'] = kind_DOUBLESTAR;
    specials['-'] = operator;
    operator1['-'] = kind_MINUS;
    specials['>'] = operator;
    operator_assign['>'] = kind_GREATEREQUAL;
    specials['<'] = operator;
    operator2_assign['<'] = kind_LEFTSHIFTEQUAL;
    specials['^'] = operator;
    operator_assign['^'] = kind_CIRCUMFLEXEQUAL;
    specials['<'] = operator;
    operator1['<'] = kind_LESS;
    specials['/'] = operator;
    operator2_assign['/'] = kind_DOUBLESLASHEQUAL;
    specials['+'] = operator;
    operator1['+'] = kind_PLUS;
    specials['%'] = operator;
    operator_assign['%'] = kind_PERCENTEQUAL;
    specials['='] = operator;
    operator1['='] = kind_ASSIGN;
    specials['/'] = operator;
    operator_assign['/'] = kind_SLASHEQUAL;
    specials['>'] = operator;
    operator2['>'] = kind_RIGHTSHIFT;
    specials['>'] = operator;
    operator2_assign['>'] = kind_RIGHTSHIFTEQUAL;
    specials['%'] = operator;
    operator1['%'] = kind_PERCENT;
    specials['/'] = operator;
    operator2['/'] = kind_DOUBLESLASH;
    specials['*'] = operator;
    operator2_assign['*'] = kind_DOUBLESTAREQUAL;
    specials['+'] = operator;
    operator_assign['+'] = kind_PLUSEQUAL;
    specials['&'] = operator;
    operator1['&'] = kind_AMPER;
    specials['<'] = operator;
    operator_assign['<'] = kind_LESSEQUAL;
    specials['>'] = operator;
    operator1['>'] = kind_GREATER;
    specials['-'] = operator;
    operator_assign['-'] = kind_MINUSEQUAL;
    specials['<'] = operator;
    operator2['<'] = kind_LEFTSHIFT;
    specials['|'] = operator;
    operator_assign['|'] = kind_VBAREQUAL;
    specials['_'] = identifier;
    specials['a'] = keyword_a;
    specials['b'] = keyword_b;
    specials['c'] = keyword_c;
    specials['d'] = keyword_d;
    specials['e'] = keyword_e;
    specials['f'] = keyword_f;
    specials['g'] = keyword_g;
    specials['h'] = identifier;
    specials['i'] = keyword_i;
    specials['j'] = identifier;
    specials['k'] = identifier;
    specials['l'] = keyword_l;
    specials['m'] = identifier;
    specials['n'] = keyword_n;
    specials['o'] = keyword_o;
    specials['p'] = keyword_p;
    specials['q'] = identifier;
    specials['r'] = keyword_r;
    specials['s'] = identifier;
    specials['t'] = keyword_t;
    specials['u'] = identifier;
    specials['v'] = identifier;
    specials['w'] = keyword_w;
    specials['x'] = keyword_x;
    specials['y'] = keyword_y;
    specials['z'] = identifier;
    specials['A'] = identifier;
    specials['B'] = identifier;
    specials['C'] = identifier;
    specials['D'] = identifier;
    specials['E'] = identifier;
    specials['F'] = keyword_F;
    specials['G'] = identifier;
    specials['H'] = identifier;
    specials['I'] = identifier;
    specials['J'] = identifier;
    specials['K'] = identifier;
    specials['L'] = identifier;
    specials['M'] = identifier;
    specials['N'] = keyword_N;
    specials['O'] = identifier;
    specials['P'] = identifier;
    specials['Q'] = identifier;
    specials['R'] = identifier;
    specials['S'] = identifier;
    specials['T'] = keyword_T;
    specials['U'] = identifier;
    specials['V'] = identifier;
    specials['W'] = identifier;
    specials['X'] = identifier;
    specials['Y'] = identifier;
    specials['Z'] = identifier;
    specials['0'] = number;
    specials['1'] = number;
    specials['2'] = number;
    specials['3'] = number;
    specials['4'] = number;
    specials['5'] = number;
    specials['6'] = number;
    specials['7'] = number;
    specials['8'] = number;
    specials['9'] = number;
    specials['#'] = comment;
    specials['.'] = dot;
    specials['\n'] = new_line;
    specials['\\'] = line_continuation;
    specials['\''] = string;
    specials['\"'] = string;
    return 0;
}
int8_t ascii_identifier[] = { 0, /* -1 == EOF */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1, /* '0' */
    1, /* '1' */
    1, /* '2' */
    1, /* '3' */
    1, /* '4' */
    1, /* '5' */
    1, /* '6' */
    1, /* '7' */
    1, /* '8' */
    1, /* '9' */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1, /* 'A' */
    1, /* 'B' */
    1, /* 'C' */
    1, /* 'D' */
    1, /* 'E' */
    1, /* 'F' */
    1, /* 'G' */
    1, /* 'H' */
    1, /* 'I' */
    1, /* 'J' */
    1, /* 'K' */
    1, /* 'L' */
    1, /* 'M' */
    1, /* 'N' */
    1, /* 'O' */
    1, /* 'P' */
    1, /* 'Q' */
    1, /* 'R' */
    1, /* 'S' */
    1, /* 'T' */
    1, /* 'U' */
    1, /* 'V' */
    1, /* 'W' */
    1, /* 'X' */
    1, /* 'Y' */
    1, /* 'Z' */
    0,
    0,
    0,
    0,
    1, /* '_' */
    0,
    1, /* 'a' */
    1, /* 'b' */
    1, /* 'c' */
    1, /* 'd' */
    1, /* 'e' */
    1, /* 'f' */
    1, /* 'g' */
    1, /* 'h' */
    1, /* 'i' */
    1, /* 'j' */
    1, /* 'k' */
    1, /* 'l' */
    1, /* 'm' */
    1, /* 'n' */
    1, /* 'o' */
    1, /* 'p' */
    1, /* 'q' */
    1, /* 'r' */
    1, /* 's' */
    1, /* 't' */
    1, /* 'u' */
    1, /* 'v' */
    1, /* 'w' */
    1, /* 'x' */
    1, /* 'y' */
    1, /* 'z' */
    0,
    0,
    0,
    0,
    0,
};
int escape_chars[] = {
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    '\"',
    -1,
    -1,
    -1,
    -1,
    '\'',
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    '\0',
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    '\\',
    -1,
    -1,
    -1,
    -1,
    '\a',
    '\b',
    -1,
    -1,
    -1,
    '\f',
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    '\n',
    -1,
    -1,
    -1,
    '\r',
    -1,
    '\t',
    -1,
    -1,
    -1,
    -1,
    -1,
};
