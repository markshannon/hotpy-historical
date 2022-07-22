#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "hotpy.h"
#include "optimise.h"
#include "specialiser.h"

extern char* trace2dot(uint8_t* ip, R_set to_do, R_dict cache, char* buffer, FILE* out);

void print_function_flowgraph(R_code_object code) {
    char *ptr, *buffer = malloc(40000);
    FILE* out;
    uint8_t* start = get_bytecodes_from_code_object(code);
    R_set to_do = new_set();
    R_set done = new_set();
    ptr = buffer + sprintf(buffer, "hotpy_flow_graph.");
    ptr += py_print(ptr, code->name);
    sprintf(ptr, ".gv");
    out = fopen(buffer, "w");
    fprintf(out, "digraph g {\n");
    fprintf(out, "graph [ size = \"7.5, 10.5\", rankdir = \"LR\" ];\n");
    fprintf(out, "\"t%x\" [ shape = \"record\", label = \"", start);
    ptr = trace2dot(start, to_do, new_empty_dict(), buffer, out);
    fprintf(out, "\" ]\n\n");
    fwrite(buffer, 1, ptr-buffer, out);
    PY_set_add((R_object)R_int_from_ssize_t((intptr_t)start), done);
    while (PY_set_empty(to_do) == (R_object)FALSE) {
        R_object o = PY_set_pop(to_do);
        uint8_t* ip = NULL;
        TYPE_ASSERT(o, int);
        ip = (uint8_t*)int_from_py_int((R_int)o);
        if (PY_set_contains(o, done) == (R_object)FALSE) {
            fprintf(out, "\"t%x\" [ shape = \"record\", label = \"", (int)ip);
            ptr = trace2dot(ip, to_do, new_empty_dict(), buffer, out);
            fprintf(out, "\" ]\n\n");
            fflush(out);
            fwrite(buffer, 1, ptr-buffer, out);
            fflush(out);
            PY_set_add(o, done);
        }
    }
    fprintf(out, "}\n");
    free(buffer);
    fclose(out);
}

void print_flowgraph(R_thread_state ts, int id) {
    char *ptr, *buffer = malloc(40000);
    FILE* out;
    R_set to_do = new_set();
    R_set done = new_set();
    R_set entries = new_set();
    R_dict_values traces = ts->trace_cache->values;
    R_dict_keys keys = traces->keys;
    int i;
    if (optimiser_controls.compile) {
        fprintf(stderr, "Can't print traces when compile is on");
        abort();
    }
    sprintf(buffer, "hotpy_flow_graph.%d.gv", id);
    out = fopen(buffer, "w");
    fprintf(out, "digraph g {\n");
    fprintf(out, "graph [ size = \"7.5, 10.5\", rankdir = \"LR\" ];\n");
    for (i = 0; i < LEN(traces); i++) {
        R_trace t = (R_trace)ITEM(traces, i);
        if (t != NULL) {
            PY_set_add((R_object)t, to_do);
            if (TYPE(ITEM(keys, i)) == type_int) {
                PY_set_add((R_object)t, entries);
            }
        }
    }
    while (PY_set_empty(to_do) == (R_object)FALSE) {
        R_object o = PY_set_pop(to_do);
        uint8_t* ip = NULL;
        R_trace t = NULL;
        if (TYPE(o) == type_trace) {
            t = (R_trace)o;
            ip = t->optimised;
        } else {
            ip = (uint8_t*)int_from_py_int((R_int)o);
        }
        if (PY_set_contains(o, done) == (R_object)FALSE) {
            fprintf(out, "\n\n\n\"t%x\" [ shape = \"record\", label = \"", (uintptr_t)ip);
            if (t) {
                if (PY_set_contains((R_object)t, entries) == (R_object)TRUE) {
                    fprintf(out, "\\<TOP LEVEL\\>|");
                }
                fprintf(out, "entered: %d|", t->execution_count);
            }
            ptr = trace2dot(ip, to_do, ts->trace_cache, buffer, out);
            fprintf(out, "\" ]\n\n");
            fflush(out);
            fwrite(buffer, 1, ptr-buffer, out);
            fflush(out);
            PY_set_add(o, done);
        }
    }
    fprintf(out, "\n\n/* All traces done */\n\n\n");
    fprintf(out, "}\n");
    free(buffer);
    fclose(out);
}

char* write_mono_link(char* out, R_mono_exec_link exit, int exit_addr, R_set to_do) {
    uint8_t* ip;
    if (exit->trace) {
        ip = exit->trace->optimised;
    } else {
        ip = exit->ip;
    }
    assert(ip);
    if (write_flow_graph_neat) {
        out += sprintf(out, "e%x [ shape = \"Mrecord\", label = \"Mono\\nLink\" ]\n", exit_addr);
    } else {
        R_type_state state = exit->state;
        out += sprintf(out, "e%x [ shape = \"Mrecord\", label = \"Mono Link\\n", exit_addr);
        if (state) {
            out = print_type_state_flow(state, out);
        }
        out += sprintf(out, "\" ]\n ");
    }
    if (exit->trace) {
        out += sprintf(out, "e%x -> t%x:l0\n", exit_addr, ip);
        PY_set_add((R_object)exit->trace, to_do);
    }
    return out;
}

char* write_poly_link(char* out, R_poly_exec_link exit, int exit_addr, R_set to_do) {
    int i;
    if (write_flow_graph_neat) {
        R_type_state state = exit->state;
        out += sprintf(out, "e%x [ shape = \"Mrecord\", label = \"Poly Link\\n", exit_addr);
        if (state) {
            out = print_type_state_flow(state, out);
        }
    } else {
        out += sprintf(out, "e%x [ shape = \"Mrecord\", label = \"Poly\\nLink", exit_addr);
    }
    for (i = 0; i < 4; i++) {
        if (exit->ips[i]) {
            out += sprintf(out, " | <p%d>%d (%d) ", i, i, exit->execution_counts[i]);
        }
    }
    out += sprintf(out, "\"]\n ");
    for (i = 0; i < 4; i++) {
        if (exit->ips[i]) {
            if (exit->traces[i]) {
                uint8_t* ip = exit->traces[i]->optimised;
                assert(ip);
                out += sprintf(out, "e%x:p%d -> t%x:l0\n", exit_addr, i, ip);
                PY_set_add((R_object)exit->traces[i], to_do);
            }
        }
    }
    return out;
}

char* write_exit_node(char* out, void* trace_addr, int index, char* label, int exit_addr, R_set to_do, R_dict cache) {
    R_exec_link exit = (R_exec_link)gvmt_pinned_object((void*)exit_addr);
    int hot;
    int mono = TYPE(exit) == type_mono_exec_link;
    if (!mono) {
        R_poly_exec_link poly = (R_poly_exec_link)exit;
        if (poly->execution_count == 0) {
            int i;
            for (i = 0; i < 4; i++) {
                poly->execution_count += poly->execution_counts[i];
            }
        }
    }
    hot = (exit->execution_count & INT_MAX) >= 8;
    if (hot) {
        if (mono) {
            out = write_mono_link(out, (R_mono_exec_link)exit, exit_addr, to_do);
        } else {
            out = write_poly_link(out, (R_poly_exec_link)exit, exit_addr, to_do);
        }
        out += sprintf(out, "t%x:l%x -> e%x [ label = \"%s (%d)\" ]\n", trace_addr, index, exit_addr, label, exit->execution_count);
    } else {
        out += sprintf(out, "e%x [ color=\"grey60\", fontcolor=\"grey60\", shape = \"Mrecord\", label = \"%s\\nLink\" ]\n ", exit_addr, mono ? "Mono" : "Poly");
        out += sprintf(out, "t%x:l%x -> e%x [ color=\"grey60\", fontcolor=\"grey60\", label = \"cold (%d)\" ]\n", trace_addr, index, exit_addr, exit->execution_count & INT_MAX);
    }
    return out;
}

