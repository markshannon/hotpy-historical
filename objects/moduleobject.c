#include "hotpy.h"    
#include "guard.h" 

void save_global(R_module module, R_str name, R_object value) {
    R_dict d;
    TYPE_ASSERT(module, module);
    TYPE_ASSERT(name, str);
    d = module->dictionary;
    dict_set_str(d, name, value);
}
   
R_module new_module(R_str name, R_str filename) {
    R_module result = gc_allocate(module);
    R_dict dict = new_module_dict(16);
    SET_TYPE(result, type_module);
    result->dictionary = dict;
    result->filename = filename;
    dict_set_str(dict, name___name__, (R_object)name);
    dict_set_str(dict, name___doc__, (R_object)None);
    dict_set_str(dict, name___package__, (R_object)None);
    return result;
}

R_object builtins_from_globals(R_object globals) {
    R_object b;
    if (IS_DICT_EXACT(globals)) {
        b = dict_get_str((R_dict)globals, name___builtins__);
    } else {
        b = HotPyObject_GetItem(globals, (R_object)name___builtins__);
    }
    if (b == NULL) 
        return (R_object)builtins->dictionary;
    else
        return b;
}

R_str module_getname(R_module m) {
    R_str name = (R_str)_HotPyObject_GenericGetAttr((R_object)m, name___name__);
    if (name == NULL || !isinstance(name, type_str)) {
        name = single_character('?');
    }
    return name;
}

R_object PY_FUNC1(module_repr, R_module m) {
    R_str name = module_getname(m);
    return (R_object)py_sprintf("<module '%s' from '%s'>", name, m->filename);
}

int guard_module_value(R_dict d, R_str name, R_object expected, R_trace trace) {
    R_module_dict_values values;
    R_object val;
    LOCK_DICT(d);
    values = (R_module_dict_values)d->values;
    val = dict_get_str(d, name);
    if (val == expected) {
        add_dict_value_guard(values, name, trace);
        UNLOCK_DICT(d);
        return 1;
    } else {
        UNLOCK_DICT(d);
        return 0;
    }
}

int guard_module_deletion(R_dict d, R_str name, R_trace trace) {
    R_module_dict_values values;
    R_object val;
    LOCK_DICT(d);
    values = (R_module_dict_values)d->values;
    val = dict_get_str(d, name);
    if (val != NULL) {
        add_dict_position_guard(values, name, trace);
        UNLOCK_DICT(d);
        return 1;
    } else {
        UNLOCK_DICT(d);
        return 0;
    }
}


