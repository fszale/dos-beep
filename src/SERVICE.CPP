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
extern ITEM                mnuHelp[3];
extern WAPISPOOLERINFO     CurrentPage;
extern CONTROL_PARAM       CntlInfo[13];
extern BOOL                Keyboard;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void Service()
{
   BOOL                ProcessOn = FALSE;
   BOOL                DrawOn = FALSE;
   long                SubMenu;
   BOOL                smloop = TRUE;
   int                 Process = 2;
   short               loop = TRUE;
   char                szBuffer[20];
   int                 ret;

   ITEM mnuMain[] =
   {
       { 7, "Select Service"   },   
       { 7, "Select Modem"     },   
       {-1, "--------------"   },   
       { 0, "Return   <Esc>"   },   
       { 0, ""                 } 
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

   SetServiceControls(&CntlInfo[0]);
   CurrSelection = SUB_FILE;
   Keyboard = FALSE;
   AltKey = TRUE;

   while(loop == TRUE)
   {
       DrawServiceBox();
       DrawControls(&CntlInfo[0], 9);
       ReDrawData(&CntlInfo[0], 9, _TBRIGHTWHITE, _TBLUE);

       while(smloop == TRUE)
       {
           switch(Process)
           {
               case 1:
               {
                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   KeyDown = TRUE;
                   sf = 0;
                   ret = (enum MENUS) MenuBar( 0, 0, mnuScreen, 3, -1, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   KeyDown = FALSE;

                   if(ret == -9)
                   {
                       sf = 1;
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
                   StatusBar("Enter service.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   CurrSelection = SUB_FILE;
                   ret = ProcessServiceEdit(&CntlInfo[0]);
                   
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
               Coord_List[10].x2 = 128;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 8;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 128;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 8;
               Coord_List[12].y1 = 40;
               Coord_List[12].x2 = 128;
               Coord_List[12].y2 = 40;

               Coord_List[13].x1 = -1;

	            SubMenu = (enum CHOICES) Menu( 4, 9, mnuMain, 0, 0);
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
                       CurrSelection= EDIT;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       sf = 1;
                       AltKey = FALSE;
                       Process = 3;
                       StatusBar("Select a service.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListServices(&CurrentPage.ServiceInfo, 4, 45, 31, 12)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szServiceName);
                           strcpy((LPSTR) CntlInfo[1].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szPhoneNumber);
                           strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.uProtocol.Pet.szPassword);
                           ConvertParams(&(CntlInfo[4].pCntlBuffer), CurrentPage.ServiceInfo.dwBaud, DOS_BAUD);
                           ConvertParams(&(CntlInfo[5].pCntlBuffer), CurrentPage.ServiceInfo.byParity, DOS_PARITY);
                           ConvertParams(&(CntlInfo[6].pCntlBuffer), CurrentPage.ServiceInfo.byDataBits, DOS_DATABITS);
                           ConvertParams(&(CntlInfo[7].pCntlBuffer), CurrentPage.ServiceInfo.byStopBits, DOS_STOPBITS);
                           strncpy((LPSTR) &CntlInfo[8].pCntlBuffer, CurrentPage.ServiceInfo.szInstalledModem, 64);
                           itoa(CurrentPage.ServiceInfo.uProtocol.Pet.wMaxMsgLength, szBuffer, 10);
                           strcpy(CntlInfo[3].pCntlBuffer, szBuffer);
                           sprintf(message,"Service %s has been selected.", CurrentPage.ServiceInfo.szServiceName);
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                  break;
                   case 1:
                       sf = 1;
                       AltKey = FALSE;
                       Process = 3;
                       StatusBar("Select an Installed Modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListInstalledModems(&CurrentPage.ModemInfo, 1, 18, 15, 50, 4)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[8].pCntlBuffer, (LPCSTR) CurrentPage.ModemInfo.szInstalledName);
                           sprintf(message,"Modem %s has been selected.", CurrentPage.ModemInfo.szInstalledName);
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   break;

                   case 3:
                   case 2:
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

               SubMenu = (enum CHOICES) Menu( 3, 12, mnuEdit, 0, 0);
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
                           if(strcmp(CntlInfo[1].pCntlBuffer, "") != 0)
                           {
                               if( Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                               {
                                   WAPISERVICEINFO ServiceInfo2;
                           
                                   memset(&CurrentPage.ServiceInfo, 0, sizeof(WAPISERVICEINFO));
                                   strcpy(CurrentPage.ServiceInfo.szServiceName, CntlInfo[0].pCntlBuffer);
                                   strcpy(CurrentPage.ServiceInfo.szPhoneNumber, CntlInfo[1].pCntlBuffer);
                                   strcpy(CurrentPage.ServiceInfo.uProtocol.Pet.szPassword, CntlInfo[2].pCntlBuffer);
                                   BackConvertParams(&CntlInfo[4], (PVOID) &CurrentPage.ServiceInfo.dwBaud, DOS_BAUD);
                                   BackConvertParams(&CntlInfo[5], (PVOID) &CurrentPage.ServiceInfo.byParity, DOS_PARITY);
                                   BackConvertParams(&CntlInfo[6], (PVOID) &CurrentPage.ServiceInfo.byDataBits, DOS_DATABITS);
                                   BackConvertParams(&CntlInfo[7], (PVOID) &CurrentPage.ServiceInfo.byStopBits, DOS_STOPBITS);
                                   if(strcmp(CntlInfo[8].pCntlBuffer, "") == 0)
                                       strcpy(CurrentPage.ServiceInfo.szInstalledModem, DEFAULT_MODEM_CONNECTION);
                                   else
                                       strcpy(CurrentPage.ServiceInfo.szInstalledModem, CntlInfo[8].pCntlBuffer);
                                   if(atoi(CntlInfo[3].pCntlBuffer) == 0)
                                       CurrentPage.ServiceInfo.uProtocol.Pet.wMaxMsgLength = 80;
                                   else
                                       CurrentPage.ServiceInfo.uProtocol.Pet.wMaxMsgLength = (WORD) atoi(CntlInfo[3].pCntlBuffer);
                                   CurrentPage.ServiceInfo.byProtocol = OUTBOUND_PET;

                                   memcpy((void *) &ServiceInfo2,(void *) &CurrentPage.ServiceInfo, sizeof(WAPISERVICEINFO) );

                                   if(Service_GetanyRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &ServiceInfo2) != CALL_SUCCESS)
                                   {
                                       if(Service_InsertRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &CurrentPage.ServiceInfo) != CALL_SUCCESS)
                                       {
                                           strcpy(message,"ERROR: Service was NOT saved.");
                                           Beep(698, 700);
                                       }
                                       else
                                       {
                                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szServiceName);
                                           strcpy((LPSTR) CntlInfo[1].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szPhoneNumber);
                                           strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.uProtocol.Pet.szPassword);
                                           ConvertParams(&(CntlInfo[4].pCntlBuffer), CurrentPage.ServiceInfo.dwBaud, DOS_BAUD);
                                           ConvertParams(&(CntlInfo[5].pCntlBuffer), CurrentPage.ServiceInfo.byParity, DOS_PARITY);
                                           ConvertParams(&(CntlInfo[6].pCntlBuffer), CurrentPage.ServiceInfo.byDataBits, DOS_DATABITS);
                                           ConvertParams(&(CntlInfo[7].pCntlBuffer), CurrentPage.ServiceInfo.byStopBits, DOS_STOPBITS);
                                           strncpy((LPSTR) &CntlInfo[8].pCntlBuffer, CurrentPage.ServiceInfo.szInstalledModem, 64);
                                           itoa(CurrentPage.ServiceInfo.uProtocol.Pet.wMaxMsgLength, szBuffer, 10);
                                           strcpy(CntlInfo[3].pCntlBuffer, szBuffer);
                                           
                                           sprintf(message,"Service %s was saved.", CurrentPage.ServiceInfo.szServiceName);
                                       }
                                   }
                                   else
                                       if(Service_DeleteRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &ServiceInfo2) == CALL_SUCCESS)
                                       {
                                           if(Service_InsertRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &CurrentPage.ServiceInfo) != CALL_SUCCESS)
                                           {
                                               strcpy(message,"ERROR: Service was NOT updated.");
                                               Beep(698, 700);
                                           }
                                           else
                                               sprintf(message,"Service %s was updated.", CurrentPage.ServiceInfo.szServiceName);
                                       }

                                   Close_DBFile(&WapiInfo, SERVICE_DB);
                               }
                           }
                           else
                           {
                               strcpy(message,"ERROR : Phone number is missing.");
                               Beep(698, 700);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Service name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 1:
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           if( Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                           {
                               memset(&CurrentPage.ServiceInfo, 0, sizeof(WAPISERVICEINFO));
                               strcpy(CurrentPage.ServiceInfo.szServiceName, CntlInfo[0].pCntlBuffer);
                               CurrentPage.ServiceInfo.byProtocol = 1;

                               if(Service_DeleteRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &CurrentPage.ServiceInfo) == CALL_SUCCESS)
                               {
                                   FreeControl(&CntlInfo[0], 9);
                                   sprintf(message,"Service %s was deleted.", CurrentPage.ServiceInfo.szServiceName);
                               }
                               else
                               {
                                   strcpy(message,"ERROR: Service was NOT deleted.");
                                   Beep(698, 700);
                               }

                               Close_DBFile(&WapiInfo, SERVICE_DB);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Service name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 2:
                       FreeControl(&CntlInfo[0], 9);
                       SetServiceControls(&CntlInfo[0]);
                       strcpy(message, "Select menu item.");
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