#include "hotpy.h"

R_object PY_FUNC2(condition_notify, R_Condition c, R_object count) {
    int32_t i, x = int_from_py_int(HotPyNumber_Index(count));
    for(i = 0; i < x; i++)
        os_condition_notify(c->condition);
    return (R_object)None;
}

R_object PY_FUNC1(condition_notify_all, R_Condition c) {
    char *msg = os_condition_notify_all(c->condition);
    if (msg) {
        raise_char(type_RuntimeError, msg);
    }
    return (R_object)None;
}

R_object PY_FUNC1(condition_acquire, R_Condition c) {
    char *msg = os_mutex_lock(c->lock);
    if (msg) {
        raise_char(type_RuntimeError, msg);
    }
    return (R_object)None;
}

R_object PY_FUNC1(condition_release, R_Condition c) {
    char *msg = os_mutex_unlock(c->lock);
    if (msg) {
        raise_char(type_RuntimeError, msg);
    }
    return (R_object)None;
}

R_object PY_FUNC2(condition_wait, R_Condition c, R_object timeout) {
    //Yet to do, if any timeout...   
    if (timeout == (R_object)None) {
        char *msg = os_condition_wait(c->lock, c->condition);
        if (msg) {
            raise_char(type_RuntimeError, msg);
        }
        return (R_object)None;
    } else {
        return (R_object)NotImplemented;
    }
}
