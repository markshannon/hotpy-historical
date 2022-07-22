#ifndef HOTPY_BOOLOBJECT_H
#define HOTPY_BOOLOBJECT_H

// Must have same layout as int, since it bool is a sublclass of int
// "length" is always one, it can be ignored.
GVMT_OBJECT(bool) {
    GVMT_OBJECT(type)* ob_type;  /* read-only */
    uintptr_t ref_count;
    int32_t always_one;
    uint32_t value;
};

int bool_value(R_object o);

R_bool truth(R_object o);

R_object PY_FUNC2(and__bool_bool, R_bool b0, R_bool b1);
R_object PY_FUNC2(or__bool_bool, R_bool b0, R_bool b1);
R_object PY_FUNC2(xor_bool_bool, R_bool b0, R_bool b1);


#endif
