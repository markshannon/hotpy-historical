#ifndef HOTPY_STRUCTS_H
#define HOTPY_STRUCTS_H

#include <inttypes.h>
#include "gvmt/gvmt.h"

GVMT_OBJECT(partial_operator) {
    GVMT_OBJECT(type)* ob_type;       /* read-only */
    uintptr_t ref_count;
    int32_t right;
    GVMT_OBJECT(type)* type;
    GVMT_OBJECT(binary_operator)* op;
};

GVMT_OBJECT(Thread) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    thread_t thread;    /* pointer */
    int32_t is_alive;
    GVMT_OBJECT(str)* name;
    GVMT_OBJECT(object)* target;
    GVMT_OBJECT(tuple)* args;
    GVMT_OBJECT(dict)* kwargs;
};

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(Token) {  // special
    GVMT_OBJECT(type)* ob_type; /* read-only */
    uintptr_t kind;
    GVMT_OBJECT(object)* data;
};

GVMT_OBJECT(comparison_operator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t arity;
    GVMT_OBJECT(hashtable)* pre_defined;
    GVMT_OBJECT(str)* name;        /* read-only */
    GVMT_OBJECT(str)* func_name;
    GVMT_OBJECT(object)* fallback;
    GVMT_OBJECT(function)* python_equivalent;
    GVMT_OBJECT(str)* rfunc_name;
};

GVMT_OBJECT(module) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(dict)* dictionary;
    GVMT_OBJECT(str)* filename;
};

GVMT_OBJECT(type_state) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t hashcode;
    GVMT_OBJECT(tuple)* frame;
    GVMT_OBJECT(tuple)* back;
};

GVMT_OBJECT(typed_machine_state) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t hashcode;
    GVMT_OBJECT(bytes)* compressed_state;
};

GVMT_OBJECT(Constants) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(str)* __file__;    /*  read-only */
    GVMT_OBJECT(tuple)* __names;    /* read-only */
    GVMT_OBJECT(tuple)* __constants;    /* read-only */
    GVMT_OBJECT(tuple)* __functions;    /* read-only */
};

GVMT_OBJECT(bytelist) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t size;
    int32_t lock;
    GVMT_OBJECT(bytearray)* array;
};

GVMT_OBJECT(thread_state) {  // special
    GVMT_OBJECT(type)* ob_type; /* read-only */
    uintptr_t ref_count;
    event_func event;    /* pointer */
    int32_t request;
    uint32_t stop_depth;
    double start_time;
    uint8_t* on_exception;    /* pointer */
    uint8_t* next_ip;    /* pointer */
    GVMT_OBJECT(frame)* current_frame;
    GVMT_OBJECT(frame)* top_frame;
    GVMT_OBJECT(object)* profiler;
    GVMT_OBJECT(guard)* profile_guards;
    GVMT_OBJECT(object)* tracer;
    GVMT_OBJECT(guard)* trace_guards;
    GVMT_OBJECT(dict)* tls_dict;
    GVMT_OBJECT(list)* repr_list;
    GVMT_OBJECT(dict)* trace_cache;
    GVMT_OBJECT(trace_record)* trace;
    GVMT_OBJECT(BaseException)* c_exception;
    GVMT_OBJECT(type_state)* optimise_type_state;
    GVMT_OBJECT(object)* frame_cache[64];
};

GVMT_OBJECT(guard) {  // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    guard_delete_func delete;    /* pointer  */
    GVMT_OBJECT(object)* object;
    GVMT_OBJECT(str)* name;
    GVMT_OBJECT(trace)* trace;
    GVMT_OBJECT(guard)* ring;
    GVMT_OBJECT(guard)* next;
};

GVMT_OBJECT(operator) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    int32_t arity;
    GVMT_OBJECT(hashtable)* pre_defined;
    GVMT_OBJECT(str)* name;    /* read-only */
    GVMT_OBJECT(str)* func_name;
    GVMT_OBJECT(object)* fallback;
    GVMT_OBJECT(function)* python_equivalent;
};

GVMT_OBJECT(Condition) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    void* condition;    /* pointer */
    void* lock;    /* pointer */
};

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(dict_table) {
    GVMT_OBJECT(type)* ob_type;  /* read-only */
    intptr_t size;
    uintptr_t length;
    int32_t load;
    int32_t used;
    GVMT_OBJECT(object)* array[];
};

GVMT_OBJECT(unary_operator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t arity;
    GVMT_OBJECT(hashtable)* pre_defined;
    GVMT_OBJECT(str)* name;    /* read-only */
    GVMT_OBJECT(str)* func_name;
    GVMT_OBJECT(object)* fallback;
    GVMT_OBJECT(function)* python_equivalent;
};

GVMT_OBJECT(binary_operator) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    int32_t arity;
    GVMT_OBJECT(hashtable)* pre_defined;
    GVMT_OBJECT(str)* name;     /* read-only */
    GVMT_OBJECT(str)* func_name;
    GVMT_OBJECT(object)* fallback;
    GVMT_OBJECT(function)* python_equivalent;
    GVMT_OBJECT(str)* rfunc_name;
};

GVMT_OBJECT(Lock) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t locked;
    void* lock;           /* pointer */
};

#endif
