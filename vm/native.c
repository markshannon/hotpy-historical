#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gvmt/native.h"
#include "gvmt/marshaller.h"
#include <assert.h>
#include <pthread.h>
#include <errno.h>

extern GVMT_Object hotpy_main(void);
extern int32_t py_fprint(void);
extern GVMT_Object start_thread(void* root);

void fatal(char* format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
    exit(1);
}

#define STACK_SPACE 1 << 16

void user_gc_out_of_memory() {
    fprintf(stderr, "Out of memory");
    exit(1);
}

int fix_limit(int val, int length) {
    if (val < 0) {
        val = length + val;
        if (val < 0)
            return 0;
        else
            return val;
    } else if (val > length) {
        return length;
    } else {
        return val;
    }
}

double high_res_time(void) {
  struct timespec ts;
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &ts);
  return ts.tv_sec + ts.tv_nsec/1000000000.0;
}

intptr_t *hotpy_create_lock(void) {
    intptr_t* lock = malloc(sizeof(intptr_t));
    *lock = GVMT_LOCK_INITIALIZER;
    return lock;
}

void hotpy_delete_lock(intptr_t * lock) {
//    gvmt_lock_destroy(lock);
    free(lock);
}

void* os_mutex_create(void) {
    pthread_mutex_t *m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(m, 0);
    return (void*)m;
}

char* os_mutex_lock(void* mutex) {
    int err = pthread_mutex_lock((pthread_mutex_t *)mutex);
    if (err)
        // Need to improve messages!
        return "Error";
    else
        return NULL;
}

char* os_mutex_unlock(void* mutex) {
    int err = pthread_mutex_unlock((pthread_mutex_t *)mutex);
    if (err)
        // Need to improve messages!
        return "Error";
    else
        return NULL;
}

char* os_mutex_destroy(void* mutex) {
    int err = pthread_mutex_destroy((pthread_mutex_t *)mutex);
    free(mutex);
    if (err)
        // Need to improve messages!
        return "Error";
    else
        return NULL;
}

void* os_condition_create(void) {
    pthread_cond_t *c = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(c, 0);
    return (void*)c;
}

char* os_condition_wait(void* mutex, void* cond) {
    int err = pthread_cond_wait((pthread_cond_t *)cond, (pthread_mutex_t *)mutex);
    if (err)
        // Need to improve messages!
        return "Error";
    else
        return NULL;
}

char* os_condition_timed_wait(void* mutex, void* cond, uint64_t nanos) {
    assert(0 && "To do");
    abort();
}

char* os_condition_notify(void* cond) {
    int err = pthread_cond_signal((pthread_cond_t *)cond);
    if (err)
        // Need to improve messages!
        return "Error";
    else
        return NULL;
}

char* os_condition_notify_all(void* cond) {
    int err = pthread_cond_broadcast((pthread_cond_t *)cond);
    if (err)
        // Need to improve messages!
        return "Error";
    else
        return NULL;
}

pthread_t this_thread(void) {
    assert(sizeof(pthread_t) == sizeof(void*));
    return pthread_self();
}

extern int py_print(void);

/** For debugging only, not safe.
 * Only works for ascii (and latin-8?) strings */
char* ascii(GVMT_Object s) {
    static char buf[80];
    gvmt_call(py_print, 2, buf, s);
    return buf;
}

int os_strtod(const char* str, double* result) {
    errno = 0;
    *result = strtod(str, NULL);
    return errno;
}

#define LOG_BYTES_PER_WORD ((sizeof(void*) >> 2) + 1)

// Make sure these two structs are in sync with typeobject.h & object.h
// or its CRASH time!


struct object_head {
    struct type_head* klass;
    uintptr_t ref_count;
};

struct var_object_head {
    struct type_head* klass;
    uintptr_t ref_count;
    uintptr_t length;
};

struct type_head {
    struct type_head* klass;
    uintptr_t ref_count;
    uint32_t flags;
    char* tp_name;
    int32_t shape[5];
    int32_t variable;
    uint32_t length;
    intptr_t dict_offset;
};

/** shape function specified in gvmt spec */
GVMT_CALL int* gvmt_user_shape(GVMT_Object obj, int* buffer) {
    struct type_head* klass = ((struct object_head*)obj)->klass;
    int32_t *from;
    intptr_t size;
    assert(klass);
    assert((((int)klass) & 1) == 0);
    // Check assumptions about word size and signed shifts.
    assert(1 << LOG_BYTES_PER_WORD == sizeof(void*));
    assert(-2 >> 2 == -1);
    if (klass->variable) {
        int last;
        struct var_object_head* var_obj = (struct var_object_head*)obj;
        from = klass->shape;
        buffer[0] = from[0]; // class
        buffer[1] = from[1]; // (ref_count?), length (+others)
        assert(klass->variable < 0 || klass->variable == sizeof(void*));
        size = (klass->variable * var_obj->length);
        size >>= LOG_BYTES_PER_WORD;
        assert(size < 0 ?
            size * sizeof(void*) <= klass->variable * var_obj->length :
            size * sizeof(void*) == klass->variable * var_obj->length);
        last = 2;
        while (from[last]) {
            buffer[last] = from[last];
            last ++;
            assert(last < GVMT_MAX_SHAPE_SIZE - 3);
        }
        buffer[last] = size;
        buffer[last+1] = 0;
        if (klass->dict_offset) {
            assert(klass->dict_offset < 0);
            if (buffer[last] < 0) {
                buffer[last+1] = 1;
                buffer[last+2] = -1;
                buffer[last+3] = 0;
                last += 2;
            } else {
                buffer[last] ++;
                buffer[last+1] = -1;
                buffer[last+2] = 0;
                last++;
            }
        }
        assert(last < GVMT_MAX_SHAPE_SIZE - 2);
        return buffer;
    } else {
        return klass->shape;
    }
}

/** length function specified in gvmt spec */
GVMT_CALL intptr_t gvmt_user_length(GVMT_Object obj) {
    struct type_head* klass = ((struct type_head*)obj)->klass;
    assert(klass->shape[0] > 0);
    assert(klass->shape[2] >= 0);
    assert(klass->shape[3] <= 0);
    assert(klass->shape[4] == 0);
    if (klass->variable) {
        intptr_t size, length;
        struct var_object_head* var_obj = (struct var_object_head*)obj;
        assert(klass->shape[1] < 0);
        assert(klass->variable < 0 || klass->variable == sizeof(void*));
        length = klass->length;
        if (klass->dict_offset) {
            length += 2 * sizeof(void*);
        }
        size = (klass->variable * var_obj->length);
        size &= -4;
        if (size > 0)
            length += size;
        else
            length -= size;
        assert((length & (sizeof(void*)-1)) == 0);
        return length;
    } else {
        assert(klass->shape[1] <= 0);
        return klass->length;
    }
}

void print_options(void) {
    printf("Standard Python options:\n");
    printf("-h print this help and exit\n");
    printf("-V print version and exit\n");
    printf("HotPy specific options:\n");
    printf("-Hno-comp do not compile bytecodes\n");
    printf("-Hno-peephole no peephole optimisation on bytecodes\n");
    printf("-Hno-spec do not specialise bytecodes\n");
    printf("-Hno-defer do not defer object creation\n");
    printf("-Hlow-level replace all operations with low-level equivalents\n");
    printf("-Hpre-spec Perform pre-specialisation during tracing\n");
    printf("-HQ debug-optimise\n");
    printf("-Hd debug\n");
    printf("-Hstats print statistics at end of run. More information in debug build.\n");
    printf("-HmXZ set max heap memory to X*Z. Z is one of k/M/G\n");
    printf("-Hp bytecode execution count(debug-build only)\n");
    printf("-Ht bytecode trace(debug-build only)\n");
    printf("-Hbase do no optimisation, same as -Hno-tracing\n");
    printf("-Hno-tracing do not using tracing, prevents any optimisation\n");
    printf("-Hflow Write traced flow-graph(s) to \"hotpy_flow_graph.N.gv\"\n");
    printf("-Hflow-neat Write traced flow-graph(s) with less type information to \"hotpy_flow_graph.N.gv\"\n");
}

void native_start_thread(void* root) {
    gvmt_start_thread(STACK_SPACE, (gvmt_func_ptr)start_thread, 1, root);
}

void write_bytes_to_gsc(FILE* out, int length, uint8_t* bytes, char* name) {
    int i;
    fprintf(out, ".opaque\n");
    fprintf(out, ".public %s\n",name);
    fprintf(out, ".label %s\n", name);
    for (i = 0; i < length; i++) {
         gsc_write_uint8_t(out, bytes[i]);
    }
    fprintf(out, ".heap\n");
};

int nlz(uint32_t x) {
   int y, m, n;

   y = -(x >> 16);      // If left half of x is 0,
   m = (y >> 16) & 16;  // set n = 16.  If left half
   n = 16 - m;          // is nonzero, set n = 0 and
   x = x >> m;          // shift x right 16.
                        // Now x is of the form 0000xxxx.
   y = x - 0x100;       // If positions 8-15 are 0,
   m = (y >> 16) & 8;   // add 8 to n and shift x left 8.
   n = n + m;
   x = x << m;

   y = x - 0x1000;      // If positions 12-15 are 0,
   m = (y >> 16) & 4;   // add 4 to n and shift x left 4.
   n = n + m;
   x = x << m;

   y = x - 0x4000;      // If positions 14-15 are 0,
   m = (y >> 16) & 2;   // add 2 to n and shift x left 2.
   n = n + m;
   x = x << m;

   y = x >> 14;         // Set y = 0, 1, 2, or 3.
   m = y & ~(y >> 1);   // Set m = 0, 1, 2, or 2 resp.
   return n + 2 - m;
}

/** Divides un by vn, m = len(un), n = len(vn). un must be overallocated by one.
ie, although only m words are valid, m+1 must have been allocated.
Both un and vn are modified, un ends up with remainder. Quotient ends up in q.
Which must have allocated at least m-n+1 words. */

void divmnu(uint16_t q[], uint16_t un[], uint16_t vn[], int m, int n) {

    const unsigned b = 65536; // Number base (16 bits).
    unsigned qhat;            // Estimated quotient digit.
    unsigned rhat;            // A remainder.
    unsigned p;               // Product of two digits.
    int s, i, j, t, k;

    assert (m >= n && n > 1 && vn[n-1] != 0);

    // Normalize by shifting v left just enough so that
    // its high-order bit is on, and shift u left the
    // same amount.  We may have to append a high-order
    // digit on the dividend; we do that unconditionally.

    s = nlz(vn[n-1]) - 16;        // 0 <= s <= 16.
    for (i = n - 1; i > 0; i--)
        vn[i] = (vn[i] << s) | (vn[i-1] >> (16-s));
    vn[0] = vn[0] << s;

    un[m] = un[m-1] >> (16-s);
    for (i = m - 1; i > 0; i--)
        un[i] = (un[i] << s) | (un[i-1] >> (16-s));
    un[0] = un[0] << s;

    for (j = m - n; j >= 0; j--) {       // Main loop.
        // Compute estimate qhat of q[j].
        qhat = (un[j+n]*b + un[j+n-1])/vn[n-1];
        rhat = (un[j+n]*b + un[j+n-1]) - qhat*vn[n-1];
    again:
        if (qhat >= b || qhat*vn[n-2] > b*rhat + un[j+n-2])
        { qhat = qhat - 1;
            rhat = rhat + vn[n-1];
            if (rhat < b) goto again;
        }

        // Multiply and subtract.
        k = 0;
        for (i = 0; i < n; i++) {
            p = qhat*vn[i];
            t = un[i+j] - k - (p & 0xFFFF);
            un[i+j] = t;
            k = (p >> 16) - (t >> 16);
        }
        t = un[j+n] - k;
        un[j+n] = t;

        q[j] = qhat;              // Store quotient digit.
        if (t < 0) {              // If we subtracted too
            q[j] = q[j] - 1;       // much, add back.
            k = 0;
            for (i = 0; i < n; i++) {
                t = un[i+j] + vn[i] + k;
                un[i+j] = t;
                k = t >> 16;
            }
            un[j+n] = un[j+n] + k;
        }
    } // End j.
    // Unnormalize remainder
    for (i = 0; i < n; i++) {
        un[i] = (un[i] >> s) | (un[i+1] << (16-s));
    }
}

void negate_array(uint16_t un[], int len) {
    int i;
    uint32_t carry = 1;
    for (i = 0 ; i < len; i++) {
        uint16_t inv = ~un[i];
        carry = inv + carry;
        un[i] = carry;
        carry = carry >> 16;
    }
}

int is_power2(unsigned x) {
    int tmp = 1;
    while (tmp < x) {
        tmp <<= 1;
    }
    return x == tmp;
}

void* class_id_lock;
extern int global_class_count;

int get_next_class_id(void) {
    int id;
    os_mutex_lock(class_id_lock);
    id = global_class_count;
    global_class_count++;
    os_mutex_unlock(class_id_lock);
    return id;
}

int main(int argc, char** argv) {
    // Initialise the gvmt layer.
    GVMT_MAX_SHAPE_SIZE = 10;
    GVMT_MAX_OBJECT_NAME_LENGTH = 200;
    class_id_lock = os_mutex_create();
    gvmt_start_machine(STACK_SPACE, (gvmt_func_ptr)hotpy_main, 2, argc, argv);
    return 0;
}

/** Trace writing code */

char* intra_edge(char* out, void* trace_addr, int from, int to, char* label) {
    out += sprintf(out, "t%x:l%x -> t%x:l%x [ penwidth= \"0.7\"", (uintptr_t)trace_addr, from, (uintptr_t)trace_addr, to);
    if (label != NULL)
         out += sprintf(out, ", label = \"%s\" ", label);
     out += sprintf(out, "]\n");
     return out;
}

char* inter_edge(char* out, void* trace_addr, int index, void* target_addr, char* label) {
    out += sprintf(out, "t%x:l%x -> t%x:l0 [ penwidth= \"0.7\"", (uintptr_t)trace_addr, index, (uintptr_t)target_addr);
    if (label != NULL)
         out += sprintf(out, ", label = \"%s\" ", label);
     out += sprintf(out, "]\n");
     return out;
}

char* write_abort_node(char* out, void* trace_addr, int index, char* label) {
    // Do nothing for now...
    return out;
}

