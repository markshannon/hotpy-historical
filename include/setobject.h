#ifndef SETOBJECT_H
#define SETOBJECT_H
#include "hotpy.h"

GVMT_OBJECT(set) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t lock; 
    GVMT_OBJECT(dict_values)* values; 
};

GVMT_OBJECT(frozenset) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t lock; 
    GVMT_OBJECT(dict_values)* values; 
};

R_set new_set(void);

/** Non-mutating methods **/

R_object PY_FUNC1(set_copy, R_set s);
R_object PY_FUNC2(set_difference, R_set s1, R_set s2);
R_object PY_FUNC2(set_intersection, R_set s1, R_set s2);
R_object PY_FUNC2(set_issubset, R_set s1, R_set s2);
R_object PY_FUNC2(set_issuperset, R_set s1, R_set s2);
R_object PY_FUNC2(set_symmetric_difference, R_set s1, R_set s2);
R_object PY_FUNC2(set_union, R_set s1, R_set s2);
R_object PY_FUNC1(set_empty, R_set s);
R_object PY_FUNC2(set_contains, R_set s, R_object o);

/***** Mutating methods *****/
 
R_object PY_FUNC2(set_add, R_set s, R_object o);
R_object PY_FUNC1(set_clear, R_set s);
R_object PY_FUNC2(set_discard, R_set s, R_object o);
R_object PY_FUNC2(set_remove, R_set s, R_object o);
R_object PY_FUNC1(set_pop, R_set s);

#ifdef NO_LOCKING

#define LOCK_SET(s) ((void)0)
#define UNLOCK_SET(s) ((void)0)

#else

#define LOCK_SET(s) if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(s), offsetof(GVMT_OBJECT(set), lock))
#define UNLOCK_SET(s) if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(s), offsetof(GVMT_OBJECT(set), lock))

#endif

#endif
