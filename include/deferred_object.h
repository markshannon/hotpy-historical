#ifndef HOTPY_DEFERRED_OBJECT_H
#define HOTPY_DEFERRED_OBJECT_H

GVMT_OBJECT(deferred_frame) {   // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t line_number;
    uint8_t* ip;    /* pointer  */
    int no_trace;
    GVMT_OBJECT(tuple)* locals;
    GVMT_OBJECT(function)* function;
    GVMT_OBJECT(list)* protect_stack;
};

GVMT_OBJECT(deferred_state) {  // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t cached_objects;
    int32_t line_number;
    uint8_t* next_ip;    /* pointer  */
    GVMT_OBJECT(list)* frame_stack;
    GVMT_OBJECT(list)* deferred_stack;
    GVMT_OBJECT(list)* protect_stack;
};

GVMT_OBJECT(deferred) {   // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t kind;
    GVMT_OBJECT(type)* type;
    GVMT_OBJECT(object)* value;
};

extern int64_t load_cache_use(uint8_t* start, R_object dummy);

R_deferred_state make_deferred_state(void);

/** Gets local n, lazily */
R_deferred deferred_local(int n);

/** Gets local n, from the deferred frame */
R_deferred deferred_frame_load_local(R_deferred_frame frame, int n);

/** Stores local n to the deferred frame */
void deferred_frame_store_local(R_deferred_frame frame, int n, R_deferred value);

/** Sets type of l. If type info is redundant return 0 otherwise return 1.
 */
int deferred_ensure(R_deferred l, R_type type);

/**Initialise frame from func, t & d. Return 1 if successful, 0 otherwise */
int deferred_init_frame(R_deferred_frame frame, R_deferred func, R_deferred t, R_deferred d);

/** Returns 1 if deferred is known to be tagged, 0 otherwise.
 */
int deferred_is_tagged(R_deferred l);

/** Return a deferred object for l1 is l2 */
R_deferred deferred_is(R_deferred l1, R_deferred l2);

/** Swap tos and nos */
void deferred_swap(R_deferred_state state, R_bytelist out);

/** Returns TRUE if comparison of l1 and l2 is true,
 *  FALSE if known to be false.
 * NULL if not known. */
R_bool deferred_compare(R_deferred l1, R_deferred l2, int opcode);

/** If tuple is deferred-tuple, unpack onto stack and return 1.
 *  Otherwise do nothing and return 0 */
int deferred_unpack(R_deferred tuple, R_deferred_state state);

/** Lazily prepend obj to tuple, return new tuple or NULL if fails */
R_deferred deferred_tuple_prepend(R_deferred obj, R_deferred tuple);

/** Lazily unpack parameters, provided that they match func. Return 1 if successful, 0 otherwise */
int deferred_native_params(R_builtin_function bf, R_deferred t, R_deferred d, R_deferred_state state);

/** Extract slot at offset from deferred l. Return NULL if cannot be done lazily. */
R_deferred deferred_slot(R_deferred l, unsigned offset);

/** Bind object and method. Return NULL if cannot be done lazily. */
R_deferred deferred_bind(R_deferred object, R_deferred method);

/** Unbind object from bound-method. Return NULL if cannot be done lazily. */
R_deferred deferred_unbind(R_deferred bm);

void deferred_prepare_bm_call(R_deferred_state state, R_bytelist out, uint8_t* ip);

/** Make a slice from lo, hi and step. Return NULL if cannot be done lazily. */
R_deferred deferred_slice(R_deferred lo, R_deferred hi, R_deferred step);

/** Make a tuple from the top n values on stack.
 *  Return NULL if cannot be done lazily. */
R_deferred deferred_tuple(int n, R_deferred_state state, R_bytelist out);

/** Make a tuple by concatenating t1 & t2.
 *  Return NULL if cannot be done lazily. */
R_deferred deferred_tuple_concat(R_deferred t1, R_deferred t2);

/** Copy dict d.
 *  Return NULL if cannot be done lazily. */
R_deferred deferred_copy_dict(R_deferred d);

/** Make a frame for the given function. Return NULL if cannot be done lazily. */
R_deferred_frame deferred_frame(R_function func);

/** Make a frame for the given deferred, provided it is a function.
  * Return NULL if cannot be done lazily. */
R_deferred_frame deferred_make_frame(R_deferred func);

/** Get an item from a sequence. Return NULL if cannot be done lazily. */
R_deferred deferred_getitem(R_deferred object, R_deferred index);

/** Return a deferred version of the constant */
R_deferred const_as_deferred(R_object value);

R_deferred deferred_empty_dict(void);

R_deferred deferred_type(R_deferred l);

/** Materialise the deferred object */
void materialise(R_deferred deferred, R_bytelist output);

/** Setitem, materialising objects as necessary */
void deferred_setitem(R_deferred object, R_deferred index, R_deferred value, R_bytelist output);

void materialise_stack(R_deferred_state state, R_bytelist out, uint8_t* ip);

/** Make the top n items on the stack all deferred or all immediate */
void homogenise_tos(int n, R_deferred_state state, R_bytelist out);

int is_tos_deferred(R_deferred_state state);

void deferred_drop(R_deferred_state state, R_bytelist out);

void deferred_drop_under(R_deferred_state state, R_bytelist out);

R_deferred deferred_pop(R_deferred_state state);

R_deferred deferred_peek(R_deferred_state state);

R_deferred deferred_pick(R_deferred_state state, int depth);

void deferred_poke(R_deferred_state state, int depth, R_deferred obj);

void deferred_pop_immediate(R_deferred_state state);

void deferred_push_immediate(R_deferred_state state);

R_deferred_frame deferred_pop_frame(R_deferred_state state);

R_deferred_frame deferred_peek_frame(R_deferred_state state);

void deferred_push_frame(R_deferred_state state, R_deferred_frame frame);

void materialise_frames(R_deferred_state state, R_bytelist out, uint8_t* ip);

/** Force any immediate value on top of stack to be saved as deferred values. */
void make_deferred(R_deferred_state state, R_bytelist out);

/** Materialise the top n items on the stack */
void materialise_top(int n, R_deferred_state state, R_bytelist out);

/** Force the top n items on the stack to be deferred */
void make_tos_deferred(int n, R_deferred_state state, R_bytelist out);

void deferred_push(R_deferred_state state, R_deferred d);

void materialise_current_frame(R_deferred_state state, R_bytelist out);

void* materialise_state_on_exit(R_deferred_state state, R_exec_link exit, R_set pinned);

uint8_t*  materialise_state_on_interpreter_exit(R_deferred_state state, uint8_t* exit, R_set pinned);

uint8_t* materialise_on_exception(R_deferred_state state, R_set pinned_objects);

void materialise_state(R_deferred_state state, R_bytelist out, uint8_t* ip);

/** Materialises the stack iff it depends on the frames */
void deferred_remove_stack_to_frame_dependencies(R_deferred_state state,
                                          R_bytelist out, uint8_t* ip);

/** Moves items to cache iff deferred frame requires them */
void deferred_remove_frame_to_frame_dependencies(R_deferred_state state,
                                          R_bytelist out, uint8_t* ip);

int match_builtin_function(R_deferred func, R_deferred t, R_deferred d, R_builtin_function bf);

/* Cached object handling. */

/** Store tos into cache and returns deferred object.  */
R_deferred deferred_cache_store_item(R_deferred_state state, R_bytelist out);

/** Returns 1 if there is space in the deferred-cache, 0 otherwise */
int space_in_deferred_cache(R_deferred_state state);

R_deferred deferred_cached_item(int index);

R_deferred deferred_i2f(R_deferred l);

R_deferred deferred_f_add(R_deferred l, R_deferred r);

R_deferred deferred_f_sub(R_deferred l, R_deferred r);

R_deferred deferred_f_mul(R_deferred l, R_deferred r);

R_deferred deferred_f_div(R_deferred l, R_deferred r);

R_deferred deferred_i_div(R_deferred l, R_deferred r);

R_deferred deferred_f_neg(R_deferred f);

void print_deferred_stack(R_deferred_state state, FILE* out);

int no_deferred_frames(R_deferred_state state);

int deferred_is_constant(R_deferred d, R_object c);
#endif

