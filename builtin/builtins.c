#include <unistd.h>
#include "hotpy.h"
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>
#include "operators.h"
#include "optimise.h"

R_object PY_FUNC1(lib_import, R_str name) {
    char buffer [200];
    R_str filename = py_sprintf("builtin/%s.py", name);
    R_module module = new_module(name, filename);
    R_frame frame = THREAD_STATE->current_frame;
    TYPE_ASSERT(name, str);
    py_print(buffer, filename);
    run_script(load_source(buffer), module, frame);
    assert(THREAD_STATE->current_frame == frame);
    return (R_object)module;
}

R_object PY_FUNC1(make_immutable, R_type t) {
    TYPE_ASSERT(t, type);
    t->flags |= IMMUTABLE_FLAG;
    return (R_object)None;
}

R_object PY_FUNC2(surrogate_new_function, R_type cls, R_function f) {
    cls->surrogate_new = f;
    return (R_object)None;
}

R_object PY_FUNC1(int_to_str, R_int i) {
    return (R_object)int_to_str(i, 10);
}

R_object PY_FUNC1(identity, R_object o) {
    return o;
}

R_object PY_FUNC1(__len__str, R_str s) {
    return (R_object)R_int_from_ssize_t(s->length);
}

R_object PY_FUNC2(_new_str, R_type str, R_object arg) {
    return (R_object)HotPyObject_Str(arg);
}

R_object PY_FUNC1(abs_int, R_int i) {
    long p0 = int_from_py_int(i);
    if (p0 < 0) {
        p0 = -p0;
        if (p0 < 0) {
            assert(0 && "To do");
            return 0; // Should return abs(-INT_MIN) as long...
        } else {
            return (R_object)R_int_from_ssize_t(p0);
        }
    } else {
        return (R_object)i;
    }
}

R_object PY_FUNC1(ord, R_str s) {
    unsigned char c;
    TYPE_ASSERT(s, str);
    if (s->length != 1) {
        char buf[80];
        sprintf(buf, "ord() expected a character, but string of length %d found", s->length);
        raise_char(type_TypeError, buf);
    }
    c = s->text[0];
    return (R_object)R_int_from_ssize_t(c);
}

R_object PY_FUNC1(abs_float, R_float f) {
    double p0 = f->value;
    if (p0 < 0) {
        f = gc_allocate(float);
        SET_TYPE(f, type_float);
        f->value = -p0;
    }
    return (R_object)f;
}

R_object PY_FUNC1(callable, R_object object) {
    if (IS_CALLABLE(object)) {
        return (R_object)TRUE;
    } else {
        return (R_object)FALSE;
    }
}

R_object PY_FUNC1(repr, R_object obj) {
    return (R_object)HotPyObject_Repr(obj);
}

R_object PY_FUNC2(hasattr, R_object object, R_str name) {
    GVMT_Object ex;
    R_object result;
    TYPE_ASSERT(name, str);
    GVMT_TRY(ex)
        _HotPyObject_GetAttr(object, name);
        result = (R_object)TRUE;
    GVMT_CATCH
        result = (R_object)FALSE;
    GVMT_END_TRY
    return result;
}

R_object PY_FUNC2(getattr, R_object object, R_str name) {
    GVMT_Object ex;
    TYPE_ASSERT(name, str);
    return _HotPyObject_GetAttr(object, name);
}

R_object PY_FUNC2(delattr, R_object object, R_str name) {
    GVMT_Object ex;
    TYPE_ASSERT(name, str);
    HotPyObject_DelAttr(object, (R_object)name);
    return (R_object)None;
}

R_object PY_FUNC3(setattr, R_object object, R_str name, R_object value) {
    _HotPyObject_SetAttr(object, name, value);
    return (R_object)None;
}

R_object PY_FUNC1(str_to_int, R_str str) {
    int i, len;
    char *buf, *end;
    long l;
    TYPE_ASSERT(str, str);
    buf = alloca(str->length + 1);
    for (i = 0, len = str->length; i < len; i++)
        buf[i] = str->text[i];
    buf[len] = 0;
    errno = 0;
    l = strtol (buf, &end, 0);
    if (errno) {
        assert(0 && "To do");
        return 0;
    }
    if (end != (buf + str->length)) {
        R_str msg = py_sprintf("invalid literal for int(): %s", str);
        raise_str(type_ValueError, msg);
    }
    return (R_object)R_int_from_ssize_t(l);
}

R_object PY_FUNC0(dir_local) {
    // Need to locals for enclosing scope
    fatal("Not implemented");
    return 0;
}

R_object PY_FUNC1(dir_class, R_type t) {
    TYPE_ASSERT(t, type);
    // Get all property names from __mro__ classes
    // Sort them.
    fatal("Not implemented\n");
    return 0;
}

R_object PY_FUNC3(_if_else, R_bool cond, R_object t, R_object f) {
    return cond->value ? t : f;
}

