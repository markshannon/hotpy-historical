#ifndef OPTIMIZE_H
#define OPTIMIZE_H

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(info_item0) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uint8_t* ip;    /* pointer */
};

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(info_item1) { // special
    GVMT_OBJECT(type)* ob_type;    /*   read-only */
    uint8_t* ip;    /* pointer  */
    GVMT_OBJECT(object)* value1;
};

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(info_item2) { // special
    GVMT_OBJECT(type)* ob_type;    /*   read-only */
    uint8_t* ip;    /* pointer  */
    GVMT_OBJECT(object)* value1;
    GVMT_OBJECT(object)* value2;
};

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(info_item3) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uint8_t* ip;    /* pointer */
    GVMT_OBJECT(object)* value1;
    GVMT_OBJECT(object)* value2;
    GVMT_OBJECT(object)* value3;
};

GVMT_OBJECT(type_info) { // special
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uint32_t kind;
    GVMT_OBJECT(object)* value;
    GVMT_OBJECT(dict_keys)* keys;
};

GVMT_OBJECT(frozen_type_info) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint32_t kind;
    GVMT_OBJECT(object)* value;
    GVMT_OBJECT(dict_keys)* keys;
};

GVMT_OBJECT(trace) {  // special
    GVMT_OBJECT(type)* ob_type;  /* read-only */
    uintptr_t ref_count;
    uint32_t execution_count;
    uint32_t enqueued;
    uint8_t* optimised;
    exit_handler compiled;       /* pointer */
    uint32_t invalidated;
    GVMT_OBJECT(set)* pinned_objects;
    GVMT_OBJECT(guard)* guard_ring;
};

GVMT_OBJECT(exec_link) { // special
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uint32_t execution_count;
    exit_handler call;    /* pointer  */
    GVMT_OBJECT(type_state)* state;
};

GVMT_OBJECT(mono_exec_link) { // special
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uint32_t execution_count;
    exit_handler call;    /* pointer  */
    GVMT_OBJECT(type_state)* state;
    GVMT_OBJECT(trace)* trace;
    uint8_t* ip;    /* pointer  */
};

GVMT_OBJECT(poly_exec_link) { // special
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uint32_t execution_count;
    exit_handler call;    /* pointer  */
    GVMT_OBJECT(type_state)* state;
    GVMT_OBJECT(trace)* traces[4];
    uint8_t* ips[4];    /* pointer  */
    uint32_t execution_counts[4];
};

GVMT_OBJECT(trace_record) { // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint8_t* start;    /* pointer */
    int32_t call_depth;
    int32_t allow_back_branch;
    int8_t is_loop;
    GVMT_OBJECT(bytelist)* instructions;
    GVMT_OBJECT(list)* info;
    GVMT_OBJECT(code_object)* initial_code_object;
    GVMT_OBJECT(set)* pinned_objects;
    GVMT_OBJECT(type_state)* initial_state;
    GVMT_OBJECT(exec_link)* exit;
    GVMT_OBJECT(list)* call_stack;
};

extern int debug_optimise;

extern int tracing_instruction_counter;
extern int on_except_instruction_counter;
extern int on_trace_instruction_counter;
extern int off_trace_instruction_counter;
extern int exec_links_counter;
extern int trace_object_init_exits_counter;
extern int trace_cache_top_level_lookups;
extern int trace_cache_exit_lookups;
extern int trace_func_enter_exits_counter;
extern int trace_exception_exit_counter;
extern int trace_method_enter_exits_counter;
extern int trace_entry_counter;

struct _tracing_stats {
    int top_level_starts;
    int exit_starts;
    int completed;
    int abandoned;
    double start_time;
    double total_time;
};

extern struct _tracing_stats tracing_stats;

struct _optimiser_controls {
    int trace;
    int specialise;
    int defer;
    int peephole;
    int compile;
    int pre_spec;
};

extern struct _optimiser_controls optimiser_controls;

void null_slot_error(R_object object, int name_index);
R_base_function binary_lookup_exact(R_operator op, R_type t0, R_type t1);

void disassembler_pass(uint8_t* start, uint8_t* end, R_code_object code, FILE* out);

int is_constant(R_type_info t);

R_object fetch_global_if_likely_constant(R_dict globals, R_str name);

void disassemble(R_code_object c, FILE* out);

GVMT_Object interpret_deoptimised(uint8_t* ip);

extern double optimising_time;

void print_trace(FILE* out, R_trace_record trace);

R_trace completed_trace(R_thread_state ts);

void close_trace(R_thread_state ts, uint8_t* ip, char* reason);

void trace_close_loop(R_trace_record trace, uint8_t op);
void add_to_trace0(R_trace_record trace, uint8_t* ip);
void add_to_trace1(R_trace_record trace, uint8_t* ip, R_object a);
void add_to_trace2(R_trace_record trace, uint8_t* ip, R_object a, R_object b);
void add_to_trace3(R_trace_record trace, uint8_t* ip, R_object a, R_object b, R_object c);
void trace_overwrite_opcode(R_trace_record trace, uint8_t* ip, uint8_t op);
void trace_drop_byte(R_trace_record trace);
void trace_add_byte(R_trace_record trace, uint8_t b);
void trace_write_addr(R_trace_record trace, void* addr);
size_t trace_length(R_trace_record trace);
R_object get_trace_info(R_trace_record trace, int offset);
void add_opcode_info(R_trace_record trace, uint8_t op, uint8_t* ip, R_object a);
#define REQUEST_NONE 0
#define REQUEST_OPTIMISE_EXIT 1
#define REQUEST_ENTER_OPTIMISED 2

#ifdef TRACING
# define TRACE_OP(x) trace_overwrite_opcode(thread_state->trace, gvmt_ip(), x)
# define TRACE0 add_to_trace0(thread_state->trace, gvmt_ip())
# define TRACE1(a) add_to_trace1(thread_state->trace, gvmt_ip(), (R_object)a)
# define TRACE2(a,b) add_to_trace2(thread_state->trace, gvmt_ip(), (R_object)a, (R_object)b)
# define TRACE3(a,b,c) add_to_trace3(thread_state->trace, gvmt_ip(), (R_object)a, (R_object)b, (R_object)c)
# define TRACE_ADDR(a) trace_write_addr(thread_state->trace, a);

#else
# define TRACE_OP(x) ((void)0)
# define TRACE0 ((void)0)
# define TRACE1(a) ((void)0)
# define TRACE2(a,b) ((void)0)
# define TRACE3(a,b,c) ((void)0)
# define TRACE_ADDR(a) ((void)0)
#endif

R_object make_key_for_exit(uint8_t* ip, R_type_state state);
void trace_required_object(R_trace_record trace, R_object o);

R_exec_link mono_exit(R_thread_state ts, R_frame f, R_exec_link exit);
R_exec_link poly_exit(R_thread_state ts, R_frame f, R_exec_link exit);
R_exec_link interpret_exit(R_thread_state ts, R_frame f, R_exec_link exit);

void *make_mono_exit(uint8_t* target_ip, R_set pinned, R_type_state state);
void *make_interpret_exit(uint8_t* target_ip, R_set pinned);
void *make_poly_exit(R_set pinned, R_type_state state);

R_exec_link clone_exec_link(R_exec_link link);

R_exec_link execute_trace(R_trace trace, R_thread_state ts, R_frame f);

extern uint8_t __drop_return[];
#define DROP_RETURN &__drop_return[0];

#define write_int16(i, v) do { (i)[0] = ((v) >> 8) & 0xff; (i)[1] = (v) & 0xff; } while(0)

#define write_int32(i, v) do { (i)[0] = ((v) >> 24) & 0xff; (i)[1] = ((v) >> 16) & 0xff; \
                               (i)[2] = ((v) >> 8) & 0xff; (i)[3] = (v) & 0xff; } while(0)

void save_trace_in_cache(uint8_t* ip, R_type_state ts, R_trace t);

R_exec_link start_trace(uint8_t *ip, R_type_state state, R_thread_state ts, R_frame f);

exit_handler increase_trace_count_and_maybe_compile(R_thread_state ts, R_trace trace);

#ifdef NDEBUG
# define EXEC_OFF_TRACE ((void)0)
# define EXEC_ON_TRACE ((void)0)
# define EXEC_TRACING ((void)0)
# define EXEC_ON_EXCEPTION ((void)0)
#else
# define EXEC_OFF_TRACE instruction_counter = &off_trace_instruction_counter;
# define EXEC_ON_TRACE instruction_counter = &on_trace_instruction_counter;
# define EXEC_TRACING  instruction_counter = &tracing_instruction_counter;
# define EXEC_ON_EXCEPTION  instruction_counter = &on_except_instruction_counter;
#endif

R_bytelist prep_compile(uint8_t* ip, int insert_check);

R_function find_surrogate_for_new(R_type cls, R_object new_obj, R_object init_obj);

void trace_setup_frame(R_trace_record trace, R_function func, R_tuple t, R_dict d);

R_exec_link interpret_optimised(R_thread_state ts, R_frame f, R_exec_link exit);
R_exec_link interpret_optimised_direct(R_thread_state ts, R_frame f, R_exec_link exit);
R_exec_link interpret_optimised_then_compile(R_thread_state ts, R_frame f, R_exec_link exit);
R_exec_link interpret_optimised_then_compile_direct(R_thread_state ts, R_frame f, R_exec_link exit);

extern R_exec_link DUMMY_LINK;

void unlink_trace(R_exec_link link);

/* Returns the new current-frame */
R_frame trace_enter_surrogate(R_function func, R_thread_state ts,
                           uint8_t* next_ip);

/* Returns the new current-frame */
R_frame enter_surrogate(R_function func, R_thread_state ts, uint8_t* next_ip);

#define PRE_SPECIALISE(o, n) \
do { \
    void *exit, *addr; \
    if (trace_length(thread_state->trace)) \
        exit = make_mono_exit(gvmt_ip(), thread_state->trace->pinned_objects, NULL); \
    else \
        exit = make_interpret_exit(gvmt_ip(), thread_state->trace->pinned_objects); \
    addr = gvmt_pin((GVMT_Object)TYPE(o)); \
    if (n == 1) { \
        trace_add_byte(thread_state->trace, OPCODE(type_ensure)); \
    } else { \
        assert(n == 2); \
        trace_add_byte(thread_state->trace, OPCODE(type_ensure2)); \
    } \
    trace_write_addr(thread_state->trace, addr); \
    trace_write_addr(thread_state->trace, exit); \
} while (0)

R_bytelist peephole_optimise(R_bytelist b);

extern int hotpy_low_level;

#endif // OPTIMIZE_H
