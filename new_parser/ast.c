#include "hotpy.h"
#include "Python-ast.h"
#include "ast.h"
#include "tokens.h"
#include "c_lexer.h"

extern char* token_kind_display[];

#define LOAD hotpy_Load_singleton
#define STORE hotpy_Store_singleton
#define DEL hotpy_Del_singleton

void illegal_state_mod(R_AST_mod o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_stmt(R_AST_stmt o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void 
illegal_state_expr_context(R_AST_expr_context o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_slice(R_AST_slice o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_boolop(R_AST_boolop o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_operator(R_AST_operator o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_unaryop(R_AST_unaryop o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_cmpop(R_AST_cmpop o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_state_excepthandler(R_AST_excepthandler o, R_AST_Visitor visitor) {
    assert(0 && "Illegal State"); 
}

void illegal_context_expr(R_AST_expr object, R_AST_Visitor visitor) {
    R_AST_expr_context ctx = (R_AST_expr_context)visitor->r0;
    R_str msg = py_sprintf("Illegal expression context"); // Improve this.
    syntax_error(msg, object->lineno, object->col_offset);
}

void store_context_attribute(R_AST_Attribute object, R_AST_Visitor visitor) {
    TYPE_ASSERT(visitor->r0, AST_expr_context);
    object->ctx = (R_AST_expr_context)visitor->r0;
}

void store_context_subscript(R_AST_Subscript object, R_AST_Visitor visitor) {
    TYPE_ASSERT(visitor->r0, AST_expr_context);
    object->ctx = (R_AST_expr_context)visitor->r0;
}

void store_context_starred(R_AST_Starred object, R_AST_Visitor visitor) {
    /* What is a starred expr? -- Partial unpack a, *t, b = x */
    assert(0 && "To do"); 
}

void store_context_name(R_AST_Name object, R_AST_Visitor visitor) {
    R_AST_expr_context ctx = (R_AST_expr_context)visitor->r0;
    TYPE_ASSERT(ctx, AST_expr_context);
    if (ctx == hotpy_Store_singleton) {
        assert(object->id == canonical_string(object->id));
        if ((object->id == name_None) ||
            (object->id == name_True) ||
            (object->id == name_False) ||
            (object->id == name___debug__)) {
            R_str msg = string_from_c_string("assignment to keyword");
            syntax_error(msg, object->lineno, object->col_offset);
        }
    }
    object->ctx = ctx;
}

void store_context_list(R_AST_List object, R_AST_Visitor visitor) {
    int i, len;
    R_list elts;
    TYPE_ASSERT(visitor->r0, AST_expr_context);
    object->ctx = (R_AST_expr_context)visitor->r0;
    elts = object->elts;
    len = LIST_SIZE(elts);
    for (i = 0; i < len; i++)
        hotpy_visit_expr((R_AST_expr)ITEM(elts->array, i), visitor);
}

void store_context_tuple(R_AST_Tuple object, R_AST_Visitor visitor) {
    if (LIST_SIZE(object->elts) == 0) {
        R_str msg = string_from_c_string("can't assign to ()");
        syntax_error(msg, object->lineno, object->col_offset);
    }
    store_context_list((R_AST_List)object, visitor);
}
extern GVMT_OBJECT(type) _AST_Visitor;
extern GVMT_OBJECT(AST_Store) hotpy_Store_singleton_object;

GVMT_OBJECT(AST_Visitor) _store_context_visitor = {
    &_AST_Visitor,
    0, /* ref_count */
    0, /* i */
    NULL, /* p */
    illegal_state_mod,
    illegal_state_mod,
    illegal_state_mod,
    illegal_state_mod,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_state_stmt,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    illegal_context_expr,
    (visit_expr_func)store_context_attribute,
    (visit_expr_func)store_context_subscript,
    (visit_expr_func)store_context_starred,
    (visit_expr_func)store_context_name,
    (visit_expr_func)store_context_list,
    (visit_expr_func)store_context_tuple,
    illegal_state_expr_context,
    illegal_state_expr_context,
    illegal_state_expr_context,
    illegal_state_expr_context,
    illegal_state_expr_context,
    illegal_state_expr_context,
    illegal_state_slice,
    illegal_state_slice,
    illegal_state_slice,
    illegal_state_boolop,
    illegal_state_boolop,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_operator,
    illegal_state_unaryop,
    illegal_state_unaryop,
    illegal_state_unaryop,
    illegal_state_unaryop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_cmpop,
    illegal_state_excepthandler,
    (R_object)&hotpy_Store_singleton_object, /* r0 */
    NULL, /* r1 */
    NULL, /* r2 */
    NULL, /* r3 */
};

R_AST_Visitor store_context_visitor = &_store_context_visitor;

void expecting_error(int kind, R_Lexer lexer) {
    R_str expecting = string_from_c_string(token_kind_display[kind]);
    R_str found = lexer_token_text(lexer);
    R_str msg = py_sprintf("expecting %s, found '%s'", expecting, found); 
    syntax_error(msg, lexer_getline(lexer), lexer_getcolumn(lexer)); 
}

void _hotpy_print_string(R_str s, R_AST_Visitor visitor) {
    FILE* out = (FILE*)visitor->p;
    fputs("\'", out);
    py_fprint(out, s);
    fputs("\'\n", out);
}

void _hotpy_print_int(int i, R_AST_Visitor visitor) {
    FILE* out = (FILE*)visitor->p;
    intptr_t indent = visitor->i;
    _hotpy_print_spaces(out, indent);
    fprintf(out, "%d\n", i);
}

void _hotpy_print_number(R_object o, R_AST_Visitor visitor) {
    R_str s;
    if (TYPE(o) == type_int) {
        s = int_to_str((R_int)o, 10);
    } else if (TYPE(o) == type_float) {
        s = float_to_str((R_float)o);
    } else {
        assert("Unexpected Type" && 0);   
        s = py_sprintf("Unexpected type: %s", TYPE_NAME(o));
    }
    _hotpy_print_string(s, visitor);
}

#define LIST_ITEM(l, i) ((l)->array->array[i])

#define MAKE_PAIR(a,b) make_tuple2((R_object)(a), (R_object)(b))

R_list _list_of_one(R_object o) {
    R_list l = make_list(1);
    LIST_ITEM(l, 0) = o;
    return l;
}

R_AST_alias alias_for_rule_import_as_name(R_str s1, R_str s2) {
    TYPE_ASSERT(s1, str);
    assert(s2 == NULL || TYPE(s2) == type_str);
    return _HotPy_AST_alias(s1, s2);
}

R_AST_stmt stmt_for_rule_try_stmt(R_list body, R_tuple others, int line, int column) {
    TYPE_ASSERT(others, tuple);
    if (LEN(others) == 2) {
        // Try - finally
        R_object o = ITEM(others, 0);
        R_list finalbody;
        TYPE_ASSERT(o, int);
        assert(int_from_py_int((R_int)o) == kind_FINALLY);
        finalbody = (R_list)ITEM(others, 1);
        TYPE_ASSERT(finalbody, list);
        return _HotPy_AST_TryFinally(body, finalbody, line, column);
    } else {
        // Try - except.
        R_list handlers, orelse, finally;
        R_AST_stmt try_except;
        assert(LEN(others) == 3);
        handlers = (R_list)ITEM(others, 0);
        orelse = (R_list)ITEM(others, 1);
        finally =  (R_list)ITEM(others, 2);
        TYPE_ASSERT(handlers, list);
        assert(orelse == NULL || TYPE(orelse) == type_list);
        try_except = _HotPy_AST_TryExcept(body, handlers, orelse, line, column);
        if (finally != NULL) {
            TYPE_ASSERT(finally, list);
            try_except = _HotPy_AST_TryFinally(list_of_one(body), finally, line, column);
        }
        return try_except;
    }
}

static R_AST_expr bool_op(R_AST_boolop op, R_list l, int line, int column) {
    assert(l != NULL);
    assert(LIST_SIZE(l) > 0);
    if (LIST_SIZE(l) == 1) {
        R_AST_expr e = (R_AST_expr)LIST_ITEM(l, 0);
        TYPE_ASSERT(e, AST_expr);
        return e;   
    } else {
        return _HotPy_AST_BoolOp(op, l, line, column);  
    }
}

R_AST_expr expr_for_rule_or_test(R_list l, int line, int column) {
    return bool_op(hotpy_Or_singleton, l, line, column);   
}

R_AST_expr expr_for_rule_and_test(R_list l, int line, int column) {
    return bool_op(hotpy_And_singleton, l, line, column);   
}

R_int int_for_rule_level(R_list l) {
    int level = 0;
    int i, size;
    R_int t;
    size = LIST_SIZE(l);
    level = size;
    for (i = 0; i < size; i++) {
        t = (R_int)LIST_ITEM(l, i);   
        TYPE_ASSERT(t, int);
        if (int_from_py_int(t) == kind_ELLIPSIS)
            level += 2;
        else
            assert(int_from_py_int(t) == kind_DOT);
    }
    return PY_SMALL_INT(level);
}

R_AST_stmt stmt_for_rule_import_from(R_tuple src, R_object names, int line, int column) {
    int level = int_from_py_int((R_int)ITEM(src, 0));
    R_str module = (R_str)ITEM(src, 1);
    TYPE_ASSERT(ITEM(src, 0), int);
    TYPE_ASSERT(module, str);
    LIST_TYPE_ASSERT(names, AST_alias);
    return _HotPy_AST_ImportFrom(module, (R_list)names, level, line, column);
}

R_AST_stmt stmt_for_rule_import_name(R_list aliases, int line, int column) {
    return _HotPy_AST_Import(aliases, line, column);
}

R_AST_stmt stmt_for_rule_funcdef(R_str name, R_AST_arguments args, R_AST_expr returns, 
                                 R_list body, int line, int column) {
    return _HotPy_AST_FunctionDef(name, args, body, NULL, returns, line, column);
}

R_AST_expr expr_for_rule_string(R_list list, int lineno, int col_offset) {
    R_str string;
    R_str_buf buf;
    int i, len;
    len = LIST_SIZE(list);
    if (len == 1) {
        string = (R_str)LIST_ITEM(list, 0);
    } else {
        buf = make_str_buf();
        for (i = 0; i < len; i++) {
            R_str s = (R_str)LIST_ITEM(list, i);
            TYPE_ASSERT(s, str);
            str_buf_append_str(buf, s);
        }
        string = str_buf_to_str(buf);
    } 
    return _HotPy_AST_Str(canonical_string(string), lineno, col_offset);
}

/* Fix this to give proper Syntax Errors, with line & column */
#define ERROR(m) raise_char(type_SyntaxError, m)

static void
process_arglist(R_list args, R_list* pos, R_list* kws, 
                R_AST_expr* star, R_AST_expr* dstar)
{
    R_object arg;
    R_type t;
    int i, len;
    int in_kws = 0;
    if (args == NULL)
        return;
    len = LIST_SIZE(args);
    *pos = make_list(4);
    for (i = 0; i < len; i++) {
        arg = LIST_ITEM(args, i);
        t = TYPE(arg);
        if (t == type_tuple) {
            R_int k = (R_int)ITEM((R_tuple)arg, 0);
            int kind = int_from_py_int(k);
            assert(kind == kind_DOUBLESTAR);
            if (i != len-1)
                ERROR("** must be last parameter"); 
            *dstar = (R_AST_expr)ITEM((R_tuple)arg, 1);
        } else {
            if (t == type_AST_keyword) {   
                if (in_kws == 0) {
                    in_kws = 1;   
                    *kws = make_list(4);
                }
                list_append(*kws, arg);
            } else {
                assert(is_subclass_of(t, type_AST_expr));
                if (isinstance(arg, type_AST_Starred)) {
                    *star = ((R_AST_Starred)arg)->value;
                    if (i == len-2) {
                        R_int k;
                        int kind;
                        arg = LIST_ITEM(args, len-1);
                        t = TYPE(arg);
                        if (t != type_tuple) {
                            ERROR("Expected ** parameter after * parameter");   
                        }
                        k = (R_int)ITEM((R_tuple)arg, 0);
                        kind = int_from_py_int(k);
                        if (kind != kind_DOUBLESTAR){
                            ERROR("Expected ** parameter after * parameter");   
                        }
                        *dstar = (R_AST_expr)ITEM((R_tuple)arg, 1);
                    } else if (i < len-2) {
                        ERROR("Too many parameters after *");
                    }
                } else {
                    if (in_kws)
                        ERROR(" non-keyword arg after keyword arg");
                    list_append(*pos, arg);
                }
            }
        }
    }
}

R_AST_stmt 
stmt_for_rule_classdef(R_str name, R_list arglist, R_list body, int line, int column) 
{
    R_list bases, keywords;
    R_AST_expr starargs, kwargs;
    process_arglist(arglist, &bases, &keywords, &starargs, &kwargs);
    return  _HotPy_AST_ClassDef(name, bases, keywords, starargs,
                                kwargs, body, NULL, line, column);
}

R_AST_stmt 
stmt_for_rule_assert_stmt(R_AST_expr test, R_AST_expr msg, int line, int column) {    
    return _HotPy_AST_Assert(test, msg, line, column);
}

R_AST_stmt 
stmt_for_rule_for_stmt(R_AST_expr target, int in, R_AST_expr iter, R_list body,
                       R_list orelse, int line, int column) {
    return _HotPy_AST_For(target, iter, body, orelse, line, column);
}

static R_AST_expr expr_from_dotted_name(R_list names, int line, int column) {
    R_AST_expr expr;
    R_str name;
    int i, size = LIST_SIZE(names);
    assert(size > 0);
    name = (R_str)LIST_ITEM(names, 0);
    expr = _HotPy_AST_Name(name, NULL, line, column);
    for (i = 1; i < size; i++) {   
        expr = _HotPy_AST_Attribute(expr, name, NULL, line, column);
    }
    return expr;
}

R_AST_stmt 
stmt_for_rule_expr_stmt(R_AST_expr value, R_object asgn, int line, int column) {
    if (asgn == NULL) {
        return _HotPy_AST_Expr(value, line, column);
    } else if (TYPE(asgn) == type_tuple) {
        // Aug asgn
        R_tuple t = (R_tuple)asgn;
        R_AST_operator op = (R_AST_operator)ITEM(t, 0);
        R_AST_expr rhs = (R_AST_expr)ITEM(t, 1);
        TYPE_ASSERT(op, AST_operator);
        TYPE_ASSERT(rhs, AST_expr);
        hotpy_visit_expr(value, store_context_visitor);
        return _HotPy_AST_AugAssign(value, op, rhs, line, column);
    } else {
        R_list l = (R_list)asgn;
        R_object o, right_most;
        int i;
        TYPE_ASSERT(asgn, list);   
        assert(LIST_SIZE(l) > 0);
        right_most = LIST_ITEM(l, LIST_SIZE(l)-1);
        // Shift all items in list to the right;
        for (i = LIST_SIZE(l)-1; i > 0; i--) {
            o = LIST_ITEM(l, i-1);
            hotpy_visit_expr((R_AST_expr)o, store_context_visitor);
            LIST_ITEM(l, i) = o;
        }
        LIST_ITEM(l, 0) = (R_object)value;
        hotpy_visit_expr(value, store_context_visitor);
        return _HotPy_AST_Assign(l, (R_AST_expr)right_most, line, column);
    }
}

R_AST_expr 
expr_for_rule_decorator(R_list name, R_list arglist, int line, int column) {
    R_list pos, keywords;
    R_AST_expr starargs, kwargs;
    R_AST_expr func;
    process_arglist(arglist, &pos, &keywords, &starargs, &kwargs);
    func = expr_from_dotted_name(name, line, column);
    return _HotPy_AST_Call(func, pos, keywords, starargs, kwargs, line, column);    
}

R_AST_stmt
stmt_for_rule_if_stmt(R_AST_expr test, R_list body, R_list elifs, 
                      R_list orelse, int line, int column) {
    R_AST_stmt _if;
    R_AST_If last;
    R_tuple _else;
    R_AST_expr else_cond;
    R_list else_body;
    int i, len;
    _if = _HotPy_AST_If(test, body, NULL, line, column);
    last = (R_AST_If)_if;
    for (i = 0; i < LIST_SIZE(elifs); i++) {
        R_AST_If pen;
        _else = (R_tuple)LIST_ITEM(elifs, i);
        else_cond = (R_AST_expr)ITEM(_else, 0);
        else_body = (R_list)ITEM(_else, 1);
        pen = last;
        last = (R_AST_If)_HotPy_AST_If(else_cond, else_body, NULL, line, column);
        pen->orelse = list_of_one(last);
    }
    last->orelse = orelse;
    return _if;
}


R_AST_stmt 
stmt_for_rule_with_stmt(R_AST_expr obj, R_AST_expr as, R_list body, int line, int col) {
    if (as) {
        hotpy_visit_expr(as, store_context_visitor);
    }
    return _HotPy_AST_With(obj, as, body, line, col);
}


static Py_UNICODE DOT = '.';

static R_str dotted_name_to_str(R_list list) {
    R_str_buf buf;
    int i, len;
    len = LIST_SIZE(list);
    assert(len > 0);
    if (len == 1) 
        return (R_str)LIST_ITEM(list, 0);
    buf = make_str_buf();
    str_buf_append_str(buf, (R_str)LIST_ITEM(list, 0));
    for (i = 1; i < len; i++) {
        R_str s = (R_str)LIST_ITEM(list, i);
        TYPE_ASSERT(s, str);
        str_buf_append_chars(buf, &DOT, 1);
        str_buf_append_str(buf, s);
    }
    return str_buf_to_str(buf);
}

R_tuple convert_dotted_name_to_R_tuple(R_list x) {
    R_str s = dotted_name_to_str(x);
    return make_tuple2((R_object)PY_SMALL_INT(0), (R_object)s);
}

R_AST_expr expr_for_rule_index(R_AST_slice index, int line, int column) {
    return _HotPy_AST_Subscript(NULL, index, LOAD, line, column);
}

R_AST_expr 
expr_for_rule_star_expr(int star, R_AST_expr expr, int line, int column) {
    if (star)
        return _HotPy_AST_Starred(expr, LOAD, line, column);
    else
        return expr;
}

    
R_AST_expr 
expr_for_rule_shift_expr(R_AST_expr x, int op_id, R_AST_expr y, int line, int column) {
    R_AST_operator op;
    if (op_id == kind_RIGHTSHIFT) {
        op = hotpy_RShift_singleton;
    } else {
        assert(op_id == kind_LEFTSHIFT);
        op = hotpy_LShift_singleton;
    }
    return  _HotPy_AST_BinOp(x, op, y, line, column);
}

R_AST_expr expr_for_rule_power(R_AST_expr e, R_tuple t, int line, int column) {
    R_AST_expr r;
    if (t == NULL)
        return e;
    TYPE_ASSERT(ITEM(t, 0), int);
    r = (R_AST_expr)ITEM(t, 1);
    TYPE_ASSERT(r, AST_expr);
    return _HotPy_AST_BinOp(e, hotpy_Pow_singleton, r, line, column);
}

R_AST_expr expr_for_rule_NAME(R_str name, int line, int column) {
    name = canonical_string(name);
    return _HotPy_AST_Name(name, LOAD, line, column);
}

R_AST_expr 
expr_for_rule_arith_expr(R_AST_expr x, int op_id, R_AST_expr y, int line, int column) {
    R_AST_operator op;
    if (op_id == kind_PLUS) {
        op = hotpy_Add_singleton;
    } else {
        assert(op_id == kind_MINUS);
        op = hotpy_Sub_singleton;
    }
    return _HotPy_AST_BinOp(x, op, y, line, column);
}


R_AST_expr expr_for_rule_FLOAT(R_str s, int line, int column) {
    R_object o = (R_object)parse_float(s, NULL);
    return _HotPy_AST_Num(o, line, column);
}

R_AST_expr expr_for_rule_INTEGER(R_str s, int line, int column) {
    R_object o = parse_int(s, 10);
    return _HotPy_AST_Num(o, line, column);
}

R_AST_expr expr_for_rule_simple(R_AST_expr e, R_list members, int line, int column) {
    int i, len;
    assert(offsetof(GVMT_OBJECT(AST_Call), func) == 
           offsetof(GVMT_OBJECT(AST_Attribute), value));
    assert(offsetof(GVMT_OBJECT(AST_Call), func) == 
           offsetof(GVMT_OBJECT(AST_Subscript), value));
    len = LIST_SIZE(members);
    for (i = 0; i < len; i++) {
        R_AST_Call member = (R_AST_Call)LIST_ITEM(members, i);   
        assert(TYPE(member) == type_AST_Call ||
               TYPE(member) == type_AST_Attribute ||
               TYPE(member) == type_AST_Subscript);
        assert(member->func == NULL);
        member->func = e;
        e = (R_AST_expr)member;
    }
    return e;
}

R_AST_expr expr_for_rule_dictorset_expr(R_AST_expr expr, int line, int column) {
    if (expr == NULL) {
        return _HotPy_AST_Dict(make_list(0), make_list(0), line, column);
    } else {
        return expr;
    }
}

R_AST_expr 
expr_for_rule_comparison(R_AST_expr left, R_list r, int line, int column) {
    int i, len;
    R_list ops, comparators;
    assert(r != NULL);
    len = LIST_SIZE(r);
    if (len == 0)
        return left;
    ops = make_list(len);
    comparators = make_list(len);
    for (i = 0; i < len; i++) {
        R_object o;
        R_tuple t = (R_tuple)LIST_ITEM(r, i);
        TYPE_ASSERT(t, tuple);
        assert(LEN(t) == 2);
        LIST_ITEM(ops, i) = ITEM(t, 0);
        o = ITEM(t, 1);
        LIST_ITEM(comparators, i) = o;
    }
    SET_LIST_SIZE(ops, len);
    SET_LIST_SIZE(comparators, len);
    return _HotPy_AST_Compare(left, ops, comparators, line, column);
}

R_AST_expr 
expr_for_rule_dictorsetmaker(R_AST_expr expr, R_object parts, int line, int column)
{
    R_AST_expr value = NULL;
    R_list left_literals = NULL;
    R_list right_literals = NULL;
    R_list comps = NULL;
    if (parts == NULL) {
        left_literals = list_of_one(expr);
    } else {
        if (TYPE(parts) == type_tuple) {
            // ':' test ( comp_for | test_pair_list )
            R_tuple t = (R_tuple)parts;
            R_list l;
            assert(LEN(t) == 2);
            l = (R_list)ITEM(t, 1);
            TYPE_ASSERT(l, list);
            if (LIST_SIZE(l) > 0 && 
                TYPE(LIST_ITEM(l, 0)) == type_AST_comprehension) {
                LIST_TYPE_ASSERT(l, AST_comprehension);
                // comp_for   
                comps = l;
                value = (R_AST_expr)ITEM(t, 0);
            } else {
                int i;
                // l  is a list of pairs.
                LIST_TYPE_ASSERT(l, tuple);
                left_literals = list_of_one(expr);
                right_literals = list_of_one(ITEM(t, 0));
                for (i = 0; i < LIST_SIZE(l); i++) {
                    R_tuple t = (R_tuple)LIST_ITEM(l, i);
                    list_append(left_literals, ITEM(t, 0));
                    list_append(right_literals, ITEM(t, 1));
                }
            }
        } else {
            //   comp_for | test_list (Set)
            R_list l = (R_list)parts;
            TYPE_ASSERT(l, list);
            if (LIST_SIZE(l) > 0 && 
                TYPE(LIST_ITEM(l, 0)) == type_AST_comprehension) {
                LIST_TYPE_ASSERT(l, AST_comprehension);
                // comp_for
                comps = l;
            } else {
                LIST_TYPE_ASSERT(l, AST_expr);
                list_insert_nolock(l, 0, (R_object)expr);
                left_literals = l;
            }
        }
    }
    if (comps == NULL) {
        if (right_literals == NULL) {
            // set literal
            return _HotPy_AST_Set(left_literals, line, column);
        } else {
            // dict literal   
            return _HotPy_AST_Dict(left_literals, right_literals, line, column);
        }
    } else {
        if (value == NULL) {
            // set comp
            return _HotPy_AST_SetComp(expr, comps, line, column);
        } else {
            // dict comp   
            return _HotPy_AST_DictComp(expr, value, comps, line, column);
        }
    }
}

R_AST_expr expr_for_rule_call(R_list arglist, int line, int column) {
    R_list pos, keywords;
    R_AST_expr starargs, kwargs;
    process_arglist(arglist, &pos, &keywords, &starargs, &kwargs);
    return _HotPy_AST_Call(NULL, pos, keywords, starargs, kwargs, line, column);
}

R_AST_alias alias_for_rule_dotted_as_name(R_list names, R_str asname) {
   R_str name = dotted_name_to_str(names);
   return _HotPy_AST_alias(name, asname);
}

R_AST_stmt 
stmt_for_rule_raise_stmt(R_tuple t, int line, int column) {
    R_AST_expr exc, cause;
    if (t == NULL) {
        exc = NULL;
        cause = NULL;
    } else {
        exc = (R_AST_expr)ITEM(t, 0);
        cause = (R_AST_expr)ITEM(t, 1);
        TYPE_ASSERT(exc, AST_expr);
        assert(cause == NULL || TYPE(cause) == type_AST_expr);
    }
    return  _HotPy_AST_Raise(exc, cause, line, column);
}

R_AST_expr expr_for_rule_attr(R_str attr, int line, int column) {
    return _HotPy_AST_Attribute(NULL, attr, LOAD, line, column);
}

R_AST_mod convert_compound_stmt_to_R_AST_mod(R_AST_stmt x) {    
    return _HotPy_AST_Interactive(list_of_one((R_object)x));
}

R_AST_mod convert_simple_stmt_to_R_AST_mod(R_list x) {
       return _HotPy_AST_Interactive(x);
}


static R_list flatten_stmt_list(R_list body) {
    //Convert lists of list into a single list.
    int i, j, len;
    R_list stmts;
    R_object item;
    len = LIST_SIZE(body);
    stmts = make_list(len);
    for (i = 0; i < len; i++) {
        item = LIST_ITEM(body, i);
        if (TYPE(item) == type_int) {
            // new line -- ignore   
        } else if (isinstance(item, type_AST_stmt)) {
            list_append(stmts, item);
        } else {
            R_list sub;
            TYPE_ASSERT(item, list);
            sub = (R_list)item;
            for (j = 0; j < LIST_SIZE(sub); j++) {
                list_append(stmts, LIST_ITEM(sub, j));
            }
        }
    }
    return stmts;
}

R_list list_for_rule_indented_block(int in, R_list l, int out) {
    return flatten_stmt_list(l);   
}

R_AST_mod mod_for_rule_file_input(R_list body) {
    R_list stmts = flatten_stmt_list(body);
    return _HotPy_AST_Module(stmts);
}

R_AST_stmt convert_yield_expr_to_R_AST_stmt(R_AST_expr x) {
    return _HotPy_AST_Expr(x, x->lineno, x->col_offset);
}

R_AST_cmpop cmpop_for_rule_is(int i) {
    if (i) 
        return hotpy_IsNot_singleton;
    else
        return hotpy_Is_singleton;
}

R_AST_expr expr_for_rule_testlist(R_list l, int trailing, int line, int column) {
    if (trailing || LIST_SIZE(l) != 1)
        return _HotPy_AST_Tuple(l, LOAD, line, column);
    else
        return (R_AST_expr)LIST_ITEM(l, 0);
}

R_object object_for_rule_argument(R_AST_expr e, R_object asgn_or_for) {
    if (asgn_or_for == NULL) {
        // Simple argument or tuple arg.
        return (R_object)e;
    } else if (TYPE(asgn_or_for) == type_list) {
        return (R_object)_HotPy_AST_GeneratorExp(e, (R_list)asgn_or_for, 
                                                 e->lineno, e->col_offset);
    } else { // keyword.
        TYPE_ASSERT(asgn_or_for, AST_expr);
        if (TYPE(e) == type_AST_Name) {
            R_str kw = ((R_AST_Name)e)->id;
            return (R_object)_HotPy_AST_keyword(kw, (R_AST_expr)asgn_or_for);
        } else {
            R_str msg = string_from_c_string("keyword can't be an expression");
            syntax_error(msg, e->lineno, e->col_offset);
            return NULL;
        }
    }
}

R_AST_mod mod_for_rule_eval_input(R_AST_expr expr, R_list newlines) {     
    return _HotPy_AST_Expression(expr);
}

R_AST_stmt stmt_for_rule_del_stmt(R_list exprs, int trailing, int line, int column) {
    int i, len;
    store_context_visitor->r0 = (R_object)DEL;
    len = LIST_SIZE(exprs);
    for (i = 0; i < len; i++)
        hotpy_visit_expr((R_AST_expr)ITEM(exprs->array, i), store_context_visitor);
    store_context_visitor->r0 = (R_object)STORE;
    return _HotPy_AST_Delete(exprs, line, column);
}

R_AST_stmt 
stmt_for_rule_while_stmt(R_AST_expr test, R_list body, R_list orelse, int line, int col) {
    return _HotPy_AST_While(test, body, orelse, line, col);
}

R_AST_expr expr_for_rule_yield_expr(R_AST_expr value, int line, int column) {
    return _HotPy_AST_Yield(value, line, column); 
}

R_AST_expr expr_for_rule_trinary_expr(R_AST_expr body, R_tuple t, int line, int column) {
    if (t == NULL) {
        return body;
    } else {
        R_AST_expr test = (R_AST_expr)ITEM(t, 0);
        R_AST_expr orelse = (R_AST_expr)ITEM(t, 1);
        return _HotPy_AST_IfExp(test, body, orelse, line, column);
    }
}

R_AST_stmt 
stmt_for_rule_decorated(R_list decorators, R_AST_stmt class_or_def, int line, int col) {
    if (decorators != NULL) {
        if (TYPE(class_or_def) == type_AST_ClassDef) {
            ((R_AST_ClassDef)class_or_def)->decorator_list = decorators;
        } else {
            TYPE_ASSERT(class_or_def, AST_FunctionDef);
            ((R_AST_FunctionDef)class_or_def)->decorator_list = decorators;
        }
    }
    return class_or_def;
}


//#define arguments_for_rule_varargslist arguments_for_rule_typedargslist

#define NAM(t) ((R_str)ITEM(t, 0))
#define ANN(t) ((R_str)ITEM(t, 1))
#define DEF(t) ((R_str)ITEM(t, 2))

static void parse_arg(R_object arg, R_str* name, R_AST_expr* ann) {
    TYPE_ASSERT(arg, AST_arg);  
    *name = ((R_AST_arg)arg)->arg;
    *ann = ((R_AST_arg)arg)->annotation;
}

R_AST_arguments arguments_for_rule_typedargslist(R_list list) {
    int i;
    R_list args;
    R_str vararg = NULL;
    R_AST_expr varargannotation = NULL;
    R_list kwonlyargs = NULL;
    R_str kwarg = NULL;
    R_AST_expr kwargannotation = NULL;
    R_list defaults = NULL;
    R_list kw_defaults = NULL;
    int have_default = 0;
    int kw_only = 0;
    int have_star = 0;
    int have_dstar = 0;
    args = make_list(LIST_SIZE(list));
    for (i = 0; i < LIST_SIZE(list); i++) {
        R_object l, r;
        R_tuple t = (R_tuple)LIST_ITEM(list, i);
        TYPE_ASSERT(t, tuple);
        assert(LEN(t) == 2);
        l = ITEM(t, 0);
        r = ITEM(t, 1);
        if (TYPE(l) == type_AST_arg) {
            if (kw_only) {
                list_append(kwonlyargs, l);
                if (r == NULL)
                    list_append(kw_defaults, (R_object)None);
                else
                    list_append(kw_defaults, r);
            } else {
                if (have_star)
                    ERROR("Cannot have positional parameters after *");
                if (have_dstar)
                    ERROR("Cannot have positional parameters after **");
                list_append(args, l);
                if (have_default) {
                    if (r == NULL)
                        ERROR("Non-default parameter cannot follow default parameter");
                    else
                        list_append(defaults, r);
                } else if (r != NULL) {
                    defaults = make_list(4);
                    have_default = 1;
                    list_append(defaults, r);
                }
            }
        } else {
            TYPE_ASSERT(l, str);
            assert(((R_str)l)->text[0] == '*');
            if (LEN((R_str)l) == 1) {
                if (have_star)
                    ERROR("More than one * in arguments");
                if (have_dstar)
                    ERROR("Cannot have ** before * in arguments");
                parse_arg(r, &vararg, &varargannotation);
                if (vararg == NULL) {
                    kw_only = 1;
                    kwonlyargs = make_list(4);
                    kw_defaults = make_list(4);
                }
                have_star = 1;
            } else {
                assert(((R_str)l)->text[1] == '*');
                if (have_dstar)
                    ERROR("More than one ** in arguments");
                parse_arg(r, &kwarg, &kwargannotation);
                assert(kwarg != NULL);
                have_dstar = 1;
            }
        }
    }
    return _HotPy_AST_arguments(args, vararg, varargannotation, kwonlyargs, kwarg,
                                kwargannotation, defaults, kw_defaults);
}

R_AST_slice slice_for_rule_sliceop(R_AST_expr step) {
    return _HotPy_AST_Slice(NULL, NULL, step);
}

R_AST_slice slice_for_rule_rslice(R_AST_expr test, R_AST_slice sliceop) {
    if (sliceop == NULL) {
        return _HotPy_AST_Slice(NULL, test, NULL);
    } else {
        TYPE_ASSERT(sliceop, AST_Slice);
        ((R_AST_Slice)sliceop)->upper = test;
        return sliceop;
    }
}

R_AST_slice slice_for_rule_slice(R_AST_expr test, R_AST_slice rslice) {
     if (rslice == NULL) {
         return _HotPy_AST_Index(test);
     } else {
         TYPE_ASSERT(rslice, AST_Slice);
         ((R_AST_Slice)rslice)->lower = test;
         return rslice;
     }
}


R_AST_slice slice_for_rule_subscriptlist(R_list slices, int trailing) {
    if (LIST_SIZE(slices) == 1 && !trailing)
        return (R_AST_slice)LIST_ITEM(slices, 0);
    else
        return _HotPy_AST_ExtSlice(slices);
}


R_list list_for_rule_comp_for(R_AST_expr target, int in, R_AST_expr iter, R_tuple opt) {
    int i, len;
    R_AST_comprehension comp;
    R_list ifs = NULL;
    R_list comps = NULL;
    if (opt != NULL) {
        TYPE_ASSERT(opt, tuple);
        ifs = (R_list)ITEM(opt, 0);
        comps = (R_list)ITEM(opt, 1);
        assert(ifs == NULL || TYPE(ifs) == type_list);
        assert(comps == NULL || TYPE(comps) == type_list);
    }
    hotpy_visit_expr(target, store_context_visitor);
    comp = _HotPy_AST_comprehension(target, iter, ifs);
    if (comps == NULL) {
        comps = list_of_one(comp);
    } else {
        list_insert_nolock(comps, 0, (R_object)comp);
    }
    return comps;
}

R_AST_expr 
expr_for_rule_testlist_comp(R_AST_expr test, R_list others, int line, int column) {
    R_object o;
    if (others == NULL) {
        return test;
    } else if (LIST_SIZE(others) > 0 && TYPE(LIST_ITEM(others, 0)) == type_AST_comprehension) {
        return _HotPy_AST_GeneratorExp(test, others, line, column);        
    } else {
        LIST_TYPE_ASSERT(others, AST_expr);
        list_insert_nolock(others, 0, (R_object)test);
        return _HotPy_AST_Tuple(others, LOAD, line, column);
    }
}

R_AST_expr expr_for_rule_list_expr(R_AST_expr e, int line, int column) {
    if (e == NULL) {
        e = _HotPy_AST_List(make_list(0), LOAD, line, column);
    } else if (TYPE(e) == type_AST_GeneratorExp) {
        SET_TYPE(e, type_AST_ListComp);
        e->visit_index = LISTCOMP_VISIT_INDEX;
    } else if (TYPE(e) == type_AST_Tuple) {
        SET_TYPE(e, type_AST_List);
        e->visit_index = LIST_VISIT_INDEX;
    } else {
        R_list l = list_of_one(e);
        e = _HotPy_AST_List(l, LOAD, line, column);
    }
    return e;
}

R_AST_arguments arguments_for_rule_varargslist(R_list list) {
    int i, len;
    R_str s;
    R_AST_arg arg;
    R_tuple t;
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        t = (R_tuple)LIST_ITEM(list, i);
        TYPE_ASSERT(t, tuple);
        assert(LEN(t) == 2);
        s = (R_str)ITEM(t, 0);
        TYPE_ASSERT(s, str);
        if (LEN(s) > 0 && s->text[0] == '*') {
            // Do nothing.
        } else {
            arg = _HotPy_AST_arg(s, NULL);
            ITEM(t, 0) = (R_object)arg;
        }
    }
    return arguments_for_rule_typedargslist(list);
}

