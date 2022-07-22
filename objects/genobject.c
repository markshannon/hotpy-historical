#include "function.h"
#include "optimise.h"
#include "opcodes.h"

/* /home/mark/Python-3.1.1/Doc/c-api/gen.rst */
R_object HotPyGen_New(R_frame frame) {
    R_generator gen;
    gen = gc_allocate(generator);
    SET_TYPE(gen, type_generator);
    gen->frame = frame;
    // Generators are finalisable, so need to register this with GC.
    // They don't seem to be, and don't need to be.
    // gvmt_gc_finalizable((GVMT_Object)gen);
    return (R_object)gen;
}

R_object PY_FUNC1(next_generator, R_generator gen) {
    R_object result;
#ifdef BOOTSTRAP
    assert("Generators not allowed in bootstrap" && 0);
    return 0;
#else
    R_thread_state ts = THREAD_STATE;
    R_frame f;
    GVMT_Value* stack;
    R_exception_handler h;
    TYPE_ASSERT(gen, generator);
    f = ts->current_frame;
    ts->current_frame = gen->frame;
    gen->frame->f_back = f;
    gen->frame->depth = f->depth+1;
    stack = gvmt_stack_top();
    h = gen->frame->exception_stack;
    while (h) {
        h->data_stack = stack;
        h = h->next;
    }
    return super_interpreter(gen->frame);
#endif
}

static void gen_close(R_generator gen) {
    // Set up for execution and immediately raise a
    // Generator exit exception.
    R_thread_state ts = THREAD_STATE;
    R_frame f;
    GVMT_Value* stack;
    R_exception_handler h;
    TYPE_ASSERT(gen, generator);
    f = ts->current_frame;
    ts->current_frame = gen->frame;
    gen->frame->f_back = f;
    gen->frame->depth = f->depth+1;
    stack = gvmt_stack_top();
    h = gen->frame->exception_stack;
    while (h) {
        h->data_stack = stack;
        h = h->next;
    }
    raise_str(type_GeneratorExit, empty_string);
}

R_object PY_FUNC1(generator_close, R_generator gen) {
    gen_close(gen);
    return (R_object)None;
}

R_object PY_FUNC1(generator_delete, R_generator gen) {
    gen_close(gen);
    return (R_object)None;
}

R_object GEN_STACK_MARKER;

void generator_restore_stack(R_generator gen) {
    GVMT_Value* stack = gvmt_stack_top();
    R_exception_handler h = gen->frame->exception_stack;
    int i;
    while (h) {
        h->data_stack = (GVMT_Value*)(h->data_stack - stack);
        h = h->next;
    }
    GVMT_PUSH(gen);
    GVMT_PUSH(GEN_STACK_MARKER);
    for (i = gen->saved - 1; i >= 0; i--) {
        R_object o = gen->stack[i];
        GVMT_PUSH(o);
    }
}

R_object generator_save_stack(void) {
    R_generator gen;
    R_exception_handler h;
    int i, count = 0;
    GVMT_Value* stack = gvmt_stack_top();
    while ((R_object)gvmt_stack_read_object(stack+count) != GEN_STACK_MARKER)
        count++;
    gen = (R_generator)gvmt_stack_read_object(stack+count+1);
    gen->saved = count;
    for (i = 0; i < count; i++) {
        gen->stack[i] = (R_object)gvmt_stack_read_object(stack+i);
    }
    gvmt_drop(count+2);
    h = gen->frame->exception_stack;
    while (h) {
        h->data_stack = stack + (intptr_t)h->data_stack;
        h = h->next;
    }
    return NULL;
}


