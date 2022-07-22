#include <assert.h>
#include "hotpy.h"
#include "operators.h"
#include "specialiser.h"
#include "deferred_object.h"

#define INFO_INITIALISED 1

#define INFO_TYPE 2
#define INFO_DEFINITE 4
#define INFO_CONSTANT 8
#define INFO_TUPLE 16

#define INFO_BOUND_METHOD 64
#define INFO_SLICE 128
#define INFO_NOT_TYPE 256
#define INFO_NOT_TAGGED 512
#define INFO_PROBABLE_TAGGED 1024
#define INFO_DEFINITE_TAGGED 2048

#define NOT_FROZEN(x) assert(TYPE(x) != type_frozen_type_info)

R_object PY_FUNC2(type_info_eq, R_type_info ti, R_object o) {
    if (is_subclass_of(TYPE(o), type_type_info)) {
        R_type_info other = (R_type_info)o;
        if (ti->kind == other->kind) {
            if (ti->value == NULL) {
                return (R_object)booleans[other->value == NULL];
            } else if (other->value == NULL) {
                return (R_object)FALSE;
            } else {
                return binary_call(operator_eq, ti->value, other->value);
            }
        } else {
            return (R_object)FALSE;
        }
    } else {
        return (R_object)FALSE;
    }
}

intptr_t hotpy_frozen_type_info_hash(R_type_info ti) {
    if (ti->value) {
        return ti->kind * 63517 + HotPyObject_Hash(ti->value);
    } else {
         return ti->kind * 63517;
    }
}

int is_initialised(R_type_info t) {
    return t->kind != 0;
}

int is_definite(R_type_info t) {
    return t->kind & INFO_DEFINITE;
}

int is_constant(R_type_info t) {
    return t->kind & INFO_CONSTANT;
}

int is_tagged(R_type_info t) {
    return t->kind & (INFO_PROBABLE_TAGGED | INFO_DEFINITE_TAGGED);
}

int is_probable_tagged(R_type_info t) {
    return t->kind & INFO_PROBABLE_TAGGED;
}

int is_definite_tagged(R_type_info t) {
    return t->kind & INFO_DEFINITE_TAGGED;
}

int is_slice(R_type_info t)  {
    return t->kind & INFO_SLICE;
}

int type_info_contadictory(R_type_info t1, R_type_info t2) {
    R_type type1 = get_type_from_info(t1);
    R_type type2 = get_type_from_info(t2);
    if ((t2->kind & INFO_CONSTANT) && ( t1->kind & INFO_CONSTANT)) {
        return t1->value != t2->value;
    }
    if (is_definite(t1)) {
        if (is_definite(t2)) {
            return type1 != type2;
        } else if (t2->kind & INFO_NOT_TYPE) {
            return type1 == (R_type)t2->value;
        }
        return 0;
    } else if (t1->kind & INFO_NOT_TYPE) {
        if (is_definite(t2)) {
            return type2 == (R_type)t1->value;
        }
        return 0;
    }
    return 0;
}

/** Return 1 if t1 is a subtype of t2 */
int type_info_is_wider(R_type_info t1, R_type_info t2) {
    R_type type1 = get_type_from_info(t1);
    R_type type2 = get_type_from_info(t2);
    if ((t2->kind & INFO_CONSTANT)) {
        if ((t1->kind & INFO_CONSTANT)) {
            return 0;
        } else if (is_definite(t1)) {
            if (is_subclass_of(TYPE(t2->value), type1)) {
                return 1;
            }
        }
        return 0;
    }
    if (!is_initialised(t1) || !is_initialised(t2)) {
        return 0;
    }
    if (is_definite(t2)) {
        if (is_definite(t1)) {
            if (is_definite_tagged(t2)) {
                if (t1->kind & INFO_NOT_TAGGED) {
                    return 0;
                }
                return type1 == type_int && !is_definite_tagged(t1);
            } else {
                return 0;
            }
        } else if (t1->kind & INFO_NOT_TYPE) {
            return type2 != (R_type)t1->value;
        } else {
            return 1;
        }
    }
    return 0;
}

R_type get_type_from_info(R_type_info info) {
    R_type t;
    if (info->kind & INFO_TUPLE) {
        return type_tuple;
    } else if (info->kind & INFO_SLICE) {
        t = type_slice;
    } else if (info->kind & INFO_BOUND_METHOD) {
        t = type_bound_method;
    } else if (info->kind & INFO_TYPE) {
        t = (R_type)info->value;
    } else if (info->kind & INFO_CONSTANT) {
        t = TYPE(info->value);
    } else if (info->kind == INFO_DEFINITE_TAGGED) {
        assert("Definite tagged values, should have type set to int");
        t = NULL;
    } else {
        t = NULL;
    }
    return t;
}

/** Writes very abbreviated type-info to buf.
 *  Returns number of chars written */
static int write_type_abbrv(char* out, R_type_info info) {
    if (!is_initialised(info)) {
        return sprintf(out, "!");
    }
    if (is_definite_tagged(info)) {
        return sprintf(out, "T");
    } else if (info->kind == INFO_NOT_TAGGED) {
        return sprintf(out, "~T");
    } else {
        R_type t = get_type_from_info(info);
        if (t) {
            if (info->kind & INFO_PROBABLE_TAGGED) {
                return sprintf(out, "t%d", t->unique_index);
            } else {
                return sprintf(out, "%d", t->unique_index);
            }
        } else if (info->kind == INFO_NOT_TYPE) {
            return sprintf(out, "~%d", ((R_type)info->value)->unique_index);
        } else {
            if (info->kind & INFO_PROBABLE_TAGGED) {
                return sprintf(out, "t");
            } else {
                assert(info->kind == 1 && info->value == NULL);
                return sprintf(out, "?");
            }
        }
    }
}

void print_type_info(FILE* out, R_type_info ti) {
    if (ti == NULL) {
        fprintf(out, "NULL");
    } else if (TYPE(ti) != type_type_info && TYPE(ti) != type_frozen_type_info) {
        fprintf(out, "---");
    } else {
        if (ti->kind & INFO_DEFINITE_TAGGED) {
            fprintf(out, "tagged ");
        } else if (ti->kind & INFO_PROBABLE_TAGGED) {
            fprintf(out, "tagged? ");
        }
        if (ti->kind & (INFO_TUPLE)) {
            int i;
            R_tuple t = (R_tuple)ti->value;
            fprintf(out, "(");
            for (i = 0; i < LEN(t); i++) {
                 print_type_info(out, (R_type_info)ITEM(t, i));
                 fprintf(out, ",");
            }
            fprintf(out, ")");
        } else if (ti->kind & (INFO_SLICE)) {
            int i;
            R_tuple t = (R_tuple)ti->value;
            fprintf(out, "slice(");
            for (i = 0; i < LEN(t); i++) {
                 print_type_info(out, (R_type_info)ITEM(t, i));
                 fprintf(out, ",");
            }
            fprintf(out, ")");
        } else if (ti->kind & INFO_CONSTANT) {
            print_short(ti->value);
        } else if (ti->kind & INFO_BOUND_METHOD) {
            print_type_info(out, bound_method_info_get_object(ti));
            fprintf(out, ".");
            print_type_info(out, bound_method_info_get_callable(ti));
        } else if (ti->kind & INFO_TYPE) {
            if ((ti->kind & INFO_DEFINITE))
                fprintf(out, "=");
            else
                fprintf(out, "~");
            print_short(ti->value);
        } else if (ti->kind == INFO_NOT_TAGGED) {
            fprintf(out, "!tagged");
        } else if (ti->kind == INFO_NOT_TYPE) {
            fprintf(out, "!");
            print_short(ti->value);
        } else {
            fprintf(out, "???");
        }
    }
}

static R_type_info make_type_info(int kind, R_object value) {
    R_type_info info = gc_allocate(type_info);
    SET_TYPE(info, type_type_info);
    info->kind = kind;
    info->value = value;
    return info;
}

static R_type_info make_frozen_type_info(int kind, R_object value) {
    R_type_info info = gc_allocate(type_info);
    SET_TYPE(info, type_frozen_type_info);
    info->kind = kind;
    info->value = value;
    return info;
}

R_type_info info_uninitialised(void) {
    return make_type_info(0, NULL);
}

R_type_info info_not_known(void) {
    return make_type_info(INFO_INITIALISED, NULL);
}

//R_type_info probable_tagged(void) {
//     return make_type_info(INFO_PROBABLE_TAGGED, NULL);
//}

R_type_info definite_tagged(void) {
     return make_type_info(INFO_TYPE | INFO_DEFINITE | INFO_DEFINITE_TAGGED, (R_object)type_int);
}

static R_type_info info_not_type(R_type_info info) {
    if (is_definite_tagged(info)) {
        return make_frozen_type_info(INFO_NOT_TAGGED, NULL);
    } else if (info->kind == INFO_NOT_TAGGED) {
        return definite_tagged();
    } else if (info->kind == INFO_NOT_TYPE) {
        return definite_type((R_type)info->value);
    } else {
        R_type t = get_type_from_info(info);
        if (t) {
            return make_frozen_type_info(INFO_NOT_TYPE, (R_object)t);
        } else {
            return info_not_known();
        }
    }
}

R_type_info probable_type(R_type type) {
    return make_type_info(INFO_TYPE, (R_object)type);
}

R_type_info definite_type(R_type type) {
    return make_type_info(INFO_TYPE | INFO_DEFINITE, (R_object)type);
}

R_type_info type_info_constant(R_object obj) {
    if (gvmt_is_tagged(obj)) {
        return make_type_info(INFO_CONSTANT | INFO_DEFINITE | INFO_DEFINITE_TAGGED, obj);
    } else {
        return make_type_info(INFO_CONSTANT | INFO_DEFINITE | INFO_NOT_TAGGED, obj);
    }
}

void type_info_make_constant(R_type_info info, R_object obj) {
    NOT_FROZEN(info);
    if (gvmt_is_tagged(obj)) {
        info->kind = INFO_CONSTANT | INFO_DEFINITE | INFO_DEFINITE_TAGGED;
    } else {
        info->kind = INFO_CONSTANT | INFO_DEFINITE | INFO_NOT_TAGGED;
    }
    info->value = obj;
}

void type_info_make_variable(R_type_info info) {
    if (info->kind & INFO_CONSTANT) {
        info->kind &= ~INFO_CONSTANT;
        info->kind |= INFO_TYPE;
        info->value = (R_object)TYPE(info->value);
    }
}

R_type_info type_info_copy(R_type_info info) {
    return make_type_info(info->kind, info->value);
}

void make_type_definite(R_type_info info) {
    NOT_FROZEN(info);
    if (info->kind & INFO_TYPE) {
        info->kind |= INFO_DEFINITE;
    }
    if (info->kind & INFO_PROBABLE_TAGGED) {
        info->kind &= ~INFO_PROBABLE_TAGGED;
        if (info->value == NULL || info->value == (R_object)type_int) {
            info->value = (R_object)type_int;
            info->kind |= INFO_DEFINITE_TAGGED | INFO_TYPE | INFO_DEFINITE;
        }
    }
    assert(!is_probable_tagged(info));
    assert(is_definite(info));
}

R_type_info info_bound_method(R_type_info object, R_type_info callable) {
    return make_type_info(INFO_BOUND_METHOD | INFO_DEFINITE,
            (R_object)make_tuple2((R_object)object, (R_object)callable));
}

R_type_info info_slice(R_type_info s1, R_type_info s2, R_type_info s3) {
    R_tuple t = make_tuple(3);
    ITEM(t, 0) = (R_object)s1;
    ITEM(t, 1) = (R_object)s2;
    ITEM(t, 2) = (R_object)s3;
    return make_type_info(INFO_SLICE | INFO_DEFINITE, (R_object)t);
}

int with_observed_type(R_type_info info, R_type observed) {
    R_type t;
    NOT_FROZEN(info);
    if (info->kind & INFO_TUPLE) {
        return observed == type_tuple;
    } else if (info->kind & INFO_CONSTANT) {
        t = TYPE(info->value);
    } else if (info->kind & INFO_TYPE) {
        t = (R_type)info->value;
    } else if (info->kind & INFO_BOUND_METHOD) {
        t = type_bound_method;
    } else if (info->kind & INFO_SLICE) {
        t = type_slice;
    } else if (info->kind == INFO_NOT_TYPE && observed == (R_type)info->value) {
        // Observed type is of type we know it NOT to be. Is this possible?
        return 0;
    } else {
        assert((info->kind & INFO_DEFINITE_TAGGED) == 0);
        t = NULL;
    }
    if (t) {
        if ((info->kind & INFO_DEFINITE) == 0)
            info->value = (R_object)observed;
        return t == observed;
    } else {
        info->kind = INFO_TYPE;
        info->value = (R_object)observed;
        return 1;
    }
}

int with_observed_value(R_type_info info, R_object observed) {
    int result;
    if (info->kind == INFO_NOT_TAGGED && gvmt_is_tagged(observed)) {
        return 0;
    }
    result = with_observed_type(info, TYPE(observed));
    if (result && (info->kind & INFO_DEFINITE_TAGGED) == 0) {
        if (gvmt_is_tagged(observed)) {
            info->kind |= INFO_PROBABLE_TAGGED;
        } else {
            info->kind &= (~INFO_PROBABLE_TAGGED);
        }
    }
    return result;
}

R_array thaw_frame(R_tuple frame) {
    int i;
    R_array thawed = create_array(LEN(frame)-1);
    for (i = 0; i < LEN(frame)-1; i++) {
        R_type_info info = (R_type_info)ITEM(frame, i);
        ITEM(thawed, i) = (R_object)make_type_info(info->kind, info->value);
    }
    return thawed;
}

R_tuple freeze_frame(R_array frame, uint8_t* ip) {
    int i;
    R_tuple frozen = make_tuple(LEN(frame)+1);
    for (i = 0; i < LEN(frame); i++) {
        R_type_info info = (R_type_info)ITEM(frame, i);
        TYPE_ASSERT(info, type_info);
        ITEM(frozen, i) = (R_object)make_frozen_type_info(info->kind, info->value);
    }
    ITEM(frozen, LEN(frame)) = (R_object)R_int_from_ssize_t((intptr_t)ip);
    return frozen;
}

R_object PY_FUNC2(type_state_eq, R_type_state ts, R_object o) {
    if (is_subclass_of(TYPE(o), type_type_state)) {
        R_type_state other = (R_type_state)o;
        int t;
        t = tuple_equals(ts->frame, other->frame) &&
            tuple_equals(ts->back, other->back);
        return (R_object)booleans[t];
    } else {
        return (R_object)FALSE;
    }
}

intptr_t hotpy_type_state_hash(R_type_state ts) {
    if (ts->hashcode == 0)
        ts->hashcode = hotpy_tuple_hash(ts->frame) ^ hotpy_tuple_hash(ts->back);
    return ts->hashcode;
}

#define BACK_TRACE_LIMIT 1

R_type_state make_type_state(R_array frame, R_tuple back, R_type_info except) {
    int i;
    R_type_state state;
    R_tuple frozen = freeze_frame(frame, NULL);
    R_type_info not_except = NULL;
    assert(back != NULL);
    if (except)
        not_except = info_not_type(except);
    if (except != NULL) {
        for (i = 0; i < LEN(frame); i++) {
            if ((R_type_info)ITEM(frame, i) == except) {
                ITEM(frozen, i) = (R_object)not_except;
            }
        }
    }
    state = gc_allocate(type_state);
    SET_TYPE(state, type_type_state);
    state->frame = frozen;
    state->back = back;
    return state;
}


void print_type_state(R_type_state state, FILE* out) {
    int i;
    fprintf(out, "(");
    for (i = 0; i < LEN(state->frame)-1; i++) {
        if (i) fprintf(out, ", ");
        print_type_info(out, (R_type_info)ITEM(state->frame, i));
    }
    fprintf(out, ") ");
}

char* print_type_state_flow(R_type_state state, char* out) {
    int i, ip;
    R_type t;
    if (LEN(state->frame)>1)
        out += write_type_abbrv(out, (R_type_info)ITEM(state->frame, 0));
    for (i = 1 ; i < LEN(state->frame)-1; i++) {
        out += sprintf(out, ", ");
        out += write_type_abbrv(out, (R_type_info)ITEM(state->frame, i));
    }
    out += sprintf(out, "\\n #%u\\n", HotPyObject_Hash((R_object)state));
    ip = int_from_py_int((R_int)ITEM(state->frame, i));
    if (ip) {
        out += sprintf(out, "\\n@ 0x%x\\n", ip);
    } else {
        out += sprintf(out, "\\n");
    }
    return out;
}

R_type_info pack_info(R_tuple t) {
    int i;
    for (i = 0; i < LEN(t); i++) {
        R_type_info item = (R_type_info)ITEM(t, i);
        assert(item);
        TYPE_ASSERT(item, type_info);
    }
    return make_type_info(INFO_DEFINITE | INFO_TUPLE, (R_object)t);
}

void unpack_info(R_type_info info, int n, GVMT_Value* array) {
    int i;
    if (info->kind & INFO_TUPLE) {
        TYPE_ASSERT(info->value, tuple);
        for (i = 0; i < n; i++) {
            R_type_info item = (R_type_info)ITEM((R_tuple)info->value, i);
            TYPE_ASSERT(item, type_info);
            gvmt_stack_write_object(array+i, (GVMT_Object)item);
        }
    } else {
        for (i = 0; i < n; i++) {
            gvmt_stack_write_object(array+i, (GVMT_Object)info_not_known());
        }
    }
}

R_type_info tuple_prepend_info(R_type_info t1, R_type_info t2) {
    if (t2->kind & INFO_TUPLE) {
       TYPE_ASSERT(t2->value, tuple);
       return make_type_info(INFO_DEFINITE | INFO_TUPLE,
                (R_object)tuple_prepend((R_object)t1, (R_tuple)t2->value));
    } else {
        return info_not_known();
    }
}

R_type_info tuple_get_item_info(R_type_info t, int index) {
    assert(t->kind & INFO_TUPLE);
    assert(LEN((R_tuple)t->value) > index);
    return (R_type_info)ITEM((R_tuple)t->value, index);
}

R_type_info tuple_concat_info(R_type_info t1, R_type_info t2) {
    if (t1->kind & t2->kind & INFO_TUPLE) {
       TYPE_ASSERT(t1->value, tuple);
       TYPE_ASSERT(t2->value, tuple);
       return make_type_info(INFO_DEFINITE | INFO_TUPLE,
           (R_object)tuple_concat((R_tuple)t1->value, (R_tuple)t2->value));
    } else {
        return definite_type(type_tuple);
    }
}

int info_tuple_size(R_type_info t) {
    if (t->kind & INFO_TUPLE) {
       TYPE_ASSERT(t->value, tuple);
       return LEN((R_tuple)t->value);
    }
    return -1;
}

void info_frame_init_from_tuple(R_array frame, R_function func, R_type_info t) {
    int i;
    R_tuple tuple = (R_tuple)t->value;
    for (i = 0; i < LEN(frame); i++) {
        ITEM(frame, i) = (R_object)info_not_known();
    }
    if (func && (t->kind & INFO_TUPLE)) {
        int kind = func->parameter_format & TUPLE_DICT_FUNCTION;
        int pcount = LEN(func->paramNames);
        TYPE_ASSERT(tuple, tuple);
        if (kind & DICT_FUNCTION) {
            pcount--;
        }
        if (kind & TUPLE_FUNCTION) {
            pcount--;
        }
        for (i = 0; i < LEN(tuple) && i < pcount; i++) {
            R_object item = ITEM(tuple, i);
            TYPE_ASSERT(item, type_info);
            ITEM(frame, i) = item;
        }
        if (kind & TUPLE_FUNCTION) {
            int star_args;
            star_args = LEN(tuple) - pcount;
            if (star_args > 0) {
                ITEM(frame, pcount) = (R_object)pack_info(tuple_slice(tuple, pcount, LEN(tuple)));
            } else {
                ITEM(frame, pcount) = (R_object)pack_info(empty_tuple);
            }
            pcount++;
        }
        if (kind & DICT_FUNCTION) {
            ITEM(frame, pcount) = (R_object)definite_type(type_dict);
        }
//    } else {
//        fprintf(stderr, "Not a tuple");
    }
}

R_type_info bound_method_info_get_object(R_type_info t) {
    if (t->kind & INFO_BOUND_METHOD) {
        return (R_type_info)ITEM((R_tuple)t->value, 0);
    } else {
        return info_not_known();
    }
}

R_type_info bound_method_info_get_callable(R_type_info t) {
    if (t->kind & INFO_BOUND_METHOD) {
        return (R_type_info)ITEM((R_tuple)t->value, 1);
    } else {
        return info_not_known();
    }
}

