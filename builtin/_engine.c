#include <unistd.h>
#include "hotpy.h"
//#include "image.h"
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <alloca.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include "dictionary.h"
#include "core_objects.h"
#include "attributes.h"
#include "descriptors.h"
#include "intobject.h"

R_object PY_FUNC1(path_exists, R_str name) {
    struct stat status;
    char *cname = alloca(name->length + 1);
    TYPE_ASSERT(name, str);
    py_print(cname, name);
    if (stat(cname, &status)) {
        return (R_object)FALSE;
    } else {
        return (R_object)TRUE;
    }
}

R_object PY_FUNC1(path_getmtime, R_str name) {
    int mtime;
    struct stat status;
    char *cname = alloca(name->length + 1);
    TYPE_ASSERT(name, str);
    py_print(cname, name); 
    if (stat(cname, &status)) {
        raise_str(type_IOError, py_sprintf("Cannot stat file: %s", name));  
    } 
    mtime = status.st_mtime;
    return (R_object)R_int_from_ssize_t(mtime);
}

extern R_object antlr_parse_file(char* filename) ;

R_object PY_FUNC1(antlr_parse, R_str str) {
    char *cname = alloca(str->length + 1);
    TYPE_ASSERT(str, str);
    py_print(cname, str); 
    return antlr_parse_file(cname);
}

