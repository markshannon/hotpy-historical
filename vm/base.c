// Are all of these headers necessary?
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <dlfcn.h>
#include <errno.h>
#include "hotpy.h"
#include "gvmt/compiler.h"
#include "optimise.h"
#include "operators.h"
#include "version.h"

#ifndef BIN_PATH
# error "Must define BIN_PATH"
#endif

/** Assorted stuff, much of which should probably go elsewhere
 */

// void Log_trace_stack(GVMT_Object* stack) { }

extern GVMT_OBJECT(type) _Constants;

GVMT_OBJECT(Constants) empty_constants = {
    &_Constants,
    0, /* ref_count */
    &empty_string_object,
    &empty_tuple_object,
    &empty_tuple_object,
    &empty_tuple_object
};

struct _optimiser_controls optimiser_controls = {
    1,  /* trace      */
    1,  /* specialise */
    1,  /* defer      */
    1,  /* peephole   */
    1,   /* compile    */
    0    /* pre-specialisation */
};

int write_flow_graph = 0;
int write_flow_graph_neat = 0;
int flow_graph_verbose = 1;
int debug_optimise = 0;
int hotpy_low_level = 0;

double optimising_time = 0.0;
void* hotpy_thread_state;
void* dl_symbols;

int_fast32_t hotpy_multi_threaded = 0;

compiled_code (*jit_compile)(uint8_t* begin, uint8_t* end, char* name);

//
//int printnstr_end(R_str str) {
//    int i, len = (16 < str->length) ? 16 : str->length;
//    uint8_t buf[16];
//    for (i = 0; i < len; i++)
//        buf[i] = str->text[str->length - len + i];
//    fwrite(buf, 1, len, stderr);
//    return len;
//}

R_exec_link DUMMY_LINK;

static R_exec_link make_dummy_link(void) {
    R_trace t;
    R_exec_link link = gc_allocate(exec_link);
    SET_TYPE(link, type_exec_link);
    return link;
}

gvmt_write_func gvmt_user_marshaller_for_object(GVMT_Object object) {
    return TYPE((R_object)object)->marshaller;
}

R_str init_vm(int argc, char** argv) {
    int i;
    char* mem = 0;
    R_list sys_argv_list;
    i = 1;
    hotpy_thread_state = gvmt_gc_add_tls_root();
    dl_symbols = dlopen(0, RTLD_LAZY | RTLD_GLOBAL);
    while (i < argc && argv[i][0] == '-') {
        char opt = argv[i][1];
        if (opt == 'H') {
            // HotPy specific options
            if (strcmp(argv[i], "-Ht") == 0) {
                trace_on = 1;
            } else if (strcmp(argv[i], "-Hp") == 0) {
                int j, k;
                profile_on = TRUE;
                prev_opcode = 0;
                for (j = 0; j < 256; j++)
                    opcode_counts[j] = 0;
                for (j = 0; j < 256; j++)
                    for (k = 0; k < 256; k++)
                        opcode_pair_counts[j*256+k] = 0;
            } else if (strcmp(argv[i], "-HQ") == 0) {
                debug_optimise = 1;
            } else if (strcmp(argv[i], "-Hs") == 0) {
                trace_show_stack = TRUE;
            } else if (strcmp(argv[i], "-Hstats") == 0) {
                print_stats = TRUE;
            } else if (strcmp(argv[i], "-Hbase") == 0) {
                optimiser_controls.trace = 0;
            } else if (strcmp(argv[i], "-Hno-spec") == 0) {
                optimiser_controls.specialise = 0;
            } else if (strcmp(argv[i], "-Hno-defer") == 0) {
                optimiser_controls.defer = 0;
            } else if (strcmp(argv[i], "-Hno-peephole") == 0) {
                optimiser_controls.peephole = 0;
            } else if (strcmp(argv[i], "-Hpre-spec") == 0) {
                optimiser_controls.pre_spec = 1;
            } else if (strcmp(argv[i], "-Hno-comp") == 0) {
                optimiser_controls.compile = 0;
            } else if (strcmp(argv[i], "-Hno-tracing") == 0) {
                optimiser_controls.trace = 0;
            } else if (strncmp(argv[i], "-Hm", 3) == 0) {
                mem = argv[i] + 3;
            } else if (strcmp(argv[i], "-Hdis") == 0) {
                disassemble_on_load = 1;
            } else if (strcmp(argv[i], "-Hlow-level") == 0) {
                hotpy_low_level = 1;
            } else if (strcmp(argv[i], "-Hflow") == 0) {
                write_flow_graph = 1;
            } else if (strcmp(argv[i], "-Hflow-neat") == 0) {
                write_flow_graph = 1;
                write_flow_graph_neat = 1;
            } else if (strcmp(argv[i], "-Hflow-compact") == 0) {
                write_flow_graph = 1;
                write_flow_graph_neat = 1;
                flow_graph_verbose = 0;
            } else {
                printf("Unknown option '%s'\n", argv[i]);
                exit(0);
            }
        } else if (opt == 'h' && argv[i][2] == 0) {
            print_options();
            exit(0);
        } else if (opt == 'O' && argv[i][2] == 0) {
            global_debug = FALSE;
        } else if (opt == 'V' && argv[i][2] == 0) {
            printf("HotPy 0.1.%d, targetting Python 3.X series\n", VERSION_NUMBER);
            exit(0);
        } else {
            printf("Unknown option %s\n", argv[i]);
            exit(0);
        }
        i++;
    }
    if (mem) {
        int l = strlen(mem);
        int size;
        char* end_ptr;
        char* end;
        errno = 0;
        if (mem[l-1] == 'k' || mem[l-1] == 'K') {
            size = 1024 * strtol(mem, &end_ptr, 10);
            end = mem+l-1;
        } else if (mem[l-1] == 'm' || mem[l-1] == 'M') {
            size = 1024 * 1024 * strtol(mem, &end_ptr, 10);
            end = mem+l-1;
        } else if (mem[l-1] == 'g' || mem[l-1] == 'G') {
            size = 1024 * 1024 * 1024 * strtol(mem, &end_ptr, 10);
            end = mem+l-1;
        } else {
            size = strtol(mem, &end_ptr, 10);
            end = mem+l;
        }
        if (errno || end_ptr != end) {
            printf("Invalid memory size: %s\n", mem);
            exit(0);
        }
        gvmt_malloc_init(size);
    } else {
        // 64 Mbytes soft limit
        gvmt_malloc_init(64 * 1024 * 1024);
    }
    GEN_STACK_MARKER = gc_allocate(object);
    SET_TYPE(GEN_STACK_MARKER, type_object);
    api_init();
    thread_init();
    DUMMY_LINK = make_dummy_link();
    if (optimiser_controls.compile) {
        // Load compiler.
        void* handle = dlopen( BIN_PATH "/hotpy_compiler.so",
                               RTLD_NOW | RTLD_LOCAL);
        if (handle == 0) {
            jit_compile = 0;
        } else {
            jit_compile = dlsym(handle, "gvmt_compile_jit");
        }
        if (jit_compile == 0) {
            fprintf(stderr, "%s", dlerror());
            exit(1);
        }
    }
    if (argc == i) {
        // Should be interactive version...
        printf("No source file specified\n");
        exit(0);
    }
    argc -= i;
    argv += i;
    sys_argv_list = make_list(argc);
    SET_LIST_SIZE(sys_argv_list, argc);
    for (i = 0; i < argc; i++) {
        ITEM(sys_argv_list->array, i) = (R_object)canonical_string(string_from_c_string(argv[i]));
    }
    save_global(sys, name_argv, (R_object)sys_argv_list);
    // TO DO -- Should be able to do this at build time.
    save_global(sys, canonical_string(string_from_c_string("executable")),
                (R_object)canonical_string(string_from_c_string(argv[0])));
    return (R_str)ITEM(sys_argv_list->array, 0);
}

void do_system_trace(R_thread_state ts, R_str event, R_object arg) {
    assert(0 && "To do...");
    // Call tracer with arguments...
}

/* Helpers for readable_name */
static int is_identifier(R_str s) {
    int i;
    if (s->length == 0)
        return 0;
    if (!isalpha(s->text[0]) && s->text[0] != '_')
        return 0;
    for (i = 1; i < s->length; i++) {
        if (!isalnum(s->text[i]) && s->text[i] != '_')
        return 0;
    }
    return 1;
}

static char *str_to_buffer(R_str s, char* prefix, char* buffer) {
    int i;
    strcpy(buffer, prefix);
    buffer += strlen(prefix);
    for (i = 0; i < s->length; i++) {
        buffer[i] = (char)s->text[i];
    }
    buffer += s->length;
    *buffer = 0;
    return buffer;
}

static void function_name(R_function func, char* buffer) {
    if (func->__code__->kind & C_NAME) {
        str_to_buffer(func->__code__->name, "func_", buffer);
    } else {
        char *p = str_to_buffer(func->__name__, "func_", buffer);
        sprintf(p, "_%x", hotpy_object_hash((R_object)func));
    }
}

static void builtin_function_name(R_builtin_function func, char* buffer) {
    str_to_buffer(func->source_name, "func_", buffer);
}

extern R_object PY_neg_int(R_object i);

/** Writes an ASCII readable name for obj into buffer */
void gvmt_user_readable_name(GVMT_Object o, char* buffer) {
    R_object obj = (R_object)o;
    R_type klass = TYPE(obj);
    R_str str;
    int i;
    if (klass == type_module) {
        str = module_getname((R_module)obj);
        str_to_buffer(str, "module_", buffer);
    } else if (klass == type_type) {
        str = ((R_type)obj)->__name__;
        str_to_buffer(str, "_", buffer);
        for(i = 1; i <= str->length; i++)
            if (buffer[i] == ' ')
                buffer[i] = '_';
    } else if (klass == type_str && is_identifier((R_str)obj)) {
        str = (R_str)obj;
        str_to_buffer(str, "str_", buffer);
    } else if (klass == type_NoneType) {
        sprintf(buffer, "NONE");
    } else if (klass == type_NotImplementedType) {
        sprintf(buffer, "NOT_IMPLEMENTED");
    } else if (klass == type_binary_operator || klass == type_unary_operator
                || klass == type_comparison_operator) {
        str = ((R_operator)obj)->name;
        str_to_buffer(str, "operator_object_", buffer);
    } else if (klass == type_function) {
        function_name((R_function)obj, buffer);
    } else if (klass == type_builtin_function) {
        builtin_function_name((R_builtin_function)obj, buffer);
    } else if (klass == type_bool) {
        R_bool b = (R_bool)obj;
        sprintf(buffer, b->value ? "_true" : "_false");
    } else if (klass == type_int) {
        str = int_to_str((R_int)obj, 10);
        str_to_buffer(str, "int_", buffer);
        if (buffer[4] == '-')
            buffer[4] = 'm';
    } else if (klass == type_tuple && ((R_tuple)obj)->length == ZERO) {
        sprintf(buffer, "empty_tuple_object");
    } else if ((R_dict_keys)obj == empty_dict_keys) {
        sprintf(buffer, "empty_dict_keys_object");
    } else if ((R_dict_values)obj == empty_module_dict_values) {
        sprintf(buffer, "empty_module_dict_values_object");
    } else if ((R_dict_values)obj == empty_dict_values) {
        sprintf(buffer, "empty_dict_values_object");
    } else if (klass == type_str && ((R_str)obj)->length == ZERO) {
        sprintf(buffer, "empty_string_object");
    } else if ((R_dict_table)obj == empty_dict_table) {
        sprintf(buffer, "empty_dict_table_object");
    } else if ((R_dict)obj == canonical_strings) {
        sprintf(buffer, "obj_canonical_strings");
    } else if ((R_tuple)obj == standard_names) {
        sprintf(buffer, "standard_names_table");
    } else if ((R_tuple)obj == operator_names) {
        sprintf(buffer, "operator_names_table");
    } else if ((R_BaseException)obj == STOP_ITERATION_OBJECT) {
        sprintf(buffer, "stop_iteration_object");
    } else {
        char* p = str_to_buffer(klass->__name__, "obj_", buffer);
        sprintf(p, "_%x", hotpy_object_hash(obj));
    }
}

