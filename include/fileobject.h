#ifndef HOTPY_FILEOBJECT_H
#define HOTPY_FILEOBJECT_H

GVMT_OBJECT(file) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t handle;   
};

R_object PY_FUNC3(open, R_str name, R_str mode, R_int buf_size);

R_bytearray read_whole_file(R_file file);

R_object PY_FUNC1(file_next, R_file file);

R_str read_whole_utf8_file_as_string(R_file f);

#endif // HOTPY_FILEOBJECT_H
