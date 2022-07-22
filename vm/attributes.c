#include <assert.h>
#include <string.h>
#include "hotpy.h"
#include "dictionary.h"
#include "attributes.h"
#include "function.h"

/** Atribute stuff.
 * Getting, setting and deleting attributes */

void attribute_error(R_type klass, R_object oname) {
    R_str cname = klass->__name__;
    R_str name = (R_str)HotPyObject_Repr(oname);
    R_str msg;
    TYPE_ASSERT(name, str);
    msg = py_sprintf("'%s' object has no attribute %s", cname, name);
    raise_str(type_AttributeError, msg);
}

R_object vm_call_special(R_object obj, R_str name) {
    R_type klass = TYPE(obj);
    R_object d = class_attribute(klass, name);
    if (d) {
        call_func call = CALLABLE(d);
        R_tuple t = make_tuple(1);
        ITEM(t, 0) = obj;
        return call(d, t, 0);
    }
    return 0;
}
