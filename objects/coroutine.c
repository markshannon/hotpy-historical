

R_coroutine new_coroutine(R_object callable) {
    R_type t = TYPE(callable);
    R_coroutine co;
    if (t == type_function || 
        (t == type_bound_method && 
         TYPE(((R_bound_method)callable)-> callable == type_function))) {
        co = gc_allocate(coroutine);
        SET_TYPE(co, type_coroutine);
        co->callable = callable;
        co->frame = NULL;
    } else {
        raise_char(type_TypeError, "Coroutine must be a python callable");
        co = NULL;
    } 
    return co;
}

void resume(R_coroutine self, R_object value) {
    if (self->frame == NULL) {
        
        
    } else {
        // Swap current frame & call
        self->yield_to = THREAD_STATE->current_frame;
        THREAD_STATE->frame = self->frame;
        THREAD_STATE->current_coroutine = self;
    }
    // Exit to dispatcher - How??
    gvmt_transfer(NULL);
}

void yield(R_object value) {
    R_coroutine co = THREAD_STATE->current_coroutine;
    if (co == NULL) 
        raise_char(type_IllegalStateException, "No current coroutine");
    assert(co->yield_to != NULL);
    // Swap context.
    co->frame = THREAD_STATE->current_frame;
    THREAD_STATE->frame = co->yield_to;
    // Exit to dispatcher - How??
    gvmt_transfer(NULL);
}

void 
