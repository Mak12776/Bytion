
#include <stdio.h>
#include "error_numbers.h"

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
    errno=EILSEQ;
    return 0;
  }
  while (*str)
  {
    if ((*str)>47 && (*str < 58))
    {
      result=result*10+(*str++)-48;
      continue;
    }
    errno=EILSEQ;
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

inline inline void ToReal(uchar in, uchar *out)
{
  if (in>0)
  {
    *(out+2)=(in%10)+48;
    in/=10;
    if (in>0)
    {
      *(out+1)=(in%10)+48;
      in/=10;
      if (in>0)
      {
        *(out)=(in%10)+48;
        return;
      }
    *(out)=' ';
    return;
    }
  *(out+1)=' ';
  *(out)=' ';
  return;
  }
  *(out+2)='0';
  *(out+1)=' ';
  *(out)=' ';
}

inline void ToChar(uchar in, uchar *out)
{
  *(out)=in;
  *(out+1)=' ';
}

inline void ToHex(uchar in, uchar *out)
{
  if ((in/16)<10)
  {
    *out=in/16+48;
  }
  else
  {
    *out=in/16+55;
  }
  if ((in%16)<10)
  {
    *(out+1)=in%16+48;
  }
  else
  {
    *(out+1)=in%16+55;
  }
}
