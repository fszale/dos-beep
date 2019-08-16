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
extern char                szModemDisplayed[60];
extern char                message[80];
extern BOOL                AltKey;
extern int                 sf;
extern BOOL                KeyDown;
extern ITEM                mnuHelp[];
extern WAPISPOOLERINFO     CurrentPage;
extern CONTROL_PARAM       CntlInfo[13];

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ProcessModemEdit(PCONTROL_PARAM pCntlInfo, short cntlNum)
{
   PCONTROL_PARAM CurrControl;
   int     ret;
   int     next = 0;
   int     retVal;
   int     Stop = FALSE;
   short   loop = 1;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);

   CurrControl = pCntlInfo + next;

   _displaycursor( _GCURSOROFF );

   while(loop == 1)
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
           {
               if ( next == cntlNum )
                   next = 0;
               else
                   next++;
               CurrControl = pCntlInfo + next;  
           }
           break;

           case SHIFT_TAB:
           {
               if ( next == 0 )
                   next = cntlNum;
               else
                   next--;
               CurrControl = pCntlInfo + next;  
           }
           break;

           case LIST:
               if(next == 0)
               {
                   if((ret = ListInstalledModems(&CurrentPage.ModemInfo, 0, 15, 15, 50, 4)) >= 0)
                   {
                       strcpy((LPSTR) (CurrControl + 0)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szModemName);   
                       strcpy((LPSTR) (CurrControl + 1)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szInitString);   
                       strcpy((LPSTR) (CurrControl + 2)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szHangupString);   
                       strcpy((LPSTR) (CurrControl + 3)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialPrefixString);   
                       strcpy((LPSTR) (CurrControl + 4)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialSuffixString);   
                       strcpy((LPSTR) (CurrControl + 5)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szResetString);   
                       strcpy((LPSTR) (CurrControl + 6)->pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szEscapeString);   
                       itoa(CurrentPage.ModemInfo.ModemInfo.byReDial_Count, (CurrControl + 7)->pCntlBuffer, 10);
                       if(CurrentPage.ModemInfo.byCOMMType == COMM_NONE)
                          strcpy((CurrControl + 8)->pCntlBuffer, "N");
                       else
                       {
                           if(CurrentPage.ModemInfo.byCOMMType == COMM_LOCAL)
                               itoa(CurrentPage.ModemInfo.byCOMMPort, (CurrControl + 8)->pCntlBuffer, 10);    
                       }

                       strcpy(szModemDisplayed, CurrentPage.ModemInfo.szInstalledName);
                       sprintf(message,"%s selected.", CurrentPage.ModemInfo.szInstalledName);
                       StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   }
                   else
                   if(ret == ALT)
                   {
                       DrawModemBox();
                       DrawControls((CurrControl + 0), 9);
                       ReDrawData((CurrControl + 0), 9, _TBRIGHTWHITE, _TBLUE);
                       return ALT;
                   }
                   DrawModemBox();
                   DrawControls((CurrControl + 0), 9);
                   ReDrawData((CurrControl + 0), 9, _TBRIGHTWHITE, _TBLUE);
               }
               if(next == 8)
               {
                   StatusBar("Select communication port.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   if((ret = ListComOptions(&CurrentPage.ModemInfo, 17, 46, 5, 3)) >= 0)
                   {
                       if(CurrentPage.ModemInfo.byCOMMType == COMM_LOCAL)
                           ConvertParams((PVOID) (pCntlInfo + 8)->pCntlBuffer, (DWORD) CurrentPage.ModemInfo.byCOMMPort, DOS_COMMPORT);
                       else
                           strcpy((pCntlInfo + 8)->pCntlBuffer, "N");
                   }
                   else
                   if(ret == ALT)
                   {
                       DrawModemBox();
                       DrawControls((CurrControl + 0), 9);
                       ReDrawData((CurrControl + 0), 9, _TBRIGHTWHITE, _TBLUE);
                       return ALT;
                   }

                   DrawModemBox();
                   DrawControls((pCntlInfo + 0), 9);
                   ReDrawData((pCntlInfo + 0), 9, _TBRIGHTWHITE, _TBLUE);
               }
           break;
 
           case ESCAPE:
               return ESCAPE;
        }
   }
   return ESCAPE;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawModemBox()
{
   char message[80];

   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, 78, _TWHITE, _TBLUE, BORDER);
   StatusBar(" ", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText(" Modem Edit ", CENTER, 0, 2, _TBLUE, _TWHITE);
   sprintf(message, "Default Modem is: %s", WapiInfo.szDefaultModemName);
   PrintText(message, CENTER, 0, 4, _TWHITE, _TBLUE);
   sprintf(message, "Modem Displayed is: %s", szModemDisplayed);
   PrintText(message, LEFT, 4, 20, _TWHITE, _TBLUE);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ListInstalledModems(LPWAPIMODEMINFO pModemInfo, WORD wFlag, short row, short col, int maxWidth, short maxEntries)
{
   ITEM *pSubList = NULL;
   int index = 0, offset  = NOT_SELECTED;
   int Height;
   int Overflow = 1;
   char buffer[50];

   if(!pModemInfo)
      return ESCAPE;

   if((pSubList = (ITEM *) malloc( MAX_INSTALLED_MODEMS * sizeof(ITEM) )) == NULL)
      return ESCAPE;

   if ( wFlag != 0 )
   {
      pSubList[index].iHilite = -1;
      strcpy((LPSTR) pSubList[index].achItem, (LPCSTR) DEFAULT_MODEM_CONNECTION);
   
      Overflow++;
      index++;
   }

   StatusBar("Loading installed modem database...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if( Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
   {
   
       if( Modem_GetfirstRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) pModemInfo) == CALL_SUCCESS )
       {
           do
           {
               pSubList[index].iHilite = -1;
               sprintf((LPSTR) pSubList[index].achItem, "%-49.49s", pModemInfo->szInstalledName);

               Overflow++;
               index++;
           }
           while ( (Modem_GetnextRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) pModemInfo) == CALL_SUCCESS) && (Overflow < MAX_INSTALLED_MODEMS));

           if( index > maxEntries )
               Height = maxEntries;
           else
               Height = index;

           StatusBar("Select an installed modem.", LEFT, 34, 25, _TBLACK, _TWHITE);

           sprintf(buffer, " Total # of modems : %d ", index);
           PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

           offset = ListBox( row, col, maxWidth, Height, pSubList, index, 0, HILIGHT, NULL);
           StatusBar("", LEFT, 34, 25, _TBLACK, _TWHITE);

           if ( offset < 0 )
           {
               free(pSubList);
               Close_DBFile(&WapiInfo, MODEM_DB);
               return offset;
           }

           strcpy((LPSTR) pModemInfo->szInstalledName, (LPCSTR) pSubList[offset].achItem);

           if ( (wFlag != 0) && (offset == 0) )
               strcpy((LPSTR) pModemInfo->szInstalledName, DEFAULT_MODEM_CONNECTION);
           else
               if ( Modem_GetanyRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) pModemInfo) != CALL_SUCCESS)
               {
                   Close_DBFile(&WapiInfo, MODEM_DB);
                   free(pSubList);
                   return ESCAPE;
               }
       }
       else
       {
           offset = ESCAPE;
           strcpy(message, "ERROR : No installed modems were found.");
       }

       Close_DBFile(&WapiInfo, MODEM_DB);
   }
   else
   {
      offset = ESCAPE;
      strcpy(message, "ERROR : Could not open installed modem database.");
   }

   free(pSubList);
   return (WORD) offset;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ListModems(LPWAPIMODEMLISTINFO pModemInfo, short row, short col, int maxWidth, short maxEntries)
{
   ITEM *pSubList = NULL;
   int index = 0, offset = NOT_SELECTED;
   int Height;
   int Overflow = 1;
   char buffer[50];

   if(!pModemInfo)
      return ESCAPE;

   if((pSubList = (ITEM *) malloc( MAX_MODEMS * sizeof(ITEM) )) == NULL)
      return ESCAPE;

   StatusBar("Loading modems database...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if( Open_DBFile(&WapiInfo, MODEMLIST_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
   {
   
      if( ModemList_GetfirstRec(&WapiInfo, MODEMLIST_TYPE1, 0, (PVOID) pModemInfo) == CALL_SUCCESS )
      {
         do
         {
            pSubList[index].iHilite = -1;
            sprintf((LPSTR) pSubList[index].achItem, "%-39.39s", pModemInfo->szModemName);

            Overflow++;
            index++;
         }
         while ( (ModemList_GetnextRec(&WapiInfo, MODEMLIST_TYPE1, 0, (PVOID) pModemInfo) == CALL_SUCCESS) && (Overflow < MAX_MODEMS));

         if( index > maxEntries )
            Height = maxEntries;
         else
            Height = index;
         
         StatusBar("Select a modem.", LEFT, 40, 25, _TBLACK, _TWHITE);

         sprintf(buffer, " Total # of modems : %d ", index);
         PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

         offset = ListBox( row, col, maxWidth, Height, pSubList, index, 0, HILIGHT, NULL);
         StatusBar("", LEFT, 40, 25, _TBLACK, _TWHITE);

         if( offset < 0 )
         {
            free(pSubList);
            Close_DBFile(&WapiInfo, MODEMLIST_DB);
            return offset;
         }

         strcpy((LPSTR) pModemInfo->szModemName, (LPCSTR) pSubList[offset].achItem);
         
         if(ModemList_GetanyRec(&WapiInfo, MODEMLIST_TYPE1, 0, (PVOID) pModemInfo) != CALL_SUCCESS)
         {
            Close_DBFile(&WapiInfo, MODEMLIST_DB);
            free(pSubList);
            return ESCAPE;
         }
      }
      else
      {
           offset = ESCAPE;
           strcpy(message, "ERROR : No modems were found.");
      }

      Close_DBFile(&WapiInfo, MODEMLIST_DB);
   }
   else
   {
      offset = ESCAPE;
      strcpy(message, "ERROR : Could not open modems database.");
   }

   free(pSubList);
   return (WORD) offset;
}


//------------------------------------------------------------------------
//
//     DosBEEP function:  BOOL InstallModem(LPWAPIMODEMINFO, PCONTROL_PARAM)
//         
//     decsription:
//
//------------------------------------------------------------------------
BOOL InstallModem(LPWAPIMODEMINFO pModemInfo, PCONTROL_PARAM pCntlInfo)
{
   BOOL ret = FALSE;
   BOOL SetDefault = FALSE;

   if( Open_DBFile(&WapiInfo, MODEMLIST_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
   {
       strcpy((LPSTR) pModemInfo->ModemInfo.szModemName,(LPCSTR) (pCntlInfo + 0)->pCntlBuffer);   

       if(ModemList_GetanyRec(&WapiInfo, MODEMLIST_TYPE1, 0, (PVOID) &(pModemInfo->ModemInfo)) == CALL_SUCCESS)
       {
           if( Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
           {
               WAPIMODEMINFO modem2;

               if(Modem_GetfirstRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) &modem2) != CALL_SUCCESS)
                   SetDefault = TRUE;

               if(Modem_InsertRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) pModemInfo) == CALL_SUCCESS)
                   ret = TRUE;

               if((SetDefault == TRUE) && (ret == TRUE))
               {
                   char szBuffer[256];

                   strcpy(WapiInfo.szDefaultModemName, pModemInfo->szInstalledName);

                   if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
                       strcpy(szBuffer, "WAPI.INI");
               
                   WriteProfileString("Options", "DefModem", WapiInfo.szDefaultModemName, szBuffer);
               }

               Close_DBFile(&WapiInfo, MODEM_DB);
           }
       }
       else
       {
           if(strcmp((pCntlInfo + 1)->pCntlBuffer, "") != 0)
               strcpy((LPSTR) pModemInfo->ModemInfo.szInitString,(LPCSTR) (pCntlInfo + 1)->pCntlBuffer);   
           else
               strcpy((LPSTR) pModemInfo->ModemInfo.szInitString,(LPCSTR) "ATQ0V1^M");   

           if(strcmp((pCntlInfo + 2)->pCntlBuffer, "") != 0)
               strcpy((LPSTR) pModemInfo->ModemInfo.szHangupString,(LPCSTR) (pCntlInfo + 2)->pCntlBuffer);   
           else
               strcpy((LPSTR) pModemInfo->ModemInfo.szHangupString,(LPCSTR) "ATH0^");   

           if(strcmp((pCntlInfo + 3)->pCntlBuffer, "") != 0)
               strcpy((LPSTR) pModemInfo->ModemInfo.szDialPrefixString,(LPCSTR) (pCntlInfo + 3)->pCntlBuffer);   
           else
               strcpy((LPSTR) pModemInfo->ModemInfo.szDialPrefixString,(LPCSTR) "ATDT");   

           if(strcmp((pCntlInfo + 4)->pCntlBuffer, "") != 0)
               strcpy((LPSTR) pModemInfo->ModemInfo.szDialSuffixString,(LPCSTR) (pCntlInfo + 4)->pCntlBuffer);   
           else
               strcpy((LPSTR) pModemInfo->ModemInfo.szDialSuffixString,(LPCSTR) "^M");   

           if(strcmp((pCntlInfo + 5)->pCntlBuffer, "") != 0)
               strcpy((LPSTR) pModemInfo->ModemInfo.szResetString,(LPCSTR) (pCntlInfo + 5)->pCntlBuffer);   
           else
               strcpy((LPSTR) pModemInfo->ModemInfo.szResetString,(LPCSTR) "ATZ^M");   

           if(strcmp((pCntlInfo + 6)->pCntlBuffer, "") != 0)
               strcpy((LPSTR) pModemInfo->ModemInfo.szEscapeString,(LPCSTR) (pCntlInfo + 6)->pCntlBuffer);
           else
               strcpy((LPSTR) pModemInfo->ModemInfo.szEscapeString,(LPCSTR) "~~~+++");
           
           if(strcmp((pCntlInfo + 7)->pCntlBuffer, "") != 0)
               pModemInfo->ModemInfo.byReDial_Count = (BYTE) atoi((pCntlInfo + 7)->pCntlBuffer);
           else
               pModemInfo->ModemInfo.byReDial_Count = 10;

           if(strcmp((pCntlInfo + 8)->pCntlBuffer, "") != 0)
           {
               if(strcmp(_strupr((pCntlInfo + 8)->pCntlBuffer), "N") == 0)
               {
                   pModemInfo->byCOMMType = COMM_NONE;
                   pModemInfo->byCOMMPort = 0;
               }
               else
               {
                   pModemInfo->byCOMMType = COMM_LOCAL;
                   BackConvertParams((pCntlInfo + 8), (PVOID) &(pModemInfo->byCOMMPort), DOS_COMMPORT);
               }
           }
           else
           {
               pModemInfo->byCOMMType = COMM_LOCAL;
               pModemInfo->byCOMMPort = 1;
           }
   
           strcpy((LPSTR) pModemInfo->ModemInfo.szOK_Response , (LPCSTR) "OK");
           strcpy((LPSTR) pModemInfo->ModemInfo.szBusy_Response , (LPCSTR) "BUSY");
           strcpy((LPSTR) pModemInfo->ModemInfo.szConnect_Response , (LPCSTR) "CONNECT");
           strcpy((LPSTR) pModemInfo->ModemInfo.szNoCarrier_Response , (LPCSTR) "NO CARRIER");
           strcpy((LPSTR) pModemInfo->ModemInfo.szNoDialtone_Response , (LPCSTR) "NO DIALTONE");
           strcpy((LPSTR) pModemInfo->ModemInfo.szNoAnswer_Response , (LPCSTR) "NO ANSWER");

           pModemInfo->ModemInfo.byReset_Delay = 2;
           pModemInfo->ModemInfo.byHangUp_Delay = 2;
           pModemInfo->ModemInfo.byEscape_Delay = 2;
           pModemInfo->ModemInfo.byInit_Delay = 2;
           pModemInfo->ModemInfo.byDial_Delay = 2;
           pModemInfo->ModemInfo.byConnect_Delay = 2;
           pModemInfo->ModemInfo.byRedial_Delay = 2;
           pModemInfo->ModemInfo.fOptions = 1;

           if( Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
           {
               WAPIMODEMINFO modem2;

               if(Modem_GetfirstRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) &modem2) != CALL_SUCCESS)
                   SetDefault = TRUE;

               Close_DBFile(&WapiInfo, MODEM_DB);
           }

           if( Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
           {
               if(Modem_InsertRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) pModemInfo) == CALL_SUCCESS)
                   ret = TRUE;

               if((SetDefault == TRUE) && (ret == TRUE))
               {
                   char szBuffer[256];

                   strcpy(WapiInfo.szDefaultModemName, pModemInfo->szInstalledName);

                   if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
                       strcpy(szBuffer, "WAPI.INI");
               
                   WriteProfileString("Options", "DefModem", WapiInfo.szDefaultModemName, szBuffer);
               }
           }

           Close_DBFile(&WapiInfo, MODEM_DB);
       }

       Close_DBFile(&WapiInfo, MODEMLIST_DB);
   }
   return ret;
}

