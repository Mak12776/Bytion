
#ifndef _ERROR_NUMBERS_H_
#define _ERROR_NUMBERS_H_

#ifdef __linux__
#include <errno.h>
#elif defined _WIN32 || defined _WIN64
#define EBADMSG 74
#define EILSEQ 84
#endif

#endif
