
#include "dictionary.h"

/** Internal helper for attribute lookup */
#define FIELD(t,o,x) (*((t*)(((char*)(o))+(x))))

void attribute_error(R_type klass, R_object name);

/*** These are the top-level methods for attribute access  */

R_object _HotPyObject_GetAttr(R_object obj, R_str name);
void _HotPyObject_SetAttr(R_object obj, R_str name, R_object v);

