
#include "hotpy.h"

static R_object make_tagged_range(int begin, int end, int step) {
    R_range r = gc_allocate(range);
    SET_TYPE(r, type_range);
    r->_step = (R_int)gvmt_tag(step);
    if (step == 1)
        raise_char(type_ValueError, "range() step argument must not be zero");
    r->_begin = (R_int)gvmt_tag(begin);
    if (step > 1) {
        if (end > begin) {
            r->_end = (R_int)gvmt_tag(end);
        } else {
             r->_end = (R_int)gvmt_tag(begin);
        }
    } else {
        if (end < begin) {
            r->_end = (R_int)gvmt_tag(end);
        } else {
            r->_end = (R_int)gvmt_tag(begin);
        }
    }
    return (R_object)r;
}

R_object make_range(R_int begin, R_int end, R_int step) {
    int cmp;
    R_range r = gc_allocate(range);
    SET_TYPE(r, type_range);
    if (!IS_INT(begin))
        begin = HotPyNumber_Index((R_object)begin);
    if (!IS_INT(end))
        end = HotPyNumber_Index((R_object)end);
    if (!IS_INT(step))
        step = HotPyNumber_Index((R_object)step);
    r->_step = step;
    cmp = py_int_int_compare(step, PY_SMALL_INT(0));
    if (cmp == 0)
        raise_char(type_ValueError, "range() step argument must not be zero");
    r->_begin = begin;
    if (cmp > 0) {
        if (py_int_int_compare(end, begin) > 0) {
            r->_end = end;
        } else {
             r->_end = begin;
        }
    } else {
        if (py_int_int_compare(end, begin) < 0) {
             r->_end = end;
        } else {
             r->_end = begin;
        }
    }
    return (R_object)r;
}

R_object PY_FUNC3(range, R_int begin, R_int end, R_int step) {
    intptr_t b, e, s;
    if (end == (R_int)None) {
        end = begin;
        begin = PY_SMALL_INT(0);
    }
    b = (intptr_t)gvmt___untag__((GVMT_Object)begin);
    e = (intptr_t)gvmt___untag__((GVMT_Object)end);
    s  = (intptr_t)gvmt___untag__((GVMT_Object)step);
    if (b & e & s & 1) {       
        return make_tagged_range(b, e, s);
    } else {
        return make_range(begin, end, step);
    }
}
    
R_range_iterator make_range_iterator(R_range range) {
    R_range_iterator it = gc_allocate(range_iterator);
    SET_TYPE(it, type_range_iterator);
    it->next = range->_begin;
    it->end = range->_end;
    it->step = range->_step;
    return it;
}

R_object PY_FUNC1(range_iterator_next, R_range_iterator it) {
    R_object result;
    int comp = py_int_int_compare(it->next, it->end);
    if (py_int_int_compare(it->step, PY_SMALL_INT(0))) {
        if (comp >= 0) {
            stop_iteration();
        }
    } else {
        if (comp <= 0) {
            stop_iteration();
        }
    }
    result = (R_object)it->next;
    it->next = (R_int)PY_add_int_int(it->next, it->step);
    return result;
}

R_object PY_FUNC1(next_int_iterator, R_int_iterator it) {
    R_int result;
    intptr_t next, end;
    next = (intptr_t)gvmt___untag__((GVMT_Object)it->next);
    end = (intptr_t)gvmt___untag__((GVMT_Object)it->end);
    if (next & end & 1) {
        if (next >= end)
            stop_iteration();
        result = (R_int)gvmt_tag(next);
        it->next = (R_int)gvmt_tag(next+2);
    } else {
        if (py_int_int_compare(it->next, it->end) >= 0)
            stop_iteration();
        result = it->next;
        it->next = (R_int)PY_add_int_int(it->next, PY_SMALL_INT(1)); 
    }
    return (R_object)result;
}

R_object PY_FUNC1(range_iter, R_range self) {
    R_int step = self->_step;
    if (py_int_is_one(step)) {
        R_int_iterator it = gc_allocate(int_iterator);
        SET_TYPE(it, type_int_iterator);
        it->next = self->_begin;
        it->end = self->_end;
        return (R_object)it;
    } else {
        return (R_object)make_range_iterator(self);
    }
}
    
R_object PY_FUNC2(range_getitem, R_range self, R_object o) {
    R_object item;
    R_int step = self->_step;
    if (py_int_is_one(step)) {
        if (IS_INT(o)) {
            item = o;
        } else {
            item = (R_object)HotPyNumber_Index(o);
        }
        item = PY_add_int_int((R_int)item, self->_begin);
        if (py_int_int_compare((R_int)item, self->_end) >= 0) {
            raise_char(type_IndexError, "range object index out of range");
        }
        return item;
    } else {
        R_int i, loc;
        if (IS_INT(o))
            i = (R_int)o;
        else
            i = HotPyNumber_Index(o);
        assert(0 && "To do - general range_getitem");
        return 0;
        //return type_type->call(type_range_iterator, 3);
    }
}

