
#ifndef HOTPY_ABSTRACT_H
#define HOTPY_ABSTRACT_H

typedef void (*event_func)(R_thread_state ts);

R_str HotPyObject_Repr(R_object o);
R_str HotPyObject_Str(R_object o);

int HotPySequence_Check(R_object o);

int HotPyIter_Check(R_object obj);

R_object HotPyIter_Next(R_object iter);

intptr_t HotPyObject_Hash(R_object o);

intptr_t HotPyObject_HashNotImplemented(R_object o);

int HotPyObject_IsTrue(R_object o);

/* Flag bits for printing: */
#define Py_PRINT_RAW	1	/* No string quotes etc. */


R_object HotPyObject_GetIter(R_object seq);

R_object HotPyIter_Next(R_object iter);

#define HotPyNumber_Or(v, w) binary_call(operator_or_, v, w)
#define HotPyNumber_Xor(v, w) binary_call(operator_xor, v, w)
#define HotPyNumber_And(v, w) binary_call(operator_and_, v, w)
#define HotPyNumber_Lshift(v, w) binary_call(operator_lshift, v, w)
#define HotPyNumber_Rshift(v, w) binary_call(operator_rshift, v, w)
#define HotPyNumber_Subtract(v, w) binary_call(operator_sub, v, w)

#define HotPyNumber_Add(v, w) binary_call(operator_add, v, w)
#define HotPyNumber_Multiply(v, w) binary_call(operator_mul, v, w)
#define HotPyNumber_FloorDivide(v, w) binary_call(operator_floordiv, v, w)
#define HotPyNumber_TrueDivide(v, w) binary_call(operator_truediv, v, w)
#define HotPyNumber_Remainder(v, w) binary_call(operator_mod, v, w)
R_object HotPyNumber_Power(R_object v, R_object w, R_object z);

#define HotPyNumber_InPlaceOr(v, w) inplace_call(operator_or_, v, w)
#define HotPyNumber_InPlaceXor(v, w) inplace_call(operator_xor, v, w)
#define HotPyNumber_InPlaceAnd(v, w) inplace_call(operator_and_, v, w)
#define HotPyNumber_InPlaceLshift(v, w) inplace_call(operator_lshift, v, w)
#define HotPyNumber_InPlaceRshift(v, w) inplace_call(operator_rshift, v, w)
#define HotPyNumber_InPlaceSubtract(v, w) inplace_call(operator_sub, v, w)

#define HotPyNumber_InPlaceAdd(v, w) inplace_call(operator_iadd, v, w)
#define HotPyNumber_InPlaceMultiply(v, w) inplace_call(operator_imul, v, w)
#define HotPyNumber_InPlaceFloorDivide(v, w) inplace_call(operator_ifloordiv, v, w)
#define HotPyNumber_InPlaceTrueDivide(v, w) inplace_call(operator_itruediv, v, w)
#define HotPyNumber_InPlaceRemainder(v, w) inplace_call(operator_imod, v, w)

R_object HotPyNumber_InPlacePower(R_object v, R_object w, R_object z);

#define HotPyNumber_Negative(o) unary_call(operator_neg, o)
#define HotPyNumber_Positive(o) unary_call(operator_pos, o)
#define HotPyNumber_Invert(o) unary_call(operator_invert, o)

R_object HotPyNumber_ToBase(R_object n, int base);

R_object HotPySequence_Concat(R_object s, R_object o);
R_object HotPySequence_Repeat(R_object o, Py_ssize_t count);
R_object HotPySequence_InPlaceConcat(R_object s, R_object o);
R_object HotPySequence_InPlaceRepeat(R_object o, Py_ssize_t count);

R_object HotPySequence_Fast(R_object s, const char* msg);

#endif // HOTPY_ABSTRACT_H
