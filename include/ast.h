#include "hotpy.h"
#include "Python-ast.h"

void expecting_error(int kind, R_Lexer lexer);

#define list_of_one(o) _list_of_one((R_object)(o))

R_list _list_of_one(R_object o);

R_AST_alias alias_for_rule_import_as_name(R_str, R_str);

#define cmpop_for_rule_LESS(t) hotpy_Lt_singleton
#define cmpop_for_rule_GREATER(t) hotpy_Gt_singleton
#define cmpop_for_rule_EQUAL(t) hotpy_Eq_singleton
#define cmpop_for_rule_GREATEREQUAL(t) hotpy_GtE_singleton
#define cmpop_for_rule_LESSEQUAL(t) hotpy_LtE_singleton
#define cmpop_for_rule_NOTEQUAL(t) hotpy_NotEq_singleton
#define cmpop_for_rule_IN(t) hotpy_In_singleton
#define convert_compound_stmt_to_R_list(x) list_of_one(x)


#define expr_for_rule_NONE(i, l, c) _HotPy_AST_Name(name_None, hotpy_Load_singleton, l, c)
#define expr_for_rule_TRUE(i, l, c) _HotPy_AST_Name(name_True, hotpy_Load_singleton, l, c)
#define expr_for_rule_FALSE(i, l, c) _HotPy_AST_Name(name_False, hotpy_Load_singleton, l, c)
#define expr_for_rule_ELLIPSIS(s, l, c) _HotPy_AST_Ellipsis(l, c)

#define stmt_for_rule_CONTINUE(i, l, c) _HotPy_AST_Continue(l, c)
#define stmt_for_rule_BREAK(i, l, c) _HotPy_AST_Break(l, c)
#define stmt_for_rule_PASS(n, l, c) _HotPy_AST_Pass(l, c)
#define cmpop_for_rule_not_in(i) hotpy_NotIn_singleton


R_AST_expr expr_for_rule_list_expr(R_AST_expr, int line, int column);

#define expr_for_rule_bracketted_expr(e, l, c) (e)

#define stmt_for_rule_nonlocal_stmt(x, l, c) _HotPy_AST_Nonlocal(x, l, c)
#define stmt_for_rule_global_stmt(x, l, c) _HotPy_AST_Global(x, l, c)
R_list list_for_rule_indented_block(int, R_list, int);


R_AST_stmt stmt_for_rule_try_stmt(R_list, R_tuple, int line, int column);
R_AST_stmt stmt_for_rule_import_from(R_tuple, R_object, int line, int column);
R_AST_stmt stmt_for_rule_funcdef(R_str, R_AST_arguments, R_AST_expr, R_list, int line, int column);
R_AST_arg arg_for_rule_NAME(R_str);
#define expr_for_rule_lambdef_nocond(a, b, l, c) _HotPy_AST_Lambda(a, b, l, c)
#define stmt_for_rule_NEWLINE(n, l, c) _HotPy_AST_Pass(l, c)

R_AST_expr expr_for_rule_or_test(R_list l, int line, int column);
R_AST_expr expr_for_rule_and_test(R_list l, int line, int column);

#define expr_for_rule_and_expr(x, o, y, l, c) \
_HotPy_AST_BinOp(x, hotpy_BitAnd_singleton, y, l, c)

#define expr_for_rule_expr(x, o, y, l, c) \
_HotPy_AST_BinOp(x, hotpy_BitOr_singleton, y, l, c)

#define expr_for_rule_xor_expr(x, o, y, l, c) \
_HotPy_AST_BinOp(x, hotpy_BitXor_singleton, y, l, c)

#define unaryop_for_rule_PLUS(x) hotpy_UAdd_singleton
#define unaryop_for_rule_MINUS(x) hotpy_USub_singleton
#define unaryop_for_rule_TILDE(x) hotpy_Invert_singleton

#define expr_for_rule_not_expr(e, l, c) _HotPy_AST_UnaryOp(hotpy_Not_singleton, e, l, c)

R_AST_expr expr_for_rule_star_expr(int star, R_AST_expr expr, int line, int column);

#define expr_for_rule_exprlist expr_for_rule_testlist

R_AST_expr expr_for_rule_testlist(R_list, int, int line, int column);

R_AST_expr expr_for_rule_string(R_list, int line, int column);
R_AST_stmt stmt_for_rule_classdef(R_str, R_list, R_list, int line, int column);

#define expr_for_rule_term(x, o, y, l, c) _HotPy_AST_BinOp(x, o, y, l, c)

R_AST_stmt stmt_for_rule_assert_stmt(R_AST_expr, R_AST_expr, int line, int column);
R_AST_stmt stmt_for_rule_for_stmt(R_AST_expr, int, R_AST_expr, R_list, R_list, int line, int column);
#define expr_for_rule_lambdef(a, b, l, c) _HotPy_AST_Lambda(a, b, l, c) 
R_tuple convert_dotted_name_to_R_tuple(R_list x);

R_AST_stmt stmt_for_rule_expr_stmt(R_AST_expr, R_object, int line, int column);
R_AST_expr expr_for_rule_decorator(R_list, R_list, int line, int column);
R_AST_stmt stmt_for_rule_if_stmt(R_AST_expr, R_list, R_list, R_list, int line, int column);
R_int int_for_rule_level(R_list);
R_AST_stmt stmt_for_rule_with_stmt(R_AST_expr, R_AST_expr, R_list, int line, int column);
R_list list_for_rule_comp_for(R_AST_expr, int, R_AST_expr, R_tuple);
R_AST_expr expr_for_rule_index(R_AST_slice, int line, int column);
#define expr_for_rule_comp_if(e, l, c) (e)
R_AST_expr expr_for_rule_shift_expr(R_AST_expr, int, R_AST_expr, int line, int column);
R_AST_expr expr_for_rule_dictorsetmaker(R_AST_expr, R_object, int line, int column);

R_AST_expr expr_for_rule_power(R_AST_expr, R_tuple, int line, int column);
R_AST_expr expr_for_rule_comparison(R_AST_expr, R_list, int line, int column);
R_AST_expr expr_for_rule_arith_expr(R_AST_expr, int, R_AST_expr, int line, int column);
R_AST_expr expr_for_rule_NAME(R_str, int line, int column);
R_AST_expr expr_for_rule_FLOAT(R_str, int line, int column);
R_AST_expr expr_for_rule_INTEGER(R_str, int line, int column);

R_AST_slice slice_for_rule_slice(R_AST_expr, R_AST_slice);
R_AST_slice slice_for_rule_sliceop(R_AST_expr);
R_AST_slice slice_for_rule_rslice(R_AST_expr, R_AST_slice);
R_AST_slice slice_for_rule_subscriptlist(R_list, int);

#define arg_for_rule_targ _HotPy_AST_arg

#define expr_for_rule_unary_expr(o, e, l, c) _HotPy_AST_UnaryOp(o, e, l, c)
R_AST_expr expr_for_rule_simple(R_AST_expr, R_list, int line, int column);
#define stmt_for_rule_return_stmt(e, l, c) _HotPy_AST_Return(e, l, c)

R_AST_expr expr_for_rule_testlist_comp(R_AST_expr, R_list, int line, int column);

#define convert_void_to_R_AST_mod convert_compound_stmt_to_R_AST_mod
R_AST_mod convert_compound_stmt_to_R_AST_mod(R_AST_stmt x);

R_AST_mod convert_simple_stmt_to_R_AST_mod(R_list x);

R_AST_alias alias_for_rule_dotted_as_name(R_list, R_str);
R_AST_expr expr_for_rule_call(R_list, int line, int column);

R_AST_stmt stmt_for_rule_raise_stmt(R_tuple, int line, int column);
R_AST_expr expr_for_rule_attr(R_str, int line, int column);
R_AST_mod mod_for_rule_file_input(R_list);
R_AST_stmt convert_yield_expr_to_R_AST_stmt(R_AST_expr x);
R_AST_cmpop cmpop_for_rule_is(int);
R_object object_for_rule_argument(R_AST_expr, R_object);
R_AST_arguments arguments_for_rule_typedargslist(R_list);

R_AST_mod mod_for_rule_eval_input(R_AST_expr, R_list);
R_AST_stmt stmt_for_rule_import_name(R_list, int line, int column);
R_AST_expr expr_for_rule_yield_expr(R_AST_expr, int line, int column);
R_AST_expr expr_for_rule_trinary_expr(R_AST_expr, R_tuple, int line, int column);

R_AST_expr expr_for_rule_dictorset_expr(R_AST_expr, int line, int column);
R_AST_stmt stmt_for_rule_decorated(R_list, R_AST_stmt, int line, int column);

R_AST_stmt stmt_for_rule_del_stmt(R_list exprs, int trailing, int line, int column);
R_AST_stmt stmt_for_rule_while_stmt(R_AST_expr, R_list, R_list, int line, int column);
R_AST_arguments arguments_for_rule_varargslist(R_list);

#define operator_for_rule_PLUSEQUAL(i) hotpy_Add_singleton
#define operator_for_rule_MINUSEQUAL(i) hotpy_Sub_singleton
#define operator_for_rule_STAREQUAL(i) hotpy_Mult_singleton
#define operator_for_rule_SLASHEQUAL(i) hotpy_Div_singleton
#define operator_for_rule_PERCENTEQUAL(i) hotpy_Mod_singleton
#define operator_for_rule_AMPEREQUAL(i) hotpy_BitAnd_singleton
#define operator_for_rule_VBAREQUAL(i) hotpy_BitOr_singleton
#define operator_for_rule_CIRCUMFLEXEQUAL(i) hotpy_BitXor_singleton
#define operator_for_rule_LEFTSHIFTEQUAL(i) hotpy_LShift_singleton
#define operator_for_rule_RIGHTSHIFTEQUAL(i) hotpy_RShift_singleton
#define operator_for_rule_DOUBLESTAREQUAL(i) hotpy_Pow_singleton
#define operator_for_rule_DOUBLESLASHEQUAL(i) hotpy_FloorDiv_singleton

#define operator_for_rule_STAR(i) hotpy_Mult_singleton
#define operator_for_rule_SLASH(i) hotpy_Div_singleton
#define operator_for_rule_PERCENT(i) hotpy_Mod_singleton
#define operator_for_rule_DOUBLESLASH(i) hotpy_FloorDiv_singleton

