#include <stdlib.h>
#include <assert.h>
#include "hotpy.h"
#include "opcodes.h"
#include "operators.h"
#include "optimise.h"
#include "specialiser.h"
#include "guard.h"

#define I_COMP_SIGNAL 217

R_specialiser make_specialiser(R_trace_record rec, R_bytelist bytes) {
    char* ptr;
    R_specialiser s;
    R_trace t = gc_allocate(trace);
    SET_TYPE(t, type_trace);
    t->pinned_objects = rec->pinned_objects;
    s = gc_allocate(specialiser);
    SET_TYPE(s, type_specialiser);
    s->out = bytes;
    s->guards = new_dict(16);
    s->trace = t;
    ptr = (char*)gvmt_pin((GVMT_Object)t);
    ptr += offsetof(GVMT_OBJECT(trace), invalidated);
    s->invalidated_address = (int32_t*)ptr;
    // Test
    s->invalidated_address[0] = 0;
    s->uninitialised_loads = make_bytearray(rec->initial_code_object->locals);
    s->back = make_list(4);
    s->back_ips = make_list(4);
    return s;
}

int ensure_type_tos(R_type_info info, uint8_t* ip, R_specialiser spec) {
    void* exit, *addr;
    if (is_constant(info)) {
        return 1;
    }
    if (!is_definite(info)) {
        R_type t = get_type_from_info(info);
        if (spec->out->size == 0) {
            return 0;
        }
        make_type_definite(info);
        assert(t);
        if (is_tagged(info)) {
            assert(t == type_int);
            exit = make_mono_exit(ip, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, info));
            bytelist_append_nolock(spec->out, OPCODE(ensure_tagged));
        } else {
            exit = make_mono_exit(ip, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, info));
            bytelist_append_nolock(spec->out, OPCODE(type_ensure));
            addr = gvmt_pin((GVMT_Object)t);
            bytelist_append_long(spec->out, (int32_t)addr);
        }
        bytelist_append_long(spec->out, (int32_t)exit);
    } else {
        if ((get_type_from_info(info) == type_int) && is_probable_tagged(info) &&
            !is_constant(info)) {
            if (spec->out->size == 0) {
                return 0;
            }
            exit = make_mono_exit(ip, spec->trace->pinned_objects,
                         make_type_state(spec->frame, spec->frozen_back, info));
            bytelist_append_nolock(spec->out, OPCODE(ensure_tagged));
            bytelist_append_long(spec->out, (int32_t)exit);
        }
    }
    return 1;
}

void ensure_type_drop(R_specialiser spec, uint8_t* ip, R_type_info wanted) {
    void* exit, *addr;
    exit = make_mono_exit(ip, spec->trace->pinned_objects,
                 make_type_state(spec->frame, spec->frozen_back, wanted));
    if (is_tagged(wanted)) {
        bytelist_append_nolock(spec->out, OPCODE(ensure_tagged_drop));
        bytelist_append_long(spec->out, (int32_t)exit);
    } else {
        bytelist_append_nolock(spec->out, OPCODE(type_ensure_drop));
        addr = gvmt_pin((GVMT_Object)get_type_from_info(wanted));
        bytelist_append_long(spec->out, (int32_t)addr);
        bytelist_append_long(spec->out, (int32_t)exit);
    }
}

void ensure_local_type(R_specialiser spec, uint8_t* ip,
                       int index, R_type_info wanted) {
    bytelist_append_nolock(spec->out, OPCODE(fast_load_frame));
    bytelist_append_nolock(spec->out, index);
    ensure_type_drop(spec, ip, wanted);
}

int ensure_type_nos(R_type_info info, uint8_t* ip, R_specialiser spec) {
    void* exit, *addr;
    if (is_constant(info)) {
        return 1;
    }
    if (!is_definite(info)) {
        R_type t = get_type_from_info(info);
        if (spec->out->size == 0) {
            return 0;
        }
        make_type_definite(info);
        assert(t);
        if (is_tagged(info)) {
            exit = make_mono_exit(ip, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, info));
            bytelist_append_nolock(spec->out, OPCODE(ensure_tagged2));
        } else {
            exit = make_mono_exit(ip, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, info));
            bytelist_append_nolock(spec->out, OPCODE(type_ensure2));
            addr = gvmt_pin((GVMT_Object)t);
            bytelist_append_long(spec->out, (int32_t)addr);
        }
        bytelist_append_long(spec->out, (int32_t)exit);
    } else {
        if ((get_type_from_info(info) == type_int) && is_probable_tagged(info)) {
            if (spec->out->size == 0) {
                return 0;
            }
            make_type_definite(info);
            exit = make_mono_exit(ip, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, info));
            bytelist_append_nolock(spec->out, OPCODE(ensure_tagged2));
            bytelist_append_long(spec->out, (int32_t)exit);
        }
    }
    return 1;
}

int ensure_class_attribute(R_specialiser spec, R_type t,
                           R_str name, R_object expected) {
    R_tuple key = make_tuple2((R_object)t, (R_object)name);
    R_object attr = dict_get(spec->guards, (R_object)key);
    if (attr == NULL) {
        if (add_guard_for_type_attribute(t, name, spec->trace, expected)) {
            dict_set(spec->guards, (R_object)key, expected);
            return 1;
        } else {
            return 0;
        }
    } else {
        return attr == expected;
    }
}

R_type_info specialise_load_slot(R_type_info desc, R_type_info obj, R_specialiser spec) {
    if (is_constant(desc) && TYPE(desc->value) == type_slot_descriptor) {
        // Must verify type -- Fix make_base.py to initialise type field.
        int offset = ((R_slot_descriptor)desc->value)->offset;
        if (offset == 0 && is_definite(obj)) {
            R_type type = get_type_from_info(obj);
            void* addr = gvmt_pin((GVMT_Object)type);
            bytelist_append_nolock(spec->out, OPCODE(drop));
            bytelist_append_nolock(spec->out, OPCODE(drop));
            bytelist_append_nolock(spec->out, OPCODE(fast_constant));
            bytelist_append_long(spec->out, (int32_t)addr);
            return type_info_constant((R_object)type);
        } else {
            bytelist_append_nolock(spec->out, OPCODE(drop_under));
            bytelist_append_nolock(spec->out, OPCODE(load_slot_imm));
            bytelist_append_nolock(spec->out, offset);
        }
    } else {
        bytelist_append_nolock(spec->out, OPCODE(load_slot));
    }
    return info_not_known();
}

void specialise_store_slot(R_type_info desc, R_type_info obj, R_specialiser spec) {
    if (is_constant(desc) && TYPE(desc->value) == type_slot_descriptor) {
        // Must verify type -- Fix make_base.py to initialise type field.
        bytelist_append_nolock(spec->out, OPCODE(drop_under));
        bytelist_append_nolock(spec->out, OPCODE(store_slot_imm));
        bytelist_append_nolock(spec->out, ((R_slot_descriptor)desc->value)->offset);
    } else {
        bytelist_append_nolock(spec->out, OPCODE(store_slot));
    }
}

int ensure_new_and_init(R_specialiser spec, R_type cls, uintptr_t new_addr, uintptr_t init_addr) {
    R_object new = (R_object)gvmt_pinned_object((void*)new_addr);
    R_object init = (R_object)gvmt_pinned_object((void*)init_addr);
    return ensure_class_attribute(spec, cls, name___new__, new) &&
           ensure_class_attribute(spec, cls, name___init__, init);
}

int ensure_no_attribute(R_specialiser spec, R_type t, R_str name) {
    R_tuple key = make_tuple2((R_object)t, (R_object)name);
    R_object attr = dict_get(spec->guards, (R_object)key);
    if (attr == NULL) {
        if (add_guard_for_type_attribute(t, name, spec->trace, NULL)) {
            dict_set(spec->guards, (R_object)key, (R_object)None);
            return 1;
        } else {
            return 0;
        }
    } else {
        return attr == (R_object)None;
    }
}

R_type_info specialise_get_class_attr(R_type_info obj, R_type_info key,
                                R_specialiser spec) {
    if (is_constant(obj) && is_constant(key) &&
        TYPE(obj->value) == type_type && TYPE(key->value) == type_str) {
        R_type cls =  (R_type)obj->value;
        R_str name = (R_str)key->value;
        R_object o = class_attribute(cls, name);
        if (o == NULL) {
            if (ensure_no_attribute(spec, cls, name)) {
                bytelist_append_nolock(spec->out, OPCODE(drop));
                bytelist_append_nolock(spec->out, OPCODE(drop));
                return optimise_constant(o, spec->out);
            }
        } else {
            if (ensure_class_attribute(spec, cls, name, o)) {
                bytelist_append_nolock(spec->out, OPCODE(drop));
                bytelist_append_nolock(spec->out, OPCODE(drop));
                return optimise_constant(o, spec->out);
            }
        }
    }
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise get_class_attr\n");
    }
    bytelist_append_nolock(spec->out, OPCODE(get_class_attr));
    return info_not_known();
}

R_type_info specialise_has_class_attr(R_type_info obj, R_type_info key,
                                R_specialiser spec) {
    if (is_constant(obj) && is_constant(key) &&
        TYPE(obj->value) == type_type && TYPE(key->value) == type_str) {
        R_type cls =  (R_type)obj->value;
        R_str name = (R_str)key->value;
        R_object o = class_attribute(cls, name);
        if (o == NULL) {
            if (ensure_no_attribute(spec, cls, name)) {
                bytelist_append_nolock(spec->out, OPCODE(drop));
                bytelist_append_nolock(spec->out, OPCODE(drop));
                bytelist_append_nolock(spec->out, OPCODE(false));
                return type_info_constant((R_object)FALSE);
            }
        } else {
            if (ensure_class_attribute(spec, cls, name, o)) {
                bytelist_append_nolock(spec->out, OPCODE(drop));
                bytelist_append_nolock(spec->out, OPCODE(drop));
                bytelist_append_nolock(spec->out, OPCODE(true));
                return type_info_constant((R_object)TRUE);
            }
        }
    }
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise has_class_attr\n");
    }
    bytelist_append_nolock(spec->out, OPCODE(has_class_attr));
    return info_not_known();
}

static void failed(R_type_info i, R_type t) {
    fprintf(stderr, "Failed to match types:");
    print_type_info(stderr, i);
    fprintf(stderr, " and ");
    py_fprint(stderr, t->__name__);
    fprintf(stderr, "\n");
}

int match_expected_type_tos(R_type_info i, R_type_info nos, R_info_item2 ti,
                                R_specialiser spec) {
    if (with_observed_value(i, ti->value1)) {
        return ensure_type_tos(i, ti->ip, spec);
    } else if (debug_optimise) {
        failed(i, TYPE(ti->value1));
    }
    return 0;
}

int match_expected_type_nos(R_type_info i, R_info_item3 ti,
                                R_specialiser spec) {
    if (with_observed_value(i, ti->value1)) {
        return ensure_type_nos(i, ti->ip, spec);
    } else if (debug_optimise) {
        failed(i, TYPE(ti->value1));
    }
    return 0;
}

int match_expected_types_binary(R_type_info l, R_type_info r, R_info_item3 ti,
                                R_specialiser spec) {
    if (with_observed_value(l, ti->value1) && with_observed_value(r, ti->value2)) {
        return ensure_type_nos(l, ti->ip, spec) && ensure_type_tos(r, ti->ip, spec);
    } else if (debug_optimise) {
        if(!with_observed_value(l, ti->value1))
            failed(l, TYPE(ti->value1));
        if(!with_observed_value(r, ti->value2))
            failed(r, TYPE(ti->value2));
    }
    return 0;
}

static R_type_info binary_return_type(R_operator op, R_type l, R_type r) {
    R_type result;
    if (TYPE(op) == type_comparison_operator) {
        return definite_type(type_bool);
    }
    if (l == type_bool)
        result = r;
    else if (r == type_bool)
        result = l;
    else if (l == type_int)
        result = r;
    else if (r == type_int)
        result = l;
    else if (r == type_float)
        result = l;
    else if (l == type_float)
        result = l;
    else if (l == type_tuple)
        result = r;
    else if (l == type_tuple)
        result = l;
    else {
        assert (l == r);
        result = l;
    }
    if (op == operator_truediv && result == type_int) {
        result = type_float;
    }
    return definite_type(result);
}

static void
emit_native_call(int pcount, R_builtin_function bf, R_specialiser spec, uint8_t* next_ip) {
    if (bf->parameter_format & FUNCTION_NO_RAISE) {
        bytelist_append_nolock(spec->out, OPCODE(native_call_no_prot));
        bytelist_append_nolock(spec->out, pcount);
        bytelist_append_long(spec->out, (int32_t)bf->executable);
    } else if (bf->parameter_format & FUNCTION_PURE) {
        bytelist_append_nolock(spec->out, OPCODE(native_call_protect));
        bytelist_append_nolock(spec->out, pcount);
        bytelist_append_long(spec->out, (int32_t)bf->executable);
        bytelist_append_long(spec->out, 0);
    } else {
        bytelist_append_nolock(spec->out, OPCODE(native_call));
        bytelist_append_nolock(spec->out, pcount);
        bytelist_append_long(spec->out, (int32_t)bf->executable);
        insert_deoptimise_check(spec, next_ip);
    }
}

R_type_info
optimise_inplace_operator(int index, R_type_info l, R_type_info r,
                          R_specialiser spec, uint8_t* original_ip) {
    R_type t1 = get_type_from_info(l);
    R_type t2 = get_type_from_info(r);
    R_operator op = operators[index];
    R_object f = class_attribute(t1, op->func_name);
    assert(is_definite(l) && is_definite(r));
    if (f == NULL) {
        ensure_no_attribute(spec, t1, op->func_name);
        return optimise_binary_operator(index, l, r, spec, original_ip);
    } else {
        if (TYPE(f) == type_builtin_function) {
            R_builtin_function bf = (R_builtin_function)f;
            ensure_class_attribute(spec, t1, op->func_name, (R_object)f);
            emit_native_call(2, bf, spec, original_ip+OPCODE_LENGTH(inplace));
            if (bf->return_type) {
                return definite_type(bf->return_type);
            } else {
                return info_not_known();
            }
        } else {
            if (debug_optimise) {
                fprintf(stderr, "Failed to specialise inplace\n");
            }
        }
    }
    bytelist_append_nolock(spec->out, OPCODE(inplace));
    bytelist_append_nolock(spec->out, index);
    insert_deoptimise_check(spec, original_ip+OPCODE_LENGTH(inplace));
    return info_not_known();
}

R_type_info optimise_binary_operator(int index, R_type_info l, R_type_info r,
                                     R_specialiser spec, uint8_t* original_ip) {
    R_type t1 = get_type_from_info(l);
    R_type t2 = get_type_from_info(r);
    R_base_function f;
    R_operator op;
    assert(is_definite(l) && is_definite(r));
    if (is_definite_tagged(l) && is_definite_tagged(r)) {
        uint8_t opcode = 0;
        R_type rtype = NULL;
        if (index == OPERATOR_ADD_INDEX || index == OPERATOR_IADD_INDEX) {
            int op_len = index == OPERATOR_ADD_INDEX ?
                         OPCODE_LENGTH(binary) : OPCODE_LENGTH(inplace);
            void* exit;
            r = definite_tagged();
            if (spec->no_overflows & 1) {
                bytelist_append_nolock(spec->out, OPCODE(i_add_no_overflow));
            } else {
                exit = make_mono_exit(original_ip+op_len, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, r));
                bytelist_append_nolock(spec->out, OPCODE(i_add));
                bytelist_append_long(spec->out, (int32_t)exit);
            }
            return r;
        } else if (index == OPERATOR_SUB_INDEX || index == OPERATOR_ISUB_INDEX) {
            int op_len = index == OPERATOR_ADD_INDEX ?
                         OPCODE_LENGTH(binary) : OPCODE_LENGTH(inplace);
            void* exit;
            r = definite_tagged();
            if (spec->no_overflows & 1) {
                bytelist_append_nolock(spec->out, OPCODE(i_sub_no_overflow));
            } else {
                exit = make_mono_exit(original_ip+op_len, spec->trace->pinned_objects, make_type_state(spec->frame, spec->frozen_back, r));
                bytelist_append_nolock(spec->out, OPCODE(i_sub));
                bytelist_append_long(spec->out, (int32_t)exit);
            }
            return r;
        } else if (index == OPERATOR_MUL_INDEX || index == OPERATOR_IMUL_INDEX) {
            opcode = OPCODE(i_mul);
            rtype = type_int;
        } else if (index == OPERATOR_RSHIFT_INDEX || index == OPERATOR_IRSHIFT_INDEX) {
            if (is_constant(r)) {
                intptr_t op = ((intptr_t)(gvmt_untag(r->value)))>> 1;
                assert(gvmt_is_tagged(r->value));
                if (op == 0) {
                    bytelist_append_nolock(spec->out, OPCODE(drop));
                    return l;
                } else if (op > 0 && op < sizeof(void*)*8) {
                    opcode = OPCODE(i_rshift);
                    rtype = type_int;
                }
            }
        } else if (index == OPERATOR_TRUEDIV_INDEX || index == OPERATOR_ITRUEDIV_INDEX) {
            opcode = OPCODE(i_div);
            rtype = type_float;
        } else if (index == OPERATOR_FLOORDIV_INDEX || index == OPERATOR_IFLOORDIV_INDEX) {
            emit_native_call(2, function_tagged_floordiv, spec, original_ip+OPCODE_LENGTH(binary));
            return definite_type(type_int);
        } else if (index == OPERATOR_MOD_INDEX || index == OPERATOR_IMOD_INDEX) {
            emit_native_call(2, function_tagged_mod, spec, original_ip+OPCODE_LENGTH(binary));
            return definite_type(type_int);
        } else if (index == OPERATOR_LE_INDEX) {
            opcode = OPCODE(i_comp_le);
            rtype = type_bool;
        } else if (index == OPERATOR_LT_INDEX) {
            opcode = OPCODE(i_comp_lt);
            rtype = type_bool;
        } else if (index == OPERATOR_GT_INDEX) {
            opcode = OPCODE(i_comp_gt);
            rtype = type_bool;
        } else if ( index == OPERATOR_GE_INDEX) {
            opcode = OPCODE(i_comp_ge);
            rtype = type_bool;
        } else if (index == OPERATOR_EQ_INDEX) {
            opcode = OPCODE(i_comp_eq);
            rtype = type_bool;
        } else if (index == OPERATOR_NE_INDEX) {
            opcode = OPCODE(i_comp_ne);
            rtype = type_bool;
        }
        if (opcode != 0) {
            bytelist_append_nolock(spec->out, opcode);
            return definite_type(rtype);
        }
    }
    if ((t1 == type_float && t2 == type_float) ||
        (t1 == type_float && t2 == type_int  ) ||
        (t1 == type_int   && t2 == type_float)) {
        uint8_t opcode = 0;
        R_type rtype = NULL;
        if (index == OPERATOR_ADD_INDEX || index == OPERATOR_IADD_INDEX) {
            opcode = OPCODE(f_add);
            rtype = type_float;
        } else if (index == OPERATOR_SUB_INDEX || index == OPERATOR_ISUB_INDEX) {
            opcode = OPCODE(f_sub);
            rtype = type_float;
        } else if (index == OPERATOR_MUL_INDEX || index == OPERATOR_IMUL_INDEX) {
            opcode = OPCODE(f_mul);
            rtype = type_float;
        } else if (index == OPERATOR_TRUEDIV_INDEX || index == OPERATOR_ITRUEDIV_INDEX) {
            opcode = OPCODE(f_div);
            rtype = type_float;
        } else if (index == OPERATOR_LE_INDEX) {
            opcode = OPCODE(f_le);
            rtype = type_bool;
        } else if (index == OPERATOR_LT_INDEX) {
            opcode = OPCODE(f_lt);
            rtype = type_bool;
        } else if (index == OPERATOR_GT_INDEX) {
            opcode = OPCODE(f_gt);
            rtype = type_bool;
        } else if ( index == OPERATOR_GE_INDEX) {
            opcode = OPCODE(f_ge);
            rtype = type_bool;
        } else if (index == OPERATOR_EQ_INDEX) {
            opcode = OPCODE(f_eq);
            rtype = type_bool;
        } else if (index == OPERATOR_NE_INDEX) {
            opcode = OPCODE(f_ne);
            rtype = type_bool;
        }
        if (opcode) {
            if (t1 == type_int) {
                bytelist_append_nolock(spec->out, OPCODE(swap));
                bytelist_append_nolock(spec->out, OPCODE(i2f));
                bytelist_append_nolock(spec->out, OPCODE(swap));
            }
            if (t2 == type_int) {
                bytelist_append_nolock(spec->out, OPCODE(i2f));
            }
            bytelist_append_nolock(spec->out, opcode);
            return definite_type(rtype);
        }
    }
    op = operators[index];
    if (TYPE(op) == type_unary_operator) {
        op = (R_operator)op->fallback;
        assert(TYPE(op) == type_binary_operator);
    }
    f = binary_lookup_exact(op, t1, t2);
    if (f) {
        if (TYPE(f) == type_builtin_function) {
            R_builtin_function bf = (R_builtin_function)f;
            emit_native_call(2, bf, spec, original_ip+OPCODE_LENGTH(binary));
            if (bf->return_type) {
                return definite_type(bf->return_type);
            } else {
                return info_not_known();
            }
        } else {
            assert(TYPE(f) == type_function);
            if (debug_optimise) {
                fprintf(stderr, "Failed to specialise binary - Python function\n");
            }
        }
    } else if (debug_optimise) {
        fprintf(stderr, "Failed to specialise binary - no direct call\n");
    }
    bytelist_append_nolock(spec->out, OPCODE(binary));
    bytelist_append_nolock(spec->out, index);
    insert_deoptimise_check(spec, original_ip+OPCODE_LENGTH(binary));
    return info_not_known();
}


R_type_info optimise_unary_operator(int index, R_type_info i,
                                    R_specialiser spec, uint8_t* original_ip) {
    R_type t = get_type_from_info(i);
    R_operator op = operators[index];
    R_object o = class_attribute(t, op->func_name);
    assert(is_definite(i));
    if (TYPE(o) == type_builtin_function) {
        if (ensure_class_attribute(spec, t, op->func_name, o)) {
            R_builtin_function bf = (R_builtin_function)o;
            emit_native_call(1, bf, spec, original_ip+OPCODE_LENGTH(unary));
            if (bf->return_type) {
                return definite_type(bf->return_type);
            } else {
                return info_not_known();
            }
        }
    }
    bytelist_append_nolock(spec->out, OPCODE(unary));
    bytelist_append_nolock(spec->out, index);
    insert_deoptimise_check(spec, original_ip+OPCODE_LENGTH(unary));
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise unary\n");
    }
    return info_not_known();
}

R_type_info optimise_is(R_type_info l, R_type_info r, R_bytelist out) {
    R_type t1 = get_type_from_info(l);
    R_type t2 = get_type_from_info(l);
    if (t1 != t2) {
        // Both t1 and t2 are different.
        // Quite likely that one is a constant, other is definite type.
        // For example (x is None) where x is already has known type.
        if (is_definite(l) && is_definite(r)) {
            bytelist_append_nolock(out, OPCODE(drop));
            bytelist_append_nolock(out, OPCODE(drop));
            bytelist_append_nolock(out, OPCODE(false));
            return type_info_constant((R_object)FALSE);
        }
    }
    // Not worth specialising, since "is" is cheaper than a type test.
    bytelist_append_nolock(out, OPCODE(is));
    return definite_type(type_bool);
}

R_type_info optimise_constant(R_object value, R_bytelist out) {
    void* addr;
    if (gvmt_is_tagged(value)) {
        int i;
        addr = gvmt_untag(value);
        i = ((intptr_t)addr) >> 1;
        if (-128 <= i && i < 128) {
            bytelist_append_nolock(out, OPCODE(byte));
            bytelist_append_nolock(out, i);
        } else {
            bytelist_append_nolock(out, OPCODE(fast_constant));
            bytelist_append_long(out, (int32_t)addr);
        }
    } else {
        addr = gvmt_pin((GVMT_Object)value);
        bytelist_append_nolock(out, OPCODE(fast_constant));
        bytelist_append_long(out, (int32_t)addr);
    }
    return type_info_constant(value);
}

void ensure_keys(R_specialiser spec, R_type_info info, R_dict_keys keys, uint8_t* original_ip) {
    if (info->keys != keys) {
        R_type t = get_type_from_info(info);
        void* keys_addr = gvmt_pin((GVMT_Object)t->keys_cache);
        PY_set_add((R_object)t->keys_cache, spec->trace->pinned_objects);
        bytelist_append_nolock(spec->out, OPCODE(check_keys));
        bytelist_append_short(spec->out, t->dict_offset);
        bytelist_append_long(spec->out, (int32_t)keys_addr);
        bytelist_append_long(spec->out, (int32_t)original_ip);
        info->keys = keys;
    }
}

static void ensure_value(R_object value, R_specialiser spec,
                         uint8_t* original_ip) {
    // Emit ensure value for observed type.
    void *addr = gvmt_pin((GVMT_Object)value);
    void *exit = make_mono_exit(original_ip, spec->trace->pinned_objects,
        make_type_state(spec->frame, spec->frozen_back, NULL));
    bytelist_append_nolock(spec->out, OPCODE(ensure_value));
    bytelist_append_nolock(spec->out, 0);
    bytelist_append_long(spec->out, (intptr_t)addr);
    bytelist_append_long(spec->out, (intptr_t)exit);
}

static R_type_info specialise_hasattr(R_type_info t, R_type_info d,
                                      R_specialiser spec, uint8_t* ip) {
    int len = info_tuple_size(t);
    if (len == 2 && d == NULL) {
        R_type_info ti = tuple_get_item_info(t, 0);
        R_type type = get_type_from_info(ti);
        R_type_info name_info = tuple_get_item_info(t, 1);
        if (is_constant(name_info)) {
            R_str name = (R_str)name_info->value;
            if (TYPE(name) == type_str && type == type_type) {
                R_object attr;
                if (!is_constant(ti)) {
                    ensure_value(ti->value, spec, ip);
                }
                type = (R_type)ti->value;
                attr = class_attribute(type, name);
                if (attr) {
                    if (ensure_class_attribute(spec, type, name, attr)) {
                        // Discard f, t & d
                        bytelist_append_nolock(spec->out, OPCODE(drop));
                        bytelist_append_nolock(spec->out, OPCODE(drop));
                        bytelist_append_nolock(spec->out, OPCODE(drop));
                        bytelist_append_nolock(spec->out, OPCODE(true));
                        return definite_type(type_bool);
                    }
                    if (debug_optimise)
                        fprintf(stderr, "Cannot ensure attribute");
                } else {
                    if (ensure_no_attribute(spec, type, name)) {
                        // Discard f, t & d
                        bytelist_append_nolock(spec->out, OPCODE(drop));
                        bytelist_append_nolock(spec->out, OPCODE(drop));
                        bytelist_append_nolock(spec->out, OPCODE(drop));
                        bytelist_append_nolock(spec->out, OPCODE(false));
                        return definite_type(type_bool);
                    }
                    if (debug_optimise)
                        fprintf(stderr, "Cannot ensure no attribute");
                }
            } else {
                 if (debug_optimise)
                     fprintf(stderr, "Wrong types\n");
            }
        } else {
            if (debug_optimise)
                fprintf(stderr, "Name not a constant\n");
        }
    }
    // Not enough info
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise f_call for hasattr\n");
    }
    bytelist_append_nolock(spec->out, OPCODE(f_call));
    insert_deoptimise_check(spec, ip + OPCODE_LENGTH(f_call));
    return definite_type(type_bool);
}

static void specialise_native_parameters(R_builtin_function bf, R_type_info t,
                                       R_type_info d, R_bytelist out) {
    int i, match, len;
    R_tuple params, param_types;
    param_types = bf->param_types;
    len = LEN(bf->paramNames);
    match = len == info_tuple_size(t) && len < 4 && (d == NULL);
    if (match) {
        for (i = 0; i < LEN(param_types); i++) {
            R_type_info ti = tuple_get_item_info(t, i);
            R_type ptype, fptype;
            if (!is_definite(ti)) {
                match = 0;
                break;
            }
            ptype = get_type_from_info(ti);
            assert (ptype != NULL);
            fptype = (R_type)ITEM(param_types, i);
            if (!is_subclass_of(ptype, fptype)) {
                match = 0;
                break;
            }
        }
    }
    if (match) {
        bytelist_append_nolock(out, OPCODE(drop));
        bytelist_append_nolock(out, OPCODE(drop_under));
        bytelist_append_nolock(out, OPCODE(unpack));
        bytelist_append_nolock(out, LEN(bf->paramNames));
        if (len == 2) {
            bytelist_append_nolock(out, OPCODE(swap));
        } else if (len == 3) {
            bytelist_append_nolock(out, OPCODE(flip3));
        }
    } else {
        void *addr = gvmt_pin((GVMT_Object)bf);
        bytelist_append_nolock(out, OPCODE(unpack_native_params));
        bytelist_append_long(out, (int32_t)addr);
    }
}

R_type_info specialise_truth(R_type_info o, R_specialiser spec, R_info_item1 ti) {
    with_observed_value(o, ti->value1);
    if (get_type_from_info(o) == type_bool) {
        if (!ensure_type_tos(o, ti->ip, spec)) {
            goto failed;
        }
        return o;
    } else if (is_tagged(o)) {
        if (is_probable_tagged(o)) {
            if (!ensure_type_tos(o, ti->ip, spec)) {
                 goto failed;
            }
        }
        if (is_constant(o)) {
            bytelist_append_nolock(spec->out, OPCODE(drop));
            if (NON_ZERO((R_int)o)) {
                bytelist_append_nolock(spec->out, OPCODE(true));
                return type_info_constant((R_object)TRUE);
            } else {
                bytelist_append_nolock(spec->out, OPCODE(false));
                return type_info_constant((R_object)FALSE);
            }
        } else if (ensure_type_tos(o, ti->ip, spec)) {
            bytelist_append_nolock(spec->out, OPCODE(byte));
            bytelist_append_nolock(spec->out, 0);
            bytelist_append_nolock(spec->out, OPCODE(i_comp_ne));
            return definite_type(type_bool);
        } else {
            goto failed;
        }
    } else {
        R_type t = get_type_from_info(o);
        if (t && t == type_NoneType && ensure_type_tos(o, ti->ip, spec)) {
            bytelist_append_nolock(spec->out, OPCODE(drop));
            bytelist_append_nolock(spec->out, OPCODE(false));
            return definite_type(type_bool);
        }
        goto failed;
    }
failed:

    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise truth for ");
        print_type_info(stderr, o);
        fprintf(stderr, "\n");
    }
    bytelist_append_nolock(spec->out, OPCODE(truth));
    insert_deoptimise_check(spec, ti->ip);
    return definite_type(type_bool);
}

R_type_info specialise_f_call(R_type_info callable, R_type_info t, R_type_info d,
                             R_specialiser spec, R_info_item3 ti) {
    // Most likely a builtin-function call, so optimise that.
    /** TO DO
    *   Add type_ensure_params bytecode to insert before
    *   native call. Can clean up in later pass.
    */
    uint8_t* original_ip = ti->ip;
    R_object observed_callable = ti->value1;
    R_type call_t = get_type_from_info(callable);
    if (call_t == type_builtin_function) {
        R_builtin_function bf;
        int pcount;
        if (is_constant(callable)) {
            bf = (R_builtin_function)callable->value;
        } else if (TYPE(observed_callable) == type_builtin_function) {
            // Ensure callable is the expected value
            void *addr = gvmt_pin((GVMT_Object)observed_callable);
            void *exit;
            R_type_state state = make_type_state(spec->frame, spec->frozen_back, NULL);
            if (spec->out->size == 0)
                exit = make_interpret_exit(original_ip, spec->trace->pinned_objects);
            else
                exit = make_mono_exit(original_ip, spec->trace->pinned_objects, state);
            bytelist_append_nolock(spec->out, OPCODE(ensure_value));
            bytelist_append_nolock(spec->out, 2);
            bytelist_append_long(spec->out, (intptr_t)addr);
            bytelist_append_long(spec->out, (intptr_t)exit);
            bf = (R_builtin_function)observed_callable;
        } else {
            goto failed;
        }
        pcount = LEN(bf->paramNames);
        TYPE_ASSERT(bf, builtin_function);
        if (bf == function_hasattr) {
            return specialise_hasattr(t, d, spec, original_ip);
        } else {
            specialise_native_parameters(bf, t, d, spec->out);
            if (debug_optimise) {
                fprintf(stderr, "Native call to ");
                py_fprint(stderr, bf->__name__);
                fprintf(stderr, "\n");
            }
            emit_native_call(pcount, bf, spec, original_ip+OPCODE_LENGTH(f_call));
            if (bf == function_object_allocate) {
                // Return type of object is value of sole parameter...
                if (info_tuple_size(t) == 1) {
                    R_type_info cls = tuple_get_item_info(t, 0);
                    if (is_constant(cls) && TYPE(cls->value) == type_type) {
                        if (((R_type)cls->value)->dict_offset > 0) {
                            R_type_info obj = definite_type((R_type)cls->value);
                            R_dict_keys k = ((R_type)cls->value)->keys_cache;
                            obj->keys = k;
                            return obj;
                        }
                    }
                }
            } else if (bf->return_type) {
                return definite_type(bf->return_type);
            }
            return info_not_known();
        }
    } else if (call_t == type_type) {
        if (is_constant(callable) && callable->value == (R_object)type_type) {
            if (info_tuple_size(t) == 1 && d == NULL) {
                R_object val;
                R_type_info t0 = tuple_get_item_info(t, 0);
                R_tuple params = (R_tuple)ti->value2;
                R_type cls;
                TYPE_ASSERT(params, tuple);
                assert(LEN(params) == 1);
                val = ITEM(params, 0);
                cls = TYPE(val);
                if (with_observed_value(t0, val)) {
                    if (!is_definite(t0)) {
                        bytelist_append_nolock(spec->out, OPCODE(over));
                        bytelist_append_nolock(spec->out, OPCODE(unpack));
                        bytelist_append_nolock(spec->out, 1);
                        ensure_type_drop(spec, original_ip, t0);
                    }
                    bytelist_append_nolock(spec->out, OPCODE(drop));
                    bytelist_append_nolock(spec->out, OPCODE(drop));
                    bytelist_append_nolock(spec->out, OPCODE(drop));
                    bytelist_append_nolock(spec->out, OPCODE(fast_constant));
                    bytelist_append_long(spec->out, (uint32_t)gvmt_pin((GVMT_Object)cls));
                    make_type_definite(t0);
                    return type_info_constant((R_object)cls);
                }
            }
        }
    }
failed:
    bytelist_append_nolock(spec->out, OPCODE(f_call));
    insert_deoptimise_check(spec, original_ip + OPCODE_LENGTH(f_call));
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise f_call for ");
        print_type_info(stderr, callable);
        fprintf(stderr, "\n");
    }
    return info_not_known();
}

R_type_info specialise_load_special(R_type_info obj, R_str name,
                                    R_specialiser spec, uint8_t* original_ip) {
    R_type t = get_type_from_info(obj);
    R_object attr = class_attribute(t, name);
    assert(is_definite(obj));
    if (attr) {
        if (ensure_class_attribute(spec, t, name, attr)) {
            get_func getter = _HotPyObject_getter(attr);
            if (getter) {
                void* addr = gvmt_pin((GVMT_Object)attr);
                if (IS_FUNCTION(attr)) {
                    optimise_constant(attr, spec->out);
                    bytelist_append_nolock(spec->out, OPCODE(bind));
                    return info_bound_method(obj, type_info_constant(attr));
                } else if (IMMUTABLE_TYPE(TYPE(attr))) {
                    bytelist_append_nolock(spec->out, OPCODE(fast_get));
                    bytelist_append_long(spec->out, (int32_t)addr);
                    bytelist_append_long(spec->out, (int32_t)getter);
                    return info_not_known();
                }
            } else {
                return optimise_constant(attr, spec->out);
            }
        }
    }
    bytelist_append_nolock(spec->out, OPCODE(load_special));
    bytelist_append_nolock(spec->out, original_ip[1]);
    insert_deoptimise_check(spec, original_ip + OPCODE_LENGTH(load_special));
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise load_special\n");
    }
    return info_not_known();
}

R_type_info specialise_has_special(R_type_info obj, R_str name,
                                    R_specialiser spec, uint8_t* original_ip) {
    R_type t = get_type_from_info(obj);
    R_object attr = class_attribute(t, name);
    if (attr) {
        if (ensure_class_attribute(spec, t, name, attr)) {
            bytelist_append_nolock(spec->out, OPCODE(drop));
            bytelist_append_nolock(spec->out, OPCODE(true));
            return type_info_constant((R_object)TRUE);
        }
    } else {
       if (ensure_no_attribute(spec, t, name)) {
            bytelist_append_nolock(spec->out, OPCODE(drop));
            bytelist_append_nolock(spec->out, OPCODE(false));
            return type_info_constant((R_object)FALSE);
        }
    }
    bytelist_append_nolock(spec->out, OPCODE(has_special));
    bytelist_append_nolock(spec->out, original_ip[1]);
    return definite_type(type_bool);
}

R_type_info specialise_has_dict(R_type_info obj, R_specialiser spec,
                                uint8_t* original_ip) {
    R_type t = get_type_from_info(obj);
    bytelist_append_nolock(spec->out, OPCODE(drop));
    if (t->dict_offset) {
        bytelist_append_nolock(spec->out, OPCODE(true));
    } else {
        bytelist_append_nolock(spec->out, OPCODE(false));
    }
    return definite_type(type_bool);
}

R_type_info specialise_value_in_object_dict_or_exit(R_type_info obj, R_str name,
                               R_specialiser spec, uint8_t* ip) {
    R_type t = get_type_from_info(obj);
    assert(is_definite(obj));
    if (t->dict_offset > 0) {
        R_dict_keys k = t->keys_cache;
        if (dict_str_in_keys(k, name)) {
            void* keys_addr = gvmt_pin((GVMT_Object)k);
            bytelist_append_nolock(spec->out, OPCODE(drop_under));
            bytelist_append_nolock(spec->out, OPCODE(fast_load_attr));
            bytelist_append_short(spec->out, t->dict_offset);
            bytelist_append_short(spec->out, probable_str_lookup(k, name));
            bytelist_append_long(spec->out, (int32_t)keys_addr);
            return info_not_known();
        }
    }
    assert(ip[0] == OPCODE(value_in_object_dict_or_exit));
    bytelist_append_nolock(spec->out, OPCODE(value_in_object_dict_or_exit));
    bytelist_append_nolock(spec->out, ip[1]);
    bytelist_append_nolock(spec->out, ip[2]);
    bytelist_append_nolock(spec->out, ip[3]);
    bytelist_append_nolock(spec->out, ip[4]);
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise value_in_object_dict_or_exit\n");
    }
    return info_not_known();
}

void specialise_value_not_in_object_dict_or_exit(R_type_info obj, R_str name,
                               R_specialiser spec, uint8_t* ip) {
    R_type t = get_type_from_info(obj);
    assert(is_definite(obj));
    if (t->dict_offset > 0) {
        R_dict_keys k = t->keys_cache;
        if (dict_str_in_keys(k, name)) {
            bytelist_append_nolock(spec->out, OPCODE(value_not_in_object_dict_or_exit));
            bytelist_append_nolock(spec->out, ip[1]);
            bytelist_append_nolock(spec->out, ip[2]);
            bytelist_append_nolock(spec->out, ip[3]);
            bytelist_append_nolock(spec->out, ip[4]);
            if (debug_optimise) {
                fprintf(stderr, "Failed to specialise value_not_in_object_dict_or_exit\n");
            }
            return;
        }
        ensure_keys(spec, obj, k, ip);
    }
    bytelist_append_nolock(spec->out, OPCODE(drop));
    bytelist_append_nolock(spec->out, OPCODE(drop));
}

void specialise_set_in_object_dict(R_type_info obj, R_type_info key,
                               R_specialiser spec, uint8_t* original_ip) {
    R_type t = get_type_from_info(obj);
    assert(original_ip[0] == OPCODE(set_in_object_dict));
    assert(is_definite(obj));
    if (is_constant(key) && TYPE(key->value) == type_str) {
        R_str name = (R_str)key->value;
        R_dict_keys k = t->keys_cache;
        if (t->dict_offset > 0 && dict_str_in_keys(k, name)) {
            void* keys_addr = gvmt_pin((GVMT_Object)k);
            // Has a dictionary && name in cached keys
            bytelist_append_nolock(spec->out, OPCODE(drop_under));
            bytelist_append_nolock(spec->out, OPCODE(fast_store_attr));
            bytelist_append_short(spec->out, t->dict_offset);
            bytelist_append_short(spec->out, probable_str_lookup(k, name));
            bytelist_append_long(spec->out, (int32_t)keys_addr);
            return;
        }
    }
    bytelist_append_nolock(spec->out, OPCODE(set_in_object_dict));
    if (debug_optimise) {
        fprintf(stderr, "Failed to specialise set_in_object_dict\n");
    }
}

void insert_deoptimise_check(R_specialiser spec, uint8_t* original_ip) {
    bytelist_append_nolock(spec->out, OPCODE(deoptimise_check));
    bytelist_append_long(spec->out, (int32_t)spec->invalidated_address);
    bytelist_append_long(spec->out, (int32_t)original_ip);
}

static R_type_info optimise_global(R_str name, R_specialiser spec, R_object globals,
                                   uint8_t* original_ip, int store) {
    R_dict d = (R_dict)globals;
    void* addr;
    int opcode;
    if (TYPE(d) == type_dict) {
        addr = gvmt_pin((GVMT_Object)d->values);
        if (guard_module_deletion(d, name, spec->trace)) {
            int index = probable_str_lookup(d->values->keys, name);
            if ((index & 0xffff) == index) {
                PY_set_add((R_object)d->values, spec->trace->pinned_objects);
                opcode = store ? OPCODE(fast_store_global) : OPCODE(fast_load_global);
                bytelist_append_nolock(spec->out, opcode);
                bytelist_append_long(spec->out, (int32_t)addr);
                bytelist_append_short(spec->out, index);
                return info_not_known();
            }
        }
    }
    addr = gvmt_pin((GVMT_Object)canonical_string(name));
    opcode = store ? OPCODE(store_global) : OPCODE(load_global);
    assert(original_ip[0] == opcode);
    assert(OPCODE_LENGTH(load_global) == OPCODE_LENGTH(store_global));
    bytelist_append_nolock(spec->out, opcode);
    bytelist_append_nolock(spec->out, original_ip[1]);
    bytelist_append_nolock(spec->out, original_ip[2]);
    insert_deoptimise_check(spec, original_ip+OPCODE_LENGTH(load_global));
    if (debug_optimise) {
        char* op = store ? "store" : "load";
        fprintf(stderr, "Failed to specialise %s_global\n", op);
    }
    return info_not_known();
}

static R_type_info optimise_load_global_const(R_dict glbls,
                            R_specialiser spec, R_str name, R_object obj) {
    if (guard_module_value(glbls, name, obj, spec->trace))
        return optimise_constant(obj, spec->out);
    else
        return NULL;
}

static R_type_info optimise_load_builtin_const(R_dict glbls,
                        R_dict builtins, R_specialiser spec,
                        R_str name, R_object obj) {
    if (guard_module_value(glbls, name, NULL, spec->trace) &&
        guard_module_value(builtins, name, obj, spec->trace))
        return optimise_constant(obj, spec->out);
    else
        return NULL;
}

R_type_info optimise_load_global(R_str name, R_specialiser spec, R_object globals,
                                 R_object builtins, uint8_t* original_ip) {
    R_dict d = (R_dict)globals;
    R_module_dict_values glbl_vals, builtin_vals;
    R_type_info ti;
    if (TYPE(d) == type_dict) {
        glbl_vals = (R_module_dict_values)d->values;
        if (TYPE(glbl_vals) == type_module_dict_values) {
            R_object expected = dict_get_str(d, name);
            if (expected == NULL) {
                d = (R_dict)builtins;
                if (TYPE(d) == type_dict) {
                    builtin_vals = (R_module_dict_values)d->values;
                    if (TYPE(builtin_vals) == type_module_dict_values) {
                        expected = dict_get_str(d, name);
                        ti = optimise_load_builtin_const((R_dict)globals,
                                    d, spec, name, expected);
                        if (ti) return ti;
                    }
                }
            } else {
                if (isinstance(expected, type_function) ||
                    isinstance(expected, type_builtin_function) ||
                    isinstance(expected, type_type)) {
                    ti = optimise_load_global_const(d, spec, name, expected);
                    if (ti) return ti;
                }
            }
        }
    }
    return optimise_global(name, spec, globals, original_ip, 0);
}

R_type_info optimise_store_global(R_str name, R_specialiser spec, R_object globals, uint8_t* original_ip) {
    return optimise_global(name, spec, globals, original_ip, 1);
}

///** Also need to check if the class is fixed */
//R_type unary_return_type(R_operator op, R_type t) {
//    if (t->flags & IMMUTABLE_FLAG) {
//        R_object o = class_attribute(t, op->func_name);
//        if (o && TYPE(o) == type_builtin_function) {
//            return t;
//        }
//    }
//    return NULL;
//}

R_array unknown_frame(size_t params, size_t len) {
    int i;
    R_array frame = create_array(len);
    for (i = 0; i < params; i++) {
        ITEM(frame, i) = (R_object)info_not_known();
    }
    for (; i < len; i++) {
        ITEM(frame, i) = (R_object)info_uninitialised();
    }
    return frame;
}

GVMT_Object print_specialiser_stack(int items, ...) {
    GVMT_Value *ap;
    int i = 0;
    ap = gvmt_stack_top();
    for (i = items-1; i >= 0; i--) {
        print_type_info(stderr, (R_type_info)gvmt_stack_read_object(ap+i));
        fprintf(stderr, " ");
    }
    return 0;
}

R_object specialiser_trace(uint8_t* ip) {
    int i, n;
    GVMT_Object o;
    char* opname;
    opname = gvmt_opcode_names_interpreter[*ip];
    if (trace_show_stack == TRUE) {
        n = gvmt_stack_depth();
        assert(n >= 0);
        fprintf(stderr, "%d items: ", n);
        if (n > 4) {
            n = 4;
            fprintf(stderr, "... ");
        }
        print_specialiser_stack(n);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "%s\n", opname);
    return NULL;
}

void null_slot_error(R_object object, int offset) {
    char buf[80];
    R_str name = type_slot_name(TYPE(object), offset);
    if (name == NULL) {
        sprintf(buf, "Slot offset %d", offset);
        name = string_from_c_string(buf);
    }
    attribute_error(TYPE(object), (R_object)name);
}

static void insert_defaults(R_tuple defaults, int count, R_bytelist out) {
    int i;
    assert(count >= 0);
    for (i = 0; i < count; i++) {
        R_object item = ITEM(defaults, LEN(defaults)-count+i);
        optimise_constant(item, out);
    }
}

void write_line_number(int line_number, R_bytelist out) {
    bytelist_append_nolock(out, OPCODE(fast_line));
    bytelist_append_nolock(out, line_number >> 8);
    bytelist_append_nolock(out, line_number);
}

