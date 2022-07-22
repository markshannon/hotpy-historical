#ifndef Py_UNICODEOBJECT_H
#define Py_UNICODEOBJECT_H

#include <stdarg.h>

/* This header defines necessary stuff to compile 
 * unicodectype.c without modification
*/

#include <ctype.h>

/* === Internal API ======================================================= */

/* --- Internal Unicode Format -------------------------------------------- */

/* Python 3.x requires unicode */
#define Py_USING_UNICODE 

/* Py_UNICODE is the native Unicode storage format (code unit) used by
   Python and represents a single Unicode element in the Unicode
   type. */


/* --- UCS-2/UCS-4 Name Mangling ------------------------------------------ */

/* Unicode API names are mangled to assure that UCS-2 and UCS-4 builds
   produce different external names and thus cause import errors in
   case Python interpreters and extensions with mixed compiled in
   Unicode width assumptions are combined. */


# define PyUnicode_AsASCIIString PyUnicodeUCS2_AsASCIIString
# define PyUnicode_AsCharmapString PyUnicodeUCS2_AsCharmapString
# define PyUnicode_AsDecodedObject PyUnicodeUCS2_AsDecodedObject
# define PyUnicode_AsDecodedUnicode PyUnicodeUCS2_AsDecodedUnicode
# define PyUnicode_AsEncodedObject PyUnicodeUCS2_AsEncodedObject
# define PyUnicode_AsEncodedString PyUnicodeUCS2_AsEncodedString
# define PyUnicode_AsEncodedUnicode PyUnicodeUCS2_AsEncodedUnicode
# define PyUnicode_AsLatin1String PyUnicodeUCS2_AsLatin1String
# define PyUnicode_AsRawUnicodeEscapeString PyUnicodeUCS2_AsRawUnicodeEscapeString
# define PyUnicode_AsUTF32String PyUnicodeUCS2_AsUTF32String
# define PyUnicode_AsUTF16String PyUnicodeUCS2_AsUTF16String
# define PyUnicode_AsUTF8String PyUnicodeUCS2_AsUTF8String
# define PyUnicode_AsUnicode PyUnicodeUCS2_AsUnicode
# define PyUnicode_AsUnicodeEscapeString PyUnicodeUCS2_AsUnicodeEscapeString
# define PyUnicode_AsWideChar PyUnicodeUCS2_AsWideChar
# define PyUnicode_ClearFreeList PyUnicodeUCS2_ClearFreelist
# define PyUnicode_Compare PyUnicodeUCS2_Compare
# define PyUnicode_Concat PyUnicodeUCS2_Concat
# define PyUnicode_Append PyUnicodeUCS2_Append
# define PyUnicode_AppendAndDel PyUnicodeUCS2_AppendAndDel
# define PyUnicode_Contains PyUnicodeUCS2_Contains
# define PyUnicode_Count PyUnicodeUCS2_Count
# define PyUnicode_Decode PyUnicodeUCS2_Decode
# define PyUnicode_DecodeASCII PyUnicodeUCS2_DecodeASCII
# define PyUnicode_DecodeCharmap PyUnicodeUCS2_DecodeCharmap
# define PyUnicode_DecodeLatin1 PyUnicodeUCS2_DecodeLatin1
# define PyUnicode_DecodeFSDefault PyUnicodeUCS2_DecodeFSDefault
# define PyUnicode_DecodeFSDefaultAndSize PyUnicodeUCS2_DecodeFSDefaultAndSize
# define PyUnicode_DecodeRawUnicodeEscape PyUnicodeUCS2_DecodeRawUnicodeEscape
# define PyUnicode_DecodeUTF32 PyUnicodeUCS2_DecodeUTF32
# define PyUnicode_DecodeUTF32Stateful PyUnicodeUCS2_DecodeUTF32Stateful
# define PyUnicode_DecodeUTF16 PyUnicodeUCS2_DecodeUTF16
# define PyUnicode_DecodeUTF16Stateful PyUnicodeUCS2_DecodeUTF16Stateful
# define PyUnicode_DecodeUTF8 PyUnicodeUCS2_DecodeUTF8
# define PyUnicode_DecodeUTF8Stateful PyUnicodeUCS2_DecodeUTF8Stateful
# define PyUnicode_DecodeUnicodeEscape PyUnicodeUCS2_DecodeUnicodeEscape
# define PyUnicode_Encode PyUnicodeUCS2_Encode
# define PyUnicode_EncodeASCII PyUnicodeUCS2_EncodeASCII
# define PyUnicode_EncodeCharmap PyUnicodeUCS2_EncodeCharmap
# define PyUnicode_EncodeDecimal PyUnicodeUCS2_EncodeDecimal
# define PyUnicode_EncodeLatin1 PyUnicodeUCS2_EncodeLatin1
# define PyUnicode_EncodeRawUnicodeEscape PyUnicodeUCS2_EncodeRawUnicodeEscape
# define PyUnicode_EncodeUTF32 PyUnicodeUCS2_EncodeUTF32
# define PyUnicode_EncodeUTF16 PyUnicodeUCS2_EncodeUTF16
# define PyUnicode_EncodeUTF8 PyUnicodeUCS2_EncodeUTF8
# define PyUnicode_EncodeUnicodeEscape PyUnicodeUCS2_EncodeUnicodeEscape
# define PyUnicode_Find PyUnicodeUCS2_Find
# define PyUnicode_Format PyUnicodeUCS2_Format
# define PyUnicode_FromEncodedObject PyUnicodeUCS2_FromEncodedObject
# define PyUnicode_FromFormat PyUnicodeUCS2_FromFormat
# define PyUnicode_FromFormatV PyUnicodeUCS2_FromFormatV
# define PyUnicode_FromObject PyUnicodeUCS2_FromObject
# define PyUnicode_FromOrdinal PyUnicodeUCS2_FromOrdinal
# define PyUnicode_FromString PyUnicodeUCS2_FromString
# define PyUnicode_FromStringAndSize PyUnicodeUCS2_FromStringAndSize
# define PyUnicode_FromUnicode PyUnicodeUCS2_FromUnicode
# define PyUnicode_FromWideChar PyUnicodeUCS2_FromWideChar
# define PyUnicode_FSConverter PyUnicodeUCS2_FSConverter
# define PyUnicode_GetDefaultEncoding PyUnicodeUCS2_GetDefaultEncoding
# define PyUnicode_GetMax PyUnicodeUCS2_GetMax
# define PyUnicode_GetSize PyUnicodeUCS2_GetSize
# define PyUnicode_IsIdentifier PyUnicodeUCS2_IsIdentifier
# define PyUnicode_Join PyUnicodeUCS2_Join
# define PyUnicode_Partition PyUnicodeUCS2_Partition
# define PyUnicode_RPartition PyUnicodeUCS2_RPartition
# define PyUnicode_RSplit PyUnicodeUCS2_RSplit
# define PyUnicode_Replace PyUnicodeUCS2_Replace
# define PyUnicode_Resize PyUnicodeUCS2_Resize
# define PyUnicode_RichCompare PyUnicodeUCS2_RichCompare
# define PyUnicode_SetDefaultEncoding PyUnicodeUCS2_SetDefaultEncoding
# define PyUnicode_Split PyUnicodeUCS2_Split
# define PyUnicode_Splitlines PyUnicodeUCS2_Splitlines
# define PyUnicode_Tailmatch PyUnicodeUCS2_Tailmatch
# define PyUnicode_Translate PyUnicodeUCS2_Translate
# define PyUnicode_TranslateCharmap PyUnicodeUCS2_TranslateCharmap
# define _PyUnicode_AsDefaultEncodedString _PyUnicodeUCS2_AsDefaultEncodedString
# define _PyUnicode_Fini _PyUnicodeUCS2_Fini
# define _PyUnicode_Init _PyUnicodeUCS2_Init
# define _PyUnicode_IsAlpha _PyUnicodeUCS2_IsAlpha
# define _PyUnicode_IsDecimalDigit _PyUnicodeUCS2_IsDecimalDigit
# define _PyUnicode_IsDigit _PyUnicodeUCS2_IsDigit
# define _PyUnicode_IsLinebreak _PyUnicodeUCS2_IsLinebreak
# define _PyUnicode_IsLowercase _PyUnicodeUCS2_IsLowercase
# define _PyUnicode_IsNumeric _PyUnicodeUCS2_IsNumeric
# define _PyUnicode_IsPrintable _PyUnicodeUCS2_IsPrintable
# define _PyUnicode_IsTitlecase _PyUnicodeUCS2_IsTitlecase
# define _PyUnicode_IsXidStart _PyUnicodeUCS2_IsXidStart
# define _PyUnicode_IsXidContinue _PyUnicodeUCS2_IsXidContinue
# define _PyUnicode_IsUppercase _PyUnicodeUCS2_IsUppercase
# define _PyUnicode_IsWhitespace _PyUnicodeUCS2_IsWhitespace
# define _PyUnicode_ToDecimalDigit _PyUnicodeUCS2_ToDecimalDigit
# define _PyUnicode_ToDigit _PyUnicodeUCS2_ToDigit
# define _PyUnicode_ToLowercase _PyUnicodeUCS2_ToLowercase
# define _PyUnicode_ToNumeric _PyUnicodeUCS2_ToNumeric
# define _PyUnicode_ToTitlecase _PyUnicodeUCS2_ToTitlecase
# define _PyUnicode_ToUppercase _PyUnicodeUCS2_ToUppercase
                                     
int _PyUnicode_IsLowercase(Py_UNICODE ch);
int _PyUnicode_IsUppercase(Py_UNICODE ch);
int _PyUnicode_IsTitlecase(Py_UNICODE ch); 
int _PyUnicode_IsLinebreak(Py_UNICODE ch);
Py_UNICODE _PyUnicode_ToLowercase(Py_UNICODE ch);
Py_UNICODE _PyUnicode_ToUppercase(Py_UNICODE ch);
Py_UNICODE _PyUnicode_ToTitlecase(Py_UNICODE ch);
int _PyUnicode_IsDecimalDigit(Py_UNICODE ch);
int _PyUnicode_IsDigit(Py_UNICODE ch);
int _PyUnicode_IsNumeric(Py_UNICODE ch);
int _PyUnicode_IsPrintable(Py_UNICODE ch);
int _PyUnicode_ToDecimalDigit(Py_UNICODE ch);
int _PyUnicode_ToDigit(Py_UNICODE ch);
double _PyUnicode_ToNumeric(Py_UNICODE ch);
int _PyUnicode_IsAlpha(Py_UNICODE ch);
                             
/* --- Internal Unicode Operations ---------------------------------------- */

/* Since splitting on whitespace is an important use case, and
   whitespace in most situations is solely ASCII whitespace, we
   optimize for the common case by using a quick look-up table
   _Py_ascii_whitespace (see below) with an inlined check.

 */
#define Py_UNICODE_ISSPACE(ch) \
	((ch) < 128U ? _Py_ascii_whitespace[(ch)] : _PyUnicode_IsWhitespace(ch))

#define Py_UNICODE_ISLOWER(ch) _PyUnicode_IsLowercase(ch)
#define Py_UNICODE_ISUPPER(ch) _PyUnicode_IsUppercase(ch)
#define Py_UNICODE_ISTITLE(ch) _PyUnicode_IsTitlecase(ch)
#define Py_UNICODE_ISLINEBREAK(ch) _PyUnicode_IsLinebreak(ch)

#define Py_UNICODE_TOLOWER(ch) _PyUnicode_ToLowercase(ch)
#define Py_UNICODE_TOUPPER(ch) _PyUnicode_ToUppercase(ch)
#define Py_UNICODE_TOTITLE(ch) _PyUnicode_ToTitlecase(ch)

#define Py_UNICODE_ISDECIMAL(ch) _PyUnicode_IsDecimalDigit(ch)
#define Py_UNICODE_ISDIGIT(ch) _PyUnicode_IsDigit(ch)
#define Py_UNICODE_ISNUMERIC(ch) _PyUnicode_IsNumeric(ch)
#define Py_UNICODE_ISPRINTABLE(ch) _PyUnicode_IsPrintable(ch)

#define Py_UNICODE_TODECIMAL(ch) _PyUnicode_ToDecimalDigit(ch)
#define Py_UNICODE_TODIGIT(ch) _PyUnicode_ToDigit(ch)
#define Py_UNICODE_TONUMERIC(ch) _PyUnicode_ToNumeric(ch)

#define Py_UNICODE_ISALPHA(ch) _PyUnicode_IsAlpha(ch)

#define Py_UNICODE_ISALNUM(ch) \
       (Py_UNICODE_ISALPHA(ch) || \
        Py_UNICODE_ISDECIMAL(ch) || \
        Py_UNICODE_ISDIGIT(ch) || \
        Py_UNICODE_ISNUMERIC(ch))

#define Py_UNICODE_COPY(target, source, length)				\
	Py_MEMCPY((target), (source), (length)*sizeof(Py_UNICODE))

#define Py_UNICODE_FILL(target, value, length) \
    do {Py_ssize_t i_; Py_UNICODE *t_ = (target); Py_UNICODE v_ = (value);\
        for (i_ = 0; i_ < (length); i_++) t_[i_] = v_;\
    } while (0)

/* Check if substring matches at given offset.  the offset must be
   valid, and the substring must not be empty */

#define Py_UNICODE_MATCH(string, offset, substring) \
    ((*((string)->str + (offset)) == *((substring)->str)) && \
    ((*((string)->str + (offset) + (substring)->length-1) == *((substring)->str + (substring)->length-1))) && \
     !memcmp((string)->str + (offset), (substring)->str, (substring)->length*sizeof(Py_UNICODE)))

/* --- Constants ---------------------------------------------------------- */

/* This Unicode character will be used as replacement character during
   decoding if the errors argument is set to "replace". Note: the
   Unicode character U+FFFD is the official REPLACEMENT CHARACTER in
   Unicode 3.0. */

#define Py_UNICODE_REPLACEMENT_CHARACTER ((Py_UNICODE) 0xFFFD)

#endif /* !Py_UNICODEOBJECT_H */
