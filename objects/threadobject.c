#include "hotpy.h"
#include "optimise.h"

static void* threads;
static gvmt_lock_t thread_synch_lock = GVMT_LOCK_INITIALIZER; 

R_object PY_FUNC5(thread_new, R_type cls, R_str name, R_object target, R_tuple args, R_dict kwargs) {
    R_Thread t = (R_Thread)_HotPyObject_New(cls);
    t->name = name;
    t->target = target;
    t->args = args;
    //Ensure all keys are strings. 
    t->kwargs = kwargs;
    return (R_object)t;
}

extern void _hotpy_register_native_thread_state(void* ts);

R_thread_state make_thread_state(void) {
    GVMT_Value* stack = gvmt_stack_top();
    R_thread_state ts = gc_allocate(thread_state); 
    SET_TYPE(ts, type_thread_state);
    ts->start_time = high_res_time();
    ts->current_frame = ts->top_frame = stop_frame();
    ts->profiler = NULL;
    ts->tracer = NULL;
    ts->tls_dict = new_empty_dict();
    ts->trace_cache = new_empty_dict();
    ts->trace = NULL;
    ts->stop_depth = 0;
    if (optimiser_controls.trace == 0)
        ts->trace = (R_trace_record)None;
    _hotpy_register_native_thread_state(gvmt_pin((GVMT_Object)ts));
    return ts;
}

R_Thread thread_init(void) {
    R_Thread main_thread;
    threads = gvmt_gc_add_tls_root();
    main_thread = (R_Thread)PY_thread_new(new_dict(0), empty_tuple, (R_object)None, 
        string_from_c_string("MainThread"), type_Thread);
    gvmt_gc_write_tls_root(threads, (GVMT_Object)main_thread);
    gvmt_gc_write_tls_root(hotpy_thread_state, (GVMT_Object)make_thread_state());
    return main_thread;
}

R_object PY_FUNC1(thread_run, R_Thread t) {
    // CALL target(*args, **kwargs);
    assert(0 && "To do");
    return (R_object)NotImplemented;
}

/** Returns 0 */
GVMT_Object start_thread(void* root) {
    R_Thread t = (R_Thread)gvmt_gc_read_root(root);
    R_object run;
    R_tuple tuple;
    gvmt_gc_free_root(root);
    t->is_alive = 1;
    gvmt_gc_write_tls_root(threads, (GVMT_Object)t);
    run = class_attribute(TYPE(t), name_run);
    tuple = make_tuple(1);
    ITEM(tuple, 0) = (R_object)t;
    CALLABLE(run)(run, tuple, 0);
    t->is_alive = 0;
    return 0;
}

static void* thread_key;

R_object PY_FUNC1(thread_start, R_Thread t) {
    void* root = gvmt_gc_add_root();
    gvmt_gc_write_root(root, (GVMT_Object)t);
    if (hotpy_multi_threaded == 0) {
        hotpy_multi_threaded = 1;
        // Lock to force a memory barrier so other CPU/threads see new value 
        gvmt_lock(&thread_synch_lock);
        gvmt_unlock(&thread_synch_lock);
    }
    native_start_thread(root);
    return (R_object)None;
}

R_object PY_FUNC0(current_thread) {
    R_Thread t = (R_Thread)gvmt_gc_read_tls_root(threads);
    assert(t != 0);
    return (R_object)t;
}

