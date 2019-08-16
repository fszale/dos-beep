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

       char                szModemDisplayed[60] = "";

extern WAPIINFO            WapiInfo;
extern char                message[80];
extern BOOL                AltKey;
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
void Modem()
{
   char                szBuffer[256];
   int                 Process = 2;
   BOOL                smloop = TRUE;
   BOOL                ProcessOn = FALSE;
   BOOL                DrawOn = FALSE;
   long                SubMenu;
   short               loop = TRUE;
   int                 ret;

   ITEM mnuMain[] =
   {
       { 0, "Create"               },   
       { 0, "Install"              },   
       { 0, "Select Installed"     },   
       { 4, "Set Default Modem"    },   
       {-1, "-----------------"    },   
       { 0, "Return      <Esc>"    },   
       { 0, ""                     } 
   };
   
   ITEM mnuEdit[] =
   {
       { 0, "Save"             },   
       { 0, "Delete"           },   
       { 0, "Clear"            },   
       { 0, ""                 } 
   };

   ITEMBAR mnuScreen[] =
   {                    
       {  7, 0,"File" },   
       { 63, 0,"Edit" },   
       {  5, 0,"Help" }
   };

   enum MENUS
   {
       SUB_FILE, EDIT, HELP
   };
   enum MENUS CurrSelection;

   Coord_List[0].x1 = 8;
   Coord_List[0].y1 = 0;
   Coord_List[0].x2 = 48;
   Coord_List[0].y2 = 0;
   Coord_List[0].hc = mnuScreen[0].achItem[mnuScreen[0].iHilite];

   Coord_List[1].x1 = 64;
   Coord_List[1].y1 = 0;
   Coord_List[1].x2 = 104;
   Coord_List[1].y2 = 0;
   Coord_List[1].hc = mnuScreen[1].achItem[mnuScreen[1].iHilite];
 
   Coord_List[2].x1 = 568;
   Coord_List[2].y1 = 0;
   Coord_List[2].x2 = 608;
   Coord_List[2].y2 = 0;
   Coord_List[2].hc = mnuScreen[2].achItem[mnuScreen[2].iHilite];
   
   Coord_List[3].x1 = -1;
   Coord_List[10].x1 = -1;
                    
   memset(szModemDisplayed, 0, sizeof(szModemDisplayed));

   SetModemControls(&CntlInfo[0]);
   CurrSelection = SUB_FILE;

   Keyboard = FALSE;
   AltKey = TRUE;

   while(loop == TRUE)
   {
       DrawModemBox();
       DrawControls(&CntlInfo[0], 9);
       ReDrawData(&CntlInfo[0], 9, _TBRIGHTWHITE, _TBLUE);

       while(smloop == TRUE)
       {
           switch(Process)
           {
               case 1:
               {
                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   SubMenu = 0;
                   sf = 0;
                   KeyDown = TRUE;
                   Keyboard = TRUE;
                   ret = (enum MENUS) MenuBar( 0, 0, mnuScreen, 3, -1, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   KeyDown = FALSE;

                   if(ret == -9)
                   {
                       sf = 1;
                       Keyboard = TRUE;
                       CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 3, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
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

                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   sf = 1;
                   MenuBar( 0, 0, mnuScreen, 3, -1, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);

                   Keyboard = TRUE;
                   AltKey = FALSE;
                   ret = ALT;
                   StatusBar("Enter modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   CurrSelection = SUB_FILE;
                   ret = ProcessModemEdit(&CntlInfo[0], 8);

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
                   MenuBar( 0, 0, mnuScreen, 3, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   smloop = FALSE;
               break;

               case 4:
                   sf = 0;
                   AltKey = FALSE;
                   Keyboard = TRUE;
                   CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 3, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
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
               Coord_List[10].x2 = 152;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 8;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 152;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 8;
               Coord_List[12].y1 = 32;
               Coord_List[12].x2 = 152;
               Coord_List[12].y2 = 32;

               Coord_List[13].x1 = 8;
               Coord_List[13].y1 = 40;
               Coord_List[13].x2 = 152;
               Coord_List[13].y2 = 40;

               Coord_List[14].x1 = 8;
               Coord_List[14].y1 = 56;
               Coord_List[14].x2 = 152;
               Coord_List[14].y2 = 56;

               Coord_List[15].x1 = -1;

	            SubMenu = Menu( 5, 10, mnuMain, (int) SubMenu, 0);
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
                   {
                       CurrSelection= EDIT;
                       SubMenu = 0;
                       Process = 3;
                   }
                   break;

                   case 0: // Create
                   {
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           memset(&CurrentPage.ModemInfo, 0, sizeof(WAPIMODEMINFO));
                           StatusBar("Select communication port.", CENTER, 0, 25, _TBLACK, _TWHITE);
                           CurrentPage.ModemInfo.byCOMMType = COMM_LOCAL;
                           CurrentPage.ModemInfo.byCOMMPort = 1;

                           strcpy(CurrentPage.ModemInfo.ModemInfo.szModemName, CntlInfo[0].pCntlBuffer);

                           if(InstallModem(&CurrentPage.ModemInfo, &CntlInfo[0]) == TRUE)
                           {
                               strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szModemName);   
                               strcpy((LPSTR) CntlInfo[1].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szInitString);   
                               strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szHangupString);   
                               strcpy((LPSTR) CntlInfo[3].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialPrefixString);   
                               strcpy((LPSTR) CntlInfo[4].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialSuffixString);   
                               strcpy((LPSTR) CntlInfo[5].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szResetString);   
                               strcpy((LPSTR) CntlInfo[6].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szEscapeString);  
                               CurrentPage.ModemInfo.ModemInfo.byReDial_Count = 10;
                               itoa(CurrentPage.ModemInfo.ModemInfo.byReDial_Count, CntlInfo[7].pCntlBuffer, 10);
                             
                               if(CurrentPage.ModemInfo.byCOMMType == COMM_LOCAL)
                                   ConvertParams((PVOID) CntlInfo[8].pCntlBuffer, (DWORD) CurrentPage.ModemInfo.byCOMMPort, DOS_COMMPORT);
                               else
                                   strcpy(CntlInfo[8].pCntlBuffer, "N");

                               sprintf(message,"Installed modem %s.", CurrentPage.ModemInfo.szInstalledName);
                               strcpy(szModemDisplayed, CurrentPage.ModemInfo.szInstalledName);
                           }
                           else
                           {
                               sprintf(message,"Could not install modem %s.", CurrentPage.ModemInfo.szInstalledName);
                               Beep(698, 700);
                           }
                       }
                       else
                       {
                           strcpy(message, "ERROR: Modem name is missing.");
                           Beep(698, 700);
                       }
                   }
                   break;

                   case 1: // Install
                   {
                       sf = 1;
                       Process = 3;
                       AltKey = FALSE;
                       DrawModemBox();
                       DrawControls(&CntlInfo[0], 9);
                       ReDrawData(&CntlInfo[0], 9, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Select a modem.", CENTER, 0, 25, _TBLACK, _TWHITE);

                       if((ret = ListModems(&CurrentPage.ModemInfo.ModemInfo, 6, 35, 40, 8)) >= 0)
                       {                     
                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szModemName);   
                           strcpy((LPSTR) CntlInfo[1].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szInitString);   
                           strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szHangupString);   
                           strcpy((LPSTR) CntlInfo[3].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialPrefixString);   
                           strcpy((LPSTR) CntlInfo[4].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialSuffixString);   
                           strcpy((LPSTR) CntlInfo[5].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szResetString);   
                           strcpy((LPSTR) CntlInfo[6].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szEscapeString);  
                           strcpy((LPSTR) CntlInfo[7].pCntlBuffer, (LPCSTR) "10");  
                           CurrentPage.ModemInfo.ModemInfo.byReDial_Count = 10;                       

                           DrawModemBox();
                           DrawControls(&CntlInfo[0], 9);
                           ReDrawData(&CntlInfo[0], 9, _TBRIGHTWHITE, _TBLUE);
  
                           StatusBar("Select communication port.", CENTER, 0, 25, _TBLACK, _TWHITE);
                           if((ret = ListComOptions(&CurrentPage.ModemInfo, 17, 46, 5, 3)) >= 0)
                           {
                               if(CurrentPage.ModemInfo.byCOMMType == COMM_LOCAL)
                                   ConvertParams((PVOID) CntlInfo[8].pCntlBuffer, (DWORD) CurrentPage.ModemInfo.byCOMMPort, DOS_COMMPORT);
                               else
                                   if(CurrentPage.ModemInfo.byCOMMType == COMM_NONE)
                                       strcpy(CntlInfo[8].pCntlBuffer, "N");

                               ReDrawData(&CntlInfo[8], 1, _TBRIGHTWHITE, _TBLUE);

                               if(InstallModem(&CurrentPage.ModemInfo, &CntlInfo[0]) == TRUE)
                               {
                                   sprintf(message,"Installed modem %s.", CurrentPage.ModemInfo.szInstalledName);
                                   strcpy(szModemDisplayed, CurrentPage.ModemInfo.szInstalledName);
                               }
                               else
                               {
                                   sprintf(message,"Could not install modem %s.", CurrentPage.ModemInfo.szInstalledName);
                                   Beep(698, 700);
                               }
                           }
                           else
                           if(ret == ALT)
                           {
                               AltKey = TRUE;
                               Process = 2;
                           }
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   }
                   break;

                   case 2: // Select Installed Modem
                   {
                       sf = 1;
                       AltKey = FALSE;
                       Process = 3;
                       StatusBar("Select an installed modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if ( (ret = ListInstalledModems(&CurrentPage.ModemInfo, 0, 15, 15, 50, 4)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szModemName);   
                           strcpy((LPSTR) CntlInfo[1].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szInitString);   
                           strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szHangupString);   
                           strcpy((LPSTR) CntlInfo[3].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialPrefixString);   
                           strcpy((LPSTR) CntlInfo[4].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szDialSuffixString);   
                           strcpy((LPSTR) CntlInfo[5].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szResetString);   
                           strcpy((LPSTR) CntlInfo[6].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.ModemInfo.szEscapeString);  
                           
                           itoa(CurrentPage.ModemInfo.ModemInfo.byReDial_Count, CntlInfo[7].pCntlBuffer, 10);
                           
                           if(CurrentPage.ModemInfo.byCOMMType == COMM_NONE)
                              strcpy(CntlInfo[8].pCntlBuffer, "N");
                           else
                               if(CurrentPage.ModemInfo.byCOMMType == COMM_LOCAL)
                                   itoa(CurrentPage.ModemInfo.byCOMMPort, CntlInfo[8].pCntlBuffer, 10);    

                           strcpy(szModemDisplayed, CurrentPage.ModemInfo.szInstalledName);
                           sprintf(message,"%s selected.", CurrentPage.ModemInfo.szInstalledName);
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   }
                   break;

                   case 3: // Set Default Modem
                   {
                       sf = 1;
                       AltKey = FALSE;
                       Process = 3;
                       StatusBar("Select an installed modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if ((ret = ListInstalledModems(&CurrentPage.ModemInfo, 0, 15, 15, 50, 4)) >= 0 )
                       {
                           strcpy(WapiInfo.szDefaultModemName, CurrentPage.ModemInfo.szInstalledName);

                           if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
                               strcpy(szBuffer, "WAPI.INI");
                           
                           if(WriteProfileString("Options", "DefModem", WapiInfo.szDefaultModemName, szBuffer) == TRUE)
                               sprintf(message,"Default modem is now %s.", WapiInfo.szDefaultModemName);
                           else
                           {
                               strcpy(message,"ERROR: Default modem could not be set.");
                               Beep(698, 700);
                           }
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   }
                   break;

                   case 4: // Return
                   case 5: // Return
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

           case EDIT:
           {
               Coord_List[10].x1 = 64;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 120;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 64;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 120;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 64;
               Coord_List[12].y1 = 32;
               Coord_List[12].x2 = 120;
               Coord_List[12].y2 = 32;

               Coord_List[13].x1 = -1;

	            SubMenu = Menu( 3, 12, mnuEdit, (int) SubMenu, 0);
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
                       CurrSelection = HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           if( Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
                           {
                               strcpy(CurrentPage.ModemInfo.szInstalledName, szModemDisplayed);

                               if(Modem_GetanyRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) &CurrentPage.ModemInfo) == CALL_SUCCESS)
                               {
                                   if(strcmp(CntlInfo[1].pCntlBuffer, "") != 0)
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szInitString,(LPCSTR) CntlInfo[1].pCntlBuffer);   
                                   else
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szInitString,(LPCSTR) "ATQ0V1^M");   

                                   if(strcmp(CntlInfo[2].pCntlBuffer, "") != 0)
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szHangupString,(LPCSTR) CntlInfo[2].pCntlBuffer);   
                                   else
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szHangupString,(LPCSTR) "ATH0^");   

                                   if(strcmp(CntlInfo[3].pCntlBuffer, "") != 0)
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szDialPrefixString,(LPCSTR) CntlInfo[3].pCntlBuffer);   
                                   else
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szDialPrefixString,(LPCSTR) "ATDT");   

                                   if(strcmp(CntlInfo[4].pCntlBuffer, "") != 0)
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szDialSuffixString,(LPCSTR) CntlInfo[4].pCntlBuffer);   
                                   else
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szDialSuffixString,(LPCSTR) "^M");   

                                   if(strcmp(CntlInfo[5].pCntlBuffer, "") != 0)
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szResetString,(LPCSTR) CntlInfo[5].pCntlBuffer);   
                                   else
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szResetString,(LPCSTR) "ATZ^M");   

                                   if(strcmp(CntlInfo[6].pCntlBuffer, "") != 0)
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szEscapeString,(LPCSTR) CntlInfo[6].pCntlBuffer);
                                   else
                                       strcpy((LPSTR) CurrentPage.ModemInfo.ModemInfo.szEscapeString,(LPCSTR) "~~~+++");
                                   
                                   if(strcmp(CntlInfo[7].pCntlBuffer, "") != 0)
                                       CurrentPage.ModemInfo.ModemInfo.byReDial_Count = (BYTE) atoi(CntlInfo[7].pCntlBuffer);
                                   else
                                       CurrentPage.ModemInfo.ModemInfo.byReDial_Count = 10;

                                   if(strcmp(CntlInfo[8].pCntlBuffer, "") != 0)
                                   {
                                       if((strcmp((CntlInfo[8].pCntlBuffer), "N") == 0) || (strcmp((CntlInfo[8].pCntlBuffer), "n") == 0))
                                       {
                                           CurrentPage.ModemInfo.byCOMMType = COMM_NONE;
                                           CurrentPage.ModemInfo.byCOMMPort = 0;
                                       }
                                       else
                                       {
                                           CurrentPage.ModemInfo.byCOMMType = COMM_LOCAL;
                                           CurrentPage.ModemInfo.byCOMMPort = (BYTE) atoi(CntlInfo[8].pCntlBuffer);
                                       }
                                   }
                                   else
                                   {
                                       CurrentPage.ModemInfo.byCOMMType = COMM_LOCAL;
                                       CurrentPage.ModemInfo.byCOMMPort = 1;
                                   }
 
                                   if(Modem_DeleteRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) &CurrentPage.ModemInfo) == CALL_SUCCESS)
                                       if(Modem_InsertRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) &CurrentPage.ModemInfo) != CALL_SUCCESS)
                                       {
                                           strcpy(message,"ERROR : Installed modem could not be updated.");
                                           Beep(698, 700);
                                       }
                                       else
                                           strcpy(message,"Installed modem was updated.");

                                   if(strcmp(szModemDisplayed, WapiInfo.szDefaultModemName) == 0)
                                   {
                                       strcpy(szModemDisplayed, CurrentPage.ModemInfo.szInstalledName);
                                       strcpy(WapiInfo.szDefaultModemName, CurrentPage.ModemInfo.szInstalledName);
                                   }

                               }
                               else
                               {
                                   strcpy(message,"ERROR : Displayed modem is not installed.");
                                   Beep(698, 700);
                               }

                               Close_DBFile(&WapiInfo, MODEM_DB);
                           }
                           else
                           {
                               strcpy(message,"ERROR : Could not access modem database.");
                               Beep(698, 700);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Modem name is missing.");
                           Beep(698, 700);
                       }
                   break;

                   case 1:
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           if(strcmp(WapiInfo.szDefaultModemName, szModemDisplayed) != 0)
                           {
                               if( Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir) == CALL_SUCCESS )
                               {
                                   strcpy(CurrentPage.ModemInfo.szInstalledName, szModemDisplayed);

                                   if(Modem_DeleteRec(&WapiInfo, MODEM_TYPE1, 0, (PVOID) &CurrentPage.ModemInfo) == CALL_SUCCESS)
                                   {
                                       strcpy(message,"Installed Modem was deleted.");
                                       FreeControl(&CntlInfo[0], 9);
                                       strcpy(szModemDisplayed, "");
                                   }
                                   else
                                   {
                                       strcpy(message,"ERROR : Installed Modem was not deleted.");                               
                                       Beep(698, 700);
                                   }
                               }

                               Close_DBFile(&WapiInfo, MODEM_DB);
                           }
                           else
                           {
                               strcpy(message,"ERROR : Can not delete a default modem.");
                               Beep(698, 700);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Modem name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 2:
                       Process = 3;
                       FreeControl(&CntlInfo[0], 9);
                       strcpy(szModemDisplayed, "");
                   break;

                   case ESCAPE:
                   case ALT:
                       Keyboard = FALSE;
                       AltKey = TRUE;
                       Process = 2;
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
                       CurrSelection = EDIT;
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
