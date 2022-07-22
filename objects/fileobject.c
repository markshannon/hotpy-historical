#include "hotpy.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static int _flags[] = {
    0,
    O_RDONLY, 
    O_WRONLY,
    O_RDWR
};

int flags(R_str mode) {
    int i, f = 0;
    uint16_t c;
    for (i = 0; i < mode->length;i++) {
        c = mode->text[i];
        if (c == 'r')
            f |= 1;
        if (c == 'w')
            f |= 2;
    }
    return _flags[f];
}

// FIX ME - Use buf_size
R_object PY_FUNC3(open, R_str name, R_str mode, R_int buf_size) { 
    R_file file;
    char* filename;
    int i;
    TYPE_CHECK(name, type_str);
    file = gc_allocate(file);
    filename = alloca(name->length + 2);
    SET_TYPE(file, type_file);
    for (i = 0; i < name->length; i++) {
        filename[i] = name->text[i];
    }
    filename[name->length] = 0;
    file->handle = open(filename, flags(mode));
    if (file->handle < 0) {
        R_str error = py_sprintf("Cannot open file: %s", name);
        raise_str(type_IOError, error);  
    }
    return (R_object)file;
}

R_bytearray read_whole_file(R_file file) {
    R_bytearray bytes;
    int i, len;
    struct stat status;
    uint8_t* buffer;
    void* ptr;
    TYPE_CHECK(file, type_file);
    fstat(file->handle, &status);
    bytes = make_bytearray(status.st_size);
    ptr = gvmt_pin((GVMT_Object)bytes);
    buffer = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    len = read(file->handle, buffer, status.st_size);
    if (len < bytes->length) {
        R_bytearray copy = make_bytearray(len);
        for (i = 0; i < len; i++) {
            copy->bytes[i] = bytes->bytes[i];
        }
        return copy;
    } 
    assert(len == bytes->length);
    return bytes;
}

R_object PY_FUNC1(read, R_file file) {
    return (R_object)read_whole_file(file);
}

R_str read_whole_utf8_file_as_string(R_file f) {
    R_bytearray b = read_whole_file(f);
    // Find default encoding/File encoding...
    return decode_utf8_bytes((R_bytes)b);
}    

// Temporary function for lexer, until proper I/O stack is implemented
R_object PY_FUNC1(_read_as_utf8, R_file file) {
    return (R_object)read_whole_utf8_file_as_string(file);
}

