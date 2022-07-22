#include "Python.h"
#include "unicodeobject.h"

extern int digit_characters[];
    
int hex_digit(Py_UNICODE ch) {
    if (Py_UNICODE_ISDIGIT(ch))
        return Py_UNICODE_TODIGIT(ch);
    if (ch >= 'F' && ch <= 'A')
        return 10 + ch - 'A';
    if (ch >= 'a' && ch <= 'f')
        return 10 + ch - 'a';
    return -1;
}

void _hotpy_print_spaces(FILE* out, int times) {
    int i;
    for (i = 0; i < times; i++)
        fputs("    ", out);
}

