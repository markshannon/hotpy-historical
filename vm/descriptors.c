#include <assert.h>
#include "dictionary.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "hotpy.h"
#include "core_objects.h"
#include "attributes.h"
#include "function.h"
#include "descriptors.h"
#include "intobject.h"

/** Descriptor get, set, call and functions for special "descriptor"
 *  objects, such as slot-descriptors and the __call__ attribute of callables.
 */

R_object not_callable(R_object object, R_tuple tuple, R_dict dict) {
    R_str cname = TYPE_NAME(object);
    raise_str(type_TypeError,  py_sprintf("'%s' object is not callable", cname));
    return NULL;
}

/************* Slots ***********/

#define SLOT(t, o, d) (*((t*)(((char*)(o))+(d)->offset)))

R_object desc_get_slot_descriptor(R_object descriptor, R_object obj, R_type cls) {
    R_slot_descriptor sd = (R_slot_descriptor)descriptor;
    if (obj) {
        R_object result;
        assert(!gvmt_is_tagged(obj));
        result = SLOT(R_object, obj, sd);
        TYPE_ASSERT(sd, slot_descriptor);
        if (result == NULL) {
            attribute_error(TYPE(obj), (R_object)sd->name);
        }
        return result;
    } else {
        return descriptor;
    }
}

void desc_del_slot_descriptor(R_object descriptor, R_object obj) {
    R_slot_descriptor sd = (R_slot_descriptor)descriptor;
    R_object old;
    TYPE_ASSERT(sd, slot_descriptor);
    old = SLOT(R_object, obj, sd);
    if (old == NULL) {
        attribute_error(TYPE(obj), (R_object)sd->name);
    }
    if (sd->flags & SLOT_WRITEABLE) {
        if (sd->flags & SLOT_NOT_NULL) {
            raise_str(type_AttributeError, py_sprintf("attribute %s cannot be deleted", sd->name));
        }
        SLOT(GVMT_Object, obj, sd) = 0;
    } else {
        raise_str(type_AttributeError, py_sprintf("%s is a read-only attribute", sd->name));
    }
}

void desc_set_slot_descriptor(R_object descriptor, R_object obj, R_object value) {
    R_slot_descriptor sd = (R_slot_descriptor)descriptor;
    TYPE_ASSERT(sd, slot_descriptor);
    assert(value);
    if (sd->flags & SLOT_WRITEABLE) {
        SLOT(R_object, obj, sd) = value;
        if (sd->type == NULL || isinstance(value, sd->type)) {
            SLOT(R_object, obj, sd) = value;
        } else {
            raise_str(type_AttributeError, py_sprintf("attribute %s must be of type %s", sd->name, TYPE_NAME(sd->type)));
        }
    } else {
        raise_str(type_AttributeError, py_sprintf("%s is a read-only attribute", sd->name));
    }
}

/******  Int slots *****/
// If doing tagging, then these should be improved

R_object desc_get_int_slot_descriptor(R_object descriptor, R_object obj, R_type cls) {
    int val;
    R_int_slot_descriptor isd = (R_int_slot_descriptor)descriptor;
    TYPE_ASSERT(isd, int_slot_descriptor);
    val = SLOT(intptr_t, obj, isd);
    return (R_object)R_int_from_ssize_t(val);
}

void desc_set_int_slot_descriptor(R_object descriptor, R_object obj, R_object value) {
    R_int_slot_descriptor isd = (R_int_slot_descriptor)descriptor;
    R_int val;
    if (value == 0)
        raise_str(type_AttributeError, py_sprintf("attribute %s cannot be deleted", isd->name));
    TYPE_ASSERT(isd, int_slot_descriptor);
    val = (R_int)value;
    if (!IS_INT(val))
        val = HotPyNumber_Index((R_object)value);
    if (isd->flags & SLOT_WRITEABLE)
        SLOT(intptr_t, obj, isd) = int_from_py_int(val);
    else
        raise_str(type_AttributeError, py_sprintf("%s is a read-only attribute", isd->name));
}

void desc_del_int_slot_descriptor(R_object descriptor, R_object obj) {
    R_int_slot_descriptor isd = (R_int_slot_descriptor)descriptor;
    raise_str(type_AttributeError, py_sprintf("attribute %s cannot be deleted", isd->name));
}

/******  Class slot *****/

R_object desc_get_class_descriptor(R_object descriptor, R_object obj, R_type cls) {
    if (obj) {
        if (gvmt_is_tagged(obj)) {
            return (R_object)type_int;
        } else {
            return (R_object)obj->ob_type;
        }
    } else {
        return (R_object)cls->ob_type;
    }
}

void desc_set_class_descriptor(R_object descriptor, R_object obj, R_object value) {
    /** Need to fix this. */
    raise_char(type_AttributeError, "'__class__' is a read-only attribute");
}

void desc_del_class_descriptor(R_object descriptor, R_object obj) {
    raise_char(type_AttributeError, "'__class__' attribute cannot be deleted");
}

///******* callable.__call__ *******/
DESC_CALL_SIGNATURE(desc_call_call_descriptor) {
    R_object obj = (R_object)ITEM(tuple, 0);
    call_func call = CALLABLE(obj);
    return call(obj, tuple, dict);
}

R_object desc_get_call_descriptor(R_object descriptor, R_object obj, R_type cls) {
    return obj;
}

void desc_set_call_descriptor(R_object descriptor, R_object obj, R_object value) {
    raise_str(type_AttributeError, py_sprintf("__call__ is a read-only attribute"));
}



