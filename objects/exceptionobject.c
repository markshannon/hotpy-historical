#include <assert.h>
#include <stdlib.h>
#include "hotpy.h"

R_exception_handler make_exception_handler(int stop, R_exception_handler next, uint8_t* ip) {
    GVMT_Value* stack = gvmt_stack_top();
    R_exception_handler h = gc_allocate(exception_handler);
    SET_TYPE(h, type_exception_handler);
    h->next = next;
    h->stop = stop;
    h->ip = ip;
    h->data_stack = stack;
    return h;
}

R_BaseException make_exception(R_type type, R_str msg) {
    R_BaseException ex = gc_allocate(BaseException);
    R_tuple args;
    SET_TYPE(ex, type);
    args = make_tuple1((R_object)msg);
    ex->args = (R_object)args;
    ex->__traceback__ = (R_traceback)None;
    ex->dictionary = new_dict(0);
    return ex;
}

static R_traceback make_traceback(R_frame f, R_traceback next) {
    R_traceback tb = gc_allocate(traceback);
    SET_TYPE(tb, type_traceback);
    tb->tb_frame = f;
    tb->tb_lineno = f->f_lineno;
    tb->tb_next = next;
    return tb;
}

void build_backtrace(R_BaseException ex) {
    R_frame frame;
    R_traceback tb = (R_traceback)None;
    frame = THREAD_STATE->current_frame;
    while (frame->depth) {
        tb = make_traceback(frame, tb);
        assert(frame->f_back);
        frame = frame->f_back;
    }
    ex->__traceback__ = tb;
}

/** Raises Exception of type 'type', with message 'msg' */
void raise_str(R_type type, R_str msg) {
    R_BaseException ex = make_exception(type, msg);
    RAISE(ex);
}

/** Raises Exception of type 'type', with message 'msg' */
void raise_char(R_type type, const char* msg) {
    R_str s_msg = string_from_c_string(msg);
    R_BaseException ex = make_exception(type, s_msg);
    RAISE(ex);
}

/** Helper for macro TYPE_CHECK */
void raise_type_error(R_type t1, R_type t2) {
    R_str msg = py_sprintf("%s is not a subclass of %s", t1->__name__, t2->__name__);
    raise_str(type_TypeError, msg);
}

void interrupt_event(R_thread_state ts) {
    raise_str(type_KeyboardInterrupt, empty_string);
}

R_object exception_tuple_from_exception(R_BaseException ex) {
    R_tuple t = make_tuple(3);
    ITEM(t, 0) = (R_object)TYPE(ex);
    ITEM(t, 1) = (R_object)ex;
    ITEM(t, 2) = (R_object)ex->__traceback__;
    return (R_object)t;
}

R_object exception_value(R_BaseException ex) {
    R_object args = ex->args;
    if (IS_TUPLE(args) && ((R_tuple)args)->length == 1) {
        return ITEM((R_tuple)args, 0);
    } else {
        return args;
    }
}

/** Print exception, inlcuding backtrace to out */
void print_exception(FILE* out, R_BaseException ex) {
    R_object args;
    R_traceback t;
    R_str msg;
    fprintf(out, "Traceback (most recent call last):\n");
    t = ex->__traceback__;
    while (t != (R_traceback)None) {
        R_frame f = t->tb_frame;
        R_str file = f->f_code->constants->__file__;
        fprintf(out, "  File \"");
        f = t->tb_frame;
        py_fprint(out, file);
        fprintf(out, "\", line %d, in ", t->tb_lineno);
        py_fprint(out, f->f_code->name);
        fprintf(out, "\n");
        t = t->tb_next;
    }
    py_fprint(out, TYPE_NAME(ex));
    msg = HotPyObject_Str(exception_value(ex));
    if (msg->length > 0) {
        fprintf(out, ": ");
        py_fprint(out, msg);
    }
    fprintf(out, "\n");
}

void handle_internal_error(GVMT_Object discard) {
    raise_char(type_InternalError, "Memory violation");
}

void gvmt_user_uncaught_exception(GVMT_Object ex) {
    // Shouldn't happen.
    abort();
}

