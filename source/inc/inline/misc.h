
#include <stdio.h>
#include <errno.h>
#include "../units.h"
#define MISC_ERR_BADINPUT 1

/*
  this functions must be inline for performance,
  so include them in any object file alone.(also
  use -O3 on gcc)
*/

inline uint ConvertToReal(const char *str)
{
  #ifdef _DEBUG
  if (!str)
  {
    fprintf(stderr, "in %s: NULL pointer !\n", __func__);
    return 0;
  }
  #endif
  uint result=0;
  if ((*str)>48 && (*str < 58))
  {
    result+=(*(str++))-48;
  }
  else
  {
    errno=MISC_ERR_BADINPUT;
    return 0;
  }
  while (*str)
  {
    if ((*str)>47 && (*str < 58))
    {
      result=result*10+(*str++)-48;
      continue;
    }
    errno=MISC_ERR_BADINPUT;
    return 0;
  }
  return result;
}

inline bool Compare(const char *str1,const char *str2)
{
  #ifdef _DEBUG
  if ((!str1) || (!str2))
  {
    fprintf(stderr, "in %s: NULL pointer !\n", __func__);
    return false;
  }
  #endif
  while ( *str1 == *str2 )
  {
    if (!( *(str1++) | *(str2++) ) )
      return true;
  }
  return false;
}

inline inline void ToReal_3Char(uchar in, uchar *out)
{
  if (in>0)
  {
    out[2]=(in%10)+48;
    in/=10;
    if (in>0)
    {
      out[1]=(in%10)+48;
      in/=10;
      if (in>0)
      {
        out[0]=(in%10)+48;
        return;
      }
    out[0]=' ';
    return;
    }
    out[1]=' ';
    out[0]=' ';
    return;
  }
  out[2]='0';
  out[1]=' ';
  out[0]=' ';
}

inline void ToChar_R3Char(uchar in, uchar *out)
{
  out[0]=' ';
  out[1]=' ';
  out[2]=in;
}

inline void ToChar_L2Char(uchar in, uchar *out)
{
  out[0]=in;
  out[1]=' ';
}

inline void ToHex_2Char(uchar in, uchar *out)
{
  out[0] = ((in/16) < 10) ? (in/16) + 48 : (in/16) + 55;
  out[1] = ((in%16) < 10) ? (in%16) + 48 : (in%16) + 55;
}
