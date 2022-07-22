#include "gvmt/gvmt.h"
#include "hotpy.h"
#include "optimise.h"
 
/** Wrapper for disassembler_pass */

void disassemble(R_code_object code, FILE* out) {  
    uint8_t* from, *to;
    from = get_bytecodes_from_code_object(code);
    fprintf(out, "Code at 0x%x:\n", from);
    if (from) {
        to = from + code->code->length;
        disassembler_pass(from, to, code, out);
    }
}
