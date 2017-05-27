
#define _VERSION "0.120.1"
#define _CREATOR "Mohammad Amin Khakzadan"
#define _CREATOR_GMAIL "mak12776@gmail.com"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if defined _WIN32 || defined _WIN64
#include <windows.h>
#endif

#include "debug.h"
#include "inc/boolean.h"
#include "inc/units.h"
#include "inc/misc.h"
#include "inc/error_numbers.h"
#include "inc/terminal_color.h"

/*
* Debug tools:
* LOGV(t, x) show value x of type t
* LOG(...)   printf(...)
* LOGN(x)    printf(x "\n")
*/

#define USAGE "Usage: %s [Option] [mode] file [file1] [file2] [file3] ...\n"
#define HELP_DOC "\
Options:\n\
  -help                                     show this help and exit\n\
  -n [number], -column-number [number]      specify number of output columns\n\
  -m [mode], -mode [mode]                   specify modes for output results\n\
                                            modes can be a string of letters\n\
  -nm, -no-mode                             restore to default mode\n\
  -d, -display                              read files and write them on stdout\n\
    Modes:                                  default mode: nothing\n\
      c                                     colorful.\n\
      l                                     show letters in another type.\n\
      d                                     show bytes in decimal.\n\
  -ds, -display-strings                     read files and write only strings on stdout\n\
  -f [pattern], -find [pattern]             find hex pattern in files (under construction)\n\
  -v, -version                              show version & creator\n\
"
#define HELP_ERR "try %s -help for more information\n"
#define _ERR_ "error: "
#define INVALID_COM "invalid command: %s\n"
#define INVALID_MODE "invalid mode: %s\n"
#define INVALID_NUMBER "invalid number: %s\n"
#define DUPLICATE_OPT "duplicate option\n"
#define MEMORY_ERR "can't allocate memory\n"
#define MISS_OPR "missing file operand\n"
#define MISS_OPT "missing option\n"
#define USE_DEFAULT_MODE "restoring default mode.\n"

#define DEFAULT_NUMBER_OF_COLOMNS 15

#define enum_type_file 1
#define enum_type_mode 2
#define enum_type_no_mode 3

#define COMM_DISPLAY 1
#define COMM_DISPLAY_STRING 2
#define COMM_FIND 3

#define MODE_NOTHING 0
#define MODE_COLOR  0b001
#define MODE_CHAR   0b010
#define MODE_NUMBER 0b100

typedef char program_mode_t;
typedef int arg_number_list_t;

// function declaration
void ReadDisplayFile(const char *filename);
void ReadDisplayStrings(const char *filename);

// variables
int Work_number=1;
char selected_option=0;
program_mode_t program_mode=MODE_NOTHING;
arg_number_list_t *arg_number_list=NULL;

uint NoColumn=DEFAULT_NUMBER_OF_COLOMNS;
#if defined _WIN32 || defined _WIN64
HANDLE hConsole;
#endif

int main(int argc, char const *argv[])
{
  register int check_index=1;

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
    printf(_ERR_ MEMORY_ERR);
    return 255;
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
          printf(_ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return EBADMSG;
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
          printf(_ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return EBADMSG;
        }
        else
        {
          selected_option=COMM_DISPLAY_STRING;
          continue;
        }
      }
      if (Compare(argv[check_index]+1, "f") || Compare(argv[check_index]+1, "find")) //find
      {
        if (selected_option)
        {
          printf(_ERR_ DUPLICATE_OPT HELP_ERR, argv[0]);
          return EBADMSG;
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
          printf(_ERR_ INVALID_NUMBER, argv[check_index]);
          return EBADMSG;
        }
        continue;
      }
      if (Compare(argv[check_index]+1, "v") || Compare(argv[check_index]+1, "version")) //version
      {
        printf("Bytion v" _VERSION "\nby " _CREATOR ", " _CREATOR_GMAIL "\n");
        return 0;
      }
      printf(_ERR_ INVALID_COM HELP_ERR, argv[check_index], argv[0]);
      return EBADMSG;
      }
    else
    {
      arg_number_list[check_index-1]=enum_type_file;
    }
  }

  if (!selected_option)
  {
    printf(_ERR_ MISS_OPT HELP_ERR, argv[0]);
    return EBADMSG;
  }

  //process
  for (check_index=0; check_index<argc-1; ++check_index)
  {
    if (arg_number_list[check_index]==enum_type_file)
    {
      switch (selected_option) {
        case COMM_DISPLAY:
          ReadDisplayFile(argv[check_index+1]);
        break;
        case COMM_DISPLAY_STRING:
          ReadDisplayStrings(argv[check_index+1]);
        break;
      }
      Work_number++;
      continue;
    }
    if (arg_number_list[check_index]==enum_type_mode)
    {
      program_mode=MODE_NOTHING;
      for (register char *check_char=(char *)argv[check_index+1]; *(check_char); check_char++)
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
            printf(_ERR_ INVALID_MODE USE_DEFAULT_MODE, argv[check_index+1]);
            program_mode = MODE_NOTHING;
            goto Continue;
          break;
        }
      }
      Continue:
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
    printf(_ERR_ MISS_OPR HELP_ERR, argv[0]);
    return EBADMSG;
  }
  return 0;
}

void ReadDisplayStrings(const char *filename)
{
  FILE *pFile;
  long size;
  uchar *buffer;

  pFile=fopen(filename, "rb");

  if (!pFile)
  {
    printf(_ERR_ "in opening %s: %s\b", filename, strerror(errno));
    return;
  }

  fseek(pFile, 0L, SEEK_END);
  size=ftell(pFile);
  rewind(pFile);
  buffer=(uchar *)malloc(size);
  if (!buffer)
  {
    fclose(pFile);
    printf(_ERR_ "in opening %s: " MEMORY_ERR, filename);
    return;
  }
  printf("[%d]: %s, %ld\n", Work_number, filename, size);

  fread(buffer, 1, size, pFile);
  fclose(pFile);

  for (register uchar *n=buffer, *str=0, *end=buffer+size; n!=end; ++n)
  {
    if (( (*n>31) && (*n<127) ) || *n==10)
    {
      if (str==0)
        str=n;
      continue;
    }
    if (*n==0)
    {
      if (str!=0 && (n-str>1))
      {
        printf("%s\n", str);
        str=0;
      }
      else
      {
        str=0;
      }
    }
    else
      str=0;
  }
  printf("^ %s, %ld ^\n", filename, size);
  free(buffer);
}

void ReadDisplayFile(const char *filename)
{
  FILE *pFile;
  long size;
  int c;
  uchar out[5]={' ', ' ', ' ', 0, 0};
  uint NoC=NoColumn;

  pFile=fopen(filename, "rb");
  if (!pFile)
  {
    printf(_ERR_ "in opening %s: %s\b", filename, strerror(errno));
    return;
  }

  fseek(pFile, 0L, SEEK_END);
  size=ftell(pFile);
  rewind(pFile);
  printf("[%d]: %s, %ld\n", Work_number, filename, size);

  switch (program_mode) {
    case 0b000:
      while ((c=getc(pFile)) != EOF) // nothing
      {
        ToHex(c, out);
        fputs(out, stdout);
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          fputc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b010:
      while ((c=getc(pFile)) != EOF) // char
      {
        if (c>32 && c<127)
        {
          ToChar(c, out);
          fputs(out, stdout);
        }
        else
        {
          ToHex(c, out);
          fputs(out, stdout);
        }
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          fputc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b011:
      while ((c=getc(pFile)) != EOF) // color, char
      {
        if (c>32 && c<127)
        {
          ToChar(c, out);
          printColored(out, COLOR_CHAR);
        }
        else
        {
          ToHex(c, out);
          if (c==0)
            printColored(out, COLOR_ZERO);
          else
            printColored(out, COLOR_DEF_BYTE);
        }
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          putc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b001:
      while ((c=getc(pFile)) != EOF) // color
      {
        ToHex(c, out);
        if (c==0)
          printColored(out, COLOR_ZERO);
        else
          printColored(out, COLOR_DEF_BYTE);
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          putc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b111:
      *(out+3)=' ';
      while ((c=getc(pFile)) != EOF) // color, char, Real
      {
        if (c>32 && c<127)
        {
          *(out+2)=' ';
          ToChar(c, out);
          printColored(out, COLOR_CHAR);
        }
        else
        {
          ToReal(c, out);
          if (c==0)
            printColored(out, COLOR_ZERO);
          else
            printColored(out, COLOR_DEF_BYTE);
        }
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          putc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b110:
      *(out+3)=' ';
      while ((c=getc(pFile)) != EOF) // char, Real
      {
        if (c>32 && c<127)
        {
          *(out+2)=' ';
          ToChar(c, out);
          fputs(out, stdout);
        }
        else
        {
          ToReal(c, out);
          fputs(out, stdout);
        }
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          putc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b101:
      *(out+3)=' ';
      while ((c=getc(pFile)) != EOF) // color, Real
      {
        ToReal(c, out);
        if (c==0)
          printColored(out, COLOR_ZERO);
        else
          printColored(out, COLOR_DEF_BYTE);
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          putc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
    case 0b100:
      *(out+3)=' ';
      while ((c=getc(pFile)) != EOF) // Real
      {
        ToReal(c, out);
        fputs(out, stdout);
        if (NoC>0)
        {
          NoC--;
        }
        else
        {
          putc('\n', stdout);
          NoC=NoColumn;
        }
      }
    break;
  }

  SetColor(COLOR_ENV);
  if (NoC==NoColumn)
    printf("^ %s, %ld ^\n", filename, size);
  else
    printf("\n^ %s, %ld ^\n", filename, size);
  fclose(pFile);
}
