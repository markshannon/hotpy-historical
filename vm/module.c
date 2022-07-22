#include "hotpy.h"
#include "dictionary.h"
#include "core_objects.h"
#include <assert.h>

R_module builtins = &module_builtins;
R_module sys = &module_sys;

/***** Module internals and global variable stuff */


void name_not_defined(R_str name) {
    raise_str(type_NameError, py_sprintf("name %s is not defined", name));  
}

R_object fetch_global_if_likely_constant(R_dict globals, R_str name) {
    R_object value = dict_get_str(globals, name);
    R_type cls;
    if (value == 0) {
        // Look in builtins
        R_object builtins = builtins_from_globals((R_object)globals);
        if (TYPE(builtins) != type_dict)
            return 0;
        value = dict_get_str((R_dict)builtins, name);
        if (value == 0)
            return 0;
    }
    cls = TYPE(value);
    if (cls == type_type || cls == type_function || cls == type_builtin_function)
        return value;
    return 0;
}

