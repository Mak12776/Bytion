#include "../debug.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "err_messages.h"
#include "units.h"
#include "inline/misc.h"


extern int Work_number;
extern program_mode_t program_mode;

void DumpFile(const char *filename)
{
  FILE *pFile;
  long size;
  long walk=0;
  int c;

  uchar raw[]="OxxxOxxx   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   |........ ........|\n";
  uchar *fbyte=raw+11;
  uchar *sbyte=raw+12;
  uchar *fchar=raw+63;
  uchar *schar=raw+64;
  register int count=0;

  pFile=fopen(filename, "rb");

  if (!pFile)
  {
    fprintf(stderr, _ERR_ "in opening %s: %s\n", filename, strerror(errno));
    return;
  }

  fseek(pFile, 0L, SEEK_END);
  size=ftell(pFile);
  rewind(pFile);
  printf("[%d]: %s, %ld\n", Work_number, filename, size);

  switch (program_mode)
  {
    case 0b000:
      while((c=getc(pFile)) != EOF)
      {
        if (count<8)
        {
          fchar[count]=((c>32) && (c<127))?c:'.';
          ToHex_2Char(c, fbyte+count*3);
          ++count;
        }
        else if (count<15)
        {
          schar[count]=((c>32) && (c<127))?c:'.';
          ToHex_2Char(c, sbyte+count*3);
          ++count;
        }
        else
        {
          raw[79]=((c>32) && (c<127))?c:'.';
          ToHex_2Char(c, raw+57);
          sprintf(raw, "%08lx", walk);
          raw[8]=' ';
          fputs(raw, stdout);
          count=0;
          walk+=16;
        }
      }
      if (count)
      {
        while (count<16)
        {
          if (count<8)
          {
            fchar[count]=' ';
            fbyte[count*3]=' ';
            fbyte[count*3+1]=' ';
            count++;
          }
          else
          {
            schar[count]=' ';
            sbyte[count*3]=' ';
            sbyte[count*3+1]=' ';
            count++;
          }
        }
        printf(raw, walk);
      }
    break;
  }

  printf("^ %s, %ld ^\n", filename, size);
  fclose(pFile);
}
