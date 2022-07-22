#include "hotpy.h"
//
//R_object PY_FUNC3(_start_new_thread, R_object callable, R_tuple args, R_dict kw_args) {
//    R_tuple start_object = make_tuple(3);
//    ITEM(start_object, 0) = callable;
//    ITEM(start_object, 1) = args;
//    ITEM(start_object, 2) = kw_args;
//    void* root = gvmt_gc_add_root();
//    gvmt_gc_write_root(root, (R_object)start_object);
//    intptr_t thr_id = native_start_thread(root);
//    return HotPyLong_FromSsize_t(thr_id);
//}

R_object PY_FUNC0(allocate_lock) {
    R_Lock l = gc_allocate(Lock);
    SET_TYPE(l, type_Lock);
    l->lock = os_mutex_create();
    return (R_object)l; 
}

// Needs finalisation.

R_object PY_FUNC1(lock_acquire, R_Lock l) {
    os_mutex_lock(l->lock);
    l->locked = 1;
    return (R_object)None;
}

R_object PY_FUNC1(lock_release, R_Lock l) {
    l->locked = 0;
    os_mutex_unlock(l->lock);
    return (R_object)None;
}

R_object PY_FUNC1(lock_locked, R_Lock l) {
    if (l->locked)
        return (R_object)TRUE;
    else
        return (R_object)FALSE;
}



