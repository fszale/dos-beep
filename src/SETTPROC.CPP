#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
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

void GetData(PCONTROL_PARAM pCntlInfo);
void SetData(PCONTROL_PARAM pCntlInfo);

extern BOOL            AltKey;

extern struct videoconfig vc;     
extern WAPIINFO        WapiInfo;
extern DosBEEPINFO     BeepInfo;
extern char            message[80];
extern int          sf;
extern BOOL         KeyDown;
extern ITEM mnuHelp[];
extern WAPISPOOLERINFO     CurrentPage;
extern CONTROL_PARAM       CntlInfo[13];

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ProcessSettingsEdit(PCONTROL_PARAM pCntlInfo)
{
   PCONTROL_PARAM  CurrControl;
   int             next = 0;
   int             retVal;
   int             Stop = FALSE;
   short           loop = 1;
   short           smloop = 1;
   int             ret;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
   
   CurrControl = pCntlInfo + next;  
   
   while(loop == 1)
   {
       if ( next == 0 )
       {
           while(smloop == 1)
           {
               StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);

               if(CurrMenu >= 0)
                   return CurrMenu;
           
               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 1;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 12;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }
       if ( next == 1 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 2;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 0;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 2 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 3;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 1;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 3 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 4;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 2;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 4 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 5;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 3;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if(next == 5)
       {
           while(smloop == 1)
           {
               StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 6;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 4;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                   case LIST:
                       StatusBar("Select an Installed Modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if((ret = ListInstalledModems(&CurrentPage.ModemInfo, 0, 18, 15, 50, 4)) >= 0)
                           strcpy((LPSTR) CurrControl->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.szInstalledName);
                       else
                       if(ret == ALT)
                       {
                           DrawSettingsScreen();
                           DrawControls(pCntlInfo, 13);
                           ReDrawData(&CntlInfo[0], 7, _TBRIGHTWHITE, _TBLUE);
                           ReDrawCheckData(&CntlInfo[7], 4, _TBRIGHTWHITE, _TBLUE);
                           ReDrawData(&CntlInfo[11], 1, _TBRIGHTWHITE, _TBLUE);
                           ReDrawCheckData(&CntlInfo[12], 1, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSettingsScreen();
                       DrawControls(pCntlInfo, 13);
                       ReDrawData(&CntlInfo[0], 7, _TBRIGHTWHITE, _TBLUE);
                       ReDrawCheckData(&CntlInfo[7], 4, _TBRIGHTWHITE, _TBLUE);
                       ReDrawData(&CntlInfo[11], 1, _TBRIGHTWHITE, _TBLUE);
                       ReDrawCheckData(&CntlInfo[12], 1, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 6 )
       {
           while(smloop == 1)
           {
               StatusBar("Hit Space Bar to toggle the check box.", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 7;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 5;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 7 )
       {
           while(smloop == 1)
           {
               //check box
               StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = CheckControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 8;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 6;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 8 )
       {
           while(smloop == 1)
           {
               //check box
               StatusBar("Hit Space Bar to toggle the check box.", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = CheckControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 9;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 7;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }


       if ( next == 9 )
       {
           while(smloop == 1)
           {
               //check box
               retVal = CheckControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 10;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 8;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 10 )
       {
           while(smloop == 1)
           {
               //check box
               StatusBar("Hit Space Bar to toggle the check box.", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = CheckControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 11;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 9;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }
       if ( next == 11 )
       {
           while(smloop == 1)
           {
               StatusBar("Hit Space Bar to toggle the check box.", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 12;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 10;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 12 )
       {
           while(smloop == 1)
           {
               //check box
               StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
               retVal = CheckControl(CurrControl, _TBRIGHTWHITE, _TBLUE, PROCESS);
               
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 0;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 11;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }

   }
   return ESCAPE;
}
