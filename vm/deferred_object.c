#include "hotpy.h"
#include "opcodes.h"
#include "optimise.h"
#include "deferred_object.h"
#include "operators.h"

#define KIND_LOCAL 1


#define KIND_TYPE 4
#define KIND_BOUND 5
#define KIND_SLICE 6
#define KIND_TUPLE 7
#define KIND_CONST 8
#define KIND_IS 9

#define KIND_F_BINARY 11
#define KIND_F_NEG 12
#define KIND_I2F 13
#define KIND_I_DIV 14
#define KIND_EMPTY_DICT 15
#define KIND_LIST 16
#define KIND_CACHED 17

static char* _names[] = {
   "",
   "LOCAL",
   "ADD",
   "SUB",
   "TYPE",
   "BOUND",
   "SLICE",
   "TUPLE",
   "CONST",
   "IS",
   "MUL",
   "F_BINARY",
   "F_NEG",
   "I2F",
   "I_DIV",
   "EMPTY_DICT",
   "LIST",
   "CACHED",
   "TOS",
   ""
};

R_deferred_state make_deferred_state(void) {
    R_deferred_state l = gc_allocate(deferred_state);
    SET_TYPE(l, type_deferred_state);
    l->frame_stack = make_list(4);
    l->deferred_stack = make_list(16);
    l->line_number = 0;
    l->next_ip = 0;
    l->cached_objects = 0;
    l->protect_stack = make_list(4);
    return l;
}

void deferred_poke(R_deferred_state state, int depth, R_deferred obj) {
    int stack_index = LIST_SIZE(state->deferred_stack)-1-depth;
    R_deferred result;
    assert(stack_index >= 0);
    assert(stack_index < LIST_SIZE(state->deferred_stack));
    ITEM(state->deferred_stack->array, stack_index) = (R_object)obj;
}

R_deferred deferred_pick(R_deferred_state state, int depth) {
    int stack_index = LIST_SIZE(state->deferred_stack)-1-depth;
    R_deferred result;
    assert(stack_index >= 0);
    assert(stack_index < LIST_SIZE(state->deferred_stack));
    result = (R_deferred)ITEM(state->deferred_stack->array, stack_index);
    assert(result);
    return result;
}

R_deferred deferred_local(int count) {
    R_deferred l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_LOCAL;
    l->type = NULL;
    l->value = (R_object)R_int_from_ssize_t(count);
    return l;
}

R_deferred deferred_tuple_prepend(R_deferred obj, R_deferred tuple) {
    R_tuple t;
    R_deferred l;
    if (tuple->kind != KIND_TUPLE)
        return NULL;
    t = (R_tuple)tuple->value;
    t = tuple_prepend((R_object)obj, t);
    l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_TUPLE;
    l->value = (R_object)t;
    l->type = type_tuple;
    return l;
}

int deferred_unpack(R_deferred tuple, R_deferred_state state) {
    int i, n;
    R_list stack = state->deferred_stack;
    R_tuple t;
    if (tuple->kind != KIND_TUPLE)
        return 0;
    t = (R_tuple)tuple->value;
    for (i = LEN(t)-1; i >= 0; i--) {
        list_append(stack, ITEM(t, i));
    }
    return 1;
}

R_deferred deferred_is(R_deferred l1, R_deferred l2) {
    R_deferred is;
    int k = l1->kind;
    if (l1 == l2) {
        return const_as_deferred((R_object)TRUE);
    }
    if (k == l2->kind) {
        if (k == KIND_LOCAL) {
            if (l1->value == l2->value) {
                return const_as_deferred((R_object)TRUE);
            }
        }
        if (k == KIND_CONST) {
            if (l1->value == l2->value) {
                return const_as_deferred((R_object)TRUE);
            } else {
                return const_as_deferred((R_object)FALSE);
            }
        }
    }
    if (l1->type && l2->type && l1->type != l2->type) {
        return const_as_deferred((R_object)FALSE);
    }
    is = gc_allocate(deferred);
    SET_TYPE(is, type_deferred);
    is->kind = KIND_IS;
    is->type = type_bool;
    is->value = (R_object)make_tuple2((R_object)l1, (R_object)l2);
    return is;
}

R_bool deferred_compare(R_deferred l1, R_deferred l2, int opcode) {
    int k = l1->kind;
    if (k != l2->kind)
        return NULL;
    if (k == KIND_CONST) {
        R_operator op;
        R_object result;
        if (opcode == OPCODE(i_comp_lt)) {
            op = operator_lt;
        } else if (opcode == OPCODE(i_comp_gt)) {
            op = operator_gt;
        } else if (opcode == OPCODE(i_comp_le)) {
            op = operator_le;
        } else if (opcode == OPCODE(i_comp_ge)) {
            op = operator_ge;
        } else if (opcode == OPCODE(i_comp_eq) || opcode == OPCODE(i_exit_eq)) {
            op = operator_eq;
        } else if (opcode == OPCODE(i_comp_ne) || opcode == OPCODE(i_exit_ne)) {
            op = operator_ne;
        } else {
            op = NULL;
            assert(0 && "Unexpected operator");
        }
        result = binary_call(op, l1->value, l2->value);
        if (TYPE(result) == type_bool) {
            return (R_bool)result;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

int deferred_is_tagged(R_deferred l) {
    if (l->kind == KIND_CONST) {
        return gvmt_is_tagged((GVMT_Object)l->value) != 0;
    } else {
        return 0;
    }
}

R_deferred deferred_empty_dict(void) {
    R_deferred l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_EMPTY_DICT;
    l->type = type_dict;
    return l;
}

static R_deferred f_binary(R_deferred l, R_deferred r, int opcode) {
    R_tuple t;
    R_deferred bin;
    t = make_tuple(3);
    ITEM(t, 0) = (R_object)l;
    ITEM(t, 1) = (R_object)r;
    ITEM(t, 2) = (R_object)R_int_from_ssize_t(opcode);
    bin = gc_allocate(deferred);
    SET_TYPE(bin, type_deferred);
    bin->kind = KIND_F_BINARY;
    bin->type = type_float;
    bin->value = (R_object)t;
    return bin;
}

R_deferred deferred_f_neg(R_deferred l) {
    R_deferred neg = gc_allocate(deferred);
    SET_TYPE(neg, type_deferred);
    neg->kind = KIND_F_NEG;
    neg->type = type_float;
    neg->value = (R_object)l;
    return neg;
}

static void materialise_as_double(R_deferred l, R_bytelist out);

static void materialise_f_binary_as_double(R_deferred f, R_bytelist out) {
    R_tuple t = (R_tuple)f->value;
    R_deferred l, r;
    l = (R_deferred)ITEM(t, 0);
    r = (R_deferred)ITEM(t, 1);
    materialise_as_double(l, out);
    materialise_as_double(r, out);
    bytelist_append_nolock(out, int_from_py_int((R_int)ITEM(t, 2)));
}

static void materialise_as_double(R_deferred l, R_bytelist out) {
    if (l->kind == KIND_F_BINARY) {
        materialise_f_binary_as_double(l, out);
    } else if (l->kind == KIND_F_NEG) {
        materialise_as_double((R_deferred)l->value, out);
        bytelist_append_nolock(out, OPCODE(d_neg));
    } else if (l->kind == KIND_I2F) {
        R_deferred i = (R_deferred)l->value;
        if (i->kind == KIND_CONST) {
            if (gvmt_is_tagged((GVMT_Object)i->value)) {
                int val = int_from_py_int((R_int)i->value);
                if (-128 <= val && val < 128) {
                    bytelist_append_nolock(out, OPCODE(d_byte));
                    bytelist_append_nolock(out, val);
                    return;
                }
            }
        }
        materialise(i, out);
        bytelist_append_nolock(out, OPCODE(i2d));
    } else if (l->kind == KIND_I_DIV) {
        R_tuple t = (R_tuple)l->value;
        materialise((R_deferred)ITEM(t, 0), out);
        materialise((R_deferred)ITEM(t, 1), out);
        bytelist_append_nolock(out, OPCODE(d_idiv));
    } else {
        materialise(l, out);
        bytelist_append_nolock(out, OPCODE(f2d));
    }
}

static void materialise_f_binary(R_deferred f, R_bytelist out) {
    R_tuple t = (R_tuple)f->value;
    R_deferred l, r;
    l = (R_deferred)ITEM(t, 0);
    r = (R_deferred)ITEM(t, 1);
    if (l->kind == KIND_F_BINARY || l->kind == KIND_F_NEG ||
        r->kind == KIND_F_BINARY || r->kind == KIND_F_NEG) {
        materialise_as_double(l, out);
        materialise_as_double(r, out);
        bytelist_append_nolock(out, int_from_py_int((R_int)ITEM(t, 2)));
        bytelist_append_nolock(out, OPCODE(d2f));
    } else {
        int op = int_from_py_int((R_int)ITEM(t, 2));
        materialise(l, out);
        materialise(r, out);
        if (op == OPCODE(d_add)) op = OPCODE(f_add);
        else if (op == OPCODE(d_sub)) op = OPCODE(f_sub);
        else if (op == OPCODE(d_mul)) op = OPCODE(f_mul);
        else if (op == OPCODE(d_div)) op = OPCODE(f_div);
        else if (op == OPCODE(d_idiv)) op = OPCODE(i_div);
        else assert(0 && "Invalid opcode");
        bytelist_append_nolock(out, op);
    }
}

static void materialise_f_neg(R_deferred f, R_bytelist out) {
    R_deferred l = (R_deferred)f->value;
    if (l->kind == KIND_F_BINARY || l->kind == KIND_F_NEG) {
        materialise_as_double(l, out);
        bytelist_append_nolock(out, OPCODE(d_neg));
        bytelist_append_nolock(out, OPCODE(d2f));
    } else {
        materialise(l, out);
        bytelist_append_nolock(out, OPCODE(f_neg));
    }
}

R_deferred deferred_f_add(R_deferred l, R_deferred r) {
    return f_binary(l, r, OPCODE(d_add));
}

R_deferred deferred_f_sub(R_deferred l, R_deferred r) {
    return f_binary(l, r, OPCODE(d_sub));
}

R_deferred deferred_f_mul(R_deferred l, R_deferred r) {
    return f_binary(l, r, OPCODE(d_mul));
}

R_deferred deferred_f_div(R_deferred l, R_deferred r) {
    return f_binary(l, r, OPCODE(d_div));
}

R_deferred deferred_i_div(R_deferred l, R_deferred r) {
    R_tuple t;
    R_deferred bin;
    t = make_tuple2((R_object)l, (R_object)r);
    bin = gc_allocate(deferred);
    SET_TYPE(bin, type_deferred);
    bin->kind = KIND_I_DIV;
    bin->type = type_float;
    bin->value = (R_object)t;
    return bin;
}

R_deferred deferred_i2f(R_deferred l) {
    R_deferred f = gc_allocate(deferred);
    SET_TYPE(f, type_deferred);
    f->kind = KIND_I2F;
    f->type = type_float;
    f->value = (R_object)l;
    return f;
}

R_deferred deferred_bind(R_deferred l, R_deferred method) {
    R_tuple t = make_tuple2((R_object)l, (R_object)method);
    R_deferred bound = gc_allocate(deferred);
    SET_TYPE(bound, type_deferred);
    bound->kind = KIND_BOUND;
    bound->type = type_bound_method;
    bound->value = (R_object)t;
    return bound;
}

R_deferred deferred_slice(R_deferred lo, R_deferred hi, R_deferred step) {
    R_deferred l;
    R_slice s = gc_allocate(slice);
    SET_TYPE(s, type_slice);
    s->start = (R_object)lo;
    s->stop = (R_object)hi;
    s->step = (R_object)step;
    l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_SLICE;
    l->type = type_slice;
    l->value = (R_object)s;
    return l;
}

int deferred_ensure(R_deferred l, R_type t) {
    if (l->type == t)
        return 0;
    l->type = t;
    return 1;
}

R_deferred deferred_frame_load_local(R_deferred_frame frame, int n) {
    return (R_deferred)ITEM(frame->locals, n);
}

void deferred_frame_store_local(R_deferred_frame frame, int n, R_deferred value) {
    ITEM(frame->locals, n) = (R_object)value;
}

R_deferred_frame deferred_frame(R_function func) {
    R_deferred_frame frame;
    R_tuple t, locals;
    int i, n, lcount;
    frame = gc_allocate(deferred_frame);
    SET_TYPE(frame, type_deferred_frame);
    lcount = func->__code__->locals;
    if (func->parameter_format & CLOSURE) {
        frame->locals = make_tuple(lcount + 1);

    } else {
        frame->locals = make_tuple(lcount);
    }
    frame->line_number = 0;
    frame->function = func;
    frame->no_trace = func->__code__->kind & FUNCTION_NO_TRACE;
    frame->protect_stack = make_list(4);
    return frame;
}

static void print_deferred(R_deferred item, FILE* out) {
    if (item) {
        if (item->kind == KIND_LOCAL) {
            fprintf(out, "LOCAL(%d) ", int_from_py_int((R_int)item->value));
        } else if (item->kind == KIND_TUPLE) {
            R_tuple t = (R_tuple)item->value;
            int i;
            fprintf(out, "(");
            for (i = 0; i < LEN(t); i++) {
                print_deferred((R_deferred)ITEM(t, i), out);
                if (i) fprintf(out, ", ");
            }
            fprintf(out, ") ");
        } else {
            fprintf(out, "%s ", _names[item->kind]);
        }
    } else {
        fprintf(out, "stack ");
    }
}

R_deferred_frame deferred_make_frame(R_deferred func) {
    if (func->kind == KIND_CONST) {
        if (TYPE(func->value) == type_function) {
            return deferred_frame((R_function)func->value);
        }
        if (debug_optimise) {
            fprintf(stderr, "Not a function: ");
            print_deferred(func, stderr);
            fprintf(stderr, "\n");
        }
    } else {
        if (debug_optimise) {
            fprintf(stderr, "Not a const: ");
            print_deferred(func, stderr);
            fprintf(stderr, "\n");
        }
    }
    return NULL;
}

R_deferred _right_part(R_tuple t, int count) {
    R_deferred l;
    t = tuple_slice(t, LEN(t)-count, LEN(t));
    l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_TUPLE;
    l->value = (R_object)t;
    return l;
}

static int _init_frame(R_deferred_frame frame, R_base_function func, R_tuple params, R_deferred d) {
    int i, format;
    format = func->parameter_format & TUPLE_DICT_FUNCTION;
    if (func->parameter_format & CLOSURE) {
        // Don't optimise closure frames yet.
        return 0;
        // To do...
        // extract func_closure & make deferred const.
        // Store deferred const in locals[0].
        // (Need to handle load/store deref as well.)
    }
    if (d->kind != KIND_EMPTY_DICT) {
        if (debug_optimise) fprintf(stderr, "Not an empty dict\n");
        return 0;
    }
    if (format == 0) {
        int pcount = LEN(func->paramNames);
        int min_params = pcount-LEN(func->defaults);
        // Default values:
        // This needs to be moved to specialiser as func.__defaults__ is writeable
        if (min_params > LEN(params) || pcount < LEN(params)) {
            if (debug_optimise) fprintf(stderr, "Wrong params\n");
            return 0;
        }
        for (i = 0; i < LEN(params); i++) {
            R_object o = ITEM(params, i);
            ITEM(frame->locals, i) = o;
        }
        for (; i < pcount; i++) {
            R_object o = ITEM(func->defaults, LEN(func->defaults)-pcount+i);
            ITEM(frame->locals, i) = (R_object)const_as_deferred(o);
        }
        return 1;
    } else if (format == TUPLE_FUNCTION) {
        int pcount = LEN(func->paramNames)-1;
        int min_params = pcount-LEN(func->defaults);
        if (min_params > LEN(params)) {
            if (debug_optimise) fprintf(stderr, "Wrong params\n");
            return 0;
        }
        for (i = 0; i < LEN(params) && i < pcount; i++) {
            R_object o = ITEM(params, i);
            ITEM(frame->locals, i) = o;
        }
        for (; i < pcount; i++) {
            R_object o = ITEM(func->defaults, LEN(func->defaults)-pcount+i);
            ITEM(frame->locals, i) = (R_object)const_as_deferred(o);
        }
        if (pcount < LEN(params)) {
            ITEM(frame->locals, pcount) = (R_object)_right_part(params, LEN(params)-pcount);
        } else {
            ITEM(frame->locals, pcount) = (R_object)_right_part(params, 0);
        }
        return 1;
    } else if (format == TUPLE_DICT_FUNCTION) {
        int pcount = LEN(func->paramNames)-2;
        int min_params = pcount-LEN(func->defaults);
        if (min_params > LEN(params)) {
            if (debug_optimise) fprintf(stderr, "Wrong params\n");
            return 0;
        }
        for (i = 0; i < LEN(params) && i < pcount; i++) {
            R_object o = ITEM(params, i);
            ITEM(frame->locals, i) = o;
        }
        for (; i < pcount; i++) {
            R_object o = ITEM(func->defaults, LEN(func->defaults)-pcount+i);
            ITEM(frame->locals, i) = (R_object)const_as_deferred(o);
        }
        if (pcount < LEN(params)) {
            ITEM(frame->locals, pcount) = (R_object)_right_part(params, LEN(params)-pcount);
        } else {
            ITEM(frame->locals, pcount) = (R_object)_right_part(params, 0);
        }
        ITEM(frame->locals, pcount+1) = (R_object)d;
        return 1;
    } else {
        int pcount = LEN(func->paramNames)-1;
        int min_params = pcount-LEN(func->defaults);
        assert(format == DICT_FUNCTION);
        if (min_params > LEN(params)) {
            if (debug_optimise) fprintf(stderr, "Wrong params\n");
            return 0;
        }
        for (i = 0; i < LEN(params); i++) {
            R_object o = ITEM(params, i);
            ITEM(frame->locals, i) = o;
        }
        for (; i < pcount; i++) {
            R_object o = ITEM(func->defaults, LEN(func->defaults)-pcount+i);
            ITEM(frame->locals, i) = (R_object)const_as_deferred(o);
        }
        ITEM(frame->locals, pcount+1) = (R_object)d;
        return 1;
    }
}

int deferred_init_frame(R_deferred_frame frame, R_deferred f, R_deferred t, R_deferred d) {
    int i, format;
    R_function func;
    R_tuple params;
    if (frame == NULL) {
        if (debug_optimise) fprintf(stderr, "Frame is not deferred");
        return 0;
    }
    if (t->kind != KIND_TUPLE) {
        if (debug_optimise) fprintf(stderr, "Not a tuple\n");
        return 0;
    }
    if (f->kind != KIND_CONST) {
        if (debug_optimise) fprintf(stderr, "Func not const\n");
        return 0;
    }
    func = (R_function)f->value;
    if (TYPE(func) != type_function) {
        if (debug_optimise) fprintf(stderr, "Not a function\n");
        return 0;
    }
    params = (R_tuple)t->value;
    return _init_frame(frame, (R_base_function)func, params, d);
}

R_deferred callable_from_deferred_bm(R_deferred l) {
    assert(l->kind == KIND_BOUND);
    return (R_deferred)ITEM((R_tuple)l->value, 1);
}

void deferred_prepare_bm_call(R_deferred_state state, R_bytelist out, uint8_t* ip) {
    R_deferred d = deferred_pop(state);
    R_deferred t = deferred_pop(state);
    R_deferred bm = deferred_pop(state);
    if (bm->kind == KIND_BOUND) {
        R_deferred self = deferred_unbind(bm);
        R_deferred callable = callable_from_deferred_bm(bm);
        assert(self && callable);
        t = deferred_tuple_prepend(self, t);
        deferred_push(state, callable);
        deferred_push(state, t);
        deferred_push(state, d);
    } else {
        deferred_push(state, bm);
        deferred_push(state, t);
        deferred_push(state, d);
        materialise_state(state, out, ip);
        bytelist_append_nolock(out, OPCODE(prepare_bm_call));
    }
}

R_deferred deferred_tuple_concat(R_deferred l1, R_deferred l2) {
    R_deferred l;
    R_tuple t1, t2;
    if (l1->kind != KIND_TUPLE || l2->kind != KIND_TUPLE) {
        return NULL;
    }
    t1 = (R_tuple)l1->value;
    t2 = (R_tuple)l2->value;
    t1 = tuple_concat(t1, t2);
    l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_TUPLE;
    l->value = (R_object)t1;
    return l;
}

R_deferred deferred_copy_dict(R_deferred d) {
    if (d->kind == KIND_EMPTY_DICT) {
        return d;
    } else {
        return NULL;
    }
}

static void push_defaults(R_builtin_function func, int pcount, R_list stack) {
    int i, required;
    R_tuple defaults = func->defaults;
    required = LEN(func->paramNames)-pcount;
    assert(required >= 0);
    assert(required <= LEN(defaults));
    for (i = 0; i < required; i++) {
        R_object item = ITEM(defaults, LEN(defaults)-required+i);
        list_append(stack, (R_object)const_as_deferred(item));
    }
}

int deferred_native_params(R_builtin_function func, R_deferred t, R_deferred d,
                           R_deferred_state state) {
    int i, pcount;
    R_tuple params;
    R_list stack = state->deferred_stack;
    if (t->kind != KIND_TUPLE) {
        fprintf(stderr, "Not a tuple\n");
        return 0;
    }
    if (d->kind != KIND_EMPTY_DICT) {
        fprintf(stderr, "Not an empty dict\n");
        return 0;
    }
    params = (R_tuple)t->value;
    pcount = func->paramNames->length;
    if (LEN(params) > pcount) {
        fprintf(stderr, "Wrong params\n");
        return 0;
    }
    if (LEN(func->defaults)+LEN(params) < pcount) {
        fprintf(stderr, "Wrong params\n");
        return 0;
    }
    for (i = 0; i < LEN(params); i++) {
        list_append(stack, ITEM(params, i));
    }
    push_defaults(func, LEN(params), stack);
    return 1;
}

R_deferred deferred_unbind(R_deferred bm) {
    if (bm->kind == KIND_BOUND) {
        return (R_deferred)ITEM((R_tuple)bm->value, 0);
    } else {
        return NULL;
    }
}

R_deferred const_as_deferred(R_object value) {
    R_deferred l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_CONST;
    l->type = TYPE(value);
    l->value = value;
    return l;
}

static void materialise_is(R_deferred is, R_bytelist output) {
    R_tuple t = (R_tuple)is->value;
    R_deferred l = (R_deferred)ITEM(t, 0);
    R_deferred r = (R_deferred)ITEM(t, 1);
    materialise(l, output);
    materialise(r, output);
    bytelist_append_nolock(output, OPCODE(is));
}

R_deferred deferred_type(R_deferred l) {
    if (l->type) {
        return const_as_deferred((R_object)l->type);
    } else {
        R_deferred type = gc_allocate(deferred);
        SET_TYPE(type, type_deferred);
        type->kind = KIND_TYPE;
        type->type = type_type;
        type->value = (R_object)l;
        return type;
    }
}

static void materialise_d_cache(R_deferred l, R_bytelist output) {
    int index = int_from_py_int((R_int)l->value);
    bytelist_append_nolock(output, OPCODE(d2f));
    bytelist_append_nolock(output, index);
}

static void materialise_i2f(R_deferred l, R_bytelist output) {
    materialise((R_deferred)l->value, output);
    bytelist_append_nolock(output, OPCODE(i2f));
}

/** Ensure all bytecodes used here are traced by the tracing interpreter */
void materialise(R_deferred l, R_bytelist output) {
    if (l->kind <= KIND_TUPLE) {
        if (l->kind == KIND_LOCAL) {
            bytelist_append_nolock(output, OPCODE(fast_load_frame));
            bytelist_append_nolock(output, int_from_py_int((R_int)l->value));
        } else if (l->kind == KIND_TYPE) {
            R_deferred val = (R_deferred)l->value;
            materialise(val, output);
            bytelist_append_nolock(output, OPCODE(type));
        } else if (l->kind == KIND_BOUND) {
            R_tuple t = (R_tuple)l->value;
            materialise((R_deferred)ITEM(t, 0), output);
            materialise((R_deferred)ITEM(t, 1), output);
            bytelist_append_nolock(output, OPCODE(bind));
        } else if (l->kind == KIND_SLICE) {
            R_slice s = (R_slice)l->value;
            materialise((R_deferred)s->start, output);
            materialise((R_deferred)s->stop, output);
            materialise((R_deferred)s->step, output);
            bytelist_append_nolock(output, OPCODE(slice));
        } else {
            int i, n;
            R_tuple t = (R_tuple)l->value;
            assert(l->kind == KIND_TUPLE);
            for (i = 0, n = LEN(t); i < n; i++) {
                materialise((R_deferred)ITEM(t, i), output);
            }
            bytelist_append_nolock(output, OPCODE(pack));
            bytelist_append_nolock(output, n);
        }
    } else {
        if (l->kind == KIND_CONST) {
            if (l->value == (R_object)None) {
                bytelist_append_nolock(output, OPCODE(none));
            } else if (l->value == (R_object)TRUE) {
                bytelist_append_nolock(output, OPCODE(true));
            } else if (l->value == (R_object)FALSE) {
                bytelist_append_nolock(output, OPCODE(false));
            } else if (gvmt_is_tagged((GVMT_Object)l->value)) {
                int val = int_from_py_int((R_int)l->value);
                if (-128 <= val && val < 128) {
                    bytelist_append_nolock(output, OPCODE(byte));
                    bytelist_append_nolock(output, val);
                } else {
                    void *addr = gvmt_untag((GVMT_Object)l->value);
                    bytelist_append_nolock(output, OPCODE(fast_constant));
                    bytelist_append_long(output, (int32_t)addr);
                }
            } else {
                void *addr = gvmt_pin((GVMT_Object)l->value);
                bytelist_append_nolock(output, OPCODE(fast_constant));
                bytelist_append_long(output, (int32_t)addr);
            }
        } else if (l->kind == KIND_IS) {
            materialise_is(l, output);
        } else if (l->kind == KIND_CACHED) {
            bytelist_append_nolock(output, OPCODE(load_from_cache));
            bytelist_append_nolock(output, int_from_py_int((R_int)l->value));
        } else if (l->kind == KIND_F_BINARY) {
            materialise_f_binary(l, output);
        } else if (l->kind == KIND_F_NEG) {
            materialise_f_binary(l, output);
        } else if (l->kind == KIND_I2F) {
            materialise_i2f(l, output);
        } else if (l->kind == KIND_I_DIV) {
            R_tuple t = (R_tuple)l->value;
            materialise((R_deferred)ITEM(t, 0), output);
            materialise((R_deferred)ITEM(t, 1), output);
            bytelist_append_nolock(output, OPCODE(i_div));
        } else if (l->kind == KIND_EMPTY_DICT) {
            bytelist_append_nolock(output, OPCODE(dictionary));
        } else {
            int i, n;
            R_tuple t;
            assert(l->kind == KIND_LIST);
            t = (R_tuple)l->value;
            for (i = 0, n = LEN(t); i < n; i++) {
                materialise((R_deferred)ITEM(t, i), output);
            }
            bytelist_append_nolock(output, OPCODE(list));
            bytelist_append_nolock(output, n);
        }
    }
}

R_deferred deferred_slot(R_deferred deferred, unsigned offset) {
    if (deferred->kind == KIND_BOUND) {
        R_tuple t = (R_tuple)deferred->value;
        if (offset == offsetof(GVMT_OBJECT(bound_method), object)) {
            return (R_deferred)ITEM(t, 0);
        } else if (offset == offsetof(GVMT_OBJECT(bound_method), callable)) {
            return const_as_deferred(ITEM(t, 1));
        }
    } else if (deferred->kind == KIND_SLICE) {
        R_slice s = (R_slice)deferred->value;
        if (offset == offsetof(GVMT_OBJECT(slice), start)) {
            return (R_deferred)s->start;
        } else if (offset == offsetof(GVMT_OBJECT(slice), stop)) {
            return (R_deferred)s->stop;
        } if (offset == offsetof(GVMT_OBJECT(slice), step)) {
            return (R_deferred)s->step;
        }
    }
    return NULL;
}

static void clean_stack(R_deferred_state state) {
    int i, stack_size = LIST_SIZE(state->deferred_stack);
    for (i = 0; i < stack_size; i++) {
        if (ITEM(state->deferred_stack->array, i)) {
            return;
        }
    }
    SET_LIST_SIZE(state->deferred_stack, 0);
}

static void flush_stack_partial(R_deferred_state state, R_bytelist out, int flush_count) {
    int i, n, stack_size, next_cache;
    assert(flush_count >= 0);
    assert(flush_count <= LIST_SIZE(state->deferred_stack));
    clean_stack(state);
    stack_size = LIST_SIZE(state->deferred_stack);
    if (stack_size == 0)
        return;
    if (flush_count > stack_size) {
        flush_count = stack_size;
    }
    if (flush_count < 4) {
        // Use stack manipulation rather than the cache.
        for (i = stack_size-flush_count; i < stack_size; i++) {
            R_deferred l = (R_deferred)ITEM(state->deferred_stack->array, i);
            if (l) {
                 materialise(l, out);
            }
        }
        if (flush_count < 2) {
            // No permuation required
        } else if (flush_count == 2) {
            if (ITEM(state->deferred_stack->array, stack_size-2) != NULL &&
                ITEM(state->deferred_stack->array, stack_size-1) == NULL) {
                bytelist_append_nolock(out, OPCODE(swap));
            }
        } else {
            int perm;
            assert(flush_count == 3);
            perm = ITEM(state->deferred_stack->array, stack_size-3) != NULL;
            perm |= (ITEM(state->deferred_stack->array, stack_size-2) != NULL) * 2;
            perm |= (ITEM(state->deferred_stack->array, stack_size-1) != NULL) * 4;
            if (perm == 0 || perm == 4 || perm == 6 || perm == 7) {
                // - - - , - - X, - X X , X X X
                // Do nothing
            } else if (perm == 1) {  // X - -
                bytelist_append_nolock(out, OPCODE(rrot));
            } else if (perm == 2) { // - X -
                bytelist_append_nolock(out, OPCODE(swap));
            }  else if (perm == 3) {  // X X -
                bytelist_append_nolock(out, OPCODE(rotate));
            } else {  // X - X
                assert(perm == 5);
                bytelist_append_nolock(out, OPCODE(rotate));
                bytelist_append_nolock(out, OPCODE(swap));
            }
        }
        return;
    }
    next_cache = state->cached_objects;
    for (i = stack_size-flush_count; i < stack_size; i++) {
        if (ITEM(state->deferred_stack->array, i) == NULL) {
            bytelist_append_nolock(out, OPCODE(store_to_cache));
            bytelist_append_nolock(out, next_cache);
            next_cache++;
        }
    }
    for (i = stack_size-flush_count; i < stack_size; i++) {
        R_deferred l = (R_deferred)ITEM(state->deferred_stack->array, i);
        if (l) {
            materialise(l, out);
        } else {
            next_cache--;
            bytelist_append_nolock(out, OPCODE(load_from_cache));
            bytelist_append_nolock(out, next_cache);
        }
    }
}

void deferred_swap(R_deferred_state state, R_bytelist out) {
    intptr_t len;
    clean_stack(state);
    len = LIST_SIZE(state->deferred_stack);
    if (len >= 2) {
        R_object tos = list_pop(state->deferred_stack);
        R_object nos = list_pop(state->deferred_stack);
        if (tos == NULL && nos == NULL) {
            bytelist_append_nolock(out, OPCODE(swap));
        }
        list_append(state->deferred_stack, tos);
        list_append(state->deferred_stack, nos);
    } else if (len == 1) {
        assert(ITEM(state->deferred_stack->array, 0) != NULL);
        list_append(state->deferred_stack, NULL);
    } else {
        bytelist_append_nolock(out, OPCODE(swap));
    }
}

static void flush_stack(R_deferred_state state, R_bytelist out) {
    flush_stack_partial(state, out, LIST_SIZE(state->deferred_stack));
}

void materialise_top(int n, R_deferred_state state, R_bytelist out) {
    if (n >= LIST_SIZE(state->deferred_stack)) {
        materialise_stack(state, out, NULL);
        return;
    }
    flush_stack_partial(state, out, n);
    if (n > LIST_SIZE(state->deferred_stack))
        LIST_SIZE(state->deferred_stack) = 0;
    else
        LIST_SIZE(state->deferred_stack) -= n;
}

void deferred_push(R_deferred_state state, R_deferred item) {
    list_append(state->deferred_stack, (R_object)item);
}

R_deferred deferred_pop(R_deferred_state state) {
    return (R_deferred)list_pop(state->deferred_stack);
}

R_deferred deferred_peek(R_deferred_state state) {
    intptr_t len = LIST_SIZE(state->deferred_stack);
    if (len == 0) {
        return NULL;
    }
    return (R_deferred)ITEM(state->deferred_stack->array, len-1);
}

void deferred_push_frame(R_deferred_state state, R_deferred_frame item) {
    list_append(state->frame_stack, (R_object)item);
}

int no_deferred_frames(R_deferred_state state) {
    return LIST_SIZE(state->frame_stack) == 0;
}

R_deferred_frame deferred_pop_frame(R_deferred_state state) {
    if (LIST_SIZE(state->frame_stack)) {
        return (R_deferred_frame)list_pop(state->frame_stack);
    } else {
        return NULL;
    }
}

R_deferred_frame deferred_peek_frame(R_deferred_state state) {
    intptr_t len = LIST_SIZE(state->frame_stack);
    if (len == 0) {
        return NULL;
    }
    return (R_deferred_frame)ITEM(state->frame_stack->array, len-1);
}

void deferred_drop(R_deferred_state state, R_bytelist out) {
    if (LIST_SIZE(state->deferred_stack)) {
        R_object item = list_pop(state->deferred_stack);
        if (item == NULL) {
            bytelist_append_nolock(out, OPCODE(drop));
        }
    } else {
        bytelist_append_nolock(out, OPCODE(drop));
    }
}

void deferred_drop_under(R_deferred_state state, R_bytelist out) {
    R_object tos;
    R_object nos;
    if (LIST_SIZE(state->deferred_stack)) {
        tos = list_pop(state->deferred_stack);
    } else {
        tos = NULL;
    }
    if (LIST_SIZE(state->deferred_stack)) {
        nos = list_pop(state->deferred_stack);
    } else {
        nos = NULL;
    }
    if (nos == NULL) {
        if (tos == NULL) {
            bytelist_append_nolock(out, OPCODE(drop_under));
        } else {
            bytelist_append_nolock(out, OPCODE(drop));
        }
    }
    list_append(state->deferred_stack, (R_object)tos);
}

static R_deferred _deferred_cache_store_item(R_deferred_state state, R_bytelist out) {
    int index = state->cached_objects;
    assert(index < 60);
    state->cached_objects++;
    bytelist_append_nolock(out, OPCODE(store_to_cache));
    bytelist_append_nolock(out, index);
    return deferred_cached_item(index);
}

R_deferred deferred_cache_store_item(R_deferred_state state, R_bytelist out) {
    if (LIST_SIZE(state->deferred_stack)) {
        R_object tos = list_pop(state->deferred_stack);
        assert(tos == NULL);
    }
    return _deferred_cache_store_item(state, out);
}

static void make_top_deferred(int n, R_deferred_state state, R_bytelist out) {
    int i;
    assert(n <= LIST_SIZE(state->deferred_stack));
    for (i = LIST_SIZE(state->deferred_stack)-1; i >= LIST_SIZE(state->deferred_stack)-n; i--) {
        R_deferred item = (R_deferred)ITEM(state->deferred_stack->array, i);
        if (item == NULL) {
            ITEM(state->deferred_stack->array, i) = (R_object)_deferred_cache_store_item(state, out);
        }
    }
}

/** Force the top n items on the stack to be deferred */
void make_tos_deferred(int n, R_deferred_state state, R_bytelist out) {
    int i, pull_ups;
    R_list new_stack;
    if (n > LIST_SIZE(state->deferred_stack)) {
        make_top_deferred(LIST_SIZE(state->deferred_stack), state, out);
        pull_ups = n - LIST_SIZE(state->deferred_stack);
        new_stack = make_list(n);
        for(i = pull_ups-1; i >= 0; i--) {
            ITEM(new_stack->array, i) = (R_object)_deferred_cache_store_item(state, out);
        }
        SET_LIST_SIZE(new_stack, pull_ups);
        for(i = 0; i < LIST_SIZE(state->deferred_stack); i++) {
            list_append(new_stack, ITEM(state->deferred_stack->array, i));
        }
        state->deferred_stack = new_stack;
    } else {
        make_top_deferred(n, state, out);
    }
}


void make_deferred(R_deferred_state state, R_bytelist out) {
    make_top_deferred(LIST_SIZE(state->deferred_stack), state, out);
}

void homogenise_tos(int n, R_deferred_state state, R_bytelist out) {
    int i, pull_ups;
    R_list new_stack;
    if (LIST_SIZE(state->deferred_stack) == 0) {
        return;
    }
    if (n <= LIST_SIZE(state->deferred_stack)) {
        int all_null = 1;
        for (i = LIST_SIZE(state->deferred_stack)-n; i < LIST_SIZE(state->deferred_stack); i++) {
            if (ITEM(state->deferred_stack->array, i) != NULL) all_null = 0;
        }
        if (all_null == 0) {
            make_top_deferred(n, state, out);
        }
    } else {
        make_tos_deferred(n, state, out);
    }
}

R_deferred deferred_tuple(int count, R_deferred_state state, R_bytelist out) {
    R_deferred l;
    int i;
    R_list stack = state->deferred_stack;
    R_tuple t = make_tuple(count);
    if (LIST_SIZE(stack) < count) {
        if (count > 2)
            return NULL;
        while (LIST_SIZE(stack) < count) {
            list_insert_nolock(state->deferred_stack, 0, NULL);
        }
    }
    make_top_deferred(count, state, out);
    for (i = 0; i < count; i++) {
        R_object o = ITEM(stack->array, LIST_SIZE(stack)-count+i);
        ITEM(t, i) = o;
    }
    LIST_SIZE(stack) -= count;
    l = gc_allocate(deferred);
    SET_TYPE(l, type_deferred);
    l->kind = KIND_TUPLE;
    l->value = (R_object)t;
    l->type = type_tuple;
    return l;
}

void materialise_stack(R_deferred_state state, R_bytelist out, uint8_t* ip) {
    if (LIST_SIZE(state->deferred_stack) && debug_optimise && ip) {
        fprintf(stderr, "Stack materialised in %s \n", gvmt_opcode_names_interpreter[*ip]);
    }
    flush_stack(state, out);
    SET_LIST_SIZE(state->deferred_stack, 0);
}

static void push_frame_values(R_deferred_frame frame, R_bytelist output) {
    int i;
    R_tuple locals;
    assert (frame != NULL);
    locals = frame->locals;
    for (i = 0; i < LEN(locals); i++) {
        R_deferred l = (R_deferred)ITEM(locals, i);
        if (l) {
            materialise(l, output);
        }
    }
}

static void materialise_exception_handler(R_exception_handler h, R_bytelist output) {
    int current_depth = gvmt_stack_depth();
//    GVMT_Value* base = gvmt_stack_top() + current_depth;
    bytelist_append_nolock(output, OPCODE(make_exception_handler));
    bytelist_append_nolock(output, h->stop);
    bytelist_append_long(output, (int32_t)h->data_stack);
    bytelist_append_long(output, (int32_t)h->ip);
}

static void materialise_frame_from_values(R_deferred_frame frame, R_bytelist output) {
    int i, explicit;
    R_tuple locals;
    void* addr;
    assert (frame != NULL);
    locals = frame->locals;
    addr = gvmt_pin((GVMT_Object)frame->function);
    explicit = LEN(locals) <= 2;
    if (explicit == 0) {
        for (i = 0; i < LEN(locals); i++) {
            if (ITEM(locals, i) == NULL) {
                explicit = 1;
            }
        }
    }
    if (explicit) {
        bytelist_append_nolock(output, OPCODE(fast_constant));
        bytelist_append_long(output, (int32_t)addr);
        bytelist_append_nolock(output, OPCODE(make_frame));
        bytelist_append_nolock(output, 0);
        bytelist_append_nolock(output, OPCODE(drop));
        for (i = LEN(locals)-1; i >= 0; i--) {
            R_deferred l = (R_deferred)ITEM(locals, i);
            if (l) {
                bytelist_append_nolock(output, OPCODE(store_frame));
                bytelist_append_nolock(output, i);
            }
        }
    } else {
        bytelist_append_nolock(output, OPCODE(fast_frame));
        bytelist_append_nolock(output, LEN(locals));
        bytelist_append_long(output, (int32_t)addr);
    }
    for (i = 0; i < LIST_SIZE(frame->protect_stack); i++) {
        R_exception_handler h = (R_exception_handler)ITEM(frame->protect_stack->array, i);
        materialise_exception_handler(h, output);
    }
    if (frame->ip) {
        bytelist_append_nolock(output, OPCODE(set_next_ip));
        bytelist_append_long(output, (int32_t)frame->ip);
    }
    if (frame->line_number) {
        bytelist_append_nolock(output, OPCODE(fast_line));
        bytelist_append_short(output, frame->line_number);
    }
}

static void flush_frame_stack(R_list frame_stack, R_bytelist output) {
    int i;
    for (i = LIST_SIZE(frame_stack)-1; i >= 0; i--) {
        push_frame_values((R_deferred_frame)ITEM(frame_stack->array, i), output);
    }
    for (i = 0; i < LIST_SIZE(frame_stack); i++) {
        R_deferred_frame f = (R_deferred_frame)ITEM(frame_stack->array, i);
        materialise_frame_from_values(f, output);
    }
}

static void flush_current_frame(R_deferred_state state, R_bytelist out) {
    if (state->line_number) {
        bytelist_append_nolock(out, OPCODE(fast_line));
        bytelist_append_short(out, state->line_number);
    }
    if (state->next_ip) {
        bytelist_append_nolock(out, OPCODE(set_next_ip));
        bytelist_append_long(out, (int32_t)state->next_ip);
    }
}

static void flush_state(R_deferred_state state, R_bytelist out) {
    int i, n;
    if (LIST_SIZE(state->frame_stack)) {
        flush_current_frame(state, out);
        flush_stack(state, out);
        flush_frame_stack(state->frame_stack, out);
    } else {
        flush_current_frame(state, out);
        flush_stack(state, out);
    }
    if (state->cached_objects) {
        bytelist_append_nolock(out, OPCODE(clear_cache));
        bytelist_append_nolock(out, state->cached_objects);
    }
}

int space_in_deferred_cache(R_deferred_state state) {
    // Leave some headroom for the immediates.
    return state->cached_objects < 50;
}

R_deferred deferred_cached_item(int index) {
    R_deferred cached = gc_allocate(deferred);
    SET_TYPE(cached, type_deferred);
    cached->kind = KIND_CACHED;
    cached->type = NULL;
    cached->value = (R_object)R_int_from_ssize_t(index);
    return cached;
}

void materialise_state(R_deferred_state state, R_bytelist out, uint8_t* ip) {
    int i;
    if (LIST_SIZE(state->frame_stack) && debug_optimise) {
        fprintf(stderr, "Frame(s) materialised in %s \n", gvmt_opcode_names_interpreter[*ip]);
    }
    flush_state(state, out);
    SET_LIST_SIZE(state->frame_stack, 0);
    SET_LIST_SIZE(state->deferred_stack, 0);
    state->line_number = 0;
    state->next_ip = 0;
    state->cached_objects = 0;
}

static R_deferred remove_frame_dependencies(R_deferred l, R_deferred_state state, R_bytelist out) {
    // Don't be thorough, only really interested in simple values and tuples.
    if (l->kind == KIND_LOCAL) {
        if (!space_in_deferred_cache(state)) return NULL;
        bytelist_append_nolock(out, OPCODE(fast_load_frame));
        bytelist_append_nolock(out, int_from_py_int((R_int)l->value));
        return _deferred_cache_store_item(state, out);
    } else if (l->kind == KIND_TUPLE){
        int i, n;
        R_tuple t = (R_tuple)l->value;
        for (i = 0, n = LEN(t); i < n; i++) {
            R_deferred item = (R_deferred)ITEM(t, i);
            item = remove_frame_dependencies(item, state, out);
            if (item == NULL) {
                return NULL;
            } else {
                ITEM(t, i) = (R_object)item;
            }
            return l;
        }
        return l;
    } else if (l->kind == KIND_CONST) {
        return l;
    } else if (l->kind == KIND_CACHED) {
        return l;
    } else if (l->kind == KIND_EMPTY_DICT) {
        return l;
    } else {
        return NULL;
    }
}

static void materialise_top_locals(R_deferred_state state, R_bytelist out) {
    R_list stack = state->deferred_stack;
    int i, top_most_locals = 0;
    R_deferred l;
    while (top_most_locals < LIST_SIZE(stack)) {
        l = (R_deferred)ITEM(stack->array, LIST_SIZE(stack) - 1 - top_most_locals);
        if (l == NULL || l->kind != KIND_LOCAL) break;
        top_most_locals++;
    }
    for (i = LIST_SIZE(stack) - top_most_locals; i < LIST_SIZE(stack); i++) {
        l = (R_deferred)ITEM(stack->array, i);
        materialise(l, out);
    }
    LIST_SIZE(stack) -= top_most_locals;
}

static int stack_remove_frame_dependencies(R_deferred_state state, R_bytelist out) {
    R_list stack = state->deferred_stack;
    int i, len;
    materialise_top_locals(state,out);
    clean_stack(state);
    len = LIST_SIZE(stack);
    if (len > 8)
        return 0;
    for (i = 0; i < len; i++) {
        R_deferred l = (R_deferred)ITEM(stack->array, i);
        if (l) {
            l = remove_frame_dependencies(l, state, out);
            if (l == NULL) return 0;
            ITEM(stack->array, i) = (R_object)l;
        }
    }
    return 1;
}

static int frame_remove_frame_dependencies(R_deferred_frame frame,
                                    R_deferred_state state, R_bytelist out) {
    int i, len;
    len = LEN(frame->locals);
    for (i = 0; i < len; i++) {
        R_deferred d = (R_deferred)ITEM(frame->locals, i);
        if (d) {
            d = remove_frame_dependencies(d, state, out);
            if (d == NULL) return 0;
            ITEM(frame->locals, i) = (R_object)d;
        }
    }
    return 1;
}

void deferred_remove_stack_to_frame_dependencies(R_deferred_state state,
                                          R_bytelist out, uint8_t* ip) {
    if (!stack_remove_frame_dependencies(state, out))
        materialise_state(state, out, ip);
}

void deferred_remove_frame_to_frame_dependencies(R_deferred_state state,
                                            R_bytelist out, uint8_t* ip) {
    int i;
    for (i = 0; i < LIST_SIZE(state->frame_stack); i++) {
        R_deferred_frame f = (R_deferred_frame)ITEM(state->frame_stack->array, i);
        if (f->no_trace == 0) {
            materialise_state(state, out, ip);
        }
        return;
    }
    for (i = 0; i < LIST_SIZE(state->frame_stack); i++) {
        R_deferred_frame f = (R_deferred_frame)ITEM(state->frame_stack->array, i);
        if (!frame_remove_frame_dependencies(f, state, out)) {
            materialise_state(state, out, ip);
        }
    }
}

void materialise_current_frame(R_deferred_state state, R_bytelist out) {
    flush_current_frame(state, out);
    state->line_number = 0;
    state->next_ip = 0;
}

void materialise_frames(R_deferred_state state, R_bytelist out, uint8_t* ip) {
    if (!stack_remove_frame_dependencies(state, out)) {
        materialise_state(state, out, ip);
    } else {
        flush_frame_stack(state->frame_stack, out);
    }
    materialise_current_frame(state, out);
}

uint8_t* materialise_on_exception(R_deferred_state state, R_set pinned) {
    void* ptr;
    uint8_t* start;
    R_bytelist exit_trace = make_bytelist(32);
    // Don't flush stack
    flush_frame_stack(state->frame_stack, exit_trace);
    flush_current_frame(state, exit_trace);
    assert(exit_trace->size != 0 && "Shouldn't call this if there is nothing to materialise");
    bytelist_append_nolock(exit_trace, OPCODE(reraise));
    ptr = gvmt_pin((GVMT_Object)exit_trace->array);
    start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    SET_TYPE(exit_trace->array, type_bytes);
    PY_set_add((R_object)exit_trace->array, pinned);
    return start;
}

void* materialise_state_on_exit(R_deferred_state state, R_exec_link exit, R_set pinned) {
    R_bytelist exit_trace = make_bytelist(32);
    void* exit_addr = gvmt_pin((GVMT_Object)exit);
    TYPE_ASSERT(exit, exec_link);
    //fprintf(stderr, "materialise_state_on_exit for exit 0x%x\n", (intptr_t)gvmt_pin((GVMT_Object)exit));
    flush_state(state, exit_trace);
    if (exit_trace->size != 0) {
        void* ptr;
        uint8_t* start;
        bytelist_append_nolock(exit_trace, OPCODE(trace_exit));
        bytelist_append_long(exit_trace, (int32_t)exit_addr);
        exit_trace = peephole_optimise(exit_trace);
        ptr = gvmt_pin((GVMT_Object)exit_trace->array);
        start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
        SET_TYPE(exit_trace->array, type_bytes);
        PY_set_add((R_object)exit_trace->array, pinned);
        return make_interpret_exit(start, pinned);
    }
    return exit_addr;
}

uint8_t*
materialise_state_on_interpreter_exit(R_deferred_state state, uint8_t*  exit, R_set pinned) {
    R_bytelist exit_trace = make_bytelist(32);
    flush_state(state, exit_trace);
    if (exit_trace->size != 0) {
        void* ptr;
        uint8_t* start;
        bytelist_append_nolock(exit_trace, OPCODE(interpret));
        bytelist_append_long(exit_trace, (int32_t)exit);
        ptr = gvmt_pin((GVMT_Object)exit_trace->array);
        start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
        SET_TYPE(exit_trace->array, type_bytes);
        PY_set_add((R_object)exit_trace->array, pinned);
        return start;
    }
    return exit;
}

void deferred_pop_immediate(R_deferred_state state) {
    if (LIST_SIZE(state->deferred_stack)) {
        R_object o = list_pop(state->deferred_stack);
        assert(o == NULL);
    }
}

void deferred_push_immediate(R_deferred_state state) {
    if (LIST_SIZE(state->deferred_stack)) {
        list_append(state->deferred_stack, NULL);
    }
}

int is_tos_deferred(R_deferred_state state) {
    return LIST_SIZE(state->deferred_stack) != 0 &&
    ITEM(state->deferred_stack->array, LIST_SIZE(state->deferred_stack)-1) != NULL;
}

void print_deferred_stack(R_deferred_state state, FILE* out) {
    int i;
    for(i = 0; i < LIST_SIZE(state->deferred_stack); i++) {
        print_deferred((R_deferred)ITEM(state->deferred_stack->array, i), out);
    }
    fprintf(out, ".\n");
}

int deferred_is_constant(R_deferred d, R_object c) {
    if (d->kind == KIND_CONST) {
        return d->value == c;
    }
    return 0;
}
