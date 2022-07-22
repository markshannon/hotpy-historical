
#include "hotpy.h"

static int default_bool(R_object o) {
    R_type klass = TYPE(o);
    R_object d = class_attribute(klass, name___len__);
    R_int i;
    if (d) {
        R_tuple t = make_tuple1(o);
        call_func f = CALLABLE(d);
        i = (R_int)f(d, t, 0);
        if (IS_INT(i)) {
            return NON_ZERO(i);
        } else { 
            raise_char(type_TypeError, "an integer is required");
            return 0;
        }
    } else {
        return 1;   
    }
}

static void not_bool_error(R_type t) {
    R_str s = py_sprintf("__bool__ should return bool, returned %s", 
                         t->__name__);
    raise_str(type_TypeError, s);
}

int bool_value(R_object obj) {
    R_type klass = TYPE(obj);
    R_object d = class_attribute(klass, name___bool__);
    R_bool b;
    if (d == 0) {
        return default_bool(obj);
    } else {
        call_func f = CALLABLE(d);
        R_tuple t = make_tuple(1);
        ITEM(t, 0) = obj;
        b = (R_bool)f(d, t, 0);
        if (IS_BOOL(b)) {
            return b->value;
        } else {
            not_bool_error(TYPE(b));
            return 0;
        }
    }
}

R_bool truth(R_object o) {
    R_type klass = TYPE(o); 
    if (klass == type_bool) {
        return (R_bool)o;
    } else {
        R_object d = class_attribute(klass, name___bool__);
        if (d == 0) {
            return booleans[default_bool(o)];
        } else {
            call_func f = CALLABLE(d);
            R_tuple t = make_tuple(1);
            R_bool b;
            ITEM(t, 0) = o;
            b = (R_bool)f(d, t, 0);
            if (IS_BOOL(b)) {
                return b;
            } else {
                not_bool_error(TYPE(b));
                return 0;
            }
        }
    }
}

R_object PY_FUNC2(_bool, R_type cls, R_object obj) {
    return (R_object)truth(obj);
}

R_object PY_FUNC2(and__bool_bool, R_bool b0, R_bool b1) {
    TYPE_ASSERT(b0, bool);
    TYPE_ASSERT(b1, bool);
    return (R_object) booleans[b0->value & b1->value];
}

R_object PY_FUNC2(or__bool_bool, R_bool b0, R_bool b1) {
    TYPE_ASSERT(b0, bool);
    TYPE_ASSERT(b1, bool);
    return (R_object) booleans[b0->value | b1->value];
}

R_object PY_FUNC2(xor_bool_bool, R_bool b0, R_bool b1) {
    TYPE_ASSERT(b0, bool);
    TYPE_ASSERT(b1, bool);
    return (R_object) booleans[b0->value ^ b1->value];
}

R_object PY_FUNC1(invert_bool, R_bool b) {
    TYPE_ASSERT(b, bool);
    return (R_object)R_int_from_ssize_t(~(b->value));
}

intptr_t hotpy_bool_hash(R_bool b) {
    return b->value;   
}


