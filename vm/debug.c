#include "hotpy.h"
#include "opcodes.h"
#include "optimise.h"

/** Debugging stuff, mainly for debugging HotPy itself
 */

/* This ought to be thread-local */
uint32_t opcode_counts[256];
uint32_t opcode_pair_counts[256*256];
uint8_t prev_opcode;

R_object safe_cast(R_object obj, R_type klass) {
    R_type cls = TYPE(obj);
    if (is_subclass_of(cls, klass)) {
        return obj;
    } else {
        char buf[200];
        R_str oname = cls->__name__;
        R_str cname = klass->__name__;
        py_print(buf, py_sprintf("%s is not subclass of %s\n",
                        oname,
                        cname));
        fatal(buf);
        return 0;
    }
}

static void _print_str(FILE* out, R_str str) {
    R_str s = py_sprintf( "\"%s\"", str);
    int i, len = LEN(s);
    for (i = 0; i < len; i++) {
        if (s->text[i] == '\n')
            s->text[i] = ' ';
    }
    py_fprint(out, s);
}

void print_short(R_object object) {
    R_type klass;
    R_str name;
    if (object) {
        if (object == (R_object)None) {
            fprintf(stderr, "None");
            return;
        } else if (object == GEN_STACK_MARKER) {
            fprintf(stderr, "MARKER");
            return;
        }
        klass = TYPE(object);
        if (klass == type_int) {
            if (gvmt_is_tagged(object))
                fprintf(stderr, "%d", int_from_py_int((R_int)object));
            else
                fprintf(stderr, "<int>");
        } else if (klass == type_float) {
            fprintf(stderr, "%g", ((R_float)object)->value);
        } else if (klass == type_bool) {
            R_bool b = (R_bool)object;
            fprintf(stderr, "%s", b->value ? " True" : "False");
        } else if (klass == type_type) {
            name = ((R_type)object)->__name__;
            py_fprint(stderr, py_sprintf( "%s", name));
        } else if (klass == type_str) {
            _print_str(stderr, (R_str)object);
        } else if (klass == type_function) {
            name = ((R_function)object)->__name__;
            py_fprint(stderr, py_sprintf( "%s()", name));
        } else if (klass == type_frame) {
            R_frame f = (R_frame)object;
            int i, len = f->length > 8 ? 8 : f->length;
            fprintf(stderr, "Frame[%d]{", f->length);
            if (len) {
                print_short(ITEM(f, 0));
                for (i = 1; i < len; i++) {
                    fprintf(stderr, ", ");
                    print_short(ITEM(f, i));
                }
                if (f->length > len)
                    fprintf(stderr, ", ...");
            }
            fprintf(stderr, "}");
        } else if (klass == type_tuple) {
            R_tuple t = (R_tuple)object;
            int i, len = t->length > 8 ? 8 : t->length;
            fprintf(stderr, "(");
            for (i = 0; i < len; i++) {
                print_short(ITEM(t, i));
                fprintf(stderr, ", ");
            }
            if (t->length > len)
                fprintf(stderr, ", ...");
            fprintf(stderr, ")");
        } else if (klass == type_bound_method) {
            R_bound_method bm = (R_bound_method)object;
            //py_fprint(stderr, "BM<");
            print_short(bm->object);
            fprintf(stderr, ".");
            print_short(bm->callable);
            //py_fprint(stderr, ">");
        } else {
            py_fprint(stderr, py_sprintf( "<%s>", klass->__name__));
        }
    } else {
        fprintf(stderr, "NULL");
    }
}

GVMT_Object print_stack(int items, ...) {
    GVMT_Value *ap;
    int i = 0;
    ap = gvmt_stack_top();
    for (i = items-1; i >= 0; i--) {
        print_short((R_object)gvmt_stack_read_object(ap+i));
        fprintf(stderr, " ");
    }
    return 0;
}

GVMT_Object second_pass_trace(uint8_t* ip, void* frame) {
    int i, n;
    n = gvmt_stack_depth();
    fprintf(stderr, "%d items: ", n);
    assert (n >= 0);
    if (n > 8) {
        n = 8;
        fprintf(stderr, "... ");
    }
    print_stack(n);
    fprintf(stderr, "\n");
    return 0;
}

R_object interpreter_trace(uint8_t* ip, R_frame frame, int tracing) {
    int i, n;
    GVMT_Object o;
    char* opname;
    if (profile_on == TRUE) {
        int i = opcode_counts[*ip];
        opcode_counts[*ip] = i + 1;
        i = opcode_pair_counts[prev_opcode*256 + *ip];
        opcode_pair_counts[prev_opcode*256 + *ip] = i + 1;
        prev_opcode = *ip;
    }
    if (trace_on == 0)
        return 0;
    opname = gvmt_opcode_names_interpreter[*ip];
    if (trace_show_stack == TRUE) {
        n = gvmt_stack_depth();
        assert(n >= 0);
        fprintf(stderr, "%d items: ", n);
 //       assert (n >= 0);
        if (n > 8) {
            n = 8;
            fprintf(stderr, "... ");
        }
        print_stack(n);
        fprintf(stderr, "\n");
    }
#ifndef BOOTSTRAP
    fprintf(stderr, "%08x: ", (int)ip);
    if (tracing) {
        assert(THREAD_STATE->trace);
        fprintf(stderr, "T");
    } else {
        if (THREAD_STATE->trace == (R_trace_record)NULL_R) {
            if (instruction_counter == &on_trace_instruction_counter)
                fprintf(stderr, "O");
            else if (instruction_counter == &on_except_instruction_counter)
                fprintf(stderr, "E");
            else
                fprintf(stderr, "I");
        } else {
            fprintf(stderr, "S");
        }
    }
#endif
    if (frame) {
        for(i = 3; i < frame->depth; i++)
            fprintf(stderr, ".");
        if (frame->f_back && frame->f_back->f_back && frame->f_back->f_back->f_code)
            fprintf(stderr, "[%x]", frame->f_back->f_back->next_ip -
                get_bytecodes_from_code_object(frame->f_back->f_back->f_code));
        if (frame->f_back && frame->f_back->f_code)
            fprintf(stderr, "[%x]", frame->f_back->next_ip -
                get_bytecodes_from_code_object(frame->f_back->f_code));
        fprintf(stderr, "[%x]", frame->next_ip -
            get_bytecodes_from_code_object(frame->f_code));
        py_fprint(stderr, frame->f_code->constants->__file__);
        i = frame->f_lineno;
        fprintf(stderr, ":%d: ", i);
//        fprintf(stderr, "%4d ", ip - get_bytecodes_from_code_object(frame->f_code));
    } else {
        fprintf(stderr, ":?: ");
    }
    fprintf(stderr, "%s ", opname);
    if (*ip == OPCODE(pack_params) || *ip == OPCODE(unpack)) {
        fprintf(stderr, "%d ", ip[1]);
    } else if (*ip == OPCODE(load_frame) || *ip == OPCODE(store_frame) ||
             *ip == OPCODE(fast_load_frame)) {
        fprintf(stderr, "%d(", ip[1]);
        if (frame)
            py_fprint(stderr, (R_str)ITEM(frame->f_code->local_names, ip[1]));
        fprintf(stderr, ")");
    } else if (*ip == OPCODE(binary) || *ip == OPCODE(unary)) {
        fprintf(stderr, "%d(", ip[1]);
        py_fprint(stderr, (R_str)ITEM(operator_names, ip[1]));
        fprintf(stderr, ")");
    } else if (*ip == OPCODE(load_global) || *ip == OPCODE(load_attr) ||
               *ip == OPCODE(store_attr)) {
        int index = (ip[1] << 8) | ip[2];
        if (index < 256) {
               py_fprint(stderr, (R_str)ITEM(standard_names, index));
        } else if (frame) {
            py_fprint(stderr, (R_str)ITEM(frame->f_code->constants->__names, index-256));
        }
//    } else if (*ip == OPCODE(load_slot) || *ip == OPCODE(store_slot)) {
//        if (frame)
//            py_fprint(stderr, (R_str)ITEM(frame->f_code->constants->__names, (ip[5] << 8) | ip[6]));
    } else if (*ip == OPCODE(fast_frame)) {
        fprintf(stderr, "%d", ip[1]);
    } else {
        int len = gvmt_opcode_lengths_interpreter[*ip];
        if (len == 2) {
            fprintf(stderr, "%d ", ip[1]);
        } else if (len == 3) {
            fprintf(stderr, "%d ", (short)((ip[1] << 8) | ip[2]));
        }
    }
    fprintf(stderr, "\n");
    return 0;
}

int check_frame_next_ip(R_frame frame) {
    uint8_t* code_start = get_bytecodes_from_code_object(frame->f_code);
    unsigned offset = frame->next_ip - code_start;
    return offset < LEN(frame->f_code->code);
}

