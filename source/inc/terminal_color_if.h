#ifndef _COLOR_SETTING_INLINE_FUNCTION_H_
#define _COLOR_SETTING_INLINE_FUNCTION_H_

#include <stdio.h>

extern char _terminal_color_fore;

inline void SetColor(const char clr)
{
  #ifdef __linux__
  if (_terminal_color_fore!=clr)
  {
    printf("\033[%dm", _terminal_color_fore=clr);
  }
  #elif defined _WIN32 || defined _WIN64
  if (_terminal_color_fore!=clr)
  {
    SetConsoleTextAttribute(hConsole, _terminal_color_fore=clr);
  }
  #endif
}

inline void printColored(const char *input, const char clr)
{
  #ifdef __linux__
  if (_terminal_color_fore==clr)
  {
    fputs(input, stdout);
  }
  else
  {
    printf("\033[%dm%s", _terminal_color_fore=clr, input);
  }
  #elif defined _WIN32 || defined _WIN64
  if (_terminal_color_fore!=clr)
  {
  SetConsoleTextAttribute(hConsole, _terminal_color_fore=clr);
  }
  fputs(input, stdout);
  #endif
}

#endif
