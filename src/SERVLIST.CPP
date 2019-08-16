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

extern struct videoconfig  vc;
extern WAPIINFO            WapiInfo;
extern char                message[80];
extern int                 sf;
extern BOOL                KeyDown;
extern BOOL                AltKey;
extern ITEM                mnuHelp[];
extern WAPISPOOLERINFO     CurrentPage;
extern CONTROL_PARAM       CntlInfo[13];

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
WORD ListBaudRate(LPSTR szBuffer)
{
   ITEM SubList[7];
   int index = 0, offset = NOT_SELECTED;
   int Overflow = 1;
   char *Bauds[7] = {"110","300","600","1200","2400","4800","9600"};

   for(index = 0; index < 7; index++)
   {
      SubList[index].iHilite = -1;
      strcpy((LPSTR) SubList[index].achItem, Bauds[index]);
   }

   offset = ListBox( 11, 45, 6, 5, SubList, index, 0, HILIGHT, NULL);
   if(offset < 0)
       return ESCAPE;
   else
       strcpy(szBuffer, Bauds[offset]);

   return (WORD) offset;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
WORD ListParity(LPSTR szBuffer) 
{
   ITEM SubList[5];
   int index = 0, offset = NOT_SELECTED;
   int Overflow = 1;
   char *Bauds[5] = {"EVEN","MARK","NONE","ODD","SPACE"};

   for(index = 0; index < 5; index++)
   {
      SubList[index].iHilite = -1;
      strcpy((LPSTR) SubList[index].achItem, Bauds[index]);
   }

   offset = ListBox( 12, 45, 6, 5, SubList, index, 0, HILIGHT, NULL);
   if(offset < 0)
       return ESCAPE;
   else
       sprintf(szBuffer, "%c", Bauds[offset][0]);

   return (WORD) offset;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
WORD ListDataBits(LPSTR szBuffer) 
{
   ITEM SubList[4];
   int index = 0, offset = NOT_SELECTED;
   int Overflow = 1;
   char *Bauds[4] = {"5","6","7","8"};

   for(index = 0; index < 4; index++)
   {
      SubList[index].iHilite = -1;
      strcpy((LPSTR) SubList[index].achItem, Bauds[index]);
   }

   offset = ListBox( 13, 45, 6, 4, SubList, index, 0, HILIGHT, NULL);
   if(offset < 0)
       return ESCAPE;
   else
       strcpy(szBuffer, Bauds[offset]);

   return (WORD) offset;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
WORD ListStopBits(LPSTR szBuffer) 
{
   ITEM SubList[2];
   int index = 0, offset = NOT_SELECTED;
   int Overflow = 1;
   char *Bauds[2] = {"1","2"};

   for(index = 0; index < 2; index++)
   {
      SubList[index].iHilite = -1;
      strcpy((LPSTR) SubList[index].achItem, Bauds[index]);
   }

   offset = ListBox( 14, 45, 6, 2, SubList, index, 0, HILIGHT, NULL);
   if(offset < 0)
       return ESCAPE;
   else
       strcpy(szBuffer, Bauds[offset]);

   return (WORD) offset;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawServiceBox()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText("  Service Edit  ", CENTER, 0, 2, _TWHITE, _TBLUE);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ListServices(LPWAPISERVICEINFO pSerInfo, short row, short col, int maxWidth, short maxEntries)
{
   ITEM *pSubList = NULL;
   int index = 0, offset = NOT_SELECTED;
   int Height;
   int Overflow = 1;
   char buffer[50];

   if(!pSerInfo)
      return ESCAPE;

   if((pSubList = (ITEM *) malloc( MAX_SERVICES * sizeof(ITEM) )) == NULL)
      return ESCAPE;

   StatusBar("Loading Service Database...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if( Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
   {
      if( Service_GetfirstRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) pSerInfo) == CALL_SUCCESS )
      {
         do
         {
            pSubList[index].iHilite = -1;
            strcpy((LPSTR) pSubList[index].achItem, (LPCSTR) pSerInfo->szServiceName);

            Overflow++;
            index++;
         }
         while ( (Service_GetnextRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) pSerInfo) == CALL_SUCCESS) && (Overflow < MAX_SERVICES));

         if( index > maxEntries )
            Height = maxEntries;
         else
            Height = index;

         StatusBar("Select Service.", LEFT, 40, 25, _TBLACK, _TWHITE);

         sprintf(buffer, " Total # of services : %d ", index);
         PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

         offset = ListBox( row, col , maxWidth, Height, pSubList, index, 0, HILIGHT, NULL);
         StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);

         if ( offset < 0 )
         {
            free(pSubList);
            Close_DBFile(&WapiInfo, SERVICE_DB);
            return offset;
         }

         strcpy((LPSTR) pSerInfo->szServiceName, (LPCSTR) pSubList[offset].achItem);
         
         if(Service_GetanyRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) pSerInfo) != CALL_SUCCESS)
         {
            Close_DBFile(&WapiInfo, SERVICE_DB);
            free(pSubList);
            return ESCAPE;
         }
      }
      else
      {
          offset = ESCAPE;
          strcpy(message, "ERROR : No services were found.");
      }
      
      Close_DBFile(&WapiInfo, SERVICE_DB);
   }
   else
   {
      offset = ESCAPE;
      strcpy(message, "ERROR : Could not open service database.");
   }


      
   free(pSubList);
   return (WORD) offset;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ProcessServiceEdit(PCONTROL_PARAM pCntlInfo)
{
   int     nret;
   LPSTR ret = "";
   PCONTROL_PARAM CurrControl;
   int next = 0;
   int retVal = 0;
   int Stop = FALSE;
   short loop = 1;
   short smloop = 1;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);

   _displaycursor( _GCURSOROFF );

   CurrControl = pCntlInfo;
   
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
                       next = 8;
                       CurrControl = pCntlInfo + next;  
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                   case LIST:
                       StatusBar("Select a Service.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((nret = ListServices(&CurrentPage.ServiceInfo, 4, 45, 31, 12)) >= 0)
                       {
                           char szBuffer[10];

                           strcpy((LPSTR) (CurrControl + 0)->pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szServiceName);
                           strcpy((LPSTR) (CurrControl + 1)->pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szPhoneNumber);
                           strcpy((LPSTR) (CurrControl + 2)->pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.uProtocol.Pet.szPassword);
                           ConvertParams(&((CurrControl + 4)->pCntlBuffer), CurrentPage.ServiceInfo.dwBaud, DOS_BAUD);
                           ConvertParams(&((CurrControl + 5)->pCntlBuffer), CurrentPage.ServiceInfo.byParity, DOS_PARITY);
                           ConvertParams(&((CurrControl + 6)->pCntlBuffer), CurrentPage.ServiceInfo.byDataBits, DOS_DATABITS);
                           ConvertParams(&((CurrControl + 7)->pCntlBuffer), CurrentPage.ServiceInfo.byStopBits, DOS_STOPBITS);
                           strncpy((LPSTR) &(CurrControl + 8)->pCntlBuffer, CurrentPage.ServiceInfo.szInstalledModem, 64);
                           itoa(CurrentPage.ServiceInfo.uProtocol.Pet.wMaxMsgLength, szBuffer, 10);
                           strcpy((CurrControl + 3)->pCntlBuffer, szBuffer);
                           sprintf(message,"Service %s has been selected.", CurrentPage.ServiceInfo.szServiceName);
                           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                       }
                       else
                       if(nret == ALT)
                       {
                           DrawServiceBox();
                           DrawControls((CurrControl + 0), 9);
                           ReDrawData((CurrControl + 0), 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawServiceBox();
                       DrawControls((CurrControl + 0), 9);
                       ReDrawData((CurrControl + 0), 9, _TBRIGHTWHITE, _TBLUE);
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
                   case LIST:
                       StatusBar("Select Baud Rate.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if(ListBaudRate(CurrControl->pCntlBuffer) == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 9);
                           ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 9);
                       ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 5 )
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
                       StatusBar("Select Parity.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if(ListParity(CurrControl->pCntlBuffer) == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 9);
                           ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 9);
                       ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
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
                   case LIST:
                       StatusBar("Select Data Bits.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if(ListDataBits(CurrControl->pCntlBuffer) == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 9);
                           ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 9);
                       ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 7 )
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
                   case LIST:
                       StatusBar("Select StopBits.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if(ListStopBits(CurrControl->pCntlBuffer) == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 9);
                           ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 9);
                       ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 8 )
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
                       next = 0;
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
                   case LIST:
                       StatusBar("Select an Installed Modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if((nret = ListInstalledModems(&CurrentPage.ModemInfo, 1, 18, 15, 50, 4)) >= 0)
                           strcpy((LPSTR) CurrControl->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.szInstalledName);
                       else
                       if(nret == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 9);
                           ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 9);
                       ReDrawData(pCntlInfo, 9, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }
   }
   return ESCAPE;
}

