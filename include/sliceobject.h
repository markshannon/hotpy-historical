#ifndef HOTPY_SLICEOBJECT_H
#define HOTPY_SLICEOBJECT_H

GVMT_OBJECT(slice) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(object)* start;    /*  read-only */
    GVMT_OBJECT(object)* stop;      /* read-only */
    GVMT_OBJECT(object)* step;      /* read-only */
};

size_t decode_slice(R_slice s, intptr_t limit, intptr_t* start_p, intptr_t* stop_p, intptr_t* step_p);


#endif
