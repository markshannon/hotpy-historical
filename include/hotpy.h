#ifndef HOTPY_H
#define HOTPY_H

#define FLATTEN 0

//#define READ_LOCKING
//#define NO_LOCKING

#define OPCODE(x) GVMT_OPCODE(interpreter, x)
#define OPCODE_LENGTH(x) GVMT_OPCODE_LENGTH(interpreter, x)

#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include "gvmt/gvmt.h"
#include "hotpy_types.h"
#include "typedefs.h"
#include "thread.h"

typedef void (* guard_delete_func)(R_guard obj);

#define PY_FUNC(name) PY_##name
#define PY_FUNC0(name) PY_##name(void)
#define PY_FUNC1(name, p1) PY_##name(p1)
#define PY_FUNC2(name, p1, p2) PY_##name(p2, p1)
#define PY_FUNC3(name, p1, p2, p3) PY_##name(p3, p2, p1)
#define PY_FUNC4(name, p1, p2, p3, p4) PY_##name(p4, p3, p2, p1)
#define PY_FUNC5(name, p1, p2, p3, p4, p5) PY_##name(p5, p4, p3, p2, p1)

typedef R_object (*unary_function)(R_object o);
typedef R_object (*binary_function)(R_object o1, R_object o2);
typedef R_trace (*compiled_code)(R_thread_state ts, R_frame f);
typedef R_exec_link (*exit_handler)(R_thread_state ts, R_frame f, R_exec_link exit);

#include "abstract.h"
#include "descriptors.h"
#include "py_api/object.h"
#include "structs.h"
#include "core_objects.h"
#include "gvmt/arena.h"
#include "types.h"
#include "object.h"
#include "intobject.h"
#include "floatobject.h"
#include "genobject.h"
#include "boolobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "strobject.h"
#include "rangeobject.h"
#include "sliceobject.h"
#include "setobject.h"
#include "typeobject.h"
#include "tupleobject.h"
#include "moduleobject.h"
#include "fileobject.h"
#include "frameobject.h"
#include "complexobject.h"
#include "function.h"
#include "exception.h"
#include "bytesobject.h"
#include "bytearrayobject.h"
#include "os.h"
#include "sequence.h"
#include "hashtable.h"
#include "str_bufobject.h"
#include "py_api/prototypes.h"
#include "unicodeobject.h"
#include "Python-ast.h"
#include "py_api/structs.h"
#include "optimise.h"

#ifdef DEBUG
#define UP_CAST(type, obj) ((R_##type)safe_cast((obj), type_##type))
#else
#define UP_CAST(type, obj) ((R_##type)(obj))
#endif

//typedef void (*trace_func)(R_object previous, ...);

R_object safe_cast(R_object obj, R_type klass);

void Log_trace(char* fmt, ...);

void print_exception(FILE* out, R_BaseException ex);

R_object super_interpreter(R_frame f);

//typedef void* PyObject;
//typedef void* StackItem;

extern struct thread_locals* thread_locals;

void raise_IOError(R_str msg);

//void raise_Exception(char *msg);

//R_module __builtins__(void);

/** Packs values from array into tuple in reverse order (since stack grows downward) */
R_tuple pack_tuple_reverse(int count, GVMT_Value* values);

int TEST_OVERFLOW(void);

void print(GVMT_Object object);

void fatal(char* format, ...);

extern int is_power2(int x);

#ifdef BOOTSTRAP
R_object
#else
R_exec_link
#endif
interpreter(uint8_t* ip_start, R_thread_state ts, R_frame f);

R_exec_link tracing_interpreter(uint8_t* ip_start, R_thread_state ts, R_frame f);

#define NONE_TYPE_ID 1

R_object binary_call(R_operator op, R_object l, R_object r);
R_object inplace_call(R_operator op, R_object l, R_object r);
R_object binary_operator(uint8_t index, R_object l, R_object r);
int binary_compare(uint8_t index, R_object o1, R_object o2);
R_object unary_call(R_operator op, R_object x);

char *short_name(R_str long_name);

R_code_object load_source(char* filename);

void os_init(void);

// Returns the first non-option argument
R_str init_vm(int argc, char** argv);

//void thread_init(struct thread_locals* tls);

extern int disassemble_on_load;
extern int write_flow_graph;
extern int write_flow_graph_neat;
extern int flow_graph_verbose;
extern uint32_t opcode_counts[];
extern uint32_t opcode_pair_counts[];
extern uint8_t prev_opcode;

extern R_int make_edge(unsigned start, unsigned end);

R_list create_flowgraph(R_bytes start_list, R_list edges, unsigned block_count);
void print_function_flowgraph(R_code_object code);

extern void* dl_symbols;

extern compiled_code (*jit_compile)(uint8_t* begin, uint8_t* end, char* name);

void print_options(void);

typedef R_object (*binary_behaviour)(R_object right, R_object left);

void do_system_trace(R_thread_state ts, R_str event, R_object arg);

R_tuple make_tuple(int size);

R_object vm_call_special(R_object obj, R_str name);

#define NOT_IMPLEMENTED_ERROR do { \
    char buf[100]; \
    sprintf(buf, "Not implemented at %s:%d", __FILE__, __LINE__); \
    raise_char(type_InternalError, buf); \
} while (0)

#define gc_allocate(type) ((R_##type)gvmt_malloc(sizeof(GVMT_OBJECT(type))))

#define gc_allocate_var(type, var, size) ((R_##type)gvmt_malloc(offsetof(GVMT_OBJECT(type), var[0]) + (size)*sizeof(((R_##type)0)->var[0])))

R_object interpreter_trace(uint8_t* ip, R_frame frame, int tracing);

void name_not_defined(R_str name);

extern double high_res_time(void);

void no_keywords(R_str name);

R_object api_init(void);

extern void* hotpy_thread_state;

extern int *instruction_counter;

extern void print_short(R_object object);

/* Returns 1 if next_ip falls within the code_object's bytes */
extern int check_frame_next_ip(R_frame frame);

/** To set this to true (1) requires a memory barrier.
 *  Once set it can never be unset, as that would be unsafe.
 *  (Another thread might have a mutex, and then not release it)
 */
extern int_fast32_t hotpy_multi_threaded;

#endif  /* HOTPY_H */

