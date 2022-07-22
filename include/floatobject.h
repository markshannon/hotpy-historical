#ifndef HOTPY_FLOATOBJECT_H
#define HOTPY_FLOATOBJECT_H

GVMT_OBJECT(float) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    double value; 
};

R_str float_to_str(R_float f);

R_float make_float(void);

R_float to_float(R_object obj);

R_object zero_division_error(char* msg);

//double parse_double(R_str s);

R_float parse_float(R_str s, R_type overflow);

#endif
