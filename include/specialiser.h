#ifndef HOTPY_SPECIALISER_H
#define HOTPY_SPECIALISER_H

GVMT_OBJECT(specialiser) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint8_t* next_ip;    /* pointer */
    int32_t* invalidated_address;    /* pointer */
    uint32_t no_overflows;
    GVMT_OBJECT(trace)* trace;
    GVMT_OBJECT(bytelist)* out;
    GVMT_OBJECT(dict)* guards;
    GVMT_OBJECT(array)* frame;
    GVMT_OBJECT(tuple)* frozen_back;
    GVMT_OBJECT(list)* back;
    GVMT_OBJECT(list)* back_ips;
    GVMT_OBJECT(bytearray)* uninitialised_loads;
};


void print_type_info(FILE* out, R_type_info ti);

int is_constant(R_type_info t);
int is_definite(R_type_info t);
int is_definite_tagged(R_type_info t);
int is_tagged(R_type_info t);
int is_probable_tagged(R_type_info t);
int is_initialised(R_type_info t);

R_type get_type_from_info(R_type_info info);

/** Returns 1 if t1 is strictly wider than t2 */
int type_info_is_wider(R_type_info t1, R_type_info t2);
int type_info_contadictory(R_type_info t1, R_type_info t2);

/** type_info constructors */
R_type_info info_uninitialised(void);
R_type_info info_not_known(void);
R_type_info probable_type(R_type type);
R_type_info definite_type(R_type type);
R_type_info type_info_constant(R_object obj);
R_type_info info_slice(R_type_info s1, R_type_info s2, R_type_info s3);

R_type_info type_info_copy(R_type_info info);

R_type_info definite_tagged(void);

R_type_info info_bound_method(R_type_info object, R_type_info callable);

R_type_info bound_method_info_get_object(R_type_info t);

R_type_info bound_method_info_get_callable(R_type_info t);

int is_canonical(R_type_info info);
R_tuple freeze_frame(R_array frame, uint8_t* ip);
R_array thaw_frame(R_tuple frame);

/** Makes a type-info from a tuple of type-infos.
 * Acts as a definite-type tuple, but can be unpacked
 * into seperate type-infos
 */
R_type_info pack_info(R_tuple t);

void unpack_info(R_type_info, int n, GVMT_Value* array);

R_type_info tuple_prepend_info(R_type_info t1, R_type_info t2);

R_type_info tuple_concat_info(R_type_info t1, R_type_info t2);

/** If this is a definite then do not change.
 *  If a probable type - make it definite.
 *  If a probable value then turn it into a definite type (with the value's type)
 */
void make_type_definite(R_type_info info);

void type_info_make_constant(R_type_info info, R_object obj);

void type_info_make_variable(R_type_info info);

/** Modifies this type_info according to the observed type *
 *  Returns 0 if a clash in types, 1 if otherwise */
int with_observed_value(R_type_info info, R_object observed);

/** Modifies this type_info according to the observed type *
 *  Returns 0 if a clash in types, 1 if otherwise */
int with_observed_type(R_type_info info, R_type observed);

#define COPY do { \
    int i, n = gvmt_opcode_lengths_interpreter[gvmt_opcode()]; \
    for(i = 0; i < n; i++) { \
        bytelist_append_nolock(specialiser->out, gvmt_ip()[i]); \
    } \
} while (0)

int match_expected_type_tos(R_type_info i, R_type_info nos, R_info_item2 t, R_specialiser spec);

int match_expected_type_nos(R_type_info i, R_info_item3 t, R_specialiser spec);

int match_expected_types_binary(R_type_info l, R_type_info r,
                                R_info_item3 t, R_specialiser spec);

R_type_info optimise_unary_operator(int index, R_type_info i,
                                    R_specialiser spec, uint8_t* original_ip);

R_type_info optimise_binary_operator(int index, R_type_info l, R_type_info r,
                                     R_specialiser spec, uint8_t* original_ip);

R_type_info optimise_inplace_operator(int index, R_type_info l, R_type_info r,
                                      R_specialiser spec, uint8_t* original_ip);

R_type_info optimise_constant(R_object value, R_bytelist out);

R_type_info specialise_f_call(R_type_info callable, R_type_info t, R_type_info d,
                             R_specialiser spec, R_info_item3 ti);

R_type_info specialise_truth(R_type_info o, R_specialiser spec, R_info_item1 ti);

R_type_info specialise_load_special(R_type_info info, R_str name,
                                   R_specialiser spec, uint8_t* original_ip);

R_type_info specialise_has_special(R_type_info info, R_str name,
                                   R_specialiser spec, uint8_t* original_ip);

R_type_info specialise_has_dict(R_type_info info, R_specialiser spec,
                                uint8_t* original_ip);

R_type_info specialise_object_dict_get(R_type_info info, R_str name,
                                   R_specialiser spec, uint8_t* original_ip);

R_type_info specialise_get_class_attr(R_type_info obj, R_type_info key,
                                   R_specialiser spec);

R_type_info specialise_has_class_attr(R_type_info obj, R_type_info key,
                                   R_specialiser spec);

R_type_info specialise_value_in_object_dict_or_exit(R_type_info obj, R_str name,
                               R_specialiser spec, uint8_t* ip);

R_type_info specialise_load_slot(R_type_info desc, R_type_info obj,
                                 R_specialiser spec);

void specialise_store_slot(R_type_info desc, R_type_info obj,
                           R_specialiser spec);

void specialise_value_not_in_object_dict_or_exit(R_type_info obj, R_str name,
                               R_specialiser spec, uint8_t* ip);

void specialise_set_in_object_dict(R_type_info obj, R_type_info key,
                               R_specialiser spec, uint8_t* original_ip);

void insert_deoptimise_check(R_specialiser spec, uint8_t* exit);

R_type_info optimise_load_global(R_str name, R_specialiser spec, R_object globals, R_object builtins, uint8_t* original_ip);

R_type_info optimise_store_global(R_str name, R_specialiser spec, R_object globals, uint8_t* original_ip);

/** Raise an exception to terminate specialisation */
#define not_worth_optimising() raise_char(type_ValueError, "")

/** Returns the size of the tuple if type-info represents a tuple of known size.
 *  Otherwise returns -1;
 */
int info_tuple_size(R_type_info t);

R_type_info tuple_get_item_info(R_type_info t, int index);

void info_frame_init_from_tuple(R_array frame, R_function func, R_type_info t);

R_bytelist post_specialise(uint8_t* start, uint8_t* end);

R_specialiser specialiser(uint8_t* start, R_trace_record t, R_type_state state, R_bytelist bytes);

R_specialiser make_specialiser(R_trace_record t, R_bytelist bytes);

R_array unknown_frame(size_t params, size_t len);

R_object specialiser_trace(uint8_t* ip);

GVMT_Object print_specialiser_stack(int items, ...);

/** Creates a type-state from the frame & back-frame. Excepting 'except'
 *  in the frame, which is set to "not known" */
R_type_state make_type_state(R_array frame, R_tuple back, R_type_info except);

void write_line_number(int line_number, R_bytelist out);

int ensure_new_and_init(R_specialiser spec, R_type cls, uintptr_t new_addr, uintptr_t init_addr);

void ensure_local_type(R_specialiser spec, uint8_t* ip, int index, R_type_info wanted);

int ensure_class_attribute(R_specialiser spec, R_type t,
                           R_str name, R_object expected);

int ensure_type_tos(R_type_info info, uint8_t* ip, R_specialiser spec);
int ensure_type_nos(R_type_info info, uint8_t* ip, R_specialiser spec);

char* write_abort_node(char* out, void* trace_addr, int index, char* label);

char* intra_edge(char* out, void* trace_addr, int from, int to, char* label);

char* inter_edge(char* out, void* trace_addr, int index, void* target_addr, char* label);

char* write_exit_node(char* out, void* trace_addr, int index, char* label, int exit_index, R_set to_do, R_dict cache);

void print_type_state(R_type_state state, FILE* out);

char* print_type_state_flow(R_type_state state, char* out);

int dict_str_in_keys(R_dict_keys keys, R_str key);

extern R_object PY_FUNC0(array_getitem_int);
extern R_object PY_FUNC0(array_setitem_int);

#endif
