#include <assert.h>
#include "hotpy.h"
#include "guard.h"

/** Guards.
 * Guards need to deoptimise code and then clean up 
 * themselves and all other guards in the same ring */
 
/** Guarded "things" are:
 *  class attributes
 *  global and builtin dictionaries
 *  sys.settrace, sys.setprofile
 */

R_guard make_guard(R_object o, R_str name, R_trace t) {
    R_guard g = gc_allocate(guard);
    SET_TYPE(g, type_guard);
    g->object = o;
    g->name = name;
    g->trace = t;
    if(t->guard_ring) {
        R_guard next = t->guard_ring->ring;
        t->guard_ring->ring = g;
        g->ring = next;
    } else {
        g->ring = g;   
        t->guard_ring = g;
    }
    return g;
}

static void delete_guard_from_dict(R_dict d, R_guard g) {
    R_str name = g->name;
    R_guard chain;
    // Two parts to chain, before self and afterwards:
    chain = (R_guard)dict_get_str(d, name);
    assert(chain);
    if (chain == g) {
        if (g->next)
            dict_set_str(d, name, (R_object)g->next);
        else
            dict_del_str(d, name);
    } else {
        while (chain->next != g) {
            chain = chain->next;  
            assert(chain);
        }
        chain->next = g->next;
    }
}

/***************** Class attribute guard ********/

void delete_attribute_guard(R_guard g) {
    R_type t = (R_type)g->object;
    TYPE_ASSERT(t, type);
    delete_guard_from_dict(t->guards, g);
}

void add_attribute_guard(R_type t, R_str name, R_trace trace) {
    R_guard g = make_guard((R_object)t, name, trace);
    g->delete = delete_attribute_guard;
    g->next = (R_guard)dict_get_str(t->guards, name);
    dict_set_str(t->guards, name, (R_object)g);
}

/*************** Dictionary guard **********/

static void delete_dict_value_guard(R_guard g) {        
    R_module_dict_values values = (R_module_dict_values)g->object;
    R_dict guards;
    TYPE_ASSERT(values, module_dict_values);
    guards = (R_dict)ITEM(values, LEN(values)-2);
    delete_guard_from_dict(guards, g);
}

void add_dict_value_guard(R_module_dict_values values, R_str key, R_trace t) {
    R_dict guards;
    R_guard g = make_guard((R_object)values, key, t);
    g->delete = delete_dict_value_guard;
    guards = (R_dict)ITEM(values, LEN(values)-2);
    if (guards == NULL) {
        guards = new_dict(4);        
        ITEM(values, LEN(values)-2) = (R_object)guards;
    }
    g->next = (R_guard)dict_get_str(guards, key);
    dict_set_str(guards, key, (R_object)g);
}

static void delete_dict_position_guard(R_guard g) {        
    R_module_dict_values values = (R_module_dict_values)g->object;
    R_dict guards;
    TYPE_ASSERT(values, module_dict_values);
    guards = (R_dict)ITEM(values, LEN(values)-1);
    delete_guard_from_dict(guards, g);
}

void add_dict_position_guard(R_module_dict_values values, R_str key, R_trace t) {
    R_dict guards;
    R_guard g = make_guard((R_object)values, key, t);
    g->delete = delete_dict_position_guard;
    guards = (R_dict)ITEM(values, LEN(values)-1);
    if (guards == NULL) {
        guards = new_dict(4);        
        ITEM(values, LEN(values)-1) = (R_object)guards;
    }
    g->next = (R_guard)dict_get_str(guards, key);
    dict_set_str(guards, key, (R_object)g);
}


/*************** Profile/Trace guard *********/

void delete_trace_guard(R_guard g) {
    R_thread_state ts = THREAD_STATE;
    R_guard start, chain;
    if (g->name == name_trace) {
        start = ts->trace_guards;
    } else {
        assert(g->name == name_profile);
        start = ts->profile_guards;
    }
    if (start == g) {
        start = g->next;
    } else {
        chain = start;
        while (chain->next != g) {
            chain = chain->next;
            assert(chain);
        }
        chain->next = g->next;
    }
    if (g->name == name_trace) {
        ts->trace_guards = start;
    } else {
        assert(g->name == name_profile);
        ts->profile_guards = start; 
    }
}
 
void add_sys_profile_guard(R_trace t) {
    R_thread_state ts = THREAD_STATE;
    R_guard g = make_guard(NULL, name_profile, t);
    g->delete = delete_trace_guard;
    g->next = ts->profile_guards;
    ts->profile_guards = g;
}

void add_sys_trace_guard(R_trace t) {
    R_thread_state ts = THREAD_STATE;
    R_guard g = make_guard(NULL, name_trace, t);
    g->delete = delete_trace_guard;
    g->next = ts->profile_guards;
    ts->profile_guards = g;
}

 
static void invalidate_trace(R_guard g) {
    R_guard ring = g->ring;
    g->trace->invalidated = 1;
    while(ring != g) {
        g->delete(g);
        ring = g->ring;
    }
}

void trigger_guard(R_guard g) {
    while (g) {
        invalidate_trace(g);
        g = g->next;
    }
}

