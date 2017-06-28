#include "../debug.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "err_messages.h"
#include "units.h"
#include "inline/misc.h"
#include "term_color_mac.h"
#include "inline/term_color.h"


extern int Work_number;
extern program_mode_t program_mode;

void DumpFile(const char *filename)
{
  FILE *pFile;
  long size;
  long walk=0;
  int c;

  register int count=0;

  uchar raw[]="OxxxOxxx   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   |........ ........|\n"; // for no mode
  uchar *fbyte=raw+11;
  uchar *sbyte=raw+12;
  uchar *fchar=raw+63;
  uchar *schar=raw+64;

  uchar tempmem[15];               // for colored mode
  register uchar tempchar;
  uchar hexstr[]="0xxx0xxx   ";
  uchar bytestr[]="00 ";

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
    case 0b000:            // nothing
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
            ++count;
          }
          else
          {
            schar[count]=' ';
            sbyte[count*3]=' ';
            sbyte[count*3+1]=' ';
            ++count;
          }
        }
        sprintf(raw, "%08lx", walk);
        raw[8]=' ';
        fputs(raw, stdout);
      }
    break;
    case 0b001:              // colored
      while ((c=getc(pFile)) != EOF)
      {
        if (count<15)
        {
          tempmem[count]=c;
          ++count;
        }
        else
        {
          sprintf(hexstr, "%08lx", walk);             // hex address
          hexstr[8]=' ';
          printColored(hexstr, COLOR_ENV);
          walk+=16;

          tempchar=tempmem[0];                           // byte 0
          ToHex_2Char(tempchar, bytestr);
          if ((tempchar>32) && (tempchar<127))
            printColored(bytestr, COLOR_CHAR);
          else
            fputs(bytestr, stdout);

          for (int i=1; i<8; ++i)
          {
            tempchar=tempmem[i];                           // byte 1 to 7
            ToHex_2Char(tempchar, bytestr);
            if ((tempchar>32) && (tempchar<127))
              printColored(bytestr, COLOR_CHAR);
            else
              printColored(bytestr, COLOR_ENV);
          }

          fputs("  ", stdout);                    // two space between bytes

          for (int i=8; i<15; ++i)
          {
            tempchar=tempmem[i];                           // byte 8 to 14
            ToHex_2Char(tempchar, bytestr);
            if ((tempchar>32) && (tempchar<127))
              printColored(bytestr, COLOR_CHAR);
            else
              printColored(bytestr, COLOR_ENV);
          }

          ToHex_2Char(c, bytestr);                       // byte 15: c
          if ((c>32) && (c<127))
            printColored(bytestr, COLOR_CHAR);
          else
            printColored(bytestr, COLOR_ENV);

          printColored("  |", COLOR_ENV);            // between bytes and chars

          tempchar=tempmem[0];                            // char 0
          if ((tempchar>32) && (tempchar<127))
            putColored(tempchar, COLOR_CHAR);
          else
            fputc('.', stdout);

          for (int i=1; i<8; ++i)
          {
            tempchar=tempmem[i];                            // char 1 to 7
            if ((tempchar>32) && (tempchar<127))
              putColored(tempchar, COLOR_CHAR);
            else
              putColored('.', COLOR_ENV);
          }

          fputc(' ', stdout);                       // one space between chars

          for (int i=8; i<15; ++i)
          {
            tempchar=tempmem[i];                            // char 8 to 14
            if ((tempchar>32) && (tempchar<127))
              putColored(tempchar, COLOR_CHAR);
            else
              putColored('.', COLOR_ENV);
          }

          if ((c>32) && (c<127))                           // char 15: c
            putColored(c, COLOR_CHAR);
          else
            putColored('.', COLOR_ENV);

          printColored("|\n", COLOR_ENV);                  // end of line
          count=0;                             /* reseting count */
        }
      }
      if (count)
      {
        sprintf(hexstr, "%08lx", walk);             // hex address
        hexstr[8]=' ';
        printColored(hexstr, COLOR_ENV);

        for (int cnt=0; cnt<16; ++cnt)             // for bytes
        {
          if (cnt==8)
          {
            fputs("  ", stdout);
          }
          if (cnt<count)
          {
            tempchar=tempmem[cnt];
            ToHex_2Char(tempchar, bytestr);
            if ((tempchar>32) && (tempchar<127))
              printColored(bytestr, COLOR_CHAR);
            else
              printColored(bytestr, COLOR_ENV);
          }
          else
          {
            bytestr[0]=' ';
            bytestr[1]=' ';
            fputs(bytestr, stdout);
          }
        }

        printColored("  |", COLOR_ENV);

        for (int cnt=0; cnt<16; ++cnt)           // for chars
        {
          if (cnt==8)
          {
            fputc(' ', stdout);
          }
          if (cnt<count)
          {
            tempchar=tempmem[cnt];
            if ((tempchar>32) && (tempchar<127))
              putColored(tempchar, COLOR_CHAR);
            else
              putColored('.', COLOR_ENV);
          }
          else
          {
            fputc(' ', stdout);
          }
        }
        printColored("|\n",COLOR_ENV);
      }
    break;
  }

  printf("^ %s, %ld ^\n", filename, size);
  fclose(pFile);
}
