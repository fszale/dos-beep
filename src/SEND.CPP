#include <io.h>
#include <fcntl.h>
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
#include "state.h"

extern struct videoconfig vc;
extern WAPIINFO            WapiInfo;
extern WAPISPOOLERINFO     CurrentPage;
extern char                message[80];
extern DosBEEPINFO         BeepInfo;
extern int                 StopProcess;
extern int                 sf;
extern BOOL                KeyDown;
extern int                 Redial_Now;
extern int                 FromPrompt;
extern BOOL                AltKey;
extern ITEM mnuHelp[3];
extern BOOL                Keyboard;

int                        doPet = FALSE;
MULT_CONTROL_PARAM         Message_CntlInfo;
//----------------------------------------------------------------
//
//  void Send_Page()
//
//----------------------------------------------------------------
void Send_Page()
{
   WAPIGROUPINFO       GroupInfo;
   WAPIMESSAGEINFO     MessageInfo;
   BOOL                ProcessOn = FALSE;
   BOOL                DrawOn = FALSE;
   BOOL                smloop = TRUE;
   int                 Process = 2;
   long                SubMenu;
   short               loop = TRUE;
   CONTROL_PARAM       CntlInfo[3];
   int                 ret;

   ITEM mnuMain[] =
   {
       { 7, "Select Subscriber"},   
       { 7, "Select Group"     },   
       {-1, "-----------------"},   
       { 5, "Send Message"     },   
       { 5, "Send To Queue"    },   
       {-1, "-----------------"},   
       { 0, "Return      <Esc>"},   
       { 0, ""                 } 
   };
   
   ITEM mnuQueue[] =
   {
       { 0, "Remove"           },   
       { 0, "Clear"            },   
       { 0, "View"             },   
       { 0, "Send"             },   
       { 0, ""                 } 
   };
   
   ITEM mnuMessage[] =
   {
       { 0, "Select Predefined"},   
       { 0, "Clear"            },   
       { 0, ""                 } 
   };

   ITEMBAR mnuScreen[] =
   {
       { 7, 0,"File"       },   
       {10, 1,"Message"    },   
       {53, 0,"Queue"      },
       { 5, 0,"Help"       }
   };
   
   enum MENUS
   {
       SUB_FILE, MESSAGES, QUEUE, HELP
   };
   enum MENUS CurrSelection;

   Coord_List[0].x1 = 8;
   Coord_List[0].y1 = 0;
   Coord_List[0].x2 = 48;
   Coord_List[0].y2 = 0;
   Coord_List[0].hc = mnuScreen[0].achItem[mnuScreen[0].iHilite];

   Coord_List[1].x1 = 64;
   Coord_List[1].y1 = 0;
   Coord_List[1].x2 = 128;
   Coord_List[1].y2 = 0;
   Coord_List[1].hc = mnuScreen[3].achItem[mnuScreen[3].iHilite];
 
   Coord_List[2].x1 = 144;
   Coord_List[2].y1 = 0;
   Coord_List[2].x2 = 192;
   Coord_List[2].y2 = 0;
   Coord_List[2].hc = mnuScreen[3].achItem[mnuScreen[3].iHilite];
   
   Coord_List[3].x1 = 568;
   Coord_List[3].y1 = 0;
   Coord_List[3].x2 = 608;
   Coord_List[3].y2 = 0;
   Coord_List[3].hc = mnuScreen[3].achItem[mnuScreen[3].iHilite];
 
   Coord_List[4].x1 = -1;
   Coord_List[10].x1 = -1;

   SetSendControls(&CntlInfo[0], &Message_CntlInfo);
   CurrSelection = SUB_FILE;
   
   Initialize_State_Table();
   Keyboard = FALSE;
   AltKey = TRUE;

   while(loop == TRUE)
   {
       DrawSendBox();
       DrawControls(&CntlInfo[0], 3);
       DrawMultControls(&Message_CntlInfo, 1);
		ReDrawData(&CntlInfo[0], 3, _TBRIGHTWHITE, _TBLUE);
		ReDrawMultilineData(&Message_CntlInfo, 1, 12);

       while(smloop == TRUE)
       {
           switch(Process)
           {
               case 1:
               {
                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   KeyDown = TRUE;
                   sf = 0;
                   ret = (enum MENUS) MenuBar( 0, 0, mnuScreen, 4, -1, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   KeyDown = FALSE;

                   if(ret == -9)
                   {
                       sf = 1;
                       CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
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
                   MenuBar( 0, 0, mnuScreen, 4, -1, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);

                   Keyboard = TRUE;
                   AltKey = FALSE;
                   StatusBar("Enter page.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   CurrSelection = SUB_FILE;
		            ret = ProcessSendEdit(&Message_CntlInfo, &CntlInfo[0]);

                   if( ret == ESCAPE )
                   {
                       free(Message_CntlInfo.pMultiBuffer);
                       return;
                   }

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
                   MenuBar( 0, 0, mnuScreen, 4, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   smloop = FALSE;
               break;

               case 4:
                   sf = 0;
                   AltKey = FALSE;
                   CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
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

               Coord_List[13].x1 = 8;
               Coord_List[13].y1 = 48;
               Coord_List[13].x2 = 152;
               Coord_List[13].y2 = 48;

               Coord_List[14].x1 = 8;
               Coord_List[14].y1 = 64;
               Coord_List[14].x2 = 152;
               Coord_List[14].y2 = 64;

               Coord_List[15].x1 = -1;

	            SubMenu = Menu( 5, 10, mnuMain, SubMenu, 0);
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
                       CurrSelection= HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection= MESSAGES;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       sf = 1;
                       AltKey = FALSE;
                       SubMenu = 0;
                       Process = 3;
                       StatusBar("Select a Subscriber.", CENTER, 0, 25, _TBLACK, _TWHITE);
		                if((ret = ListSubscribers(&CurrentPage.SubscriberInfo, 3, 45, 30, 8)) >= 0)
                       {
		                    strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) CurrentPage.SubscriberInfo.szSubscriberName);   
                           sprintf(message,"Subscriber %s has been selected.", CntlInfo[0].pCntlBuffer);
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
                       SubMenu = 0;
                       Process = 3;
                       StatusBar("Select a Group.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListGroups(&GroupInfo, 15, 20, (int) 40, (int) 6)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) GroupInfo.szGroupName);
                           sprintf(message,"Group %s has been selected.", CntlInfo[0].pCntlBuffer);
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   break;
                   case 2:
                       CurrMenu = -1;
                   case 3:
                   {
                       if(CurrMenu >= 0)
                       {
                           Process = 3;
                           SubMenu = 0;
                           AddToQueue(&CntlInfo[0]);
                       }
                       else
                       {
                           BOOL MessageError = FALSE;
                           SubMenu = 0;
                           short index = 0;
                           Process = 3;
		                    if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
		                    {
                   	        if((strcmp(Message_CntlInfo.pMultiBuffer, "") != 0) || (strcmp(CntlInfo[2].pCntlBuffer, "") != 0))
		                        {
		                            if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
		                            {
		                                Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir);
		                                Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir);
		                                Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir);
                                                     
                                       strcpy(CurrentPage.SubscriberInfo.szSubscriberName, CntlInfo[0].pCntlBuffer);

                                       if(WapiInfo.dwOptions & FLG_MSG_RECALL_ENABLED)
                                       {
                                           if( Open_DBFile(&WapiInfo, MESSAGE_DB, WapiInfo.szPredefinedMsgs) == CALL_SUCCESS )
                                           {
                                               strcpy(MessageInfo.szTitle, "*LAST MESSAGE");

                                               if(Message_GetanyRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) != CALL_SUCCESS)
                                                   Message_InsertRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo, Message_CntlInfo.pMultiBuffer);
                                               else
                                                   if(Message_DeleteRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) == CALL_SUCCESS)
                                                       Message_InsertRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo, Message_CntlInfo.pMultiBuffer);

                                               Close_DBFile(&WapiInfo, MESSAGE_DB);
                                           }
                                       }

                                       if(Subscriber_GetanyRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &CurrentPage.SubscriberInfo) == CALL_SUCCESS)
                                       {
                                           char    szLastFile[128];
                                           HWAPIPAGEID wapiID;

                                           StatusBar("Sending a message.", CENTER, 0, 25, _TBLACK, _TWHITE);
                                           wapiID = CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL);
                                           strcpy(szLastFile, CurrentPage.szFileName);
                                           if(InsertMessage(CntlInfo[0].pCntlBuffer, &CntlInfo[2], szLastFile, wapiID) != TRUE)
                                               MessageError = TRUE;
                                           else
                                               index++;
                                       }
                                       else
                                       {
		                                    if(Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS)
                                           {
                                               Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo);
                                               do
                                               {
                                                   if(strcmp(GroupInfo.szGroupName, CntlInfo[0].pCntlBuffer) == 0)
                                                   {
                                                       char    szLastFile[128];
                                                       HWAPIPAGEID wapiID;

                                                       StatusBar("Adding messages to the queue.", CENTER, 0, 25, _TBLACK, _TWHITE);
                                                       wapiID = CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL);
                                                       strcpy(szLastFile, CurrentPage.szFileName);
                                                       if(InsertMessage(GroupInfo.szSubscriberName, &CntlInfo[2], szLastFile, wapiID) != TRUE)
                                                           MessageError = TRUE;

                                                       while((Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS) &&
                                                           (strcmp(CntlInfo[0].pCntlBuffer, GroupInfo.szGroupName) == 0))
                                                       {
                                                           if(InsertMessage(GroupInfo.szSubscriberName, &CntlInfo[2], szLastFile, CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL)) != TRUE)
                                                               MessageError = TRUE;
                                                           else
                                                               index++;
                                                       }

                                                       break;
                                                   }
                                               }
                                               while(Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS);

		                                        Close_DBFile(&WapiInfo, GROUP_DB);
                                           }
                                       }

                                       if(index == 1)
                                           sprintf(message, "%d message, was added to the queue.", index);
                                       else
                                           sprintf(message, "%d messages, were added to the queue.", index);
                                       StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);

		                                Close_DBFile(&WapiInfo, MODEM_DB);
		                                Close_DBFile(&WapiInfo, SERVICE_DB);
		                                Close_DBFile(&WapiInfo, SUBSCRIBER_DB);

                                       if(!MessageError)
                                       {
                                           doPet = FALSE;
                                           memset(&CurrentPage, 0, sizeof(WAPISPOOLERINFO));

                                           if ((ret = Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage)) == CALL_SUCCESS)
                                               doPet = TRUE;
                                           else
                                           {
                                               StatusBar("ERROR: No pages available.", CENTER, 0, 25, _TBLACK, _TWHITE);
                                               Beep(698, 700);
		                                    }

		                                    Close_DBFile(&WapiInfo, SPOOLER_DB);
		 
                                           // start state machine
                                           if ( doPet == TRUE )
                                           {
		                                        short Redials; 

                                               StatusBar("Paging, Please Wait ...", CENTER, 0, 25, _TBLACK, _TWHITE);
                                               FromPrompt = FALSE;
                                               Redial_Now = TRUE;
                                               StopProcess = FALSE;

                                               for(Redials = 0; (Redial_Now == TRUE) && (Redials < CurrentPage.ModemInfo.ModemInfo.byReDial_Count); Redials++)
      	                                            Do_PET(&CurrentPage);
       
                                               memset(&CurrentPage, 0, sizeof(WAPISPOOLERINFO));
                                           }
                                       }
                                       else
                                       {
                                           strcpy(message, "ERROR: Could not create a page.");
                                           Beep(698, 700);
                                       }
                                   }
                                   else
                                   {
                                       strcpy(message, "ERROR: Could not access Spooler database.");
                                       Beep(698, 700);
		                            }
                               }
                               else
                               {
                                       strcpy(message, "ERROR: Message text is missing.");
                                       Beep(698, 700);
                               }
		                    }
		                    else
                           {	                    
                                   strcpy(message, "ERROR: Subscriber or group name is missing.");
                                   Beep(698, 700);
	                        }
                       }
                   }
                   break;
                   case 4:
                       if(CurrMenu >= 0)
                       {
                           free(Message_CntlInfo.pMultiBuffer);
                           SubMenu = 0;
                           return;
                       }
                       else
                       {
                           Process = 3;
                           SubMenu = 0;
                           AddToQueue(&CntlInfo[0]);
                       }
                   break;

                   case 6:
                       free(Message_CntlInfo.pMultiBuffer);
                       SubMenu = 0;
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
           case MESSAGES:
           {
               Coord_List[10].x1 = 64;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 128;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 64;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 128;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = -1;

	            SubMenu = (enum CHOICES) Menu( 3, 17, mnuMessage, 0, MESSAGES);
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
                       CurrSelection = QUEUE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       sf = 1;
                       AltKey = FALSE;
                       Process = 3;
		                if((ret = ListPredefMessages(&MessageInfo, 3, 45, 30, 8)) >= 0)
                       {
                           int     nPDMHandle;
                           char    szBuffer[256];

                           if (WapiInfo.szPredefinedMsgs[strlen(WapiInfo.szPredefinedMsgs) - 1] != '\\')
      	                        strcat(WapiInfo.szPredefinedMsgs, "\\");

                           sprintf(szBuffer, "%s%s", WapiInfo.szPredefinedMsgs, MessageInfo.szFileName);
                           if((nPDMHandle = _open(szBuffer, _O_RDWR)) != -1)
                           {
                               long length = _filelength(nPDMHandle);
    
                               memset(Message_CntlInfo.pMultiBuffer, 0, (Message_CntlInfo.nCntlLength * Message_CntlInfo.nCntlLines));

                               if(length > (Message_CntlInfo.nCntlLength * Message_CntlInfo.nCntlLines))
                                   length = (Message_CntlInfo.nCntlLength * Message_CntlInfo.nCntlLines) - 1;

                               _read(nPDMHandle, (void *) Message_CntlInfo.pMultiBuffer, (unsigned) length);
                               
                               if(WapiInfo.dwOptions & FLG_LENGTH_COUNT)
                               {
                                   if(WapiInfo.dwOptions & FLG_SIGNATURE_ENABLE)
                                   {
                                       char szBuffer[256];
                                       char szBuffer2[256];

                                       if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == TRUE)
                                           GetProfileString("Signature", "DefaultSignature", NULL, szBuffer2, sizeof(szBuffer2), szBuffer);

                                       length += strlen(szBuffer2) + 1;
                                   }

                                   if(WapiInfo.dwOptions & FLG_TIMESTAMP_ENABLED)
                                       length += 12;
                               }
                               
                               itoa((int) length, CntlInfo[1].pCntlBuffer, 10);
                               close(nPDMHandle);
                               sprintf(message, "Selected predefined message %s.", MessageInfo.szFileName);
                           }
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }

                       DrawSendBox();
                       DrawControls(&CntlInfo[0], 3);
                       DrawMultControls(&Message_CntlInfo, 1);
		                ReDrawData(&CntlInfo[0], 3, _TBRIGHTWHITE, _TBLUE);
		                ReDrawMultilineData(&Message_CntlInfo, 1, 12);
                   break;
                   case 1:
                       Process = 3;
		                FreeControl(&CntlInfo[0], 2);
		                FreeMultilineControl(&Message_CntlInfo, 1);
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
           case QUEUE:
           {
               Coord_List[10].x1 = 144;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 192;
               Coord_List[10].y2 = 16;

               Coord_List[11].x1 = 144;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 192;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 144;
               Coord_List[12].y1 = 32;
               Coord_List[12].x2 = 192;
               Coord_List[12].y2 = 32;

               Coord_List[13].x1 = 144;
               Coord_List[13].y1 = 40;
               Coord_List[13].x2 = 192;
               Coord_List[13].y2 = 40;

               Coord_List[14].x1 = -1;

	            SubMenu = (enum CHOICES) Menu( 4, 22, mnuQueue, 0, 0);
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
                       CurrSelection = MESSAGES;
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
                       RemoveFromQueue();
                   break;
                   case 1:
                       Process = 3;
                       ClearQueue();
                   break;
                   case 2:
                       Process = 3;
                       ViewQueue();
                   break;
                   case 3: // send queue
                   {
                       Process = 3;
                       doPet = FALSE;
		                if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
                       {
                           if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) != CALL_SUCCESS)
                           {
                               strcpy(message, "No messages were found.");
                               StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);

                               Close_DBFile(&WapiInfo, SPOOLER_DB);
                           }
                           else
                           {
                               doPet = TRUE;
		                        Close_DBFile(&WapiInfo, SPOOLER_DB);
		                
                               // start state machine
                               if ( doPet == TRUE )
                               {
		                            short Redials; 

                                   StatusBar("Paging, Please Wait ...", CENTER, 0, 25, _TBLACK, _TWHITE);
                                   FromPrompt = FALSE;
                                   Redial_Now = TRUE;
                                   StopProcess = FALSE;

                                   for(Redials = 0; (Redial_Now == TRUE) && (Redials < CurrentPage.ModemInfo.ModemInfo.byReDial_Count); Redials++)
      	                                Do_PET(&CurrentPage);
       
                                   memset(&CurrentPage, 0, sizeof(WAPISPOOLERINFO));
                               }
                           }
                       }
                       else
                       {
                           strcpy(message, "ERROR : Could not access spooler database.");
                           Beep(698, 700);
                       }
                   }
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
                       CurrSelection = QUEUE;
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