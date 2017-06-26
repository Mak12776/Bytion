#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err_messages.h"
#include "term_color_mac.h"
#include "inline/term_color.h"
#include "inline/misc.h"

extern int Work_number;
extern uint NoColumn;
extern program_mode_t program_mode;

void ReadDisplayStrings(const char *filename)
{
  FILE *pFile;
  long size;
  uchar *buffer;

  pFile=fopen(filename, "rb");

  if (!pFile)
  {
    printf(_ERR_ "in opening %s: %s\n", filename, strerror(errno));
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
    printf(_ERR_ "in opening %s: %s\n", filename, strerror(errno));
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
