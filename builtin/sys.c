#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hotpy.h"
#include "intobject.h"
#include "guard.h"

R_object PY_FUNC1(_abort, R_int i) {
    TYPE_ASSERT(i, int);
    exit(int_from_py_int(i));
    return 0;
}

R_object PY_FUNC1(settrace, R_object tracefunc) {
    R_thread_state ts = THREAD_STATE;
    if (tracefunc == (R_object)None) {
        ts->tracer = 0;
    } else if (TYPE(tracefunc)->call == 0) {
        raise_char(type_TypeError, "tracefunc must be a callable");
    } else {
        trigger_guard(ts->trace_guards);
        ts->trace_guards = 0;
        ts->tracer = tracefunc;
    }
    return (R_object)None;
}

R_object PY_FUNC1(setrecursionlimit, R_int i) {
    TYPE_CHECK(i, type_int);
    sys_recursion_limit = int_from_py_int(i);
    return (R_object)None;
}

R_object PY_FUNC0(getrecursionlimit) {
    return (R_object)R_int_from_ssize_t(sys_recursion_limit);
}

R_object PY_FUNC0(gettrace) {
    R_thread_state ts = THREAD_STATE;
    if (ts->tracer)
        return ts->tracer;
    else
        return (R_object)None;
}

R_object PY_FUNC1(setprofile, R_object profiler) {
    R_thread_state ts = THREAD_STATE;
    if (profiler == (R_object)None) {
        ts->profiler = 0;
    } else if (TYPE(profiler)->call == 0) {
        raise_char(type_TypeError, "profiler must be a callable");
    } else  {
        trigger_guard(ts->profile_guards);
        ts->profile_guards = 0;
        ts->profiler = profiler;
    }
    return (R_object)None;
}

R_object PY_FUNC0(getprofile) {
    R_thread_state ts = THREAD_STATE;
    if (ts->profiler)
        return ts->profiler;
    else
        return (R_object)None;
}

