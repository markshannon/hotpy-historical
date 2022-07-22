
#include "hotpy.h"
#define min(a,b) ((a)<(b)?(a):(b))


void init_frame(R_frame frame, GVMT_Object* params, int pcount) {
    int i;
    for (i = 0; i < pcount; i++) {
        R_object o = (R_object)params[i];
        ITEM(frame, pcount-1-i) = o;
    }        
}

R_object load_local(R_frame frame, R_str name) {
    if (frame->array[0]) {
        if (IS_DICT_EXACT(frame->array[0])) {
            return dict_get_str((R_dict)  frame->array[0], name); 
        }
    }
    assert("To do " && 0);    
    return 0;
}

void store_local(R_frame frame, R_str name, R_object val) {
    if (frame->array[0]) {
        if (IS_DICT_EXACT(frame->array[0])) {
            dict_set_str((R_dict)  frame->array[0], name, val); 
        } else {
            assert("To do " && 0);     
        }
    } else {
        assert("To do " && 0);
    }
}

static void copy_fast_to_dict(R_frame f, R_dict locals) {
    R_tuple names = f->f_code->local_names;
    size_t i, len = f->length;
    assert(locals);
    for (i = 0; i < len; i++) {
        TYPE_ASSERT(ITEM(names, i), str);
        dict_set_str(locals, (R_str)ITEM(names, i), ITEM(f, i));
    }
}

R_object PY_FUNC0(locals) {
    R_frame f = THREAD_STATE->current_frame; 
    R_dict locals = f->scopes->locals;
    if (locals == NULL) {
        locals = new_dict(f->length);
    }
    copy_fast_to_dict(f, locals);
    return (R_object)locals;
}

/** sys._getframe() */
R_object PY_FUNC1(_getframe, R_int depth) {
    R_frame frame = THREAD_STATE->current_frame;
    int d = int_from_py_int(depth);
    TYPE_ASSERT(depth, int);
    if (d < 0)
        raise_char(type_ValueError, "negative depth");
    while (d && frame) {
        frame = frame->f_back;
        --d;
    }
    if (frame)
        return (R_object)frame;
    else
        raise_char(type_ValueError, "call stack is not deep enough");
    return 0;
}

R_object PY_FUNC0(globals) {
    R_frame frame = THREAD_STATE->current_frame;
    return frame->scopes->module;
}

