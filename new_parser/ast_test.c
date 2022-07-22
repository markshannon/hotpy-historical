#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <alloca.h>
#include "tokens.h"
#include "core_objects.h"
#include "hotpy.h"
#include "c_lexer.h"
#include "Python-ast.h"
#include "ast.h"
#include "new_parser/parser.h"

extern GVMT_Object init_tokens(void);

GVMT_Object hotpy_main(int argc, char **argv) {
    GVMT_Object ex, print_ex;
    R_object file, reader;
    R_Lexer lexer;
    int t;
    R_str filename;
    R_AST_mod ast;
    R_AST_Visitor print_visitor;
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
        ast = parse_file_input(lexer);
        print_visitor = hotpy_make_print_visitor();
        print_visitor->p0 = (void*)stdout;
        hotpy_visit_mod(ast, print_visitor);
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
