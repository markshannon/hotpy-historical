#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include "hotpy.h"
#include "core_objects.h"
#include "version.h"
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include "intobject.h"
#include "optimise.h"

/******* Bytecode loading and parser interface code */

/* Internal stuff */

#define read8(in, x) x = in[0]; ++in
#define read16(in, x) x = (((in)[0] << 8) | (in)[1]); in+=2

int disassemble_on_load = 0;

static R_tuple load_constants(unsigned char** bytes_pointer) {
    unsigned char* bytes = *bytes_pointer;
    uint32_t i, j, count;
    R_tuple result;
    read16(bytes, count);
//    fprintf(stderr, "Constants: %d\n", count);
    result = make_tuple(count);
    for (i = 0 ; i < count; i++) {
        long long ll;
        int type, size;
        union {
                 double d;
                 long long l;
             } u;
        read8(bytes, type);
        read16(bytes, size);
        if (type == 1) {
            R_float f = gc_allocate(float);
            ll = (((long long)bytes[0]) << 56) |
                 (((long long)bytes[1]) << 48) |
                 (((long long)bytes[2]) << 40) |
                 (((long long)bytes[3]) << 32) |
                 (((long long)bytes[4]) << 24) |
                 (((long long)bytes[5]) << 16) |
                 (((long long)bytes[6]) << 16) |
                 ((long long)bytes[7]) ;
            u.l = ll;
            // This assumes IEEE floating point.
//            fprintf(stderr, "    %gf\n", u.d);
            SET_TYPE(f, type_float);
            f->value = u.d;
//            fprintf(stderr, "Loading float: %g\n", u.d);
            ITEM(result, i) = (R_object)f;
        } else if (type == 3) {
            // FIX ME - size is number of bytes. should be number of chars.
            R_str s = string_from_utf8_bytes(size, bytes);
            ITEM(result, i) = (R_object)canonical_string(s);

//            fprintf(stderr, "    \"");
//            py_fprint(stderr, s);
//            fprintf(stderr, "\"\n");
        } else if (type == 2) {
            fatal("Byte constant not supported yet");
        } else if (type == 0) { // Integer
            ITEM(result, i) = (R_object)int_from_byte_array(bytes, size);
        } else {
            fatal("Incorrect type");
        }
        bytes += size;
    }
    *bytes_pointer = bytes;
    return result;
}

static R_tuple load_names(unsigned char** bytes_pointer) {
    unsigned char* bytes = *bytes_pointer;   
    uint32_t i, count;
    R_tuple result;
    read16(bytes, count);
//    fprintf(stderr, "Names: %d\n", count);
    result = make_tuple(count);
    for (i = 0; i < count; i++) {
        R_str str;
        unsigned char* data = bytes;
        read16(bytes, int len);
        // FIX ME - size is number of bytes. should be number of chars.
        str = string_from_utf8_bytes(len, bytes);
//        fprintf(stderr, "    ");
//        py_fprint(stderr, str);
//        fprintf(stderr, "\n");
        bytes += (len + 1) & -2;
        ITEM(result, i) = (R_object)canonical_string(str);
    }
    *bytes_pointer = bytes;
    return result;
}

/* TO DO - This repeatedly looks for the same file, mark when loaded and
 * do not reload */
static void ensure_lib_loaded(R_Constants module) {
    // Find library file for module...
    void* handle; 
    char buf[400];
    R_str fname = module->__file__;
    int len = py_print(buf, py_sprintf("%s", fname)); 
    buf[len -2] = 's';
    buf[len -1] = 'o';    
    handle = dlopen(buf, RTLD_NOW | RTLD_GLOBAL);
    if (handle == 0) {
        char* p = buf + py_print(buf, py_sprintf("Cannot load dynamic library for %s.\n", fname));
        sprintf(p, "%s", dlerror());
        fprintf(stderr, "%s\n", p);
        raise_char(type_ImportError, buf);
    }   
}    
 
static behaviour loadNative(R_Constants module, R_str name) {
    // Find library file for module...
    char buf[400];
    behaviour result;
    // Lookup symbol in library
    ensure_lib_loaded(module);
    py_print(buf, py_sprintf("PY_%s", name));
    result = (behaviour)dlsym(dl_symbols, buf);
    if (result == 0) {
        fprintf(stderr, "Cannot find %s in dynamic library\n", buf);
        py_print(buf, py_sprintf("Cannot find %s in dynamic library\n", name));
        raise_char(type_ImportError, buf);
    }
    return result;
}

R_str name_from_index(R_Constants consts, int index) {
    R_str name;
    if (index < 256) {
        name = (R_str)ITEM(standard_names, index);
    } else {
        name = (R_str)ITEM(consts->__names, index-256);
    }
    assert(name != NULL);
    return name;
}

static R_tuple load_functions(unsigned char** bytes_pointer, R_Constants consts) {
    unsigned char* bytes = *bytes_pointer;   
    uint32_t i, j, count, code_length, locals, dummy;
    R_tuple result, local_names;
    read16(bytes, count);
    result = make_tuple(count);
    for (i = 0; i < count; i++) {
        R_str code_name;
        unsigned short name;
        R_code_object code = gc_allocate(code_object);
        SET_TYPE(code, type_code_object);
        ITEM(result, i) = (R_object)code;
        read16(bytes, name);
        code_name = name_from_index(consts, name);
        code->names = consts->__names;
        code->name = code_name;
        read16(bytes, code->kind );
        read16(bytes, code->params );
        read16(bytes, locals );
        code->locals = locals;
        read16(bytes, code_length);
        code->constants = consts;
        code->code = gc_allocate_var(bytes, bytes, code_length);
        SET_TYPE(code->code, type_bytes);
        code->code->length = code_length;
        for (j = 0; j < code_length; j++)
            code->code->bytes[j] = bytes[j];
        bytes += code_length;
        if (code_length & 1)
            ++bytes;
        code->local_names = local_names = make_tuple(locals);
        for (j = 0; j < locals; j++) {
            read16(bytes, name);
            ITEM(local_names, j) = (R_object)name_from_index(consts, name);
        }
        if (disassemble_on_load) {
            fprintf(stderr, "Function ");
            py_fprint(stderr, code->name);
            fprintf(stderr, " kind:%d %d parameters, %d locals:\n", 
                    code->kind, code->params, code->locals);
            disassemble(code, stderr);
            fprintf(stderr, "\n");
        }
#ifndef BOOTSTRAP
        if (write_flow_graph) 
            print_function_flowgraph(code);
#endif
    }
    *bytes_pointer = bytes;
    return result;
}

static R_code_object build_module(unsigned char* bytes) {
    int i, j, count, index, number, dummy;
    R_code_object script;
    int x, script_length;
    R_Constants consts = gc_allocate(Constants);
    R_str file_name;
    SET_TYPE(consts, type_Constants);
    read16(bytes, number);
//    fprintf(stderr, "Magic number: %d\n", number);
    if (number != MAGIC_NUMBER)
        raise_char(type_ImportError, "Incorrect magic number");
    read16(bytes, number);
//    fprintf(stderr, "Version number: %d\n", number);
    if (number != VERSION_NUMBER) {
        fprintf(stdout, "%d %d\n", number, VERSION_NUMBER);
        raise_char(type_ImportError, "Incorrect version number");
    }
    consts->__names = load_names(&bytes);
    consts->__constants = load_constants(&bytes);
    read16(bytes, index);
    file_name = (R_str)ITEM(consts->__constants, index);
    consts->__file__ = file_name;
    consts->__functions = load_functions(&bytes, consts);
    read16(bytes, count);
    for (i = 0 ; i < count; i++) {
        read16(bytes, x);
        read16(bytes, x);
    }
    script = gc_allocate(code_object);
    SET_TYPE(script, type_code_object);
    script->name = name___main__;
    script->kind = 0;
    script->params = 0;
    script->locals = 0;
    read16(bytes, script_length);
    script->constants = consts;
    script->names = consts->__names;
    script->code = gc_allocate_var(bytes, bytes, script_length);
    SET_TYPE(script->code, type_bytes);
    script->code->length = script_length;
    for (i = 0; i < script_length; i++)
        script->code->bytes[i] = bytes[i];
    bytes += script_length;
    if (script_length & 1)
        ++bytes;
    script->local_names = empty_tuple;
    if (disassemble_on_load) {
        fprintf(stderr, "Module ");
        py_fprint(stderr, script->name);
        disassemble(script, stderr);
        fprintf(stderr, "\n");
    }
    return script;
}

/** External interface  */

R_object PY_FUNC1(c_function, R_function f) {
    R_builtin_function result;
    R_code_object code = f->__code__;
    TYPE_CHECK(f, type_function);   
    result = gc_allocate(builtin_function);
    SET_TYPE(result, type_builtin_function);
    result->parameter_format = f->parameter_format;
    result->executable = loadNative(code->constants, code->name);
    result->__name__ = f->__name__;
    result->source_name = f->__name__;
    result->paramNames = f->paramNames;
    result->defaults = f->defaults;
    if (is_empty(f->__annotations__))
        result->param_types = empty_tuple;
    else
        result->param_types = get_types_from_annotation(f);
    result->return_type = get_return_type_from_annotation(f);
    return (R_object)result;
}

R_object PY_FUNC3(build_module, R_bytes b, R_str name, R_str filename) {
    R_code_object script;
    R_module ns;
    void* ptr;
    uint8_t* bytes;
    TYPE_CHECK(name, type_str);
    TYPE_CHECK(b, type_bytes);
    ptr = gvmt_pin((GVMT_Object)b);
    bytes = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytes), bytes);
    script = build_module(bytes);
    ns = new_module(name, filename);
    run_script(script, ns, THREAD_STATE->current_frame);
    return (R_object)ns;
}

R_object antlr_parse_file(char* filename) {
    pid_t pd;
    pd = fork();
    if (pd == 0) {
        execlp("java", " ", "-classpath", BIN_PATH ":/usr/share/java/antlr.jar",
              "parser.Python", filename, NULL);
        exit(0);
        printf("In child process -- cannot reach here!\n");
    } else {
        int status;
        waitpid(pd, &status, 0);
    }
    return (R_object)None;
}

R_code_object load_source(char* fname) {
    long file_t, file_c_t;
    int length = strlen(fname);
    char* filename = alloca(length + 16);
    R_bytearray b;
    unsigned char* bytes;
    R_code_object code;
    pid_t pd;
    R_file f;
//    fprintf(stderr, "Loading: %s\n", fname);
    // Is source younger than compiled version or does compiled version not exist?
    file_t = file_timestamp(fname);
    strcpy(filename, fname);
    filename[length] = '.';
    filename[length + 1] = 'h';
    filename[length + 2] = 'o';
    filename[length + 3] = 't';
    filename[length + 4] = 'p';
    filename[length + 5] = 'y';
    filename[length + 6] = '.';
    filename[length + 7] = 'p';
    filename[length + 8] = 'y';
    filename[length + 9] = 'c';
    filename[length + 10] = 0;
    file_c_t = file_timestamp(filename);
    if (file_t >= file_c_t) {
        antlr_parse_file(fname);
    }
//    printf("Compiled file name: %s\n", filename_c);
    f = (R_file)PY_open(R_int_from_ssize_t(0), py_sprintf("r"), string_from_c_string(filename));
    b = read_whole_file(f);
    bytes = ((unsigned char*)gvmt_pin((GVMT_Object)b)) + offsetof(GVMT_OBJECT(bytes), bytes);
    code = build_module(bytes);
    return code;
}




