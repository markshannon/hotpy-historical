#include "hotpy.h"
#include "Python-ast.h"
#include "ast.h"
#include "tokens.h"
#include "c_lexer.h"
#include "symtable.h"

#define TABLE ((R_symtable)visitor->r0)
#define SCOPE ((R_symtable_scope)visitor->r1)

extern GVMT_OBJECT(type) _symtable;
R_type type_symtable = &_symtable;

extern GVMT_OBJECT(type) _symtable_scope;
R_type type_symtable_scope = &_symtable_scope;

#define SET_FLAG(sym, f) \
do { \
    R_int i = PY_SMALL_INT(f); \
    R_dict d = ((R_symtable_scope)visitor->r1)->ste_symbols; \
    dict_set_str(d, sym, (R_object)i); \
} while (0)
    
#define EXIT_BLOCK do { \
    R_object o = (R_object)SCOPE->ste_enclosing; \
    visitor->r0 = o; \
} while (0)

static GVMT_OBJECT(symtable_scope)*
_enter_block(GVMT_OBJECT(symtable_scope)* enclosing, _Py_block_ty block, 
                                         R_str name, R_object node, int line) {
    R_symtable_scope scope = gc_allocate(symtable_scope);
    SET_TYPE(scope, type_symtable_scope);
    scope->ste_type = block;
    if (enclosing && enclosing->ste_type == FunctionBlock &&
        block == FunctionBlock)
        scope->ste_nested = 1;
    scope->ste_node = node;
    scope->ste_symbols = new_empty_dict();
    scope->ste_name = name;
    scope->ste_enclosing = enclosing;
    return scope;
}

#define ENTER_BLOCK(name, block, node, line) \
do { \
    R_symtable_scope new_scope = \
        _enter_block(SCOPE, block, name, (R_object)node, line); \
    new_scope->ste_table = TABLE; \
    dict_set(TABLE->st_blocks, (R_object)node, (R_object)new_scope); \
    visitor->r0 = (R_object)new_scope; \
} while (0) 

static void symbol_visit_expr_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_expr)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_expr)*)ITEM(list->array, i);
        hotpy_visit_expr(object, visitor);
    }
}

static void symbol_visit_stmt_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_stmt)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_stmt)*)ITEM(list->array, i);
        hotpy_visit_stmt(object, visitor);
    }
}

void symbol_visit_Module(R_AST_Module object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Module);
    symbol_visit_stmt_list(object->body, visitor);
}

void symbol_visit_Interactive(R_AST_Interactive object,
                                R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Interactive);
    symbol_visit_stmt_list(object->body, visitor);
}

void symbol_visit_Expression(R_AST_Expression object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Expression);
    hotpy_visit_expr(object->body, visitor);
}

void symbol_visit_Suite(R_AST_Suite object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Suite);
    symbol_visit_stmt_list(object->body, visitor);
}

static void symbol_visit_arg(R_AST_arg object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_arg);
    SET_FLAG(object->arg, DEF_PARAM);
}

static void symbol_visit_annotation(R_AST_arg object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_arg);
    if (object->annotation != NULL) 
        hotpy_visit_expr(object->annotation, visitor);
}

static void symbol_visit_annotations(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_arg)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_arg)*)ITEM(list->array, i);
        symbol_visit_annotation(object, visitor);
    }
}

static void symbol_visit_arg_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_arg)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_arg)*)ITEM(list->array, i);
        symbol_visit_arg(object, visitor);
    }
}

void symbol_visit_arguments(R_AST_arguments object, R_AST_Visitor visitor, R_object node, R_str name) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_arguments);
    symbol_visit_expr_list(object->defaults, visitor);
    symbol_visit_expr_list(object->kw_defaults, visitor);
    symbol_visit_annotations(object->args, visitor);
    if (object->varargannotation != NULL) 
        hotpy_visit_expr(object->varargannotation, visitor);
    if (object->kwargannotation != NULL) 
        hotpy_visit_expr(object->kwargannotation, visitor);
    ENTER_BLOCK(name, FunctionBlock, node, ((R_AST_stmt)node)->lineno);
    symbol_visit_arg_list(object->args, visitor);
    if (object->vararg != NULL) {
        SET_FLAG(object->vararg, USE);
        SCOPE->ste_varargs = 1;
    }
    symbol_visit_arg_list(object->kwonlyargs, visitor);
    if (object->kwarg != NULL) {
        SET_FLAG(object->kwarg, USE);
        SCOPE->ste_varkeywords = 1;
    }
}

void symbol_visit_FunctionDef(R_AST_FunctionDef object,
                                R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_FunctionDef);
    SET_FLAG(object->name, DEF_LOCAL);
    symbol_visit_arguments(object->args, visitor, (R_object)object, object->name);
    symbol_visit_stmt_list(object->body, visitor);
    symbol_visit_expr_list(object->decorator_list, visitor);
    EXIT_BLOCK;
}

static void symbol_visit_keyword(R_AST_keyword object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_keyword);
    hotpy_visit_expr(object->value, visitor);
}

static void symbol_visit_keyword_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_keyword)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_keyword)*)ITEM(list->array, i);
        symbol_visit_keyword(object, visitor);
    }
}

void symbol_visit_ClassDef(R_AST_ClassDef object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ClassDef);
    SET_FLAG(object->name, DEF_LOCAL);
    symbol_visit_expr_list(object->bases, visitor);
    symbol_visit_keyword_list(object->keywords, visitor);
    if (object->starargs != NULL) 
        hotpy_visit_expr(object->starargs, visitor);
    if (object->kwargs != NULL) 
        hotpy_visit_expr(object->kwargs, visitor);
    symbol_visit_expr_list(object->decorator_list, visitor);
    ENTER_BLOCK(object->name, ClassBlock, object, object->lineno);
    symbol_visit_stmt_list(object->body, visitor);
    EXIT_BLOCK;
}

void symbol_visit_Return(R_AST_Return object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Return);
    if (object->value != NULL) {
        hotpy_visit_expr(object->value, visitor);
        SCOPE->ste_returns_value = 1;
        if (SCOPE->ste_generator) {
            R_str msg = string_from_c_string(
                            "'return' with argument inside generator");
            syntax_error(msg, object->lineno, object->col_offset);
        }
    }
}

void symbol_visit_Delete(R_AST_Delete object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Delete);
    symbol_visit_expr_list(object->targets, visitor);
    SCOPE->ste_deletes = 1;
}

void symbol_visit_Assign(R_AST_Assign object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Assign);
    symbol_visit_expr_list(object->targets, visitor);
    hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_AugAssign(R_AST_AugAssign object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_AugAssign);
    hotpy_visit_expr(object->target, visitor);
    hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_For(R_AST_For object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_For);
    hotpy_visit_expr(object->target, visitor);
    hotpy_visit_expr(object->iter, visitor);
    symbol_visit_stmt_list(object->body, visitor);
    symbol_visit_stmt_list(object->orelse, visitor);
}

void symbol_visit_While(R_AST_While object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_While);
    hotpy_visit_expr(object->test, visitor);
    symbol_visit_stmt_list(object->body, visitor);
    symbol_visit_stmt_list(object->orelse, visitor);
}

void symbol_visit_If(R_AST_If object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_If);
    hotpy_visit_expr(object->test, visitor);
    symbol_visit_stmt_list(object->body, visitor);
    symbol_visit_stmt_list(object->orelse, visitor);
}

void symbol_visit_With(R_AST_With object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_With);
    hotpy_visit_expr(object->context_expr, visitor);
    if (object->optional_vars != NULL) 
        hotpy_visit_expr(object->optional_vars, visitor);
    symbol_visit_stmt_list(object->body, visitor);
}

void symbol_visit_Raise(R_AST_Raise object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Raise);
    if (object->exc != NULL) 
        hotpy_visit_expr(object->exc, visitor);
    if (object->cause != NULL) 
        hotpy_visit_expr(object->cause, visitor);
}


static void symbol_visit_excepthandler_list(R_list list,
                                              R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_excepthandler)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_excepthandler)*)ITEM(list->array, i);
        hotpy_visit_excepthandler(object, visitor);
    }
}

void symbol_visit_TryExcept(R_AST_TryExcept object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_TryExcept);
    symbol_visit_stmt_list(object->body, visitor);
    symbol_visit_excepthandler_list(object->handlers, visitor);
    symbol_visit_stmt_list(object->orelse, visitor);
}

void symbol_visit_TryFinally(R_AST_TryFinally object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_TryFinally);
    symbol_visit_stmt_list(object->body, visitor);
    symbol_visit_stmt_list(object->finalbody, visitor);
}

void symbol_visit_Assert(R_AST_Assert object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Assert);
    hotpy_visit_expr(object->test, visitor);
    if (object->msg != NULL) 
        hotpy_visit_expr(object->msg, visitor);
}

static void import_alias(R_object o, R_AST_Visitor visitor) {
    R_str name;
    if (TYPE(o) == type_str) {
        name = (R_str)o;
        SET_FLAG(name, DEF_IMPORT);
    } else {
        R_AST_alias a = (R_AST_alias)o;
        if (a->asname) {
            name = a->asname;
        } else {
            name = a->name;
        }
        if (LEN(name) == 1 && name->text[0] == '*') {
            if (SCOPE->ste_type != ModuleBlock) {
                    int lineno = SCOPE->ste_lineno;
                    R_str msg = string_from_c_string(
                        "import * only allowed at module level");
                    syntax_error(msg, lineno, 0);
            }
            SCOPE->ste_unoptimized |= OPT_IMPORT_STAR;
        } else {
            SET_FLAG(name, DEF_IMPORT);
        }
    }
}

static void import_list(R_list aliases, R_AST_Visitor visitor) {
    int i, len;
    if (aliases == NULL)
        return;
    len = LIST_SIZE(aliases);
    for (i = 0; i < len; i++) {
        R_object a = ITEM(aliases->array, i);
        import_alias(a, visitor);
    }
}

void symbol_visit_Import(R_AST_Import object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Import);
    import_list(object->names, visitor);
}

void symbol_visit_ImportFrom(R_AST_ImportFrom object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ImportFrom);
    import_list(object->names, visitor);
}

static void warn_after(const char *stmt, const char *what, R_str name) {
    R_str s, w, msg;
    s = string_from_c_string(stmt);
    w = string_from_c_string(what);
    msg = py_sprintf("%s is %s before %s declaration", name, w, s);
    // HotPyWarning(type_SyntaxWarning, msg);
}

void global(R_str name, R_AST_Visitor visitor) {
    R_int flag = (R_int)dict_get_str(SCOPE->ste_symbols, name);
    int cur = int_from_py_int(flag);
    if (cur & (DEF_LOCAL | USE)) {
        if (cur & DEF_LOCAL) 
            warn_after("global", "assigned to", name);
        else
            warn_after("global", "used", name);
    }
    SET_FLAG(name, DEF_GLOBAL);
}

void symbol_visit_Global(R_AST_Global object, R_AST_Visitor visitor) {
    R_list globals;
    int i, len;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Global);
    globals = object->names;
    len = LIST_SIZE(globals);
    for (i = 0; i < len; i++) {
        global((R_str)ITEM(globals->array, i), visitor); 
    }
}

void nonlocal(R_str name, R_AST_Visitor visitor) {
    R_int flag = (R_int)dict_get_str(SCOPE->ste_symbols, name);
    int cur = int_from_py_int(flag);
    if (cur & (DEF_LOCAL | USE)) {
        if (cur & DEF_LOCAL) 
            warn_after("nonlocal", "assigned to", name);
        else
            warn_after("nonlocal", "used", name);
    }
    SET_FLAG(name, DEF_NONLOCAL);
}

void symbol_visit_Nonlocal(R_AST_Nonlocal object, R_AST_Visitor visitor) {
    R_list nonlocals;
    int i, len;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Nonlocal);
    nonlocals = object->names;
    len = LIST_SIZE(nonlocals);
    for (i = 0; i < len; i++) {
        nonlocal((R_str)ITEM(nonlocals->array, i), visitor); 
    }
}

void symbol_visit_Expr(R_AST_Expr object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Expr);
    hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_Pass(R_AST_Pass object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_Break(R_AST_Break object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_Continue(R_AST_Continue object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_BoolOp(R_AST_BoolOp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BoolOp);
    symbol_visit_expr_list(object->values, visitor);
}

void symbol_visit_BinOp(R_AST_BinOp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BinOp);
    hotpy_visit_expr(object->left, visitor);
    hotpy_visit_expr(object->right, visitor);
}

void symbol_visit_UnaryOp(R_AST_UnaryOp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_UnaryOp);
    hotpy_visit_expr(object->operand, visitor);
}

void symbol_visit_Lambda(R_AST_Lambda object, R_AST_Visitor visitor) {
    R_str lambda;
    if (object == NULL) {
        return;
    }
    lambda = string_from_c_string("lambda");
    TYPE_ASSERT(object, AST_Lambda);
    SET_FLAG(lambda, DEF_NONLOCAL);
    symbol_visit_arguments(object->args, visitor, (R_object)object, lambda);
    hotpy_visit_expr(object->body, visitor);
    EXIT_BLOCK;
}

void symbol_visit_IfExp(R_AST_IfExp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_IfExp);
    hotpy_visit_expr(object->test, visitor);
    hotpy_visit_expr(object->body, visitor);
    hotpy_visit_expr(object->orelse, visitor);
}

void symbol_visit_Dict(R_AST_Dict object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Dict);
    symbol_visit_expr_list(object->keys, visitor);
    symbol_visit_expr_list(object->values, visitor);
}

void symbol_visit_Set(R_AST_Set object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Set);
    symbol_visit_expr_list(object->elts, visitor);
}

void symbol_visit_comprehension(R_AST_comprehension object,
                                  R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_comprehension);
    hotpy_visit_expr(object->target, visitor);
    hotpy_visit_expr(object->iter, visitor);
    symbol_visit_expr_list(object->ifs, visitor);
}

static void symbol_visit_comprehension_list(R_list list,
                                              R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_comprehension)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_comprehension)*)ITEM(list->array, i);
        symbol_visit_comprehension(object, visitor);
    }
}

void symbol_visit_ListComp(R_AST_ListComp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ListComp);
    hotpy_visit_expr(object->elt, visitor);
    symbol_visit_comprehension_list(object->generators, visitor);
}

void symbol_visit_SetComp(R_AST_SetComp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_SetComp);
    hotpy_visit_expr(object->elt, visitor);
    symbol_visit_comprehension_list(object->generators, visitor);
}

void symbol_visit_DictComp(R_AST_DictComp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_DictComp);
    hotpy_visit_expr(object->key, visitor);
    hotpy_visit_expr(object->value, visitor);
    symbol_visit_comprehension_list(object->generators, visitor);
}

void symbol_visit_GeneratorExp(R_AST_GeneratorExp object,
                                 R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_GeneratorExp);
    hotpy_visit_expr(object->elt, visitor);
    symbol_visit_comprehension_list(object->generators, visitor);
}

void symbol_visit_Yield(R_AST_Yield object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Yield);
    SCOPE->ste_generator = 1;
    if (SCOPE->ste_returns_value) {
        R_str msg = string_from_c_string(
                            "'return' with argument inside generator");
            syntax_error(msg, object->lineno, object->col_offset);
    }
    if (object->value != NULL) 
        hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_Compare(R_AST_Compare object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Compare);
    hotpy_visit_expr(object->left, visitor);
    symbol_visit_expr_list(object->comparators, visitor);
}

void symbol_visit_Call(R_AST_Call object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Call);
    hotpy_visit_expr(object->func, visitor);
    symbol_visit_expr_list(object->args, visitor);
    symbol_visit_keyword_list(object->keywords, visitor);
    if (object->starargs != NULL) 
        hotpy_visit_expr(object->starargs, visitor);
    if (object->kwargs != NULL) 
        hotpy_visit_expr(object->kwargs, visitor);
}

void symbol_visit_Num(R_AST_Num object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_Str(R_AST_Str object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_Bytes(R_AST_Bytes object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_Ellipsis(R_AST_Ellipsis object, R_AST_Visitor visitor) {
    /* Do nothing */
}

void symbol_visit_Attribute(R_AST_Attribute object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Attribute);
    hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_Subscript(R_AST_Subscript object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Subscript);
    hotpy_visit_expr(object->value, visitor);
    hotpy_visit_slice(object->slice, visitor);
}

void symbol_visit_Starred(R_AST_Starred object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Starred);
    hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_Name(R_AST_Name object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Name);
    if (object->ctx == hotpy_Load_singleton)
        SET_FLAG(object->id, USE);
    else
        SET_FLAG(object->id, DEF_LOCAL);
    if (object->id == name_super &&
        object->ctx == hotpy_Load_singleton &&
        SCOPE->ste_type == FunctionBlock) {
        SET_FLAG(name___class__, USE);
    }
}

void symbol_visit_List(R_AST_List object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_List);
    symbol_visit_expr_list(object->elts, visitor);
}

void symbol_visit_Tuple(R_AST_Tuple object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Tuple);
    symbol_visit_expr_list(object->elts, visitor);
}

void symbol_visit_Load(R_AST_Load object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Store(R_AST_Store object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Del(R_AST_Del object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_AugLoad(R_AST_AugLoad object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_AugStore(R_AST_AugStore object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Param(R_AST_Param object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Slice(R_AST_Slice object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Slice);
    if (object->lower != NULL) 
        hotpy_visit_expr(object->lower, visitor);
    if (object->upper != NULL) 
        hotpy_visit_expr(object->upper, visitor);
    if (object->step != NULL) 
        hotpy_visit_expr(object->step, visitor);
}

static void symbol_visit_slice_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_slice)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_slice)*)ITEM(list->array, i);
        hotpy_visit_slice(object, visitor);
    }
}

void symbol_visit_ExtSlice(R_AST_ExtSlice object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ExtSlice);
    symbol_visit_slice_list(object->dims, visitor);
}

void symbol_visit_Index(R_AST_Index object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Index);
    hotpy_visit_expr(object->value, visitor);
}

void symbol_visit_And(R_AST_And object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Or(R_AST_Or object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Add(R_AST_Add object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Sub(R_AST_Sub object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Mult(R_AST_Mult object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Div(R_AST_Div object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Mod(R_AST_Mod object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Pow(R_AST_Pow object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_LShift(R_AST_LShift object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_RShift(R_AST_RShift object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_BitOr(R_AST_BitOr object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_BitXor(R_AST_BitXor object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_BitAnd(R_AST_BitAnd object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_FloorDiv(R_AST_FloorDiv object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Invert(R_AST_Invert object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Not(R_AST_Not object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_UAdd(R_AST_UAdd object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_USub(R_AST_USub object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Eq(R_AST_Eq object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_NotEq(R_AST_NotEq object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Lt(R_AST_Lt object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_LtE(R_AST_LtE object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Gt(R_AST_Gt object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_GtE(R_AST_GtE object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_Is(R_AST_Is object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_IsNot(R_AST_IsNot object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_In(R_AST_In object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_NotIn(R_AST_NotIn object, R_AST_Visitor visitor) {
    assert(0 && "Illegal state");
}

void symbol_visit_ExceptHandler(R_AST_ExceptHandler object,
                                  R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ExceptHandler);
    if (object->type != NULL) 
        hotpy_visit_expr(object->type, visitor);
    if (object->name != NULL) 
        SET_FLAG(object->name, DEF_LOCAL);
    symbol_visit_stmt_list(object->body, visitor);
}

R_symtable hotpy_symboltable_for(R_str name, R_AST_mod module) {
    R_AST_Visitor visitor = hotpy_make_symbol_visitor();
    R_symtable table;
    R_symtable_scope scope;
    table = gc_allocate(symtable);
    table->st_filename = name;
    table->st_blocks = new_empty_dict();
    visitor->r0 = (R_object)table;
    ENTER_BLOCK(name, ModuleBlock, (R_object)module, 1);
    table->st_top = SCOPE;
    hotpy_visit_mod(module, visitor);
    return table;
}

#ifdef SYMTABLE_TEST

GVMT_Object hotpy_main(int argc, char **argv) {
    GVMT_Object ex, print_ex;
    R_object file, reader;
    R_Lexer lexer;
    int t;
    R_str filename;
    R_AST_mod ast;
    R_symtable symbols;
    gvmt_warn_on_unexpected_parameter_usage = 0;
    filename = init_vm(argc, argv);
    THREAD_STATE->current_frame = stop_frame();
    GVMT_TRY(ex)
        gvmt_malloc_init(1 << 24);
        init_tokens();
        file = (R_object)PY_open(R_int_from_ssize_t(0), py_sprintf("r"), filename);
        reader = _HotPyObject_GetAttr(file, string_from_c_string("_read_as_utf8"));
        TYPE_ASSERT(reader, bound_method);
        lexer = new_lexer(reader, filename);
        ast = parse_file_input(lexer);
        symbols = hotpy_symboltable_for(filename, ast);
        // Need to print something...
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

#endif 

