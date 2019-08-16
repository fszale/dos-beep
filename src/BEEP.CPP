#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <graph.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "menu.h"
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"
#include "dbfile.h"
#include "dosbeep.h"
#include "comm.h"
#include "state.h"
#include "timerf.h"
#include "com.h"
#include "defs.h"

 
CONTROL_PARAM      CntlInfo[13];
WAPIINFO           WapiInfo;

BOOL               AltKey = FALSE;
long               TimerT = 0;
long               TimerD = 0;
long               TimerC = 0;
long               TimerL = 0;
long               TimerS = 0;
long               TimerH = 0;

WORD               TimerTalk = TIMER_OFF;
WORD               TimerDial = TIMER_OFF;
WORD               TimerConnect = TIMER_OFF;
WORD               TimerLogon = TIMER_OFF;
WORD               TimerSend = TIMER_OFF;
WORD               TimerHangUp = TIMER_OFF;


void Sleep(clock_t _wait);

void Beep(unsigned duration, unsigned frequency)
{
   int control;

   if(frequency)
   {
       if(duration < 75)
           duration = 75;
       _outp( 0x43, 0xb6 );
       frequency = (unsigned) (1193180L / frequency);
       _outp( 0x42, (char) frequency );
       _outp( 0x42, (char) (frequency >> 8) );
       control = _inp( 0x61 );
       _outp( 0x61, control | 0x3 );
   }

   Sleep((clock_t) duration);

   if(frequency)
       _outp( 0x61, control);
}

void Sleep(clock_t _wait)
{
   clock_t goal;
   goal = _wait + clock();
   while(goal > clock());
}
