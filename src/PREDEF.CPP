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
extern BOOL                Keyboard;

extern CONTROL_PARAM       CntlInfo[13];
extern MULT_CONTROL_PARAM  Message_CntlInfo;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void Predefined()
{
   WAPIMESSAGEINFO    MessageInfo;
   BOOL                ProcessOn = FALSE;
   BOOL                DrawOn = FALSE;
   BOOL                smloop = TRUE;
   int                 Process = 2;
   long                SubMenu;
   short               loop = TRUE;
   int                 ret;
   
   ITEM mnuMain[] =
   {
       { 0, "Select Message"   },   
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
                    
   SetPredefControls(&CntlInfo[0], &Message_CntlInfo);
   CurrSelection = SUB_FILE;
   Keyboard = FALSE;
   AltKey = TRUE;

   while( loop == TRUE )
   {
       DrawPredefinedScreen();
       DrawControls(&CntlInfo[0], 2);
       DrawMultControls(&Message_CntlInfo, 1);
		ReDrawData(&CntlInfo[0], 2, _TBRIGHTWHITE, _TBLUE);
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
                   CurrSelection = SUB_FILE;
                   StatusBar("Enter predefined message.", CENTER, 0, 25, _TBLACK, _TWHITE);
		            ret = ProcessPredefEdit(&Message_CntlInfo, &CntlInfo[0]);

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
               Coord_List[11].y1 = 32;
               Coord_List[11].x2 = 128;
               Coord_List[11].y2 = 32;

               Coord_List[12].x1 = -1;

	            SubMenu = (enum CHOICES) Menu( 3, 9, mnuMain, 0, SUB_FILE);
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
                       StatusBar("Select a predefined message.", CENTER, 0, 25, _TBLACK, _TWHITE);
		                if((ret = ListPredefMessages(&MessageInfo, 3, 45, 30, 8)) >= 0)
                       {
                           int     nPDMHandle;
                           char    szBuffer[256];

		                    strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) MessageInfo.szTitle);   
                           
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
                               itoa((int) length, CntlInfo[1].pCntlBuffer, 10);
                               close(nPDMHandle);
                           }
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   break;

                   case 1:
                   case 2:
                       free(Message_CntlInfo.pMultiBuffer);
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
                           if(strcmp(Message_CntlInfo.pMultiBuffer, "") != 0)
                           {
                               if( Open_DBFile(&WapiInfo, MESSAGE_DB, WapiInfo.szPredefinedMsgs) == CALL_SUCCESS )
                               {
                                   strcpy(MessageInfo.szTitle, CntlInfo[0].pCntlBuffer);

                                   if(Message_GetanyRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) != CALL_SUCCESS)
                                   {
                                       if(Message_InsertRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo, Message_CntlInfo.pMultiBuffer) != CALL_SUCCESS)
                                       {
                                           strcpy(message,"ERROR: Message was not saved.");
                                           Beep(698, 700);
                                       }
                                       else
                                           sprintf(message,"Message %s was saved.", MessageInfo.szTitle);
                                   }
                                   else
                                       if(Message_DeleteRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) == CALL_SUCCESS)
                                       {
                                           if(Message_InsertRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo, Message_CntlInfo.pMultiBuffer) != CALL_SUCCESS)
                                           {
                                               strcpy(message,"ERROR: Message was not updated.");
                                               Beep(698, 700);
                                           }
                                           else
                                               sprintf(message,"Message %s was updated.", MessageInfo.szTitle);
                                       }

                                   Close_DBFile(&WapiInfo, MESSAGE_DB);
                               }
                           }
                           else
                           {
                               strcpy(message,"ERROR : Message text is required.");
                               Beep(698, 700);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Message name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 1:
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           if( Open_DBFile(&WapiInfo, MESSAGE_DB, WapiInfo.szPredefinedMsgs) == CALL_SUCCESS )
                           {
                               strcpy(MessageInfo.szTitle, CntlInfo[0].pCntlBuffer);
                               if(Message_GetanyRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) == CALL_SUCCESS)
                               {
                                   if(Message_DeleteRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) != CALL_SUCCESS)
                                   {
                                       strcpy(message,"ERROR: Message was not deleted.");
                                       Beep(698, 700);
                                   }
                                   else
                                   {
                                       sprintf(message,"Message %s was deleted.", MessageInfo.szTitle);
		                                FreeControl(&CntlInfo[0], 2);
		                                FreeMultilineControl(&Message_CntlInfo, 1);
                                   }
                               }
                               Close_DBFile(&WapiInfo, MESSAGE_DB);
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Message name is missing.");
                           Beep(698, 700);
                       }
                   break;
                   case 2:
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
int ProcessPredefEdit(PMULT_CONTROL_PARAM pMessage_CntlInfo, PCONTROL_PARAM pCntlInfo)
{  
   WAPIMESSAGEINFO    MessageInfo;
   int  ret;
   int  loop = 1;
   int  smloop = 1;
   int  next = 0;
   int  retVal;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);

   _displaycursor( _GCURSOROFF );
   
   while(loop == 1)
   {
       if ( next == 0 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl(pCntlInfo, _TBRIGHTWHITE, _TBLUE, PROCESS);
           
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 1;
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 1;
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                   case LIST:
                       StatusBar("Select a predefined message.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(pCntlInfo, _TBRIGHTWHITE, _TBLUE, HILIGHT);
		                if((ret = ListPredefMessages(&MessageInfo, 3, 45, 30, 8)) >= 0)
                       {
                           int     nPDMHandle;
                           char    szBuffer[256];

		                    strcpy((LPSTR) pCntlInfo->pCntlBuffer, (LPCSTR) MessageInfo.szTitle);   
                           
                           if (WapiInfo.szPredefinedMsgs[strlen(WapiInfo.szPredefinedMsgs) - 1] != '\\')
      	                        strcat(WapiInfo.szPredefinedMsgs, "\\");

                           sprintf(szBuffer, "%s%s", WapiInfo.szPredefinedMsgs, MessageInfo.szFileName);
                           if((nPDMHandle = _open(szBuffer, _O_RDWR)) != -1)
                           {
                               long length = _filelength(nPDMHandle);
    
                               memset(pMessage_CntlInfo->pMultiBuffer, 0, (pMessage_CntlInfo->nCntlLength * pMessage_CntlInfo->nCntlLines));

                               if(length > (pMessage_CntlInfo->nCntlLength * pMessage_CntlInfo->nCntlLines))
                                   length = (pMessage_CntlInfo->nCntlLength * pMessage_CntlInfo->nCntlLines) - 1;

                               _read(nPDMHandle, (void *) pMessage_CntlInfo->pMultiBuffer, (unsigned) length);
                               itoa((int) length, (pCntlInfo + 1)->pCntlBuffer, 10);
                               close(nPDMHandle);
                           }

                       }
                       else
                       if(ret == ALT)
                       {
                           DrawPredefinedScreen();
                           DrawControls(pCntlInfo, 2);
                           DrawMultControls(pMessage_CntlInfo, 1);
		                    ReDrawData(pCntlInfo, 2, _TBRIGHTWHITE, _TBLUE);
		                    ReDrawMultilineData(pMessage_CntlInfo, 1, 12);
                           return ALT;
                       }
                       DrawPredefinedScreen();
                       DrawControls(pCntlInfo, 2);
                       DrawMultControls(pMessage_CntlInfo, 1);
		                ReDrawData(pCntlInfo, 2, _TBRIGHTWHITE, _TBLUE);
		                ReDrawMultilineData(pMessage_CntlInfo, 1, 12);
                       StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 1 )
       {
           while(smloop == 1)
           {
               retVal = ProcessMultiLineControl(pMessage_CntlInfo, pMessage_CntlInfo->nCntlLines, (pCntlInfo + 1), _TBRIGHTWHITE, _TBLUE, 0);
           
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 0;
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 0;
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

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawPredefinedScreen()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   Box( 8, 3, 12, vc.numtextcols - 7, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText(" Predefined Edit ", CENTER, 0, 2, _TBLUE, _TWHITE);
   PrintText("Message Text:", LEFT, 4, 7, _TWHITE, _TBLUE);
}
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
int ListPredefMessages(LPWAPIMESSAGEINFO pMessageInfo, short row, short col, int maxWidth, short maxEntries)
{       
   ITEM *pSubList = NULL;
   int index = 0, offset = NOT_SELECTED;
   int Height;
   int Overflow = 1;
   char buffer[50];

   if(!pMessageInfo)
      return ESCAPE;

   if((pSubList = (ITEM *) malloc( MAX_MESSAGES * sizeof(ITEM) )) == NULL)
      return ESCAPE;

   StatusBar("Loading Predefined Messages Database...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if( Open_DBFile(&WapiInfo, MESSAGE_DB, WapiInfo.szPredefinedMsgs) == CALL_SUCCESS )
   {
      if( Message_GetfirstRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) pMessageInfo) == CALL_SUCCESS )
      {
         do
         {
            pSubList[index].iHilite = -1;
            strcpy((LPSTR) pSubList[index].achItem, (LPCSTR) pMessageInfo->szTitle);

            Overflow++;
            index++;
         }
         while ( (Message_GetnextRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) pMessageInfo) == CALL_SUCCESS) && (Overflow < MAX_MESSAGES ));

         if( index > maxEntries )
            Height = maxEntries;
         else
            Height = index;

         StatusBar("Select predefined message.", LEFT, 34, 25, _TBLACK, _TWHITE);

         sprintf(buffer, " Total # of messages : %d ", index);
         PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

         offset = ListBox( row, col, maxWidth, Height, pSubList, index, 0, HILIGHT, NULL);
         StatusBar("", CENTER, 34, 25, _TBLACK, _TWHITE);

         if ( offset < 0 )
         {
            free(pSubList);
            Close_DBFile(&WapiInfo, MESSAGE_DB);
            return offset;
         }

         strcpy((LPSTR) pMessageInfo->szTitle, (LPCSTR) pSubList[offset].achItem);
         
         if(Message_GetanyRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) pMessageInfo) != CALL_SUCCESS)
         {
            Close_DBFile(&WapiInfo, MESSAGE_DB);
            free(pSubList);
            return ESCAPE;
         }
      }
      else
      {
           offset = ESCAPE;
           strcpy(message, "ERROR : No predefined messages were found.");
      }

      Close_DBFile(&WapiInfo, MESSAGE_DB);
   }
   else
   {
      offset = ESCAPE;
      strcpy(message, "ERROR : Could not open predefined message database.");
   }

   free(pSubList);          

   return (WORD) offset;    
}

