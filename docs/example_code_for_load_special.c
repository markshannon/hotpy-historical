

load_special_or_goto: {
    int offset = ARG() & 255;
    R_str name = ARG() >> 8;
    R_object obj = POP();
    R_object desc;
    R_object getter;
    if ((desc = special_from_type(obj, name)) == NULL) {
        TRACE_RECORD_EXIT_ARG(NO_SPECIAL_OR_EXIT, 1, obj);
        JUMP_BY(offset); 
    } 
    if ((IS_BINDABLE(desc)) { 
        TRACE_RECORD_EXIT_ARG(LOAD_SPECIAL_OR_EXIT, 1, obj);
        PUSH(bind_method(desc, obj));
        FAST_DISPATCH();
    } else {
        TRACE_RECORD(COPY, 0);
        TRACE_RECORD_EXIT_ARG(SPECIAL_FROM_TYPE_OR_EXIT, 1, obj);
        TRACE_RECORD(SWAP, 0);
        PUSH(desc);
        PUSH(obj);
        TRACE_SETUP_CALL(&as_attr_function);
        DISPATCH();
    }
}



