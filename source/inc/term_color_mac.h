#ifndef _COLOR_SETTING_MACROS_H_
#define _COLOR_SETTING_MACROS_H_

#ifdef __linux__
#define COL_BLACK 30
#define COL_RED 31
#define COL_GREEN 32
#define COL_YELLOW 33
#define COL_BLUE 34
#define COL_MAGENTA 35
#define COL_CYAN 36
#define COL_WHITE 37
#define COL_DEFAULT 39
#elif defined _WIN32 || defined _WIN64
#define COL_BLACK 0
#define COL_RED 12
#define COL_GREEN 10
#define COL_YELLOW 14
#define COL_BLUE 9
#define COL_MAGENTA 13
#define COL_CYAN 11
#define COL_WHITE 15
#define COL_GRAY 8
#define COL_DEFAULT 7
#endif

#ifdef __linux__ // linux color setting

#define COLOR_DEF_BYTE COL_DEFAULT
#define COLOR_CHAR COL_YELLOW
#define COLOR_ZERO COL_WHITE

#define COLOR_ENV COL_DEFAULT

#elif defined _WIN32 || defined _WIN64 //windows color setting

#define COLOR_DEF_BYTE COL_GRAY
#define COLOR_CHAR COL_YELLOW
#define COLOR_ZERO COL_DEFAULT

#define COLOR_ENV COL_DEFAULT

#endif // elif defined _WIN32 || defined _WIN64

#endif // ifndef _COLOR_SETTING_MACROS_H_
