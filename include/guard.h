
void trigger_guard(R_guard g);

void add_attribute_guard(R_type t, R_str name, R_trace invalidate);

void add__guard(R_type t, R_trace invalidate);

void add_dict_value_guard(R_module_dict_values values, R_str key, R_trace t);

void add_dict_position_guard(R_module_dict_values values, R_str key, R_trace t);

/** Returns 1 on success, 0 on failure (if expected value is not current value */
int add_guard_for_type_attribute(R_type type, R_str name, R_trace trace, 
                                  R_object expected_value);

void add_sys_profile_guard(R_trace c);

void add_sys_trace_guard(R_trace c);

//void add_guard_for_function___code__(R_function f, R_trace t,
//                                     R_code_object expected_value);

