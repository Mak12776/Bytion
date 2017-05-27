
#ifndef _BYTION_MISC_H_
#define _BYTION_MISC_H_

#include <stdio.h>
#include "units.h"
#include "boolean.h"
#include "error_numbers.h"

extern inline bool Compare(const char *str1,const char *str2);
extern inline uint ConvertToReal(const char *str);
extern inline void ToReal(uchar in, uchar *out);
extern inline void ToHex(uchar in, uchar *out);
extern inline void ToChar(uchar in, uchar *out);

#endif
