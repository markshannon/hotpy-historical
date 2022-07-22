#ifndef HOTPY_THREAD_H
#define HOTPY_THREAD_H

/*** Thread stuff ***/

#define THREAD_STATE ((R_thread_state)gvmt_gc_read_tls_root(hotpy_thread_state))

typedef struct hotpy_condition* condition_t;
// Pointer to undefined struct - just for type checking
typedef struct hotpy_thread* thread_t;

R_Thread thread_init(void);

thread_t this_thread(void);

GVMT_Object start_thread(void* root);

void native_start_thread(void* root);

#endif

void* os_mutex_create(void);
char* os_mutex_lock(void* mutex);
char* os_mutex_unlock(void* mutex);
char* os_mutex_destroy(void* mutex);

void* os_condition_create(void);
char* os_condition_wait(void* mutex, void* cond);
char* os_condition_timed_wait(void* mutex, void* cond, uint64_t nanos);
char* os_condition_notify(void* cond);
char* os_condition_notify_all(void* cond);

