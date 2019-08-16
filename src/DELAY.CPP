#include <stdio.h>
#include <time.h>
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"


void Delay(WORD seconds);
void Delay(WORD seconds)
{
   char szTime[10];
   int  nStartTime;
   int  nCurrentTime;
   WORD  counter = 0;

   _strtime(szTime);
   nStartTime = atoi( (LPCSTR) &szTime[6] );

   do
   {
      _strtime(szTime);
      nCurrentTime = atoi( (LPCSTR) &szTime[6] );

      if(nCurrentTime > nStartTime)
      {
         counter += (WORD) (nCurrentTime - nStartTime);
         nStartTime = nCurrentTime;
      }
   }
   while(counter <= seconds);
}
