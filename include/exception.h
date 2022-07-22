#ifndef HOTPY_EXCEPTION_H
#define HOTPY_EXCEPTION_H


/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(exception_handler) {  // special
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    int stop;
    GVMT_Value* data_stack;    /* pointer  */
    uint8_t* ip;    /* pointer  */
    GVMT_OBJECT(exception_handler)* next;
    GVMT_OBJECT(exec_link)* link;
};

GVMT_OBJECT(BaseException) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(traceback)* __traceback__;    /*  read-only */
    GVMT_OBJECT(object)* args;    /* read-write */
    GVMT_OBJECT(dict)* dictionary;
};

GVMT_OBJECT(traceback) {  //  special
    GVMT_OBJECT(type)* ob_type;
    uint32_t tb_lineno;  // read-only
    GVMT_OBJECT(frame)* tb_frame;  // read-only
    GVMT_OBJECT(traceback)* tb_next;   // read-only
};

#ifdef BOOTSTRAP
#define RAISE(ex) { R_BaseException __ex = (ex); build_backtrace(ex); \
                    gvmt_raise((GVMT_Object)__ex); }
#else
#define RAISE(ex) { R_BaseException __ex = (ex); build_backtrace(ex); \
                    gvmt_transfer((GVMT_Object)__ex); }
#endif

void raise_char(R_type type, const char* message);
void raise_str(R_type type, R_str message);
void raise_type_error(R_type t1, R_type t2);

R_BaseException make_exception(R_type type, R_str msg);

R_object exception_tuple_from_exception(R_BaseException ex);

void build_backtrace(R_BaseException ex);

R_exception_handler make_exception_handler(int stop, R_exception_handler next, uint8_t* ip);

#ifdef BOOTSTRAP
# define stop_iteration() raise_str(type_StopIteration, empty_string);
#else
# define stop_iteration() gvmt_transfer((GVMT_Object)STOP_ITERATION_OBJECT);
#endif

void load_attr_error(R_object object, int key_index);

#endif
