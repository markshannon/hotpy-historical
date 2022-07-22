#ifndef HOTPY_FRAMEOBJECT_H
#define HOTPY_FRAMEOBJECT_H


GVMT_OBJECT(frame) {   // special 
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uintptr_t length;  
    uint32_t f_lineno;    /*  read-only */
    uint8_t* next_ip; 
    uint32_t depth;   
    GVMT_OBJECT(frame)* f_back;    /*  read-only */
    GVMT_OBJECT(exception_handler)* exception_stack;  
    GVMT_OBJECT(code_object)* f_code;    /*  read-only */
    GVMT_OBJECT(scopes)* scopes;  
    GVMT_OBJECT(object)* array[];  
};

GVMT_OBJECT(scopes) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(dict)* locals;   
    GVMT_OBJECT(object)* module;   
    GVMT_OBJECT(object)* builtins; 
};                                     

void intialise_frame(int parameter_count, R_tuple t, R_dict d, R_frame frame);

R_frame function_frame(R_function func, R_frame caller);

R_frame push_frame(R_frame f);

R_frame stop_frame(void);

R_frame new_frame(R_code_object code_obj, R_scopes scopes, R_frame caller);

void init_frame(R_frame frame, GVMT_Object* params, int pcount);

void init_frame_td(R_frame frame, R_function func, R_tuple t, R_dict d);

R_object load_local(R_frame frame, R_str name);

void store_local(R_frame frame, R_str name, R_object val);

#endif
