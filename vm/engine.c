#include "hotpy.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <alloca.h>
#include "function.h"
#include "optimise.h"
#include "opcodes.h"

#ifdef NDEBUG
# define DEBUG_INC(x) ((void)0)
# define DEBUG(x)
#else
# define DEBUG_INC(x) x++
# define DEBUG(x) x
#endif

/** Main entry point to HotPy */

extern void os_init(void);
extern print_flowgraph(R_thread_state ts, int id);

int* instruction_counter;
int tracing_instruction_counter;
int on_except_instruction_counter;
int on_trace_instruction_counter;
int off_trace_instruction_counter;
int exec_links_counter;
int trace_object_init_exits_counter;
int trace_cache_top_level_lookups;
int trace_cache_exit_lookups;
int trace_exception_exit_counter;
int trace_func_enter_exits_counter;
int trace_method_enter_exits_counter;
int trace_entry_counter;
int return_exit_counter;
static int compiler_counter;
static int compiled_bytes_counter;

extern int is_canonical(R_type_info i);

R_object make_key_for_exit(uint8_t* ip, R_type_state state) {
    if (state) {
        R_tuple key = make_tuple(2);
        ITEM(key, 0) = (R_object)R_int_from_ssize_t((intptr_t)ip);
        ITEM(key, 1) = (R_object)state;
        return (R_object)key;
    } else {
        uintptr_t i = (uintptr_t)ip;
        return (R_object)R_int_from_ssize_t(i);
    }
}

static R_trace find_trace_in_cache(R_thread_state ts, uint8_t* ip, R_type_state state) {
    R_object key = make_key_for_exit(ip, state);
    DEBUG_INC(trace_cache_exit_lookups);
    return (R_trace)dict_get(ts->trace_cache, key);
}

void unlink_trace(R_exec_link link) {
    assert("TO DO -- Fix unlink" && 0);
//    R_trace trace = link->trace;
//    assert(trace->invalidated && "Calling unlink_trace on valid trace");
//    link->execution_count = 0;
//    link->trace = NULL;
}

R_exec_link interpret_optimised(R_thread_state ts, R_frame f, R_exec_link exit) {
    R_trace trace = ((R_mono_exec_link)exit)->trace;
    DEBUG(if(trace_on) fprintf(stderr, "optimised exit\n");)
    TYPE_ASSERT(exit, mono_exec_link);
    exit->execution_count++;
    if (trace->invalidated) {
        exit->execution_count = 0;
        ((R_mono_exec_link)exit)->trace = NULL;
        return interpreter(((R_mono_exec_link)exit)->ip, ts, f);
    } else {
        EXEC_ON_TRACE;
        return interpreter(trace->optimised, ts, f);
    }
}

R_exec_link interpret_optimised_direct(R_thread_state ts, R_frame f, R_exec_link exit) {
    TYPE_ASSERT(exit, mono_exec_link);
    exit->execution_count++;
    EXEC_ON_TRACE;
    return interpreter(((R_mono_exec_link)exit)->ip, ts, f);
}

extern print_optimised_trace(FILE* out, R_bytelist insts);

static exit_handler do_compilation(uint8_t* ip, R_bool insert_check) {
    exit_handler compiled_code;
    void* ptr;
    uint8_t* start;
    char name[30];
    R_bytelist bytes = prep_compile(ip, insert_check == TRUE);
    if (debug_optimise) {
        fprintf(stderr, "\nPrepped for compile:\n");
        print_optimised_trace(stderr, bytes);
    }
    ptr = gvmt_pin((GVMT_Object)bytes->array);
    start = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    sprintf(name, "Trace-%x", ip);
    compiled_code = (exit_handler)jit_compile(start, start+bytes->size, name);
    compiler_counter++;
    compiled_bytes_counter += bytes->size;
    return compiled_code;
}

R_list compiler_queue = NULL;
static int prods = 0;

static void add_to_compiler_queue(R_object object) {
    if (compiler_queue == NULL)  {
        compiler_queue = make_list(16);
    }
    list_append(compiler_queue, object);
    ((R_trace)object)->execution_count |= INT_MIN;
    //fprintf(stderr, "Compile queue length: %d\n", LIST_SIZE(compiler_queue));
}

static void do_decay(R_thread_state ts) {
    int i, n, count;
    for (i = 0, n = LIST_SIZE(compiler_queue); i < n; i++) {
        R_object o = ITEM(compiler_queue->array, i);
        count = ((R_trace)o)->execution_count & INT_MAX;
        ((R_trace)o)->execution_count = INT_MIN | (count >> 2);
    }
}

static void prod_compiler(R_thread_state ts) {
    unsigned count, i, at, n;
    R_object last, next = NULL;
    double exec_time, compile_time, current_time;
    static double last_decay = 0;
    current_time = high_res_time();
    exec_time = current_time - ts->start_time;
    if (compiler_queue == NULL || LIST_SIZE(compiler_queue) == 0) {
        return;
    }
    if (current_time - last_decay > 0.02) {
        last_decay = current_time;
        do_decay(ts);
    }
    compile_time = gvmt_total_compilation_time / 1000000000.0;
    prods++;
    if (exec_time * 0.25 <= compile_time + 0.02) {
        return;
    }
    next = ITEM(compiler_queue->array, 0);
    count = ((R_trace)next)->execution_count;
    at = 0;
    for (i = 1, n = LIST_SIZE(compiler_queue); i < n; i++) {
        R_object o = ITEM(compiler_queue->array, i);
        if (((R_trace)o)->execution_count > count) {
            at = i;
            count = ((R_trace)o)->execution_count;
            next = o;
        }
    }
    assert(next);
    LIST_SIZE(compiler_queue)--;
    last = ITEM(compiler_queue->array, LIST_SIZE(compiler_queue));
    ITEM(compiler_queue->array, at) = last;
    if (TYPE(next) == type_trace) {
        R_trace trace = (R_trace)next;
        trace->compiled = do_compilation(trace->optimised, TRUE);
        //fprintf(stderr, "Compiled trace");
    } else {
        assert("Cannot compile exec_link" && 0);
        //R_exec_link exit = (R_exec_link)next;
        //assert(exit->trace == NULL);
        //exit->call = do_compilation(exit->ip, FALSE);
        //fprintf(stderr, "Compiled exit");
    }
    //fprintf(stderr, " with execution count %d\n", count & INT_MAX);
    //fprintf(stderr, "Compile queue length: %d\n", LIST_SIZE(compiler_queue));
    //fprintf(stderr, "Prods: %d\n", prods);
}

exit_handler increase_trace_count_and_maybe_compile(R_thread_state ts, R_trace trace) {
    if (trace->compiled) {
        return trace->compiled;
    }
    trace->execution_count += 32;
    if (trace->execution_count > 300 && trace->enqueued == 0) {
        trace->enqueued = 1;
        add_to_compiler_queue((R_object)trace);
        prod_compiler(ts);
        if (trace->compiled) {
            return trace->compiled;
        }
    } else if (trace->enqueued) {
        prod_compiler(ts);
        if (trace->compiled) {
            return trace->compiled;
        }
    }
    return NULL;
}

R_exec_link interpret_optimised_then_compile_direct(R_thread_state ts, R_frame f, R_exec_link exit) {
    int count = exit->execution_count;
    exit->execution_count = count+1;
    if ((count & 31) == 0) {
        if ((count >> 5) == 10) {
            add_to_compiler_queue((R_object)exit);
            prod_compiler(ts);
            if (exit->call) {
                return exit;
            }
        } else if ((count >> 5) > 10) {
            prod_compiler(ts);
            if (exit->call) {
                return exit;
            }
        }
    }
    EXEC_ON_TRACE;
    TYPE_ASSERT(exit, mono_exec_link);
    return interpreter(((R_mono_exec_link)exit)->ip, ts, f);
}


R_exec_link interpret_optimised_then_compile(R_thread_state ts, R_frame f, R_exec_link exit) {
    int count;
    R_trace trace = ((R_mono_exec_link)exit)->trace;
    TYPE_ASSERT(exit, mono_exec_link);
    if (trace->invalidated) {
        exit->execution_count = 0;
        ((R_mono_exec_link)exit)->trace = NULL;
        return interpreter(((R_mono_exec_link)exit)->ip, ts, f);
    }
    count = exit->execution_count;
    exit->execution_count = count+1;
    if ((count & 31) == 0) {
        exit_handler compiled;
        assert(trace != NULL);
        compiled = increase_trace_count_and_maybe_compile(ts, trace);
        if (compiled) {
            exit->call = compiled;
            return exit->call(ts, f, exit);
        }
    }
    EXEC_ON_TRACE;
    return interpreter(trace->optimised, ts, f);
}

void update_exit(R_mono_exec_link exit, R_trace trace) {
    exit->trace = trace;
    if (exit->call == mono_exit) {
        if (trace->compiled) {
            exit->call = trace->compiled;
        } else {
            if (optimiser_controls.compile) {
                exit->call = interpret_optimised_then_compile;
            } else {
                exit->call = interpret_optimised;
            }
        }
    }
}

R_object super_interpreter(R_frame start_frame) {
    R_exec_link next;
    R_thread_state ts = THREAD_STATE;
    R_type_state type_state;
    R_object raised;
    R_frame f;
    void* ptr;
    int tracing_external = (ts->trace != NULL);
    uintptr_t depth = start_frame->depth;
    uintptr_t old_depth = ts->stop_depth;
    ts->stop_depth = depth;
    ts->current_frame = start_frame;
    ts->request = REQUEST_NONE;
    raised = (R_object)gvmt_push_current_state();
    f = ts->current_frame;
    if (f->depth < depth) {
        ts->stop_depth = old_depth;
        gvmt_pop_state();
        if (raised) {
            gvmt_transfer((GVMT_Object)raised);
        } else {
            R_object result = (R_object)gvmt_stack_read_object(gvmt_stack_top());
            assert(ts->request == REQUEST_NONE);
            gvmt_drop(1);
            return result;
        }
    }
    if (raised == NULL) {
        uint8_t *ip;
        if (ts->next_ip) {
            ip = ts->next_ip;
            ts->next_ip = NULL;
        } else {
            ip = f->next_ip;
        }
        if (ts->request == REQUEST_ENTER_OPTIMISED) {
            ts->request = REQUEST_NONE;
            if (ts->trace) {
                // Already tracing
                next = interpreter(ip, ts, f);
            } else {
                R_type_state state = ts->optimise_type_state;
                R_trace trace = find_trace_in_cache(ts, ip, state);
                if (trace == NULL) {
                    next = start_trace(ip, state, ts, f);
                } else {
                    DEBUG(instruction_counter = &on_trace_instruction_counter;)
                    DEBUG_INC(trace_entry_counter);
                    trace->execution_count++;
                    next = execute_trace(trace, ts, f);
                }
            }
        } else {
            assert(ts->request == REQUEST_NONE);
            EXEC_OFF_TRACE;
            next = interpreter(ip, ts, f);
        }
    } else {
        //Handle exception
        R_exception_handler h;
        R_frame original_frame;
        R_BaseException ex = (R_BaseException)raised;
        // Some object was raised
        assert (IS_RAISEABLE(ex));
        if (!tracing_external && ts->trace) {
            // Discard trace
            tracing_stats.total_time += high_res_time() - tracing_stats.start_time;
            tracing_stats.abandoned++;
            ts->trace = NULL;
        }
        if (ts->on_exception) {
            uint8_t* bytes = ts->on_exception;
            ts->on_exception = NULL;
            GVMT_PUSH(raised);
            EXEC_ON_EXCEPTION;
            interpreter(bytes, ts, f);
            assert(0 && "Should not return!");
        }
        do {
            h = f->exception_stack;
            if (h && (h->stop == 0 || TYPE(raised) == type_StopIteration)) {
                GVMT_Value* handler_stack = h->data_stack;
                GVMT_Value* current_stack = gvmt_stack_top();
                assert (handler_stack >= current_stack);
                gvmt_drop(handler_stack - current_stack);
                f->exception_stack = h->next;
                break;
            }
            f = f->f_back;
            if (f->depth < depth) {
                gvmt_pop_state();
                gvmt_transfer((GVMT_Object)raised);
            }
        } while (1);
        if (ex->__traceback__ == (R_traceback)None) {
            if (ex == STOP_ITERATION_OBJECT) {
                ex = make_exception(type_StopIteration, empty_string);
            }
            build_backtrace(ex);
        }
        ts->current_frame = f;
        if (h->stop == 0)
            GVMT_PUSH(ex);
        DEBUG(if (instruction_counter == &on_trace_instruction_counter))
            DEBUG_INC(trace_exception_exit_counter);
        if (h->link) {
            next = h->link;
        } else {
            EXEC_OFF_TRACE;
            next = interpreter(h->ip, ts, f);
        }
    }
    EXEC_ON_TRACE;
    do {
        while (ts->event == NULL) {
            assert(next);
            assert(ts->stop_depth <= ts->current_frame->depth);
            DEBUG_INC(trace_entry_counter);
            next = next->call(ts, ts->current_frame, next);
            assert(next);
            assert(ts->stop_depth <= ts->current_frame->depth);
            DEBUG_INC(trace_entry_counter);
            next = next->call(ts, ts->current_frame, next);
        };
        ts->event(ts);
        ts->event = NULL;
    } while (1);
    return NULL;
}

R_exec_link execute_trace(R_trace code, R_thread_state ts, R_frame f) {
    if (code->invalidated) {
        return interpreter(f->next_ip, ts, f);
    }
    if (code->compiled) {
        return code->compiled(ts, f, DUMMY_LINK);
    } else {
        assert(code->optimised);
        EXEC_ON_TRACE;
        return interpreter(code->optimised, ts, f);
    }
}

/** Return GVMT_Object to tell GVMT compiler that this is non-native,
 but really only returning zero/non-zero */
GVMT_Object hotpy_main(int argc, char** argv) {
    GVMT_Object ex, print_ex;
    R_str filename;
    R_module module;
    R_code_object script;
    char* buf;
    // Silence annoying warnings.
    gvmt_warn_on_unexpected_parameter_usage = 0;
    GVMT_TRY(ex)
        os_init();
        filename = init_vm(argc, argv);
        buf = alloca(filename->length+1);
        py_print(buf, filename);
        module = new_module(name___main__, filename);
        script = load_source(buf);
        run_script(script, module, THREAD_STATE->top_frame);
        if (write_flow_graph) {
            // Should do this for all threads.
            print_flowgraph(THREAD_STATE, 1);
        }
        ex = 0;
    GVMT_CATCH
        if (TYPE((R_object)ex) == type_SystemExit) {
            ex = 0;
        } else {
            GVMT_TRY(print_ex)
                print_exception(stdout, (R_BaseException)ex);
            GVMT_CATCH
                fatal("Exception in printing exception\n");
            GVMT_END_TRY
        }
    GVMT_END_TRY
    if (profile_on == TRUE) {
        float total = 0.0;
        int i, j;
        for (i = 0; i < 256; i++) {
            total += opcode_counts[i];
        }
        for (i = 0; i < 256; i++) {
            if (opcode_counts[i])
                printf("%-24.23s %-10d  %0.2f%%\n", gvmt_opcode_names_interpreter[i],
                       opcode_counts[i], 100.0 *opcode_counts[i]/total);
        }
        for (i = 1; i < 256; i++) {
            for (j = 1; j < 256; j++) {
                if (opcode_pair_counts[i*256 + j] > total/1000)
                    printf("%-24.23s %-24.23s %0.3f%%\n", gvmt_opcode_names_interpreter[i],
                            gvmt_opcode_names_interpreter[j],
                            100.0 *opcode_pair_counts[i*256 + j]/total);
            }
        }
    }
    if (print_stats == TRUE) {
        double code_gen;
        fprintf(stderr, "%d top-level traces started\n", tracing_stats.top_level_starts);
        fprintf(stderr, "%d side traces started\n", tracing_stats.exit_starts);
        fprintf(stderr, "%d traces completed\n", tracing_stats.completed);
        fprintf(stderr, "%d traces abandoned\n", tracing_stats.abandoned);
        fprintf(stderr, "%d top level traces (main thread)\n", THREAD_STATE->trace_cache->values->size);
        fprintf(stderr, "Total tracing time: %f ms\n", tracing_stats.total_time * 1000);
        fprintf(stderr, "Total optimisation time: %f ms\n", optimising_time * 1000);
        fprintf(stderr, "%d compilations\n", compiler_counter);
        fprintf(stderr, "%d instruction bytes compiled\n", compiled_bytes_counter);
        fprintf(stderr, "Total compilation time: %f ms", gvmt_total_compilation_time/1000000.0);
        code_gen = 100.0*gvmt_total_codegen_time/gvmt_total_compilation_time;
        if (gvmt_total_compilation_time == 0)
            fprintf(stderr, "\n");
        else
            fprintf(stderr, " (%.1f%% code gen)\n", code_gen);
        fprintf(stderr, "%d minor collections in %f ms\n", gvmt_minor_collections,
               gvmt_minor_collection_time/1000000.0);
        fprintf(stderr, "%d major collections in %f ms\n", gvmt_major_collections,
               gvmt_major_collection_time/1000000.0);
        fprintf(stderr, "Total collection time: %f ms\n",
               gvmt_total_collection_time/1000000.0);
        fprintf(stderr, "Virtual heap size: %d kbytes\n",
               (int)(gvmt_virtual_heap_size/1024));
        fprintf(stderr, "Real heap size: %d kbytes\n",
               (int)(gvmt_real_heap_size/1024));
        fprintf(stderr, "Nursery size: %d kbytes\n",
               (int)(gvmt_nursery_size/1024));
        fprintf(stderr, "Memory passed to allocators: %d Mbytes\n",
               (int)(gvmt_bytes_passed_to_allocators/(1024.0 * 1024)));
        // fprintf(stderr, "Live memory in mature space: %d kbytes\n",
        //        (int)(gvmt_mature_space_residency()/1024));
#ifndef NDEBUG
        fprintf(stderr, "%u instructions on trace\n", on_trace_instruction_counter);
        fprintf(stderr, "%u instructions off trace\n", off_trace_instruction_counter);
        fprintf(stderr, "%u instructions in exception handling\n", on_except_instruction_counter);
        fprintf(stderr, "%u instructions tracing\n", tracing_instruction_counter);
        fprintf(stderr, "%u traces executed\n", trace_entry_counter);
        fprintf(stderr, "%u total instructions\n", on_trace_instruction_counter
                + on_except_instruction_counter +
                off_trace_instruction_counter + tracing_instruction_counter);
        fprintf(stderr, "%u trace exits\n", exec_links_counter);
        fprintf(stderr, "%u trace-cache top-level lookups\n", trace_cache_top_level_lookups);
        fprintf(stderr, "%u trace-cache exit lookups\n", trace_cache_exit_lookups);
        fprintf(stderr, "%u trace object-init exits\n", trace_object_init_exits_counter);
        fprintf(stderr, "%u trace func-enter exits\n", trace_func_enter_exits_counter);
        fprintf(stderr, "%u trace method-enter exits\n", trace_method_enter_exits_counter);
        fprintf(stderr, "%u trace exception exits\n", trace_exception_exit_counter);
        fprintf(stderr, "%u return exits\n", return_exit_counter);
#endif
    }
    return ex;
}

void* make_mono_exit(uint8_t* target_ip, R_set pinned, R_type_state state) {
    R_mono_exec_link exit = gc_allocate(mono_exec_link);
    SET_TYPE(exit, type_mono_exec_link);
    exit->call = mono_exit;
    exit->ip = target_ip;
    exit->state = state;
    PY_set_add((R_object)exit, pinned);
    return gvmt_pin((GVMT_Object)exit);
}

void* make_interpret_exit(uint8_t* target_ip, R_set pinned) {
    R_mono_exec_link exit = gc_allocate(mono_exec_link);
    SET_TYPE(exit, type_mono_exec_link);
    exit->call = interpret_exit;
    exit->ip = target_ip;
    PY_set_add((R_object)exit, pinned);
    return gvmt_pin((GVMT_Object)exit);
}

void* make_poly_exit(R_set pinned, R_type_state state) {
    R_poly_exec_link exit = gc_allocate(poly_exec_link);
    SET_TYPE(exit, type_poly_exec_link);
    exit->call = poly_exit;
    exit->state = state;
    PY_set_add((R_object)exit, pinned);
    return gvmt_pin((GVMT_Object)exit);
}

R_exec_link mono_exit(R_thread_state ts, R_frame f, R_exec_link exit) {
    R_mono_exec_link mono;
    TYPE_ASSERT(exit, mono_exec_link);
    mono = (R_mono_exec_link)exit;
    DEBUG(if(trace_on) fprintf(stderr, "mono exit to %x\n", mono->ip);)
    exit->execution_count++;
    if (exit->execution_count < 8) {
        ts->request = REQUEST_NONE;
        assert(mono->ip);
    } else {
        R_trace trace = find_trace_in_cache(ts, mono->ip, mono->state);
        if (trace) {
            update_exit(mono, trace);
            assert(ts->current_frame == f);
            assert(exit->call != mono_exit);
            return exit->call(ts, f, exit);
        }
        ts->request = REQUEST_ENTER_OPTIMISED;
        ts->optimise_type_state = mono->state;
    }
    ts->next_ip = mono->ip;
    assert(ts->current_frame == f);
    gvmt_transfer(NULL);
    return NULL;
}

R_exec_link interpret_exit(R_thread_state ts, R_frame f, R_exec_link exit) {
    R_mono_exec_link mono;
    TYPE_ASSERT(exit, mono_exec_link);
    mono = (R_mono_exec_link)exit;
    DEBUG(if(trace_on) fprintf(stderr, "interpret exit to %x\n", mono->ip);)
    EXEC_OFF_TRACE;
    return interpreter(mono->ip, ts, f);
}

R_exec_link poly_exit(R_thread_state ts, R_frame f, R_exec_link exit) {
    R_poly_exec_link poly;
    DEBUG(if(trace_on) fprintf(stderr, "poly exit (%x)\n", f->next_ip);)
    TYPE_ASSERT(exit, poly_exec_link);
    poly = (R_poly_exec_link)exit;
    if (poly->ips[0] == f->next_ip) {
        R_trace trace = poly->traces[0];
        assert(trace);
        poly->execution_counts[0]++;
        return execute_trace(trace, ts, f);
    } else {
        int i;
        for (i = 1; i < 4; i++) {
            if (poly->ips[i] == f->next_ip) {
                R_trace trace = poly->traces[i];
                poly->execution_counts[i]++;
                if (trace) {
                    if (poly->execution_counts[i] > poly->execution_counts[i-1]+4) {
                        uint8_t *tmp_ip1, *tmp_ip2;
                        uint32_t tmp_exe;
                        R_trace tmp_trace;
                        tmp_trace = poly->traces[i-1];
                        poly->traces[i] = tmp_trace;
                        poly->traces[i-1] = trace;
                        tmp_exe = poly->execution_counts[i];
                        poly->execution_counts[i] = poly->execution_counts[i-1];
                        poly->execution_counts[i-1] = tmp_exe;
                        tmp_ip1 = poly->ips[i-1];
                        tmp_ip2 = poly->ips[i];
                        poly->ips[i] = tmp_ip1;
                        poly->ips[i-1] = tmp_ip2;
                    }
                    return execute_trace(trace, ts, f);
                } else {
                    if (poly->execution_counts[i] < 12) {
                        ts->request = REQUEST_NONE;
                    } else {
                        R_trace trace = find_trace_in_cache(ts, f->next_ip, exit->state);
                        if (trace) {
                            poly->traces[i] = trace;
                            return execute_trace(trace, ts, f);
                        }
                        ts->request = REQUEST_ENTER_OPTIMISED;
                        ts->optimise_type_state = NULL;
                    }
                    gvmt_transfer(NULL);
                    return NULL;
                }
            }
        }
        for (i = 1; i < 4; i++) {
            if (poly->execution_counts[i] < 4) {
                poly->execution_counts[i] = 1;
                poly->ips[i] = f->next_ip;
                poly->traces[i] = NULL;
                break;
            }
        }
        ts->request = REQUEST_NONE;
        gvmt_transfer(NULL);
        return NULL;
    }
}

