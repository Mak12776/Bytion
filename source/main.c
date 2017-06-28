
#define _VERSION "0.122.1"
#define _CREATOR "Mohammad Amin Khakzadan"
#define _CREATOR_GMAIL "mak12776@gmail.com"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "inc/units.h"
#include "inc/err_messages.h"
#include "inc/term_color_mac.h"

#include "inc/display.h"
#include "inc/dump.h"

#include "inc/inline/misc.h"

#if defined _WIN32 || defined _WIN64
#include <windows.h>
#endif

/*
* Debug tools:
* LOGV(t, x) show value x of type t
* LOG(...)   printf(...)
* LOGN(x)    printf(x "\n")
*/

#define USAGE "Usage: %s [Option] file [file1] [file2] [file3] ...\n"
#define HELP_DOC "\
Options:\n\
  -help                                     show this help and exit\n\
  -n [number], -column-number [number]      specify number of output columns\n\
  -m [mode], -mode [mode]                   specify modes for output results\n\
                                            modes can be a string of letters\n\
  -nm, -no-mode                             restore to default mode\n\
  -d, -display                              read files and write them on stdout\n\
  -ds, -display-strings                     read files and write only strings on stdout\n\
  -dm, -dump                                dump files byte into stdout\n\
  -v, -version                              show version & creator\n\
\n\
Modes:\n\
  for display:                              default mode: nothing\n\
  c                                         colorful.\n\
  l                                         show ascii letters.\n\
  d                                         show bytes in decimal.\n\
  for dump:                                 default mode: nothing\n\
  c                                         colorful.\n\
  s                                         print space instead of dot\n\
"

#define DEFAULT_NUMBER_OF_COLOMNS 15

#define enum_type_file 1
#define enum_type_mode 2
#define enum_type_no_mode 3

#define COMM_DISPLAY 1
#define COMM_DISPLAY_STRING 2
#define COMM_DUMP_FILE 3
#define COMM_FIND 4

#define MODE_NOTHING 0                 // do not change this macro values
#define MODE_COLOR   0b0001 // display, dump
#define MODE_CHAR    0b0010 // display
#define MODE_NUMBER  0b0100 // display
#define MODE_NO_DOT  0b1000 // dump

// variables
char selected_option=0;
program_mode_t program_mode=MODE_NOTHING;
arg_number_list_t *arg_number_list=NULL;

int Work_number=1;
uint NoColumn=DEFAULT_NUMBER_OF_COLOMNS;
char _terminal_color_fore=COLOR_ENV;

#if defined _WIN32 || defined _WIN64
HANDLE hConsole;
#endif

int main(int argc, char const *argv[])
{
  int check_index=1;

  #ifdef _DEBUG
  printf("---debuging mode---\n");
  #endif

  #if defined _WIN32 || defined _WIN64
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // SetConsoleTextAttribute(hConsole, Color_fore=COLOR_ENV);
  #endif

  if (argc == 1)
  {
    printf(USAGE HELP_ERR, argv[0], argv[0]);
    return 0;
  }

  arg_number_list=calloc(argc-1, sizeof(arg_number_list_t));
  if (!arg_number_list)
  {
    fprintf(stderr, _ERR_ MEMORY_ERR);
    return 1;
  }

  // get infromation
  for (check_index=1; check_index<argc; ++check_index)
  {
    if (argv[check_index][0]=='-')
    {
      if (Compare(argv[check_index]+1, "help")) //help
      {
        printf(USAGE "\n" HELP_DOC, argv[0]);
        return 0;
      }
      if (Compare(argv[check_index]+1, "d") || Compare(argv[check_index]+1, "display")) //display
      {
        if (selected_option)
        {
          fprintf(stderr, _ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return 1;
        }
        else
        {
          selected_option=COMM_DISPLAY;
          continue;
        }
      }
      if (Compare(argv[check_index]+1, "ds") || Compare(argv[check_index]+1, "display-strings")) // display-strings
      {
        if (selected_option)
        {
          fprintf(stderr, _ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return 1;
        }
        else
        {
          selected_option=COMM_DISPLAY_STRING;
          continue;
        }
      }
      if (Compare(argv[check_index]+1, "dm") || Compare(argv[check_index]+1, "dump")) // dump
      {
        if (selected_option)
        {
          fprintf(stderr, _ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return 1;
        }
        else
        {
          selected_option=COMM_DUMP_FILE;
          continue;
        }
      }
      if (Compare(argv[check_index]+1, "f") || Compare(argv[check_index]+1, "find")) //find
      {
        if (selected_option)
        {
          fprintf(stderr, _ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return 1;
        }
        else
        {
          selected_option=COMM_FIND;
          continue;
        }
      }
      if (Compare(argv[check_index]+1, "m") || Compare(argv[check_index]+1, "mode")) //mode
      {
        arg_number_list[check_index++]=enum_type_mode;
        continue;
      }
      if (Compare(argv[check_index]+1, "nm") || Compare(argv[check_index]+1, "no-mode")) //no-mode
      {
        arg_number_list[check_index-1]=enum_type_no_mode;
        continue;
      }
      if (Compare(argv[check_index]+1, "n") || Compare(argv[check_index]+1, "column-number")) //column-number
      {
        errno=0;
        NoColumn=ConvertToReal(argv[++check_index]);
        if (errno)
        {
          fprintf(stderr, _ERR_ INVALID_NUMBER, argv[check_index]);
          return 1;
        }
        continue;
      }
      if (Compare(argv[check_index]+1, "v") || Compare(argv[check_index]+1, "version")) //version
      {
        printf("Bytion v" _VERSION "\nby " _CREATOR ", " _CREATOR_GMAIL "\n");
        return 0;
      }
      fprintf(stderr, _ERR_ INVALID_COM HELP_ERR, argv[check_index], argv[0]);
      return 1;
      }
    else
    {
      arg_number_list[check_index-1]=enum_type_file;
    }
  }

  if (!selected_option)
  {
    fprintf(stderr, _ERR_ MISS_OPT HELP_ERR, argv[0]);
    return 1;
  }

  //process
  for (check_index=0; check_index<argc-1; ++check_index)
  {
    if (arg_number_list[check_index]==enum_type_file) // check for files
    {
      if (Work_number!=1)
        fputc('\n', stdout);
      switch (selected_option)
      {
        case COMM_DISPLAY:
          ReadDisplayFile(argv[check_index+1]);
        break;
        case COMM_DISPLAY_STRING:
          ReadDisplayStrings(argv[check_index+1]);
        break;
        case COMM_DUMP_FILE:
          DumpFile(argv[check_index+1]);
        break;
      }
      Work_number++;
      continue;
    }
    if (arg_number_list[check_index]==enum_type_mode) // check for mode command
    {
      if (selected_option==COMM_DISPLAY)              // check for display modes
      {
        program_mode=MODE_NOTHING;
        for (char *check_char=(char *)argv[check_index+1]; *(check_char); ++check_char)
        {
          switch (*check_char)
          {
            case 'c':
              program_mode |= MODE_COLOR;
            break;
            case 'l':
              program_mode |= MODE_CHAR;
            break;
            case 'd':
              program_mode |= MODE_NUMBER;
            break;
            default:
              fprintf(stderr, _ERR_ INVALID_MODE USE_DEFAULT_MODE, argv[check_index+1]);
              program_mode = MODE_NOTHING;
              goto OuterContinue;
            break;
          }
        }
      }
      else if (selected_option==COMM_DUMP_FILE)          // check for dump modes
      {
        program_mode=MODE_NOTHING;
        for (char *check_char=(char *)argv[check_index+1]; *(check_char);++check_char)
        {
          switch (*check_char)
          {
            case 'c':
              program_mode |= MODE_COLOR;
            break;
            case 's':
              program_mode |= MODE_NO_DOT;
            break;
            default:
              fprintf(stderr, _ERR_ INVALID_MODE USE_DEFAULT_MODE, argv[check_index+1]);
              program_mode = MODE_NOTHING;
              goto OuterContinue;
            break;
          }
        }
      }
      OuterContinue:
      continue;
    }
    if (arg_number_list[check_index]==enum_type_no_mode)
    {
      program_mode=MODE_NOTHING;
      continue;
    }
  }

  if (Work_number==1)
  {
    fprintf(stderr, _ERR_ MISS_FILE HELP_ERR, argv[0]);
    return 1;
  }

  return 0;
}
