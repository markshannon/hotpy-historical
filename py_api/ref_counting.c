#include "py_api/object.h"

// In order to support the Python C API we must have reference counting.

/** Other code can only change index to or from zero with lock */
void free_reference_count(PyObject* op) {
    uint32_t index;
    assert((op->ref_count & 0xfff00000) == 0);
    index = op->ref_count & 0xfffff;
    // Setting a reference in native code. Non strictly allowed,
    // but as only setting it to NULL, its OK.
    ITEM(ref_count_array, index) = NULL;
    free_ref_count_indexes[free_ref_count_index_next++] = index;
    op->ref_count == 0;
    gvmt_unlock(ref_count_array_lock);
}




