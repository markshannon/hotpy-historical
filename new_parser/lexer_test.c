#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <alloca.h>
#include "tokens.h"
#include "core_objects.h"
#include "hotpy.h"
#include "c_lexer.h"

extern GVMT_Object init_tokens(void);

extern void print_short(R_object object);

GVMT_Object hotpy_main(int argc, char **argv) {
    GVMT_Object ex, print_ex;
    R_object file, reader;
    R_Lexer lexer;
    int kind;
    R_str filename, text;
    gvmt_warn_on_unexpected_parameter_usage = 0;
    filename = init_vm(argc, argv);
    THREAD_STATE->current_frame = stop_frame();
    GVMT_TRY(ex)
        gvmt_malloc_init(1 << 24, 0);
        init_tokens();
        file = (R_object)PY_open(R_int_from_ssize_t(0), py_sprintf("r"), filename);
        reader = _HotPyObject_GetAttr(file, string_from_c_string("_read_as_utf8"));
        TYPE_ASSERT(reader, bound_method);
        lexer = new_lexer(reader, filename);
        do {
            kind = lexer_token_kind(lexer);
            text = lexer_token_text(lexer);
            fprintf(stderr, "%s\t\t\'", token_kind_names[kind]);
            py_fprint(stderr, text);
            fprintf(stderr, "\' line: %d, column: %d.\n", lexer_getline(lexer), lexer_getcolumn(lexer));
            lexer_advance(lexer);
//            fprintf(stderr, "indent_level: %d. new_line: %d. open_brackets: %d. indent: %d.\n", 
//                    lexer->indent_level, lexer->new_line, lexer->open_brackets, lexer->indents[lexer->indent_level]);
        } while (kind != kind_ENDMARKER);
        ex = 0;
    GVMT_CATCH
        GVMT_TRY(print_ex)
            print_exception(stderr, (R_BaseException)ex);
        GVMT_CATCH
            fatal("Exception in printing exception\n");
        GVMT_END_TRY
    GVMT_END_TRY
    return ex;
}
