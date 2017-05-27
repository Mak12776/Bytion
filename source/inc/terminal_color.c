
#include "terminal_color.h"

char Color_fore=COLOR_ENV;

extern inline void SetColor(const char clr)
{
  #ifdef __linux__
  if (Color_fore!=clr)
  {
    printf("\033[%dm", Color_fore=clr);
  }
  #elif defined _WIN32 || defined _WIN64
  if (Color_fore!=clr)
  {
    SetConsoleTextAttribute(hConsole, Color_fore=clr);
  }
  #endif
}

extern inline void printColored(const char *input, const char clr)
{
  #ifdef __linux__
  if (Color_fore==clr)
  {
    fputs(input, stdout);
  }
  else
  {
    printf("\033[%dm%s", Color_fore=clr, input);
  }
  #elif defined _WIN32 || defined _WIN64
  if (Color_fore!=clr)
  {
  SetConsoleTextAttribute(hConsole, Color_fore=clr);
  }
  fputs(input, stdout);
  #endif
}
