#include "hotpy.h"
#include "longobject.h"

R_object PY_FUNC1(make_slice, R_tuple args) {
    R_slice slice;
    R_object start, stop, step;
    if (args->length > 1) {
        if (args->length == 2) {
            start = ITEM(args, 0);
            stop = ITEM(args, 1);
            step = (R_object)None;
        } else if (args->length == 3) {
            start = ITEM(args, 0);
            stop = ITEM(args, 1);
            step = ITEM(args, 2);
        } else {
            char buf[80];
            sprintf(buf, "slice expected at most 3 arguments, got %d", args->length);
            raise_char(type_TypeError, buf);
            start = stop = step = NULL;
        }
    } else {
        if (args->length == 1) {
            start = (R_object)None;
            stop = ITEM(args, 0);
            step = (R_object)None;
        } else {
            char buf[80];
            sprintf(buf, "slice expected at least 1 argument, got %d", args->length);
            raise_char(type_TypeError, buf);
            start = stop = step = NULL;
        }
    }
    slice = gc_allocate(slice);
    SET_TYPE(slice, type_slice);
    slice->start = start;
    slice->stop = stop;
    slice->step = step;
    return (R_object)slice;
}

intptr_t slice_index(R_object o) {
    if (!IS_INT(o)) {
        o = (R_object)HotPyNumber_Index(o);
    }
    return int_from_py_int((R_int)o);
}

/** Returns the length of the slice */
// This code is copied almost verbatim from CPython */
size_t decode_slice(R_slice s, intptr_t limit, intptr_t* start_p, intptr_t* stop_p, intptr_t* step_p) {
    intptr_t start, stop, step;
    if (s->step == (R_object)None) {
        step = 1;
    } else if (gvmt_is_tagged(s->step)) {
        step = as_int(s->step);
    } else {
        step = slice_index(s->step);
    }
    if (s->start == (R_object)None) {
        start = step < 0 ? limit-1 : 0;
    } else {
        if (gvmt_is_tagged(s->start))
            start = as_int(s->start);
        else
            start = slice_index(s->start);
        if ((unsigned)start >= limit) {
            if (start < 0)
                start += limit;
            if (start < 0) 
                start = (step < 0) ? -1 : 0;
            if (start >= limit) 
                start = (step < 0) ? limit - 1 : limit;
        }
    }
	if (s->stop == (R_object)None) {
		stop = step < 0 ? -1 : limit;
	} else {
        if (gvmt_is_tagged(s->stop))
            stop = as_int(s->stop);
        else
    	    stop = slice_index(s->stop);
    	if ((unsigned)stop >= limit) {
            if (stop < 0) 
                stop += limit;
            if (stop < 0) 
                stop = (step < 0) ? -1 : 0;
            if (stop >= limit)
                stop = (step < 0) ? limit - 1 : limit;
        }
	}
    *start_p = start;
    *stop_p = stop;
    *step_p = step; 
	if ((step < 0 && stop >= start) || (step > 0 && start >= stop)) {
		return 0;
	} else if (step < 0) {
		return (stop-start+1)/(step)+1;
	} else {
		return (stop-start-1)/(step)+1;
	}
}

