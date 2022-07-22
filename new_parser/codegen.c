#include "hotpy.h"
#include "Python-ast.h"
#include "ast.h"
#include "tokens.h"
#include "c_lexer.h"
#include "symtable.h"
#include "opcodes.h"
#include "operators.h"
#include "standard_names.h"
#include <malloc.h>
#include "version.h"

#define LOAD hotpy_Load_singleton
#define STORE hotpy_Store_singleton
#define DEL hotpy_Del_singleton

extern GVMT_OBJECT(type) _code_builder;
R_type type_code_builder = &_code_builder;

R_code_builder new_code_builder(void) {
    R_code_builder builder = gc_allocate(code_builder);
    SET_TYPE(builder, type_code_builder);
    ZERO_REF_COUNT(builder);
    builder->size = 0;
    builder->lock = GVMT_LOCK_INITIALIZER;
    builder->array = make_bytearray(128);
    builder->local_names = make_list(4);
    return builder;
}

#define SCOPE ((R_symtable_scope)visitor->r0)
#define CODE_BUILDER ((R_code_builder)visitor->r1)
#define NAMES ((R_dict)visitor->r2)
#define CONSTS ((R_dict)visitor->r3)
#define PROTECTION_DEPTH visitor->i

#define WRITE_BYTE(x) do { \
    R_bytelist code = (R_bytelist)visitor->r1; \
    if (code->size < LEN(code->array)) { \
        code->array->bytes[code->size] = x; \
        code->size++; \
    } else { \
        bytelist_append_nolock(code, x); \
    } \
} while(0)

#define WRITE_OP(x) WRITE_BYTE(OPCODE(x))
#define WRITE_SHORT(x) bytelist_append_short((R_bytelist)visitor->r1, x)
#define PATCH(x) /* To do -- Patch jump from x to HERE() */
#define PATCH_BREAKS() /* To do -- Patch in break statements */


static void write_name(R_str name, R_AST_Visitor visitor) {
    R_int index = (R_int)dict_get_str(NAMES, name);
    bytelist_append_short((R_bytelist)visitor->r1, int_from_py_int(index));
}

#define BEGIN_SCOPE(object) { \
    R_symtable_scope _outer_scope = SCOPE; \
    R_code_builder _outer_builder = CODE_BUILDER; \
    int _outer_protection_depth = PROTECTION_DEPTH; \
    visitor->r0 = dict_get(SCOPE->ste_table->st_blocks, (R_object)object); \
    visitor->r1 = (R_object)new_code_builder();

#define END_SCOPE visitor->r0 = (R_object)_outer_scope; \
    visitor->r1 = (R_object)_outer_builder; \
    PROTECTION_DEPTH = _outer_protection_depth; \
}

static void codegen_visit_expr_list(R_list list, R_AST_Visitor visitor) {
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

static void codegen_visit_stmt_list(R_list list, R_AST_Visitor visitor) {
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

void codegen_visit_Module(R_AST_Module object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Module);
    codegen_visit_stmt_list(object->body, visitor);
}

void codegen_visit_Interactive(R_AST_Interactive object,
                                R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Interactive);
    codegen_visit_stmt_list(object->body, visitor);
}

void codegen_visit_Expression(R_AST_Expression object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Expression);
    hotpy_visit_expr(object->body, visitor);
}

void codegen_visit_Suite(R_AST_Suite object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Suite);
    codegen_visit_stmt_list(object->body, visitor);
}

static void write_annotation(R_str key, R_AST_expr value, R_AST_Visitor visitor) {
    WRITE_OP(over);
    hotpy_visit_expr(value, visitor);
    WRITE_OP(setitem);
    write_name(key, visitor);
}

static void codegen_visit_arg(R_AST_arg object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_arg);
    if (object->annotation != NULL)
        write_annotation(object->arg, object->annotation, visitor);
}

static void codegen_visit_arg_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_arg)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_arg)*)ITEM(list->array, i);
        codegen_visit_arg(object, visitor);
    }
}

void codegen_defaults_and_annotations(R_AST_arguments object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_arguments);
    if (object->defaults) {
        codegen_visit_expr_list(object->defaults, visitor);
        WRITE_OP(pack);
        WRITE_BYTE(LIST_SIZE(object->defaults));
    } else {
        WRITE_OP(empty_tuple);
    }
    // What to do with kw_defaults ?
    if (object->kw_defaults) {
        assert("To do" && 0);
        // codegen_visit_expr_list(object->kw_defaults, visitor);
    }
    WRITE_OP(dictionary);
    codegen_visit_arg_list(object->args, visitor);
    codegen_visit_arg_list(object->kwonlyargs, visitor);
    if (object->vararg && object->varargannotation)
        write_annotation(object->vararg, object->varargannotation, visitor);
    if (object->kwarg && object->kwargannotation)
        write_annotation(object->kwarg, object->kwargannotation, visitor);
}

void codegen_visit_FunctionDef(R_AST_FunctionDef object,
                                R_AST_Visitor visitor) {
    int kind = 0;
    R_symtable_scope scope;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_FunctionDef);
    codegen_defaults_and_annotations(object->args, visitor);
    if (object->returns != NULL) {
        write_name(name_return, visitor);
        hotpy_visit_expr(object->returns, visitor);
    }
    WRITE_OP(make_func);
    BEGIN_SCOPE(object)
        if (SCOPE->ste_varargs)
            kind = TUPLE_FUNCTION;
        if (SCOPE->ste_generator)
            kind |= GENERATOR_FUNCTION;
        if (SCOPE->ste_varkeywords)
            kind |= DICT_FUNCTION;
        if (SCOPE->ste_varkeywords)
            kind |= DICT_FUNCTION;
        if (!SCOPE->ste_deletes)
            kind |= NO_DELETES;
        CODE_BUILDER->kind = kind;
        if (object->args && object->args->args)
            CODE_BUILDER->params = LIST_SIZE(object->args->args);
        write_name(object->name, visitor);
        WRITE_SHORT(kind);
        WRITE_SHORT(LIST_SIZE(object->args->args));
        codegen_visit_stmt_list(object->body, visitor);
    END_SCOPE
    codegen_visit_expr_list(object->decorator_list, visitor);
}

static void codegen_visit_keyword_list(R_list list, R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_keyword)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_keyword)*)ITEM(list->array, i);
        write_annotation(object->arg, object->value, visitor);
    }
}

void codegen_visit_ClassDef(R_AST_ClassDef object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ClassDef);
    WRITE_OP(new_scope);
    codegen_visit_stmt_list(object->body, visitor);
    WRITE_OP(pop_scope);
    codegen_visit_expr_list(object->bases, visitor);
    // Need to fix this up...
    assert("To do" && 0);
//    if (object->bases
//    WRITE_OP(dictionary);
//    codegen_visit_keyword_list(object->keywords, visitor);
    if (object->starargs != NULL)
        hotpy_visit_expr(object->starargs, visitor);
    if (object->kwargs != NULL)
        hotpy_visit_expr(object->kwargs, visitor);
    WRITE_OP(make_class);
    write_name(object->name, visitor);
    codegen_visit_expr_list(object->decorator_list, visitor);
}

void codegen_visit_Return(R_AST_Return object, R_AST_Visitor visitor) {
    int i;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Return);
    if (object->value == NULL) {
        WRITE_OP(none);
    } else {
        hotpy_visit_expr(object->value, visitor);
    }
    for (i = 0; i < PROTECTION_DEPTH; i++) {
        WRITE_OP(pop_handler);
    }
    WRITE_OP(return);
}

void codegen_visit_Delete(R_AST_Delete object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Delete);
    codegen_visit_expr_list(object->targets, visitor);
}

void codegen_visit_Assign(R_AST_Assign object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Assign);
    codegen_visit_expr_list(object->targets, visitor);
    hotpy_visit_expr(object->value, visitor);
}

void codegen_load_name(R_str name, R_AST_Visitor visitor) {
    assert("To do" && 0);
}

void codegen_store_name(R_str name, R_AST_Visitor visitor) {
    assert("To do" && 0);
}

void codegen_del_name(R_str name, R_AST_Visitor visitor) {
    assert("To do" && 0);
}

void write_inplace(R_AST_operator op, R_AST_Visitor visitor) {
    assert("To do" && 0);
}

void codegen_visit_AugAssign(R_AST_AugAssign object, R_AST_Visitor visitor) {
    R_type t = TYPE(object->target);
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_AugAssign);
    if (t == type_AST_Attribute) {
        R_AST_Attribute attr = (R_AST_Attribute)object->target;
        hotpy_visit_expr(attr->value, visitor);
        WRITE_OP(copy);
        WRITE_OP(load_attr);
        write_name(attr->attr, visitor);
        hotpy_visit_expr(object->value, visitor);
        write_inplace(object->op, visitor);
        WRITE_OP(swap);
        write_name(attr->attr, visitor);
        WRITE_OP(store_attr);
    } else if (t == type_AST_Subscript) {
        R_AST_Subscript sub = (R_AST_Subscript)object->target;
        hotpy_visit_expr(sub->value, visitor);
        hotpy_visit_slice(sub->slice, visitor);
        WRITE_OP(two_copy);
        WRITE_OP(getitem);
        hotpy_visit_expr(object->value, visitor);
        write_inplace(object->op, visitor);
        WRITE_OP(rrot);
        WRITE_OP(setitem);
    } else if (t == type_AST_Name) {
        R_AST_Name name = (R_AST_Name)object->target;
        codegen_load_name(name->id, visitor);
        hotpy_visit_expr(object->value, visitor);
        write_inplace(object->op, visitor);
        codegen_store_name(name->id, visitor);
    } else {
        assert("Invalid node type for augmented assignment" && 0);
    }
}

void write_line(R_AST_stmt stmt, R_AST_Visitor visitor) {
    WRITE_OP(line);
    WRITE_SHORT(stmt->lineno);
}

R_AST_expr new_temp(R_AST_Visitor visitor) {
    R_str name = int_to_str(PY_SMALL_INT(SCOPE->ste_tmpname), 10);
    R_AST_expr temp = _HotPy_AST_Name(name, hotpy_Store_singleton, 0, 0);
    SCOPE->ste_tmpname++;
    return temp;
}

void codegen_store_expr(R_AST_expr expr, R_AST_Visitor visitor) {
    assert("To do" && 0);
}

#define HERE() CODE_BUILDER->size
#define SET_CONTINUE(patch) visitor->p = (void*)(patch)
#define GET_CONTINUE() ((intptr_t)visitor->p)

void write_jump(int opcode, int patch, R_AST_Visitor visitor) {
    int inst = CODE_BUILDER->size;
    int offset = patch-inst;
    WRITE_BYTE(opcode);
    bytelist_append_short((R_bytelist)visitor->r1, offset);
}

#define WRITE_JUMP(x, p) write_jump(OPCODE(x), p, visitor)
#define WRITE_LINE(s) write_line((R_AST_stmt)s, visitor)

void codegen_visit_For(R_AST_For object, R_AST_Visitor visitor) {
    int patch;
    R_AST_expr temp;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_For);
    temp = new_temp(visitor);
    WRITE_LINE(object);
    hotpy_visit_expr(object->iter, visitor);
    WRITE_OP(iter);
    codegen_store_expr(temp, visitor);
    patch = HERE();
    SET_CONTINUE(patch);
    hotpy_visit_expr(temp, visitor);
    WRITE_OP(next);
    codegen_store_expr(object->target, visitor);
    codegen_visit_stmt_list(object->body, visitor);
    WRITE_JUMP(end_loop, patch);
    codegen_visit_stmt_list(object->orelse, visitor);
    PATCH_BREAKS();
}

int codegen_branch_expr(R_AST_expr test, int on_true,  R_AST_Visitor visitor) {
    assert("To do" && 0);
    return 0;
}

void codegen_visit_While(R_AST_While object, R_AST_Visitor visitor) {
    int here, patch, patch2;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_While);
    WRITE_LINE(object);
    patch = codegen_branch_expr(object->test, 0, visitor);
    here = HERE();
    SET_CONTINUE(here);
    codegen_visit_stmt_list(object->body, visitor);
    patch2 = codegen_branch_expr(object->test, 0, visitor);
    WRITE_OP(end_loop);
    PATCH(patch);
    PATCH(patch2);
    codegen_visit_stmt_list(object->orelse, visitor);
    PATCH_BREAKS();
}

void codegen_visit_If(R_AST_If object, R_AST_Visitor visitor) {
    int patch, patch2;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_If);
    WRITE_LINE(object);
    patch = codegen_branch_expr(object->test, 0, visitor);
    codegen_visit_stmt_list(object->body, visitor);
    if (object->orelse) {
        patch2 = HERE();
        WRITE_OP(done_if);
        WRITE_SHORT(0);
        PATCH(patch);
        codegen_visit_stmt_list(object->orelse, visitor);
        PATCH(patch2);
    } else {
        PATCH(patch);
    }
}

void codegen_visit_With(R_AST_With object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_With);
    WRITE_LINE(object);
    assert(0 && "To do...");
//    hotpy_visit_expr(object->context_expr, visitor);
//    if (object->optional_vars != NULL)
//        hotpy_visit_expr(object->optional_vars, visitor);
//    codegen_visit_stmt_list(object->body, visitor);
}

void codegen_visit_Raise(R_AST_Raise object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Raise);
    WRITE_LINE(object);
    if (object->exc != NULL) {
        hotpy_visit_expr(object->exc, visitor);
        if (object->cause != NULL) {
            hotpy_visit_expr(object->cause, visitor);
            WRITE_OP(over);
            WRITE_SHORT(standard_name_index___cause__);
            WRITE_OP(store_attr);
        }
        WRITE_OP(raise);
    } else {
        // Pick this up in symtable.
        // Put into temp at except:
        // Load temp here...
        // Load temp holding current exception.
        // Otherwise...
        WRITE_OP(current_exception);
        WRITE_OP(reraise);
    }
}

void codegen_visit_ExceptHandler(R_AST_ExceptHandler object,
                                    R_AST_Visitor visitor) {
    assert("Illegal state" && 0);
}

static int codegen_visit_excepthandler(R_AST_ExceptHandler object,
                                  R_AST_Visitor visitor, int patch) {
    assert(object != NULL);
    TYPE_ASSERT(object, AST_ExceptHandler);
    if (patch == 0) {
        R_str msg = string_from_c_string("default 'except:' must be last");
        syntax_error(msg, object->lineno, object->col_offset);
    }
    PATCH(patch);
    if (object->type != NULL) {
        hotpy_visit_expr(object->type, visitor);
        WRITE_OP(type_check);
        patch = HERE();
        WRITE_OP(on_false);
        WRITE_SHORT(0);
    } else {
        patch = 0;
    }
    if (object->name != NULL)
        codegen_store_name(object->name, visitor);
    else
        WRITE_OP(drop);
    codegen_visit_stmt_list(object->body, visitor);
    return patch;
}

static int codegen_visit_excepthandler_list(R_list list,
                                          R_AST_Visitor visitor, int patch) {
    int i, len;
    GVMT_OBJECT(AST_ExceptHandler)* object;
    if (list == NULL) {
        // Is this possible?
        return patch;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_ExceptHandler)*)ITEM(list->array, i);
        TYPE_ASSERT(object, AST_ExceptHandler);
        patch = codegen_visit_excepthandler(object, visitor, patch);
    }
    return patch;
}

void codegen_visit_TryExcept(R_AST_TryExcept object, R_AST_Visitor visitor) {
    int patch, patch2, patch3;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_TryExcept);
    WRITE_LINE(object);
    patch = HERE();
    WRITE_OP(protect);
    PROTECTION_DEPTH++;
    WRITE_SHORT(0);
    codegen_visit_stmt_list(object->body, visitor);
    patch2 = HERE();
    WRITE_OP(end_protect);
    PROTECTION_DEPTH--;
    patch = codegen_visit_excepthandler_list(object->handlers, visitor, patch);
    if (patch) {
        PATCH(patch);
        WRITE_OP(reraise);
    }
    if (object->orelse) {
        patch3 = HERE();
        WRITE_OP(jump);
        WRITE_SHORT(0);
        PATCH(patch2);
        codegen_visit_stmt_list(object->orelse, visitor);
        PATCH(patch3);
    } else {
        PATCH(patch2);
    }
}

void codegen_visit_TryFinally(R_AST_TryFinally object, R_AST_Visitor visitor) {
    int patch, patch2, patch3;
    R_AST_expr temp;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_TryFinally);
    temp = new_temp(visitor);
    WRITE_LINE(object);
    WRITE_OP(none);
    codegen_store_expr(temp, visitor);
    patch = HERE();
    WRITE_OP(protect);
    PROTECTION_DEPTH++;
    WRITE_SHORT(0);
    codegen_visit_stmt_list(object->body, visitor);
    patch2 = HERE();
    WRITE_OP(end_protect);
    PROTECTION_DEPTH--;
    WRITE_SHORT(0);
    PATCH(patch);
    codegen_store_expr(temp, visitor);
    PATCH(patch2);
    codegen_visit_stmt_list(object->finalbody, visitor);
    hotpy_visit_expr(temp, visitor);
    WRITE_OP(none);
    WRITE_OP(is);
    patch = HERE();
    WRITE_OP(on_true);
    WRITE_SHORT(0);
    hotpy_visit_expr(temp, visitor);
    WRITE_OP(reraise);
    PATCH(patch);
}

void codegen_visit_Assert(R_AST_Assert object, R_AST_Visitor visitor) {
    int patch, patch2, patch3;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Assert);
    WRITE_LINE(object);
    patch = codegen_branch_expr(object->test, 1, visitor);
    WRITE_OP(load_global);
    WRITE_SHORT(standard_name_index_AssertionError);
    if (object->msg != NULL) {
        hotpy_visit_expr(object->msg, visitor);
        WRITE_OP(pack_params);
        WRITE_BYTE(1);
        WRITE_OP(f_call);
    }
    WRITE_OP(raise);
    PATCH(patch);
}

void codegen_visit_Import(R_AST_Import object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Import);
    WRITE_LINE(object);
    assert(0 && "To do...");
    //codegen_visit_alias_list(object->names, visitor);
}

void codegen_visit_ImportFrom(R_AST_ImportFrom object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ImportFrom);
    WRITE_LINE(object);
    assert(0 && "To do...");
    //codegen_visit_identifier(object->module, visitor);
    //codegen_visit_alias_list(object->names, visitor);
    //codegen_visit_int(object->level, visitor);
}

void codegen_visit_Global(R_AST_Global object, R_AST_Visitor visitor) {
    /* No code generated */
}

void codegen_visit_Nonlocal(R_AST_Nonlocal object, R_AST_Visitor visitor) {
    /* No code generated */
}

void codegen_visit_Expr(R_AST_Expr object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Expr);
    WRITE_LINE(object);
    hotpy_visit_expr(object->value, visitor);
}

void codegen_visit_Pass(R_AST_Pass object, R_AST_Visitor visitor) {
    /* No code generated */
}

void codegen_visit_Break(R_AST_Break object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    // Breaks are always forward jumps, so need patching.
    TYPE_ASSERT(object, AST_Break);
    assert(0 && "To do...");
}

void codegen_visit_Continue(R_AST_Continue object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    // Continues are always backward jumps.
    TYPE_ASSERT(object, AST_Continue);
    WRITE_JUMP(continue, GET_CONTINUE());
}

void codegen_visit_BoolOp(R_AST_BoolOp object, R_AST_Visitor visitor) {
    int i, len;
    int *patches;
    R_AST_expr expr;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BoolOp);
    len = LIST_SIZE(object->values);
    patches = malloc(sizeof(int)*len);
    for (i = 0; i < len-1; i++) {
        expr = (GVMT_OBJECT(AST_expr)*)ITEM(object->values->array, i);
        hotpy_visit_expr(expr, visitor);
        WRITE_OP(copy);
        patches[i] = HERE();
        if (object->op == hotpy_Or_singleton)
            WRITE_OP(on_true);
        else
            WRITE_OP(on_false);
        WRITE_SHORT(0);
        WRITE_OP(drop);
    }
    for (i = 0; i < len-1; i++) {
        PATCH(patches[i]);
    }
    free(patches);
    expr = (GVMT_OBJECT(AST_expr)*)ITEM(object->values->array, len-1);
    hotpy_visit_expr(expr, visitor);
}

void codegen_visit_BinOp(R_AST_BinOp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BinOp);
    hotpy_visit_expr(object->left, visitor);
    hotpy_visit_expr(object->right, visitor);
    hotpy_visit_operator(object->op, visitor);
}

void codegen_visit_UnaryOp(R_AST_UnaryOp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_UnaryOp);
    hotpy_visit_expr(object->operand, visitor);
    hotpy_visit_unaryop(object->op, visitor);
}

/*********** TO HERE.... **********/

void codegen_visit_Lambda(R_AST_Lambda object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Lambda);
    codegen_defaults_and_annotations(object->args, visitor);
    hotpy_visit_expr(object->body, visitor);
}

void codegen_visit_IfExp(R_AST_IfExp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_IfExp);
    hotpy_visit_expr(object->test, visitor);
    hotpy_visit_expr(object->body, visitor);
    hotpy_visit_expr(object->orelse, visitor);
}

void write_dict_element(R_AST_expr key, R_AST_expr value, R_AST_Visitor visitor) {
    WRITE_OP(over);
    hotpy_visit_expr(value, visitor);
    WRITE_OP(setitem);
    hotpy_visit_expr(key, visitor);
}

void codegen_visit_Dict(R_AST_Dict object, R_AST_Visitor visitor) {
    int i, len;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Dict);
    WRITE_OP(dictionary);
    len = LIST_SIZE(object->keys);
    assert(len == LIST_SIZE(object->values));
    for (i = 0; i < len; i++) {
        R_AST_expr key = (GVMT_OBJECT(AST_expr)*)ITEM(object->keys->array, i);
        R_AST_expr value = (GVMT_OBJECT(AST_expr)*)ITEM(object->values->array, i);
        write_dict_element(key, value, visitor);
    }
}

void codegen_visit_Set(R_AST_Set object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Set);
    codegen_visit_expr_list(object->elts, visitor);
}

void codegen_visit_comprehension(R_AST_comprehension object,
                                  R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_comprehension);
    assert("To do" && 0);
    hotpy_visit_expr(object->target, visitor);
    hotpy_visit_expr(object->iter, visitor);
    codegen_visit_expr_list(object->ifs, visitor);
}

static void codegen_visit_comprehension_list(R_list list,
                                              R_AST_Visitor visitor) {
    int i, len;
    GVMT_OBJECT(AST_comprehension)* object;
    if (list == NULL) {
        return;
    }
    len = LIST_SIZE(list);
    for (i = 0; i < len; i++) {
        object = (GVMT_OBJECT(AST_comprehension)*)ITEM(list->array, i);
        codegen_visit_comprehension(object, visitor);
    }
}

void codegen_visit_ListComp(R_AST_ListComp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ListComp);
    hotpy_visit_expr(object->elt, visitor);
    codegen_visit_comprehension_list(object->generators, visitor);
}

void codegen_visit_SetComp(R_AST_SetComp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_SetComp);
    hotpy_visit_expr(object->elt, visitor);
    codegen_visit_comprehension_list(object->generators, visitor);
}

void codegen_visit_DictComp(R_AST_DictComp object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_DictComp);
    hotpy_visit_expr(object->key, visitor);
    hotpy_visit_expr(object->value, visitor);
    codegen_visit_comprehension_list(object->generators, visitor);
}

void codegen_visit_GeneratorExp(R_AST_GeneratorExp object,
                                 R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_GeneratorExp);
    hotpy_visit_expr(object->elt, visitor);
    codegen_visit_comprehension_list(object->generators, visitor);
}

void codegen_visit_Yield(R_AST_Yield object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Yield);
    if (object->value != NULL)
        hotpy_visit_expr(object->value, visitor);
}

void codegen_visit_Compare(R_AST_Compare object, R_AST_Visitor visitor) {
    int i, len;
    R_AST_expr right;
    R_AST_cmpop op;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Compare);
    op = (R_AST_cmpop)ITEM(object->ops->array, 0);
    right = (R_AST_expr)ITEM(object->comparators->array, 0);
    hotpy_visit_expr(object->left, visitor);
    hotpy_visit_expr(right, visitor);
    hotpy_visit_cmpop(op, visitor);
    len = LIST_SIZE(object->comparators);
    assert(len == LIST_SIZE(object->ops));
    if (len > 1) {
        assert("To do" && 0);
        //for (i = 1; i < len; i++) {
        //    left = (R_AST_expr)ITEM(object->comparators->array, i-1);
        //    op = (R_AST_cmpop)ITEM(object->ops->array, i);
        //    right = (R_AST_expr)ITEM(object->comparators->array, i);
        //}
    }
}

void codegen_visit_Call(R_AST_Call object, R_AST_Visitor visitor) {
    int tuple_dict;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Call);
    hotpy_visit_expr(object->func, visitor);
    tuple_dict = (object->keywords && LIST_SIZE(object->keywords)) ||
                 object->starargs || object->kwargs;
    if (tuple_dict) {
        if (object->keywords == NULL || LIST_SIZE(object->keywords) == 0) {
            if (object->starargs != NULL) {
                codegen_visit_expr_list(object->args, visitor);
            } else {
                WRITE_OP(empty_tuple);
            }
        } else {
            codegen_visit_expr_list(object->args, visitor);
            WRITE_OP(pack);
            WRITE_BYTE(LIST_SIZE(object->args));
            if (object->starargs != NULL) {
                hotpy_visit_expr(object->starargs, visitor);
                WRITE_OP(tuple_concat);
            }
        }
        if (object->kwargs != NULL) {
            hotpy_visit_expr(object->kwargs, visitor);
            WRITE_OP(copy_dict);
        } else {
            WRITE_OP(dictionary);
        }
        codegen_visit_keyword_list(object->keywords, visitor);
    } else {
        codegen_visit_expr_list(object->args, visitor);
        WRITE_OP(pack_params);
        WRITE_BYTE(LIST_SIZE(object->args));
    }
    WRITE_OP(f_call);
}

static void codegen_constant(R_object o, R_AST_Visitor visitor) {
    R_object val = dict_get(CONSTS, o);
    intptr_t index;
    if (val == NULL) {
        index = CONSTS->values->size;
        dict_set(CONSTS, o, (R_object)PY_SMALL_INT(index));
    } else {
        assert(gvmt_is_tagged(val));
        index = ((intptr_t)gvmt_untag(val)) >> 1;
    }
    WRITE_OP(constant);
    WRITE_SHORT(index);
}

void codegen_visit_Num(R_AST_Num object, R_AST_Visitor visitor) {
    R_object o;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Num);
    o = object->n;
    if (gvmt_is_tagged((GVMT_Object)o)) {
        intptr_t val = ((intptr_t)gvmt_untag(o)) >> 1;
        if (((int8_t)val) == val) {
            WRITE_OP(byte);
            WRITE_BYTE(val);
            return;
        }
    }
    codegen_constant(o, visitor);
}

void codegen_visit_Str(R_AST_Str object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Str);
    codegen_constant((R_object)object->s, visitor);
}

void codegen_visit_Bytes(R_AST_Bytes object, R_AST_Visitor visitor) {
    R_bytes bytes;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Bytes);
    // Convert str to ascii, Syntax error on failure.
    assert("To do" && 0);
    bytes = NULL;
    codegen_constant((R_object)bytes, visitor);
}

void codegen_visit_Ellipsis(R_AST_Ellipsis object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Ellipsis);
    codegen_constant((R_object)Ellipsis, visitor);
}

void codegen_visit_Attribute(R_AST_Attribute object, R_AST_Visitor visitor) {
    int op;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Attribute);
    hotpy_visit_expr(object->value, visitor);
    if (object->ctx == LOAD) {
        op = OPCODE(load_attr);
    } else if (object->ctx == STORE) {
        op = OPCODE(store_attr);
    } else {
        assert(object->ctx == DEL);
        op = OPCODE(delete_attr);
    }
    WRITE_BYTE(op);
    write_name(object->attr, visitor);
}

void codegen_visit_Subscript(R_AST_Subscript object, R_AST_Visitor visitor) {
    int op;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Subscript);
    hotpy_visit_expr(object->value, visitor);
    hotpy_visit_slice(object->slice, visitor);
    if (object->ctx == LOAD) {
        op = OPCODE(getitem);
    } else if (object->ctx == STORE) {
        op = OPCODE(setitem);
    } else {
        assert(object->ctx == DEL);
        op = OPCODE(delitem);
    }
    WRITE_BYTE(op);
}

void codegen_visit_Starred(R_AST_Starred object, R_AST_Visitor visitor) {
    int op;
    char* msg;
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Starred);
    if (object->ctx == STORE) {
        /* In all legitimate cases, the Starred node was already replaced
         * by codegen_visit_list/codegen_visit_tuple. */
        msg = "starred assignment target must be in a list or tuple";
	} else {
        msg = "can use starred expression only as assignment target";
    }
    syntax_error(string_from_c_string(msg), object->lineno, object->col_offset);
}

void codegen_visit_Name(R_AST_Name object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Name);
    // Need to identify type. Ie local(fast), nonlocal, global or generic(name)
    // Also store/load/delete.
    if (object->ctx == LOAD) {
        codegen_load_name(object->id, visitor);
    } else if (object->ctx == STORE) {
        codegen_store_name(object->id, visitor);
    } else {
        assert(object->ctx == DEL);
        codegen_del_name(object->id, visitor);
    }
}

void codegen_visit_List(R_AST_List object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_List);
    assert("To do" && 0);

    codegen_visit_expr_list(object->elts, visitor);
    //codegen_visit_expr_context(object->ctx, visitor);
}

void codegen_visit_Tuple(R_AST_Tuple object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Tuple);
    assert("To do" && 0);

    codegen_visit_expr_list(object->elts, visitor);
    //codegen_visit_expr_context(object->ctx, visitor);
}

void codegen_visit_Load(R_AST_Load object, R_AST_Visitor visitor) {
    assert("Illegate state" && 0);
}

void codegen_visit_Store(R_AST_Store object, R_AST_Visitor visitor) {
    assert("Illegate state" && 0);
}

void codegen_visit_Del(R_AST_Del object, R_AST_Visitor visitor) {
    assert("Illegate state" && 0);
}

void codegen_visit_AugLoad(R_AST_AugLoad object, R_AST_Visitor visitor) {
    assert("Illegate state" && 0);
}

void codegen_visit_AugStore(R_AST_AugStore object, R_AST_Visitor visitor) {
    assert("Illegate state" && 0);
}

void codegen_visit_Param(R_AST_Param object, R_AST_Visitor visitor) {
    assert("Illegate state" && 0);
}

void codegen_visit_Slice(R_AST_Slice object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Slice);
    if (object->lower != NULL) {
        hotpy_visit_expr(object->lower, visitor);
    } else {
        WRITE_OP(none);
    }
    if (object->upper != NULL) {
        hotpy_visit_expr(object->upper, visitor);
    } else {
        WRITE_OP(none);
    }
    if (object->step != NULL) {
        hotpy_visit_expr(object->step, visitor);
    } else {
        WRITE_OP(none);
    }
    WRITE_OP(slice);
}

static void codegen_visit_slice_list(R_list list, R_AST_Visitor visitor) {
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

void codegen_visit_ExtSlice(R_AST_ExtSlice object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_ExtSlice);
    codegen_visit_slice_list(object->dims, visitor);
    WRITE_OP(pack);
    WRITE_BYTE(LIST_SIZE(object->dims));
}

void codegen_visit_Index(R_AST_Index object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Index);
    hotpy_visit_expr(object->value, visitor);
}

void codegen_visit_And(R_AST_And object, R_AST_Visitor visitor) {
    assert("Illegal state" && 0);
}

void codegen_visit_Or(R_AST_Or object, R_AST_Visitor visitor) {
    assert("Illegal state" && 0);
}

#define WRITE_BINARY(x) WRITE_OP(binary); WRITE_BYTE(OPERATOR_##x##_INDEX)

void codegen_visit_Add(R_AST_Add object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Add);
    WRITE_BINARY(ADD);
}

void codegen_visit_Sub(R_AST_Sub object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Sub);
    WRITE_BINARY(SUB);
}

void codegen_visit_Mult(R_AST_Mult object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Mult);
    WRITE_BINARY(MUL);
}

void codegen_visit_Div(R_AST_Div object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Div);
    WRITE_BINARY(TRUEDIV);
}

void codegen_visit_Mod(R_AST_Mod object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Mod);
    WRITE_BINARY(MOD);
}

void codegen_visit_Pow(R_AST_Pow object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Pow);
    WRITE_BINARY(POW);
}

void codegen_visit_LShift(R_AST_LShift object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_LShift);
    WRITE_BINARY(LSHIFT);
}

void codegen_visit_RShift(R_AST_RShift object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_RShift);
    WRITE_BINARY(RSHIFT);
}

void codegen_visit_BitOr(R_AST_BitOr object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BitOr);
    WRITE_BINARY(OR_);
}

void codegen_visit_BitXor(R_AST_BitXor object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BitXor);
    WRITE_BINARY(XOR);
}

void codegen_visit_BitAnd(R_AST_BitAnd object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_BitAnd);
    WRITE_BINARY(AND_);
}

void codegen_visit_FloorDiv(R_AST_FloorDiv object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_FloorDiv);
    WRITE_BINARY(FLOORDIV);
}

void codegen_visit_Invert(R_AST_Invert object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Invert);
    WRITE_OP(unary);
    WRITE_BYTE(OPERATOR_INVERT_INDEX);
}

void codegen_visit_Not(R_AST_Not object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Not);
    WRITE_OP(not);
}

void codegen_visit_UAdd(R_AST_UAdd object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_UAdd);
    WRITE_OP(unary);
    WRITE_BYTE(OPERATOR_POS_INDEX);
}

void codegen_visit_USub(R_AST_USub object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_USub);
    WRITE_OP(unary);
    WRITE_BYTE(OPERATOR_NEG_INDEX);
}

void codegen_visit_Eq(R_AST_Eq object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Eq);
    WRITE_BINARY(EQ);
}

void codegen_visit_NotEq(R_AST_NotEq object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_NotEq);
    WRITE_BINARY(NE);
}

void codegen_visit_Lt(R_AST_Lt object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Lt);
    WRITE_BINARY(LT);
}

void codegen_visit_LtE(R_AST_LtE object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_LtE);
    WRITE_BINARY(LE);
}

void codegen_visit_Gt(R_AST_Gt object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Gt);
    WRITE_BINARY(GT);
}

void codegen_visit_GtE(R_AST_GtE object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_GtE);
    WRITE_BINARY(GE);
}

void codegen_visit_Is(R_AST_Is object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_Is);
    WRITE_OP(is);
}

void codegen_visit_IsNot(R_AST_IsNot object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_IsNot);
    WRITE_OP(is);
    WRITE_OP(not);
}

void codegen_visit_In(R_AST_In object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_In);
    WRITE_OP(contains);
}

void codegen_visit_NotIn(R_AST_NotIn object, R_AST_Visitor visitor) {
    if (object == NULL) {
        return;
    }
    TYPE_ASSERT(object, AST_NotIn);
    WRITE_OP(contains);
    WRITE_OP(not);
}

// For use in imports. -- To do...
//void codegen_visit_alias(R_AST_alias object, R_AST_Visitor visitor) {
//    if (object == NULL) {
//        return;
//    }
//    TYPE_ASSERT(object, AST_alias);
//    codegen_visit_identifier(object->name, visitor);
//    if (object->asname != NULL)
//        codegen_visit_identifier(object->asname, visitor);
//}
//
//static void codegen_visit_alias_list(R_list list, R_AST_Visitor visitor) {
//    int i, len;
//    GVMT_OBJECT(AST_alias)* object;
//    if (list == NULL) {
//        return;
//    }
//    len = LIST_SIZE(list);
//    for (i = 0; i < len; i++) {
//        object = (GVMT_OBJECT(AST_alias)*)ITEM(list->array, i);
//        codegen_visit_alias(object, visitor);
//    }
//}

#define write8 fputc
#define write16(x, o) fputc((x)>>8, o); fputc((x), o)

static void write_utf8(R_str name, FILE* out) {
    assert("To do" && 0);
}
static R_tuple tuple_from_index_dict(R_dict d) {
    R_tuple t;
    int i, ptr, len = d->values->size;
    R_dict_keys keys = d->values->keys;
    R_dict_values values = d->values;
    t = make_tuple(len);
    ptr = 0;
    for (i = 0; i < len; i++) {
        R_object name;
        R_int val;
        int index;
        while (ITEM(values, ptr) == 0)
            ptr++;
        val = (R_int)ITEM(values, ptr);
        TYPE_ASSERT(val, int);
        index = int_from_py_int(val);
        name = ITEM(keys, ptr);
        ITEM(t, index) = name;
        ptr++;
    }
    return t;
}

/** Move to same file as load_constant */
static void write_constant(R_object obj, FILE* out) {
    R_type t = TYPE(obj);
    if (t == type_int) {
        write8(0, out);
        assert("To do" && 0);
        //write_int_as_byte_array((R_int)obj);
    } else if (t == type_float) {
        union {
             double d;
             long long l;
        } u;
        long long ll;
        write8(1, out);
        u.d = ((R_float)obj)->value;
        ll = u.l;
        write8(ll >> 56, out);
        write8(ll >> 48, out);
        write8(ll >> 40, out);
        write8(ll >> 32, out);
        write8(ll >> 24, out);
        write8(ll >> 16, out);
        write8(ll >> 8, out);
        write8(ll, out);
    } else if (t == type_str) {
        write8(3, out);
        write_utf8((R_str)obj, out);
    } else {
        // Need to support None, NotImplemented, True, False & tuples.
        assert("To do" && 0);
    }
}

static void write_constants(R_tuple t, FILE* out) {
    int i, len = LEN(t);
    write16(len, out);
    for (i = 0; i < len; i++) {
         write_constant(ITEM(t, i), out);
    }
}

static void write_code_header(R_code_builder code, FILE* out) {
    write16(code->name, out);
    write16(code->kind, out);
    write16(code->params, out);
    write16(LIST_SIZE(code->local_names), out);
}

static void write_code_local_names(R_code_builder code, FILE* out) {
    int i, len = LIST_SIZE(code->local_names);
    R_array a = code->local_names->array;
    for (i = 0; i < len; i++) {
        int index = int_from_py_int((R_int)ITEM(a, i));
        write16(index, out);
    }
}

void write_bytes(R_bytes bytes, FILE* out) {
    assert("To do -- Should be in bytearray or File" && 0);
}

static void write_names(R_tuple t, FILE* out) {
    //Ignore first 256 items;
    int i, len;
    len = LEN(t);
    assert(len >= 256);
    write16(len-256, out);
    for (i = 256; i < len; i++) {
        write_utf8((R_str)ITEM(t, i), out);
    }
}

static void write_code_objects(R_tuple t, FILE* out) {
    int i, len;
    len = LEN(t);
    write16(len, out);
    for (i = 0; i < len; i++) {
        R_code_builder co = (R_code_builder)ITEM(t, i);
        TYPE_ASSERT(co, code_builder);
        write_code_header(co, out);
        write16(co->array->length, out);
        write_bytes((R_bytes)co->array, out);
        write_code_local_names(co, out);
    }
}

void write_pyc_file(R_code_builder code, R_Constants consts, FILE* out) {
    int i, len;
    R_tuple t;
    write16(MAGIC_NUMBER, out);
    write16(VERSION_NUMBER, out);
    assert("To do -- Ensure filename is stored and loaded as utf8 in parser/loader" && 0);
    write_names(consts->__names, out);
    write_constants(consts->__constants, out);
    write_utf8(consts->__file__, out);
    write_code_objects(consts->__functions, out);
    write16(code->array->length, out);
    write_bytes((R_bytes)code->array, out);
}


