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
void Subscriber()
{
   BOOL    ProcessOn = FALSE;
   BOOL    DrawOn = FALSE;
   int     Process = 2;
   BOOL    smloop = TRUE;
   long    SubMenu;
   short   loop = TRUE;
   int     ret;
   
   ITEM mnuMain[] =
   {
       { 0, "Select Subscriber"},   
       { 1, "Select Service"   },   
       {-1, "-----------------"},   
       { 0, "Return      <Esc>"},   
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

   //memset(&CurrentPage.SubscriberInfo, 0, sizeof(WAPISUBSCRIBERINFO));
   SetSubscriberControls(&CntlInfo[0]);
   CurrSelection = SUB_FILE;
   Keyboard = FALSE;
   AltKey = TRUE;

   while( loop == TRUE )
   {
       DrawSubscriberBox();
       DrawControls(&CntlInfo[0], 3);
       ReDrawData(&CntlInfo[0], 3, _TBRIGHTWHITE, _TBLUE);

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
                   StatusBar("Enter subscriber.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   CurrSelection = SUB_FILE;
                   ret = ProcessSubscriberEdit(&CntlInfo[0]);
                    
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
               Coord_List[10].x2 = 152;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 8;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 152;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 8;
               Coord_List[12].y1 = 40;
               Coord_List[12].x2 = 152;
               Coord_List[12].y2 = 40;

               Coord_List[13].x1 = -1;

	            SubMenu = (enum CHOICES) Menu( 4, 10, mnuMain, 0, SUB_FILE);
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
                       StatusBar("Select a Subscriber.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListSubscribers(&CurrentPage.SubscriberInfo, 15, 20, 40, 6)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szSubscriberName);
                           strcpy((LPSTR) CntlInfo[1].pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szPIN);
                           strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szServiceName);
                           sprintf(message,"Subscriber %s has been selected.", CurrentPage.SubscriberInfo.szSubscriberName);
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
                       StatusBar("Select a Service.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListServices(&CurrentPage.ServiceInfo, 15, 20, 40, 6)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[2].pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szServiceName);
                           sprintf(message,"Service %s has been selected.", CurrentPage.ServiceInfo.szServiceName);
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   break;

                   case 2:
                   case 3:
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

	            SubMenu = (enum CHOICES) Menu( 3, 12, mnuEdit, 0, EDIT);
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
                               if(strcmp(CntlInfo[2].pCntlBuffer, "") != 0)
                               {
                                   if( Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                                   {
                                       strcpy(CurrentPage.ServiceInfo.szServiceName, CntlInfo[2].pCntlBuffer);

                                       if(Service_GetanyRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &CurrentPage.ServiceInfo) != CALL_SUCCESS)
                                       {
                                           strcpy(message,"ERROR : Service name not found in the database.");
                                           Beep(698, 700);
                                       }
                                       else
                                       {
                                           if( Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                                           {
                                               WAPISUBSCRIBERINFO SubscriberInfo2;

                                               memset(&CurrentPage.SubscriberInfo, 0, sizeof(WAPISUBSCRIBERINFO));
                                               strcpy(CurrentPage.SubscriberInfo.szSubscriberName, CntlInfo[0].pCntlBuffer);
                                               strcpy(CurrentPage.SubscriberInfo.szPIN, CntlInfo[1].pCntlBuffer);
                                               strcpy(CurrentPage.SubscriberInfo.szServiceName, CntlInfo[2].pCntlBuffer);

                                               memcpy((void *) &SubscriberInfo2,(void *) &CurrentPage.SubscriberInfo, sizeof(WAPISUBSCRIBERINFO) );

                                               if(Subscriber_GetanyRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &SubscriberInfo2) != CALL_SUCCESS)
                                               {
                                                   if(Subscriber_InsertRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &CurrentPage.SubscriberInfo) != CALL_SUCCESS)
                                                   {
                                                       strcpy(message,"ERROR: Subscriber was NOT saved.");
                                                       Beep(698, 700);
                                                   }
                                                   else
                                                       sprintf(message,"Subscriber %s was saved.", CurrentPage.SubscriberInfo.szSubscriberName);
                                               }
                                               else
                                                   if(Subscriber_DeleteRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &SubscriberInfo2) == CALL_SUCCESS)
                                                   {
                                                       if(Subscriber_InsertRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &CurrentPage.SubscriberInfo) != CALL_SUCCESS)
                                                       {
                                                           strcpy(message,"ERROR: Subscriber was NOT updated!");
                                                           Beep(698, 700);
                                                       }
                                                       else
                                                           sprintf(message,"Subscriber %s was updated.", CurrentPage.SubscriberInfo.szSubscriberName);
                                                   }

                                               Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
                                           }
                                       }
                                       Close_DBFile(&WapiInfo, SERVICE_DB);
                                   }
                                   else
                                   {
                                       strcpy(message,"ERROR : Could not access service database.");
                                       Beep(698, 700);
                                   }
                               }           
                               else
                               {
                                   strcpy(message,"ERROR : Service name is missing.");
                                   Beep(698, 700);
                               }
                           }
                           else
                           {
                               strcpy(message,"ERROR : Pin number is missing.");
                               Beep(698, 700);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Subscriber name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 1:
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           if( Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                           {
                               memset(&CurrentPage.SubscriberInfo, 0, sizeof(WAPISUBSCRIBERINFO));
                               strcpy(CurrentPage.SubscriberInfo.szSubscriberName, CntlInfo[0].pCntlBuffer);

                               if(Subscriber_DeleteRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &CurrentPage.SubscriberInfo) == CALL_SUCCESS)
                               {
                                   FreeControl(&CntlInfo[0], 3);
                                   sprintf(message,"Subscriber %s was deleted.", CurrentPage.SubscriberInfo.szSubscriberName);
                               }
                               else
                               {
                                   strcpy(message,"ERROR: Subscriber was NOT deleted.");
                                   Beep(698, 700);
                               }

                               Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Subscriber name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 2:
                       Process = 3;
                       FreeControl(&CntlInfo[0], 3);
                       SetSubscriberControls(&CntlInfo[0]);
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
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ProcessSubscriberEdit(PCONTROL_PARAM pCntlInfo)
{
   int             ret;
   PCONTROL_PARAM  CurrControl;
   int             next = 0;
   int             retVal;
   int             Stop = FALSE;
   short           loop = 1;
   short           smloop = 1;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
   
   CurrControl = pCntlInfo;  
   
   while(loop == 1)
   {
       if ( next == 0 )
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
                       next = 1;
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
                   case LIST:
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if((ret = ListSubscribers(&CurrentPage.SubscriberInfo, 15, 20, 40, 6)) >= 0)
                       {
                           strcpy((LPSTR) (CurrControl + 0)->pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szSubscriberName);
                           strcpy((LPSTR) (CurrControl + 1)->pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szPIN);
                           strcpy((LPSTR) (CurrControl + 2)->pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szServiceName);
                           sprintf(message,"Subscriber %s has been selected.", CurrentPage.SubscriberInfo.szSubscriberName);
                           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                       }
                       else
                       if(ret == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 3);
                           ReDrawData(pCntlInfo, 3, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 3);
                       ReDrawData(pCntlInfo, 3, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
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

       if(next == 2)
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
                       next = 0;
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
                   case LIST:
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);
                       if((ret = ListServices(&CurrentPage.ServiceInfo, 15, 20, 40, 6)) >= 0)
                           strcpy((LPSTR) CurrControl->pCntlBuffer, (LPCSTR) CurrentPage.ServiceInfo.szServiceName);
                       else
                       if(ret == ALT)
                       {
                           DrawSubscriberBox();
                           DrawControls(pCntlInfo, 3);
                           ReDrawData(pCntlInfo, 3, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawSubscriberBox();
                       DrawControls(pCntlInfo, 3);
                       ReDrawData(pCntlInfo, 3, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }
   }
   return ESCAPE;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawSubscriberBox()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText(" Subscriber Edit ", CENTER, 0, 2, _TBLUE, _TWHITE);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ListSubscribers(LPWAPISUBSCRIBERINFO pSubscriberInfo, short row, short col, int maxWidth, short maxEntries)
{
   ITEM *pSubList = NULL;
   int index = 0, offset = NOT_SELECTED;
   int Height;
   int Overflow = 1;
   char buffer[50];

   if(!pSubscriberInfo)
      return ESCAPE;

   if((pSubList = (ITEM *) malloc( MAX_SUBSCRIBERS * sizeof(ITEM) )) == NULL)
      return ESCAPE;

   StatusBar("Loading Subscriber Database...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if( Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
   {
   
      if( Subscriber_GetfirstRec(&WapiInfo, SUBSCRIBER_DB, 0, (PVOID) pSubscriberInfo) == CALL_SUCCESS )
      {
         do
         {
            pSubList[index].iHilite = -1;
            strcpy((LPSTR) pSubList[index].achItem, (LPCSTR) pSubscriberInfo->szSubscriberName);

            Overflow++;
            index++;
         }
         while ( (Subscriber_GetnextRec(&WapiInfo, SUBSCRIBER_DB, 0, (PVOID) pSubscriberInfo) == CALL_SUCCESS) && (Overflow < MAX_SUBSCRIBERS ));

         if( index > maxEntries )
            Height = maxEntries;
         else
            Height = index;

         StatusBar("Select Subscriber.", LEFT, 40, 25, _TBLACK, _TWHITE);

         sprintf(buffer, " Total # of subscribers : %d ", index);
         PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

         offset = ListBox( row, col, maxWidth, Height, pSubList, index, 0, HILIGHT, NULL);
         
         StatusBar("", CENTER, 40, 25, _TBLACK, _TWHITE);

         if ( offset < 0)
         {
            free(pSubList);
            Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
            return offset;
         }

         strcpy((LPSTR) pSubscriberInfo->szSubscriberName, (LPCSTR) pSubList[offset].achItem);
         
         if(Subscriber_GetanyRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) pSubscriberInfo) != CALL_SUCCESS)
         {
            Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
            free(pSubList);
            return ESCAPE;
         }
      }
      else
      {
           offset = ESCAPE;
           strcpy(message, "ERROR : No subscribers were found.");
      }


      Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
   }
   else
   {
      offset = ESCAPE;
      strcpy(message, "ERROR : Could not open subscriber database.");
   }

   free(pSubList); 
   return (WORD) offset;
}

