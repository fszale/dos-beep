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

extern struct videoconfig  vc;     
extern WAPIINFO            WapiInfo;
extern DosBEEPINFO         BeepInfo;
extern char                message[80];
extern int                 sf;
extern BOOL                KeyDown;
extern ITEM                mnuHelp[3];
extern WAPISPOOLERINFO     CurrentPage;
extern CONTROL_PARAM       CntlInfo[13];
extern BOOL                Keyboard;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void Settings()
{
   int     ret;
   BOOL    smloop = TRUE;
   int     Process = 2;
   BOOL    ProcessOn = FALSE;
   BOOL    DrawOn = FALSE;
   long    SubMenu;
   short   loop = TRUE;
   
   ITEM mnuMain[] =
   {
       { 7, "Select Installed Modem"   },   
       { 0, "Save"                     },   
       { 1, "Clear"                    },   
       {-1, "----------------------"   },   
       { 0, "Return           <Esc>"   },   
       { 0, ""                         } 
   };
   
   ITEMBAR mnuScreen[] =
   {                  
       { 70, 0,"File" },
       {  5, 0,"Help" }
   };

   enum MENUS
   {
       SUB_FILE, HELP
   };
   enum MENUS CurrSelection;

   Coord_List[0].x1 = 8;
   Coord_List[0].y1 = 0;
   Coord_List[0].x2 = 48;
   Coord_List[0].y2 = 0;
   Coord_List[0].hc = mnuScreen[0].achItem[mnuScreen[0].iHilite];

   Coord_List[1].x1 = 568;
   Coord_List[1].y1 = 0;
   Coord_List[1].x2 = 608;
   Coord_List[1].y2 = 0;
   Coord_List[1].hc = mnuScreen[1].achItem[mnuScreen[1].iHilite];
   
   Coord_List[2].x1 = -1;
   Coord_List[10].x1 = -1;

   SetSettingsControls(&CntlInfo[0]);
   SetData(&CntlInfo[0]);
   CurrSelection = SUB_FILE;
   Keyboard = FALSE;
   AltKey = TRUE;

   while( loop == TRUE )
   {
       DrawSettingsScreen();
       DrawControls(&CntlInfo[0], 13);
       ReDrawData(&CntlInfo[0], 7, _TBRIGHTWHITE, _TBLUE);
       ReDrawCheckData(&CntlInfo[7], 4, _TBRIGHTWHITE, _TBLUE);
       ReDrawData(&CntlInfo[11], 1, _TBRIGHTWHITE, _TBLUE);
       ReDrawCheckData(&CntlInfo[12], 1, _TBRIGHTWHITE, _TBLUE);

       while(smloop == TRUE)
       {
           switch(Process)
           {
               case 1:
               {
                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   KeyDown = TRUE;
                   sf = 0;
                   ret = (enum MENUS) MenuBar( 0, 0, mnuScreen, 2, -1, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   KeyDown = FALSE;

                   if(ret == -9)
                   {
                       sf = 1;
                       CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 2, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   }
                   else
                       CurrSelection = (enum MENUS) ret;

                   if((CurrSelection == ALT) || (CurrSelection == ESCAPE))
                   {
                       Process = 2;
                       AltKey = TRUE;
                   }
                   else
                       smloop = FALSE;
               }
               break;

               case 2:
               {
                   int of = 1;
                   int ret;

                   StatusBar(" ", CENTER, 0, 25, _TBLACK, _TWHITE);
                   sf = 1;
                   MenuBar( 0, 0, mnuScreen, 2, -1, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);

                   Keyboard = TRUE;
                   AltKey = FALSE;
                   StatusBar("Enter settings.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   CurrSelection = SUB_FILE;
                   ret = ProcessSettingsEdit(&CntlInfo[0]);
                   if( ret == ESCAPE )
                       return;

                   if( (CurrMenu >= 0) &&(CurrMenu < 10) )
                   {
                       CurrSelection = (enum MENUS) CurrMenu;
                       SubMenu = 0;
                       of = 3;
                   } 

                   if(ret == ALT)
                       of = 1;

                   Process = of;
                   SubMenu = 0;
               }
               break;

               case 3:
                   MenuBar( 0, 0, mnuScreen, 2, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   smloop = FALSE;
               break;

               case 4:
                   sf = 0;
                   AltKey = FALSE;
                   CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 2, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   if((CurrSelection == ALT) || (CurrSelection == ESCAPE))
                   {
                       Process = 2;
                       AltKey = TRUE;
                   }
                   else
                       smloop = FALSE;
               break;
           }
       }

       smloop = TRUE;
       strcpy(message, "Select menu item.");
 
       switch(CurrSelection)
       {
           case SUB_FILE:
           {
               Coord_List[10].x1 = 8;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 192;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 8;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 192;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 8;
               Coord_List[12].y1 = 32;
               Coord_List[12].x2 = 192;
               Coord_List[12].y2 = 32;

               Coord_List[13].x1 = 8;
               Coord_List[13].y1 = 48;
               Coord_List[13].x2 = 192;
               Coord_List[13].y2 = 48;

               Coord_List[14].x1 = -1;

	            SubMenu = (enum CHOICES) Menu( 4, 13, mnuMain, SubMenu, 0);
               if(CurrMenu == -1)
               {
                   Process = 2;
                   AltKey = FALSE;
                   SubMenu = 0;
                   break;
               }

               if(CurrMenu >= 0)
               {
                   if(CurrMenu < 10)
                   {
                       Process = 3;
                       CurrSelection = (enum MENUS) CurrMenu;
                       SubMenu = 0;
                       break;
                   }    
                   else
                   {
                       CurrMenu -= 10;
                       SubMenu = CurrMenu;
                   }
               }            

               switch(SubMenu)
               {
                   case PREVIOUS:
                       CurrSelection = HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection= HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       sf = 1;
                       AltKey = FALSE;
                        Process = 3;
                       StatusBar("Select an Installed Modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListInstalledModems(&CurrentPage.ModemInfo, 0, 18, 15, 50, 4)) >= 0)
                           strcpy((LPSTR) CntlInfo[5].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.szInstalledName);
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                  break;
                   case 1:
                       Process = 3;
                       GetData(&CntlInfo[0]);
                   break;
                   case 2:
                       Process = 3;
                       FreeControl(&CntlInfo[0], 13);
                   break;

                   case 3:
                   case 4:
                       return;
                   break;

                   case ALT:
                       Keyboard = FALSE;
                       AltKey = TRUE;
                       Process = 2;
                   break;

                   case ESCAPE:
                       Process = 4;
                   break;
               }
           }
           break;
           case HELP:
           {
               Coord_List[10].x1 = 568;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 616;
               Coord_List[10].y2 = 16;
   
               Coord_List[11].x1 = 568;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 616;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = -1;

	            SubMenu = Menu( 3, 74, mnuHelp, SubMenu, 0);
               if(CurrMenu == -1)
               {
                  Process = 2;
                  AltKey = FALSE;
                  SubMenu = 0;
                  break;
               }

               if(CurrMenu >= 0)
               {
                   if(CurrMenu < 10)
                   {
                       Process = 3;
                       CurrSelection = (enum MENUS) CurrMenu;
                       SubMenu = 0;
                       break;
                   }    
                   else
                   {
                       CurrMenu -= 10;
                       SubMenu = CurrMenu;
                   }
               }

               switch(SubMenu)
               {
                   case PREVIOUS:
                       CurrSelection = SUB_FILE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection = SUB_FILE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       DrawMainScreen();
                       MenuBar( 0, 0, mnuScreen, 2, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                       LoadHelpFile();
                       Process = 3;
                       if(CurrMenu >= 0)
                       {
                           AltKey = FALSE;
                           Process = 2;
                       }
                   break;
                   case 1:
                       AboutBox();
                       Process = 3;
                       if(CurrMenu >= 0)
                       {
                           AltKey = FALSE;
                           Process = 2;
                       }
                   break;
                   
                   case ALT:
                       AltKey = FALSE;
                       SubMenu = 0;
                       Process = 2;
                   break;

                   case ESCAPE:
                       SubMenu = 0;
                       Process = 4;
                   break;
               }
           }
           break;
       }
   }
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawSettingsScreen()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText(" Settings Edit ", CENTER, 0, 2, _TBLUE, _TWHITE);

   PrintText("    Text length includes  ", LEFT, 5, 20, _TWHITE, _TBLUE);
}

//-----------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------
void GetData(PCONTROL_PARAM pCntlInfo)
{
   char szBuffer[256] = "";
   char szBuffer2[128];
   
   StatusBar("Saving WAPI.INI...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
       strcpy(szBuffer, "WAPI.INI");

   WapiInfo.dwOptions = 0;

   strcpy( WapiInfo.szSpoolDir, (pCntlInfo + 0)->pCntlBuffer);
   if (WapiInfo.szSpoolDir[strlen(WapiInfo.szSpoolDir) - 1] != '\\')
       strcat(WapiInfo.szSpoolDir, "\\");
   WriteProfileString("Directories", "Spool", (pCntlInfo + 0)->pCntlBuffer, szBuffer);

   strcpy( WapiInfo.szPhoneBookDir, (pCntlInfo + 1)->pCntlBuffer);
   if (WapiInfo.szPhoneBookDir[strlen(WapiInfo.szPhoneBookDir) - 1] != '\\')
       strcat(WapiInfo.szPhoneBookDir, "\\");
   WriteProfileString("Directories", "Phonebook", (pCntlInfo + 1)->pCntlBuffer, szBuffer);

   strcpy( WapiInfo.szDataDir, (pCntlInfo + 2)->pCntlBuffer);
   if (WapiInfo.szDataDir[strlen(WapiInfo.szDataDir) - 1] != '\\')
       strcat(WapiInfo.szDataDir, "\\");
   WriteProfileString("Directories", "DataFiles", (pCntlInfo + 2)->pCntlBuffer, szBuffer);

   strcpy( WapiInfo.szPredefinedMsgs, (pCntlInfo + 3)->pCntlBuffer);
   if (WapiInfo.szPredefinedMsgs[strlen(WapiInfo.szPredefinedMsgs) - 1] != '\\')
       strcat(WapiInfo.szPredefinedMsgs, "\\");
   WriteProfileString("Directories", "Predefined", (pCntlInfo + 3)->pCntlBuffer, szBuffer);

   strcpy( WapiInfo.szLogDir, (pCntlInfo + 4)->pCntlBuffer);
   if (WapiInfo.szLogDir[strlen(WapiInfo.szLogDir) - 1] != '\\')
       strcat(WapiInfo.szLogDir, "\\");
   WriteProfileString("Directories", "LogDirectory", (pCntlInfo + 4)->pCntlBuffer, szBuffer);

   strcpy( WapiInfo.szDefaultModemName, (pCntlInfo + 5)->pCntlBuffer);
   WriteProfileString("Options", "DefModem", (pCntlInfo + 5)->pCntlBuffer, szBuffer);

   WriteProfileString("Signature", "DefaultSignature", (pCntlInfo + 6)->pCntlBuffer, szBuffer);
   strcpy(BeepInfo.szDefaultSignature, (pCntlInfo + 6)->pCntlBuffer);

   if(strcmp((pCntlInfo + 7)->pCntlBuffer, "X") == 0)
       WapiInfo.dwOptions |= FLG_SIGNATURE_ENABLE;

   if(strcmp((pCntlInfo + 8)->pCntlBuffer, "X") == 0)
       WapiInfo.dwOptions |= FLG_TIMESTAMP_ENABLED;

   if(strcmp((pCntlInfo + 9)->pCntlBuffer, "X") == 0)
       WapiInfo.dwOptions |= FLG_MSG_RECALL_ENABLED;

   if(strcmp((pCntlInfo + 10)->pCntlBuffer, "X") == 0)
   {
       WapiInfo.dwOptions |= OPT_LOG_TRANSACTIONS;
       WapiInfo.dwOptions |= OPT_LOG_ERRORS;
   }

   BeepInfo.nDaysOld = atoi((pCntlInfo + 11)->pCntlBuffer);
   if(strcmp((pCntlInfo + 11)->pCntlBuffer, "") == 0)
       WriteProfileString("Options", "PurgeOldAge", "7", szBuffer);
   else
       WriteProfileString("Options", "PurgeOldAge", (pCntlInfo + 11)->pCntlBuffer, szBuffer);

   if(strcmp((pCntlInfo + 12)->pCntlBuffer, "X") == 0)
       WapiInfo.dwOptions |= FLG_LENGTH_COUNT;

   ltoa(WapiInfo.dwOptions, szBuffer2, 10);
   WriteProfileString("Options", "Options", szBuffer2, szBuffer);
   ltoa(WapiInfo.dwFlags, szBuffer2, 10);
   WriteProfileString("Options", "Flags", szBuffer2, szBuffer);

   StatusBar("Changes to WAPI.INI were saved.", CENTER, 0, 25, _TBLACK, _TWHITE);
}
//-----------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------
void SetData(PCONTROL_PARAM pCntlInfo)
{
   char szBuffer[256] = "";
   char szBuffer2[256] = "";

   strcpy((pCntlInfo + 0)->pCntlBuffer, WapiInfo.szSpoolDir);
   strcpy((pCntlInfo + 1)->pCntlBuffer, WapiInfo.szPhoneBookDir);
   strcpy((pCntlInfo + 2)->pCntlBuffer, WapiInfo.szDataDir);
   strcpy((pCntlInfo + 3)->pCntlBuffer, WapiInfo.szPredefinedMsgs);
   strcpy((pCntlInfo + 4)->pCntlBuffer, WapiInfo.szLogDir);
   strcpy((pCntlInfo + 5)->pCntlBuffer, WapiInfo.szDefaultModemName);

   strcpy((pCntlInfo + 6)->pCntlBuffer, BeepInfo.szDefaultSignature);

   if(WapiInfo.dwOptions & FLG_SIGNATURE_ENABLE)
       strcpy((pCntlInfo + 7)->pCntlBuffer, "X");

   if(WapiInfo.dwOptions & FLG_TIMESTAMP_ENABLED)
       strcpy((pCntlInfo + 8)->pCntlBuffer, "X");

   if(WapiInfo.dwOptions & FLG_MSG_RECALL_ENABLED)
       strcpy((pCntlInfo + 9)->pCntlBuffer, "X");

   if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
       strcpy((pCntlInfo + 10)->pCntlBuffer, "X");

   itoa(BeepInfo.nDaysOld, (pCntlInfo + 11)->pCntlBuffer, 10);

   if(WapiInfo.dwOptions & FLG_LENGTH_COUNT)
       strcpy((pCntlInfo + 12)->pCntlBuffer, "X");
}


