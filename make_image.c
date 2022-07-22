#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "hotpy.h"
#include <stdarg.h>
#include <string.h>
#include "dictionary.h"
#include "core_objects.h"
#include "attributes.h"
#include <ctype.h>
#include "intobject.h"
#include "opcodes.h"
#include "operators.h"
#include "py_api/object.h"
#include "standard_names.h"
#include "optimise.h"
#include "symtable.h"

GVMT_Object hotpy_main(int argc, char** argv) {
    int i, len, first;
    FILE *c;
    char buf[256];
    char upper[256];
    char* arg;
    GVMT_Object ex, ex2, mod_sys, mod_dict;
    R_frame f;
    GSC_Stream m;
    R_str filename;
    R_function func;
    R_module engine;
    R_bytes code;
    // Silence annoying warnings.
    gvmt_warn_on_unexpected_parameter_usage = 0;
    // Don't want compiler
    optimiser_controls.compile = 0;
    filename = init_vm(argc, argv);
    if (filename->length > 250)
        fatal("File name too long\n");
    len = py_print(buf, filename);
    c = fopen(buf, "w");
    for (i = 0; i < len; i++)
        upper[i] = toupper(buf[i]);
    upper[len-2] = '_';
    upper[len] = '\0';
    GVMT_TRY(ex)
        save_global(builtins, name___builtins__, (R_object)builtins);
        run_script(load_source("builtin/builtins.py"), builtins, THREAD_STATE->top_frame);

        m = make_gsc_stream(c);
        assert(m);
        fprintf(c, ".heap\n");
        gvmt_marshal((GVMT_Object)canonical_strings, m);
        gvmt_marshal((GVMT_Object)standard_names, m);
        gvmt_marshal((GVMT_Object)operator_names, m);
        gvmt_marshal((GVMT_Object)type_bound_method, m);
        gvmt_marshal((GVMT_Object)type_complex, m);
        gvmt_marshal((GVMT_Object)type_frame, m);
        gvmt_marshal((GVMT_Object)type_listiterator, m);
        gvmt_marshal((GVMT_Object)type_tupleiterator, m);
        gvmt_marshal((GVMT_Object)type_guard, m);
        gvmt_marshal((GVMT_Object)type_bytes, m);
        gvmt_marshal((GVMT_Object)type_bytelist, m);
        gvmt_marshal((GVMT_Object)type_set, m);
        gvmt_marshal((GVMT_Object)type_char_array, m);
        gvmt_marshal((GVMT_Object)type_dict_proxy_iterator, m);
        gvmt_marshal((GVMT_Object)type_dict_keys, m);
        gvmt_marshal((GVMT_Object)type_dict_values, m);
        gvmt_marshal((GVMT_Object)type_range, m);
        gvmt_marshal((GVMT_Object)type_range_iterator, m);
        gvmt_marshal((GVMT_Object)type_operator, m);
        gvmt_marshal((GVMT_Object)type_traceback, m);
        gvmt_marshal((GVMT_Object)type_partial_operator, m);
        gvmt_marshal((GVMT_Object)type_binary_operator, m);
        gvmt_marshal((GVMT_Object)type_unary_operator, m);
        gvmt_marshal((GVMT_Object)type_dict_values_iterator, m);
        gvmt_marshal((GVMT_Object)type_Thread, m);
        gvmt_marshal((GVMT_Object)type_Lock, m);
        gvmt_marshal((GVMT_Object)type_ellipsis, m);
        gvmt_marshal((GVMT_Object)type_Condition, m);
        gvmt_marshal((GVMT_Object)type_str_buf, m);
        gvmt_marshal((GVMT_Object)type_trace_record, m);
        gvmt_marshal((GVMT_Object)type_Lexer, m);
        gvmt_marshal((GVMT_Object)type_Token, m);
        gvmt_marshal((GVMT_Object)type_SyntaxError, m);
        gvmt_marshal((GVMT_Object)type_exception_handler, m);
        gvmt_marshal((GVMT_Object)type_thread_state, m);
        gvmt_marshal((GVMT_Object)type_info_item0, m);
        gvmt_marshal((GVMT_Object)type_info_item1, m);
        gvmt_marshal((GVMT_Object)type_info_item2, m);
        gvmt_marshal((GVMT_Object)type_info_item3, m);
        gvmt_marshal((GVMT_Object)type_binary_api, m);
        gvmt_marshal((GVMT_Object)type_binary_pred_api, m);
        gvmt_marshal((GVMT_Object)type_binary_r_api, m);
        gvmt_marshal((GVMT_Object)type_objint_api, m);
        gvmt_marshal((GVMT_Object)type_unary_api, m);
        gvmt_marshal((GVMT_Object)type_unary_pred_api, m);
        gvmt_marshal((GVMT_Object)type_getset_wrapper, m);
        gvmt_marshal((GVMT_Object)type_objintobj_api, m);
        gvmt_marshal((GVMT_Object)type_len_api, m);
        gvmt_marshal((GVMT_Object)type_new_api, m);
        gvmt_marshal((GVMT_Object)type_compare_api, m);
        gvmt_marshal((GVMT_Object)type_hash_api, m);
        gvmt_marshal((GVMT_Object)type_method_api, m);
        gvmt_marshal((GVMT_Object)type_class_method, m);
        gvmt_marshal((GVMT_Object)type_static_method, m);
        gvmt_marshal((GVMT_Object)type_setitem_api, m);
        gvmt_marshal((GVMT_Object)type_type_info, m);
        gvmt_marshal((GVMT_Object)type_frozen_type_info, m);
        gvmt_marshal((GVMT_Object)type_type_state, m);
        gvmt_marshal((GVMT_Object)type_specialiser, m);
        gvmt_marshal((GVMT_Object)empty_tuple, m);
        gvmt_marshal((GVMT_Object)empty_dict_keys, m);
        gvmt_marshal((GVMT_Object)empty_dict_values, m);
        gvmt_marshal((GVMT_Object)empty_module_dict_values, m);
        gvmt_marshal((GVMT_Object)type_deferred, m);
        gvmt_marshal((GVMT_Object)type_deferred_frame, m);
        gvmt_marshal((GVMT_Object)type_deferred_state, m);
        gvmt_marshal((GVMT_Object)type_trace, m);
        gvmt_marshal((GVMT_Object)type_exec_link, m);
        gvmt_marshal((GVMT_Object)type_mono_exec_link, m);
        gvmt_marshal((GVMT_Object)type_poly_exec_link, m);
        gvmt_marshal((GVMT_Object)type_str_iterator, m);
        gvmt_marshal((GVMT_Object)FALSE, m);
        gvmt_marshal((GVMT_Object)TRUE, m);
        gvmt_marshal((GVMT_Object)STOP_ITERATION_OBJECT, m);
        gvmt_marshal((GVMT_Object)type_AST_Visitor, m);
        gvmt_marshal((GVMT_Object)type_AST_mod, m);
        gvmt_marshal((GVMT_Object)type_AST_Module, m);
        gvmt_marshal((GVMT_Object)type_AST_Interactive, m);
        gvmt_marshal((GVMT_Object)type_AST_Expression, m);
        gvmt_marshal((GVMT_Object)type_AST_Suite, m);
        gvmt_marshal((GVMT_Object)type_AST_stmt, m);
        gvmt_marshal((GVMT_Object)type_AST_FunctionDef, m);
        gvmt_marshal((GVMT_Object)type_AST_ClassDef, m);
        gvmt_marshal((GVMT_Object)type_AST_Return, m);
        gvmt_marshal((GVMT_Object)type_AST_Delete, m);
        gvmt_marshal((GVMT_Object)type_AST_Assign, m);
        gvmt_marshal((GVMT_Object)type_AST_AugAssign, m);
        gvmt_marshal((GVMT_Object)type_AST_For, m);
        gvmt_marshal((GVMT_Object)type_AST_While, m);
        gvmt_marshal((GVMT_Object)type_AST_If, m);
        gvmt_marshal((GVMT_Object)type_AST_With, m);
        gvmt_marshal((GVMT_Object)type_AST_Raise, m);
        gvmt_marshal((GVMT_Object)type_AST_TryExcept, m);
        gvmt_marshal((GVMT_Object)type_AST_TryFinally, m);
        gvmt_marshal((GVMT_Object)type_AST_Assert, m);
        gvmt_marshal((GVMT_Object)type_AST_Import, m);
        gvmt_marshal((GVMT_Object)type_AST_ImportFrom, m);
        gvmt_marshal((GVMT_Object)type_AST_Global, m);
        gvmt_marshal((GVMT_Object)type_AST_Nonlocal, m);
        gvmt_marshal((GVMT_Object)type_AST_Expr, m);
        gvmt_marshal((GVMT_Object)type_AST_Pass, m);
        gvmt_marshal((GVMT_Object)type_AST_Break, m);
        gvmt_marshal((GVMT_Object)type_AST_Continue, m);
        gvmt_marshal((GVMT_Object)type_AST_expr, m);
        gvmt_marshal((GVMT_Object)type_AST_BoolOp, m);
        gvmt_marshal((GVMT_Object)type_AST_BinOp, m);
        gvmt_marshal((GVMT_Object)type_AST_UnaryOp, m);
        gvmt_marshal((GVMT_Object)type_AST_Lambda, m);
        gvmt_marshal((GVMT_Object)type_AST_IfExp, m);
        gvmt_marshal((GVMT_Object)type_AST_Dict, m);
        gvmt_marshal((GVMT_Object)type_AST_Set, m);
        gvmt_marshal((GVMT_Object)type_AST_ListComp, m);
        gvmt_marshal((GVMT_Object)type_AST_SetComp, m);
        gvmt_marshal((GVMT_Object)type_AST_DictComp, m);
        gvmt_marshal((GVMT_Object)type_AST_GeneratorExp, m);
        gvmt_marshal((GVMT_Object)type_AST_Yield, m);
        gvmt_marshal((GVMT_Object)type_AST_Compare, m);
        gvmt_marshal((GVMT_Object)type_AST_Call, m);
        gvmt_marshal((GVMT_Object)type_AST_Num, m);
        gvmt_marshal((GVMT_Object)type_AST_Str, m);
        gvmt_marshal((GVMT_Object)type_AST_Bytes, m);
        gvmt_marshal((GVMT_Object)type_AST_Ellipsis, m);
        gvmt_marshal((GVMT_Object)type_AST_Attribute, m);
        gvmt_marshal((GVMT_Object)type_AST_Subscript, m);
        gvmt_marshal((GVMT_Object)type_AST_Starred, m);
        gvmt_marshal((GVMT_Object)type_AST_Name, m);
        gvmt_marshal((GVMT_Object)type_AST_List, m);
        gvmt_marshal((GVMT_Object)type_AST_Tuple, m);
        gvmt_marshal((GVMT_Object)type_AST_expr_context, m);
        gvmt_marshal((GVMT_Object)type_AST_Load, m);
        gvmt_marshal((GVMT_Object)type_AST_Store, m);
        gvmt_marshal((GVMT_Object)type_AST_Del, m);
        gvmt_marshal((GVMT_Object)type_AST_AugLoad, m);
        gvmt_marshal((GVMT_Object)type_AST_AugStore, m);
        gvmt_marshal((GVMT_Object)type_AST_Param, m);
        gvmt_marshal((GVMT_Object)type_AST_slice, m);
        gvmt_marshal((GVMT_Object)type_AST_Slice, m);
        gvmt_marshal((GVMT_Object)type_AST_ExtSlice, m);
        gvmt_marshal((GVMT_Object)type_AST_Index, m);
        gvmt_marshal((GVMT_Object)type_AST_boolop, m);
        gvmt_marshal((GVMT_Object)type_AST_And, m);
        gvmt_marshal((GVMT_Object)type_AST_Or, m);
        gvmt_marshal((GVMT_Object)type_AST_operator, m);
        gvmt_marshal((GVMT_Object)type_AST_Add, m);
        gvmt_marshal((GVMT_Object)type_AST_Sub, m);
        gvmt_marshal((GVMT_Object)type_AST_Mult, m);
        gvmt_marshal((GVMT_Object)type_AST_Div, m);
        gvmt_marshal((GVMT_Object)type_AST_Mod, m);
        gvmt_marshal((GVMT_Object)type_AST_Pow, m);
        gvmt_marshal((GVMT_Object)type_AST_LShift, m);
        gvmt_marshal((GVMT_Object)type_AST_RShift, m);
        gvmt_marshal((GVMT_Object)type_AST_BitOr, m);
        gvmt_marshal((GVMT_Object)type_AST_BitXor, m);
        gvmt_marshal((GVMT_Object)type_AST_BitAnd, m);
        gvmt_marshal((GVMT_Object)type_AST_FloorDiv, m);
        gvmt_marshal((GVMT_Object)type_AST_unaryop, m);
        gvmt_marshal((GVMT_Object)type_AST_Invert, m);
        gvmt_marshal((GVMT_Object)type_AST_Not, m);
        gvmt_marshal((GVMT_Object)type_AST_UAdd, m);
        gvmt_marshal((GVMT_Object)type_AST_USub, m);
        gvmt_marshal((GVMT_Object)type_AST_cmpop, m);
        gvmt_marshal((GVMT_Object)type_AST_Eq, m);
        gvmt_marshal((GVMT_Object)type_AST_NotEq, m);
        gvmt_marshal((GVMT_Object)type_AST_Lt, m);
        gvmt_marshal((GVMT_Object)type_AST_LtE, m);
        gvmt_marshal((GVMT_Object)type_AST_Gt, m);
        gvmt_marshal((GVMT_Object)type_AST_GtE, m);
        gvmt_marshal((GVMT_Object)type_AST_Is, m);
        gvmt_marshal((GVMT_Object)type_AST_IsNot, m);
        gvmt_marshal((GVMT_Object)type_AST_In, m);
        gvmt_marshal((GVMT_Object)type_AST_NotIn, m);
        gvmt_marshal((GVMT_Object)type_AST_comprehension, m);
        gvmt_marshal((GVMT_Object)type_AST_excepthandler, m);
        gvmt_marshal((GVMT_Object)type_AST_ExceptHandler, m);
        gvmt_marshal((GVMT_Object)type_AST_arguments, m);
        gvmt_marshal((GVMT_Object)type_AST_arg, m);
        gvmt_marshal((GVMT_Object)type_AST_keyword, m);
        gvmt_marshal((GVMT_Object)type_AST_alias, m);
        gvmt_marshal((GVMT_Object)type_symtable, m);
        gvmt_marshal((GVMT_Object)type_symtable_scope, m);
        gvmt_marshal((GVMT_Object)type_code_builder, m);
        for (i = 0; i < OPERATOR_COUNT; i++) {
             gvmt_marshal((GVMT_Object)operators[i], m);
        }
        fprintf(c, ".roots\n");
        /* booleans */
        fprintf(c, ".public booleans\n");
        fprintf(c, ".label booleans\n");
        fprintf(c, "address _false\n");
        fprintf(c, "address _true\n");
        fprintf(c, ".public canonical_strings\n");
        fprintf(c, ".label canonical_strings\n");
        fprintf(c, "address obj_canonical_strings\n");
        fprintf(c, ".opaque\n");
        fprintf(c, ".public global_class_count\n");
        fprintf(c, ".label global_class_count\n");
        fprintf(c, "uint32 %d\n", global_class_count);
        fclose(c);
    GVMT_CATCH
        GVMT_TRY(ex2)
            print_exception(stderr, (R_BaseException)ex);
        GVMT_CATCH
            fatal("Exception in printing exception");
        GVMT_END_TRY
        exit(2);
    GVMT_END_TRY
    return ex;
}


