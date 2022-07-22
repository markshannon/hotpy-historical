#include "hotpy.h"
#include "optimise.h"
#include "opcodes.h"
#include "specialiser.h"

#ifdef NDEBUG
# define DEBUG_INC(x) ((void)0)
# define DEBUG(x)
#else
# define DEBUG_INC(x) x++
# define DEBUG(x) x
#endif

struct _tracing_stats tracing_stats;

uint8_t __drop_return[] = {
    OPCODE(drop),
    OPCODE(return)
};

R_trace_record make_trace_record(uint8_t* start, R_code_object initial_code_object,
                                 R_type_state initial_state) {
    R_trace_record t = gc_allocate(trace_record);
    SET_TYPE(t, type_trace_record);
    t->start = start;
    t->allow_back_branch = 0;
    t->instructions = make_bytelist(128);
    t->info = make_list(128);
    t->initial_code_object = initial_code_object;
    t->pinned_objects = new_set();
    t->initial_state = initial_state;
    t->call_stack = make_list(4);
    return t;
}

static R_object info0(uint8_t* ip) {
    R_info_item0 info = gc_allocate(info_item0);
    SET_TYPE(info, type_info_item0);
    info->ip = ip;
    return (R_object)info;
}

static R_object info1(uint8_t* ip, R_object a) {
    R_info_item1 info = gc_allocate(info_item1);
    SET_TYPE(info, type_info_item1);
    info->ip = ip;
    info->value1 = a;
    return (R_object)info;
}

static R_object info2(uint8_t* ip, R_object a, R_object b) {
    R_info_item2 info = gc_allocate(info_item2);
    SET_TYPE(info, type_info_item2);
    info->ip = ip;
    info->value1 = a;
    info->value2 = b;
    return (R_object)info;
}

static R_object info3(uint8_t* ip, R_object a, R_object b, R_object c) {
    R_info_item3 info = gc_allocate(info_item3);
    SET_TYPE(info, type_info_item3);
    info->ip = ip;
    info->value1 = a;
    info->value2 = b;
    info->value3 = c;
    return (R_object)info;
}

void trace_overwrite_opcode(R_trace_record trace, uint8_t* ip, uint8_t op) {
    int inst_len = gvmt_opcode_lengths_interpreter[*ip];
    trace->instructions->array->bytes[trace->instructions->size-inst_len] = op;
}

void trace_drop_byte(R_trace_record trace) {
    bytelist_pop(trace->instructions);
    list_pop(trace->info);
    assert(LIST_SIZE(trace->info) == trace->instructions->size);
}

void trace_add_byte(R_trace_record trace, uint8_t b) {
    bytelist_append_nolock(trace->instructions, b);
    list_append_nolock(trace->info, (R_object)None);
    assert(LIST_SIZE(trace->info) == trace->instructions->size);
}

size_t trace_length(R_trace_record trace) {
    return trace->instructions->size;
}

static void add_info_to_trace(R_trace_record trace, uint8_t* ip, R_object info) {
    int inst_len = gvmt_opcode_lengths_interpreter[*ip];
    R_bytelist insts;
    int i;
    insts = trace->instructions;
    for (i = 0; i < inst_len; i++) {
        bytelist_append_nolock(insts, ip[i]);
    }
    list_append_nolock(trace->info, info);
    while (LIST_SIZE(trace->info) < trace->instructions->size)
        list_append_nolock(trace->info, (R_object)None);
    assert(LIST_SIZE(trace->info) == trace->instructions->size);
}

R_object get_trace_info(R_trace_record trace, int offset) {
    return ITEM(trace->info->array, offset);
}

void trace_write_addr(R_trace_record trace, void* addr) {
    assert(sizeof(int32_t) == sizeof(void*));
    bytelist_append_long(trace->instructions, (int32_t)addr);
    while (LIST_SIZE(trace->info) < trace->instructions->size)
        list_append_nolock(trace->info, (R_object)None);
    assert(LIST_SIZE(trace->info) == trace->instructions->size);
}

void trace_close_loop(R_trace_record trace, uint8_t op) {
    int offset;
    R_bytelist insts;
    insts = trace->instructions;
    offset = -insts->size;
    trace->is_loop = 1;
    bytelist_append_nolock(insts, op);
    bytelist_append_nolock(insts, offset >> 8);
    bytelist_append_nolock(insts, offset);
    list_append_nolock(trace->info, (R_object)None);
    list_append_nolock(trace->info, (R_object)None);
    list_append_nolock(trace->info, (R_object)None);
    assert(LIST_SIZE(trace->info) == trace->instructions->size);
}

void add_opcode_info(R_trace_record trace, uint8_t op, uint8_t* ip, R_object a) {
    R_object info = info1(ip, a);
    bytelist_append_nolock(trace->instructions, op);
    list_append_nolock(trace->info, info);
}

void add_to_trace0(R_trace_record trace, uint8_t* ip) {
    R_object info = info0(ip);
    add_info_to_trace(trace, ip, info);
}

void add_to_trace1(R_trace_record trace, uint8_t* ip, R_object a) {
    R_object info = info1(ip, a);
    add_info_to_trace(trace, ip, info);
}

void add_to_trace2(R_trace_record trace, uint8_t* ip, R_object a, R_object b) {
    R_object info = info2(ip, a, b);
    add_info_to_trace(trace, ip, info);
}

void add_to_trace3(R_trace_record trace, uint8_t* ip, R_object a, R_object b, R_object c) {
    R_object info = info3(ip, a, b, c);
    add_info_to_trace(trace, ip, info);
}

extern void disassemble_trace(uint8_t* start, uint8_t* end, R_list trace_info,
                              R_code_object initial_co, FILE* out);

void print_trace(FILE* out, R_trace_record trace) {
#ifdef BOOTSTRAP
    assert (0);
#else
    uint8_t* bytecodes;
    void *ptr = gvmt_pin((GVMT_Object)trace->instructions->array);
    bytecodes = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    disassemble_trace(bytecodes, bytecodes+trace->instructions->size,
                      trace->info, trace->initial_code_object, out);
#endif
}

void print_optimised_trace(FILE* out, R_bytelist instructions) {
#ifdef BOOTSTRAP
    assert (0);
#else
    uint8_t* bytecodes;
    void *ptr = gvmt_pin((GVMT_Object)instructions->array);
    bytecodes = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    fprintf(out, "Trace %x\n", bytecodes);
    disassemble_trace(bytecodes, bytecodes+instructions->size,
                      NULL, NULL, out);
#endif
}

#ifndef BOOTSTRAP
R_exec_link start_trace(uint8_t *ip, R_type_state state, R_thread_state ts, R_frame f) {
    assert(dict_get(ts->trace_cache, make_key_for_exit(ip, state)) == NULL);
    assert(ts->trace == NULL);
    tracing_stats.start_time = high_res_time();
    ts->trace = make_trace_record(ip, f->f_code, state);
    if (state) {
        ts->trace->call_depth = 0; //LEN(exit->state->back);
        tracing_stats.exit_starts++;
        DEBUG(if(trace_on) fprintf(stderr, "EXIT LOOKUP\n");)
    } else {
        tracing_stats.top_level_starts++;
        DEBUG_INC(trace_cache_top_level_lookups);
        DEBUG(if(trace_on) fprintf(stderr, "TOP LEVEL LOOKUP\n");)
    }
    tracing_stats.exit_starts++;
    EXEC_TRACING;
    return tracing_interpreter(ip, ts, f);
}

static R_trace get_trace(R_trace_record rec) {
    void* ptr = gvmt_pin((GVMT_Object)rec->instructions->array);
    R_trace t = gc_allocate(trace);
    SET_TYPE(t, type_trace);
    t->pinned_objects = rec->pinned_objects;
    t->invalidated = 0;
    t->guard_ring = NULL;
    return t;
}

R_specialiser specialise_trace(R_trace_record trace) {
    int i;
    R_specialiser spec;
    GVMT_Value *end_stack, *start_stack = gvmt_stack_top();
    R_bytelist insts = trace->instructions;
    R_bytelist bytes = make_bytelist(insts->size * 2);
    void* ptr = gvmt_pin((GVMT_Object)insts->array);
    uint8_t* start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    R_type_state tstate = trace->initial_state;
    spec = specialiser(start, trace, trace->initial_state, bytes);
    // Return the stack to its original state.
    end_stack = gvmt_stack_top();
    assert(end_stack < start_stack);
    gvmt_drop(start_stack - end_stack);
    if (debug_optimise) {
        fprintf(stderr, "\nSpecialised trace:\n");

        print_optimised_trace(stderr, spec->out);
    }
    ptr = gvmt_pin((GVMT_Object)bytes->array);
    start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    spec->out = post_specialise(start, start + bytes->size);
    if (debug_optimise) {
        fprintf(stderr, "\nPost-Specialised trace:\n");

        print_optimised_trace(stderr, spec->out);
    }
    return spec;
}

extern R_bytelist peephole(uint8_t* start, uint8_t* end);
extern R_bytelist deferred_gen(uint8_t* start, uint8_t* end, R_trace t);
extern R_object deferred_cleanup(uint8_t* start, uint8_t* end);

/** Local optimisations **/
R_bytelist peephole_optimise(R_bytelist b) {
    void* ptr = gvmt_pin((GVMT_Object)b->array);
    uint8_t* start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    return peephole(start, start+b->size);
}

R_bytelist deferred_codegen(R_bytelist b, R_trace t) {
    void* ptr = gvmt_pin((GVMT_Object)b->array);
    uint8_t* start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    if (debug_optimise) {
        fprintf(stderr, "\nDeferred Object Creation (pre-cleanup):\n");
        b = deferred_gen(start, start+b->size, t);
        print_optimised_trace(stderr, b);
        fprintf(stderr, "\nDeferred Object Creation:\n");
    } else {
        b = deferred_gen(start, start+b->size, t);
    }
    ptr = gvmt_pin((GVMT_Object)b->array);
    start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    deferred_cleanup(start, start+b->size);
    return b;
}

static R_trace improve_trace(R_trace_record trace) {
    R_bytelist bytes;
    void* ptr;
    R_trace result;
    R_specialiser spec;
    double t = high_res_time();
    if (optimiser_controls.specialise) {
        spec = specialise_trace(trace);
        result = spec->trace;
        bytes = spec->out;
    } else {
        bytes = trace->instructions;
        result = get_trace(trace);
    }
    if (optimiser_controls.defer) {
        if (debug_optimise) {
            bytes = deferred_codegen(bytes, result);
            print_optimised_trace(stderr, bytes);
        } else {
            bytes = deferred_codegen(bytes, result);
        }
    }
    if (optimiser_controls.peephole) {
        bytes = peephole_optimise(bytes);
        if (debug_optimise) {
            fprintf(stderr, "\nImproved trace:\n");
            print_optimised_trace(stderr, bytes);
        }
    }
    // bytearray is unhashable, so convert to hashable form.
    SET_TYPE(bytes->array, type_bytes);
    PY_set_add((R_object)bytes->array, result->pinned_objects);
    ptr = gvmt_pin((GVMT_Object)bytes->array);
    result->optimised = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    optimising_time += high_res_time() - t;
    return result;
}

void close_trace(R_thread_state ts, uint8_t* exit_ip, char* reason) {
    if (debug_optimise) {
        fprintf(stderr, "Closing trace: %s\n", reason);
    }
    trace_add_byte(ts->trace, OPCODE(interpret));
    trace_write_addr(ts->trace, exit_ip);
    completed_trace(ts);
}

void insert_trace_in_cache(R_thread_state ts, uint8_t* ip,
                           R_type_state state, R_trace trace) {
    R_object key = make_key_for_exit(ip, state);
    assert(dict_get(ts->trace_cache, key) == NULL);
    dict_set(ts->trace_cache, key, (R_object)trace);
}

R_trace completed_trace(R_thread_state ts) {
    R_trace_record rec = ts->trace;
    R_trace trace;
    R_object key;
    void *ptr;
    tracing_stats.total_time += high_res_time() - tracing_stats.start_time;
    tracing_stats.completed++;
    if (debug_optimise) {
        fprintf(stderr, "\nTrace completed:\n");
        fprintf(stderr, "Starting at %x\n", rec->start);
        if (rec->initial_state) {
            print_type_state(rec->initial_state, stderr);
            fprintf(stderr, "\n");
        } else {
            fprintf(stderr, "No initial state\n");
        }
        fprintf(stderr, "Call depth at end: %d\n", rec->call_depth);
        print_trace(stderr, rec);
    }
    trace = improve_trace(rec);
    insert_trace_in_cache(ts, rec->start, rec->initial_state, trace);
    ts->trace =  NULL;
    return trace;
}

void trace_setup_frame(R_trace_record trace, R_function func, R_tuple t, R_dict d) {
    R_object info = info1(NULL, (R_object)func);
    bytelist_append_nolock(trace->instructions, OPCODE(make_frame));
    list_append_nolock(trace->info, info);
    trace_add_byte(trace, 2);
    trace_add_byte(trace, OPCODE(init_frame));
    // TO DO ensure func is in pinned objects for trace.
}

R_function find_surrogate_for_new(R_type cls, R_object new_obj, R_object init_obj) {
    R_function surrogate;
    if (cls->surrogate_new) {
        surrogate = cls->surrogate_new;
    } else {
        surrogate = function_type_call;
    }
    if (surrogate->bytecodes == NULL)
        surrogate->bytecodes = get_bytecodes_from_code_object(surrogate->__code__);
    return surrogate;
}

R_frame enter_surrogate(R_function func, R_thread_state ts, uint8_t* next_ip) {
    R_frame frame;
    if (func->bytecodes == NULL)
        func->bytecodes = get_bytecodes_from_code_object(func->__code__);
    // Push new frame
    frame = function_frame(func, ts->current_frame);
    ts->current_frame->next_ip = next_ip;
    ts->current_frame = frame;
    return frame;
}

R_frame trace_enter_surrogate(R_function func, R_thread_state ts,
                           uint8_t* next_ip) {
    ts->trace->call_depth++;
    // Record trace.
    trace_add_byte(ts->trace, OPCODE(set_next_ip));
    trace_write_addr(ts->trace, next_ip);
    trace_add_byte(ts->trace, OPCODE(fast_frame));
    trace_add_byte(ts->trace, LEN(func->paramNames));
    trace_write_addr(ts->trace, gvmt_pin((GVMT_Object)func));
    return enter_surrogate(func, ts, next_ip);
}

#endif

