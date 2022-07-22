#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <inttypes.h> 
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>   
#include "gvmt/arena.h"

/** OS (just Unix at moment) specific stuff */

void fatal(char* format, ...);

time_t file_timestamp(char* name) {
    struct stat status;
    int err = stat(name, &status);
    if (err) {
        return 0; 
    } else {
        return status.st_mtime;
    }
}

typedef void (*event_func)(void);

struct ts {  // special
    void* ob_type; /* read-only */
    uintptr_t ref_count;
    event_func event;    /* pointer */   
};

static __thread struct ts* thread_state;

void _hotpy_register_native_thread_state(void* ts) {
    thread_state = (struct ts*)ts;
}

//extern void handle_internal_error(int sig_num);
extern void interrupt_event(void);

void handle_interrupt(int sig) {
    thread_state->event = interrupt_event;
}

void os_init(void) {
    signal(SIGINT, handle_interrupt);
    //signal(SIGSEGV, handle_internal_error);
}

