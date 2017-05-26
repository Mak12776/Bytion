
#ifndef _DEBUG_HEADER_
#define _DEBUG_HEADER_

#ifdef _DEBUG
#define LOGV(t,x) printf("Log; %d: ", __LINE__);printf( #x ": %" #t "\n", x);
#define LOG(...) printf("Log; %d: ", __LINE__);printf(__VA_ARGS__);
#define LOGN(x) printf("Log; %d: ", __LINE__);printf( x "\n");
#else
#define LOGV(t,x)
#define LOG(...)
#define LOGN(x)
#endif

#endif
