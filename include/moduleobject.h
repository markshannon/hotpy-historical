#ifndef HOTPY_MODULEOBJECT_H
#define HOTPY_MODULEOBJECT_H

R_module new_module(R_str name, R_str filename);

void save_global(R_module module, R_str name, R_object value);

R_object builtins_from_globals(R_object globals);

R_str module_getname(R_module m);

int guard_module_value(R_dict d, R_str name, R_object expected, R_trace trace);

int guard_module_deletion(R_dict d, R_str name, R_trace trace);

#endif                              
