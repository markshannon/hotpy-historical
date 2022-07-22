#ifndef HOTPY_GENOBJECT_H
#define HOTPY_GENOBJECT_H

GVMT_OBJECT(generator) {  // special
    GVMT_OBJECT(type)* ob_type;  /* read-only */
    uintptr_t ref_count;
    uintptr_t saved;
    GVMT_OBJECT(object)* stack[8];
    GVMT_OBJECT(frame)* frame;
};

R_object HotPyGen_New(R_frame frame);

extern R_object GEN_STACK_MARKER;

void generator_restore_stack(R_generator gen);

R_object generator_save_stack(void);

#endif
