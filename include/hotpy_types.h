#ifndef HOTPY_TYPES_H
#define HOTPY_TYPES_H

#include <inttypes.h>

typedef intptr_t Py_ssize_t;

typedef long long PY_LONG_LONG;

typedef uint16_t Py_UNICODE;

typedef struct {
	int cf_flags;  /* bitmask of CO_xxx flags relevant to future */
} PyCompilerFlags;

#endif
