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

BOOL FindSubscriber(ITEM *Member, int Count, LPSTR retBuffer);

extern struct videoconfig vc;
extern WAPIINFO            WapiInfo;
extern char                message[80];
extern int                 sf;
extern BOOL                KeyDown;
extern BOOL                AltKey;
extern ITEM                mnuHelp[3];
extern CONTROL_PARAM       CntlInfo[13];
extern WAPISPOOLERINFO     CurrentPage;
extern BOOL                Keyboard;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void Group()
{
   WAPIGROUPINFO   GroupInfo;
   char            buffer[40];    
   BOOL            smloop = TRUE;
   int             Process = 2;
   BOOL            ProcessOn = FALSE;
   BOOL            DrawOn = FALSE;
   long            SubMenu;
   short           loop = TRUE;
   ITEM            *Member, *NonMember;
   int             MemberCount = 0, NonMemberCount = 0;
   int             ret;

   ITEM mnuMain[] =
   {
       { 0, "Select Group"         },   
       { 0, "Add Subscriber"       },   
       { 1, "Remove Subscriber"    },   
       {-1, "-----------------"    },   
       { 0, "Return      <Esc>"    },   
       { 0, ""                     } 
   };
   
   ITEM mnuEdit[] =
   {
       { 0, "Save"     },   
       { 0, "Delete"   },   
       { 0, "Clear"    },   
       { 0, ""         } 
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

   if(SetGroupData(&Member, &NonMember, &MemberCount, &NonMemberCount) == FALSE)
       return;
   SetGroupControls(&CntlInfo[0]);
   CurrSelection = SUB_FILE;
   Keyboard = FALSE;
   AltKey = TRUE;

   while( loop == TRUE )
   {
       DrawGroupScreen();
       DrawControls(&CntlInfo[0], 1);
       ReDrawData(&CntlInfo[0], 1, _TBRIGHTWHITE, _TBLUE);
       ListBox( 10, 5, 30, 10, NonMember, NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
       ListBox( 10, 42, 30, 10, Member, MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
       sprintf(buffer, "Count : %03d", NonMemberCount);
       PrintText(buffer, LEFT, 28, 9, _TBRIGHTWHITE, _TBLUE);
       sprintf(buffer, "Count : %03d", MemberCount);
       PrintText(buffer, LEFT, 65, 9, _TBRIGHTWHITE, _TBLUE);


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
                   StatusBar("Enter group.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   CurrSelection = SUB_FILE;
                   ret = ProcessGroupEdit(&CntlInfo[0], Member, NonMember, &MemberCount, &NonMemberCount);

                   if( ret == ESCAPE )
                   {
                        free(NonMember);
                        free(Member);
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
               Coord_List[13].y1 = 48;
               Coord_List[13].x2 = 152;
               Coord_List[13].y2 = 48;

               Coord_List[14].x1 = -1;

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
                       CurrSelection = EDIT;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       sf = 1;
                       AltKey = FALSE;
                       Process = 3;
                       StatusBar("Select a Group.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if((ret = ListGroups(&GroupInfo, 3, 43, 31, 4)) >= 0)
                       {
                           strcpy((LPSTR) CntlInfo[0].pCntlBuffer, (LPCSTR) GroupInfo.szGroupName);
                           FillGroupListBox(Member, &MemberCount, "##ALL_NON_MEMBERS_INTERNAL##", NonMember, &NonMemberCount);
                           FillGroupListBox(Member, &MemberCount, CntlInfo[0].pCntlBuffer, NonMember, &NonMemberCount);
                           sprintf(message,"Group %s has been selected.", GroupInfo.szGroupName);
                       }
                       else
                       if(ret == ALT)
                       {
                           AltKey = TRUE;
                           Process = 2;
                       }
                   break;
                   case 1: //add
                   {
                       char retBuffer[32];
                       Process = 3;
                       if(NonMemberCount > 0)
                       {
                           if(ListEntries(NonMember, NonMemberCount, retBuffer, 10, 5, 30, 10) != NOT_SELECTED)
                           {
                               AddItem(Member, &MemberCount, retBuffer);
                               DeleteItem(NonMember, &NonMemberCount, retBuffer);
                           }
                       }
                   }
                   break;
                   case 2:  //remove
                   {
                       char retBuffer[32];
                       Process = 3;
                       StatusBar("Select group member to remove.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       if(MemberCount > 0)
                       {
                           if(ListEntries(Member, MemberCount, retBuffer, 10, 42, 30, 10) != NOT_SELECTED)
                           {
                               DeleteItem(Member, &MemberCount, retBuffer);
                               AddItem(NonMember, &NonMemberCount, retBuffer);
                           }
                       }
                   }
                   break;
                   case 3:
                   case 4:
                       free(NonMember);
                       free(Member);
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

                   case 0: // save
                   {
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           int GroupCount;
                           int index;

                           if( Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                           {
                               GroupCount = 0;
                               if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
                               {
                                   do
                                   {
                                       if(strcmp(GroupInfo.szGroupName, CntlInfo[0].pCntlBuffer) == 0)
                                           GroupCount++;
                                   }
                                   while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS));
                               }

                               while(GroupCount)
                               {
                                   StatusBar("Updating group...", CENTER, 0, 25, _TBLACK, _TWHITE);
                                   if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
                                   {
                                       do
                                       {
                                           if(strcmp(GroupInfo.szGroupName, CntlInfo[0].pCntlBuffer) == 0)
                                           {
                                               Group_DeleteRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo);
                                               break;
                                           }
                                       }
                                       while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS));
                                   }
                                   GroupCount--;
                               }

                               StatusBar("Saving group...", CENTER, 0, 25, _TBLACK, _TWHITE);
                               for(index = 0; index < MemberCount; index++)
                               {
                                   strcpy(GroupInfo.szGroupName, CntlInfo[0].pCntlBuffer);
                                   strcpy(GroupInfo.szSubscriberName, (Member + index)->achItem);
                                   if(Group_InsertRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) != CALL_SUCCESS)
                                   {
                                       StatusBar("ERROR : Could not insert a member.", CENTER, 0, 25, _TBLACK, _TWHITE);
                                       Beep(698, 700);
                                   }
                               }

                               strcpy(message, "Group was successfully saved.");
                               Close_DBFile(&WapiInfo, GROUP_DB);
                           }
                       }
                       else
                       {
                           strcpy(message, "ERROR : Group name is missing.");
                           Beep(698, 700);
                       }
                   }
                   break;

                   case 1: // delete
                   {
                       Process = 3;
                       if(strcmp(CntlInfo[0].pCntlBuffer, "") != 0)
                       {
                           int GroupCount;
                           int index;

                           if( Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
                           {
                               GroupCount = 0;
                               if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
                               {
                                   do
                                   {
                                       if(strcmp(GroupInfo.szGroupName, CntlInfo[0].pCntlBuffer) == 0)
                                           GroupCount++;
                                   }
                                   while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS));
                               }

                               while(GroupCount)
                               {
                                   StatusBar("Deleting group...", CENTER, 0, 25, _TBLACK, _TWHITE);
                                   if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
                                   {
                                       do
                                       {
                                           if(strcmp(GroupInfo.szGroupName, CntlInfo[0].pCntlBuffer) == 0)
                                           {
                                               Group_DeleteRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo);
                                               break;
                                           }
                                       }
                                       while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS));
                                   }
                                   GroupCount--;
                               }

                               Close_DBFile(&WapiInfo, GROUP_DB);
                       
                               memset(CntlInfo[0].pCntlBuffer, 0, sizeof(CntlInfo[0].pCntlBuffer));
                               for(index = 0; index < MemberCount; index++)
                                   memset((Member + index)->achItem, 0, sizeof((Member + index)->achItem));
                               MemberCount = 0;

                               strcpy(message, "Group was successfully deleted.");
                           }
                       }
                       else
                       {
                           strcpy(message,"ERROR : Group name is missing.");
                           Beep(698, 700);
                       }
                   }
                   break;

                   case 2: // clear
                   {
                       int index;

                       Process = 3;
                       memset(CntlInfo[0].pCntlBuffer, 0, sizeof(CntlInfo[0].pCntlBuffer));
                       for(index = 0; index < MemberCount; index++)
                           memset((Member + index)->achItem, 0, sizeof((Member + index)->achItem));
                       MemberCount = 0;
                       for(index = 0; index < NonMemberCount; index++)
                           memset((NonMember + index)->achItem, 0, sizeof((NonMember + index)->achItem));
                       NonMemberCount = 0;
                       FillGroupListBox(Member, &MemberCount, "##ALL_NON_MEMBERS_INTERNAL##", NonMember, &NonMemberCount);
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
int ProcessGroupEdit(PCONTROL_PARAM pCntlInfo, ITEM *Member, ITEM *NonMember, int *MemberCount, int *NonMemberCount)
{
   WAPIGROUPINFO   GroupInfo;
   char            buffer[40];    
   int             ret;
   PCONTROL_PARAM  CurrControl;
   int             next = 0;
   int             retVal;
   int             Stop = FALSE;
   short           loop = 1;
   short           smloop = 1;
   char            retBuffer[50];
   int             iCur = 0;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);

   CurrControl = pCntlInfo + next;  
   
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
                       smloop = 0;
                   break;

                   case SHIFT_TAB:
                       next = 2;
                       smloop = 0;
                   break;

                   case ESCAPE:
                       return ESCAPE;
                   break;

                   case LIST:
                   {        
                       StatusBar("Select a Group.", CENTER, 0, 25, _TBLACK, _TWHITE);
                       ProcessControl(CurrControl, _TBRIGHTWHITE, _TBLUE, HILIGHT);

                       if((ret = ListGroups(&GroupInfo, 3, 43, 31, 4)) >= 0)
                       {
                           strcpy((LPSTR) CurrControl->pCntlBuffer, (LPCSTR) GroupInfo.szGroupName);
                           FillGroupListBox(Member, MemberCount, "##ALL_NON_MEMBERS_INTERNAL##", NonMember, NonMemberCount);
                           FillGroupListBox(Member, MemberCount, CurrControl->pCntlBuffer, NonMember, NonMemberCount);
                       }
                       else
                       if(ret == ALT)
                       {
                           DrawGroupScreen();
                           DrawControls(pCntlInfo, 1);
                           ReDrawData(pCntlInfo, 1, _TBRIGHTWHITE, _TBLUE);
                           ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                           ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                           sprintf(buffer, "Count : %03d", *NonMemberCount);
                           PrintText(buffer, LEFT, 28, 9, _TBRIGHTWHITE, _TBLUE);
                           sprintf(buffer, "Count : %03d", *MemberCount);
                           PrintText(buffer, LEFT, 65, 9, _TBRIGHTWHITE, _TBLUE);
                           return ALT;
                       }
                       DrawGroupScreen();
                       DrawControls(pCntlInfo, 1);
                       ReDrawData(pCntlInfo, 1, _TBRIGHTWHITE, _TBLUE);
                       ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                       ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                       sprintf(buffer, "Count : %03d", *NonMemberCount);
                       PrintText(buffer, LEFT, 28, 9, _TBRIGHTWHITE, _TBLUE);
                       sprintf(buffer, "Count : %03d", *MemberCount);
                       PrintText(buffer, LEFT, 65, 9, _TBRIGHTWHITE, _TBLUE);
                       StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);
                   }
                   break;
               }
           }
           smloop = 1;
       }

       if ( next == 1 )
       {
           iCur = 0;
           while(smloop == 1)
           {
               // member list box
               retVal = ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, iCur, HILIGHT|COPY_SELECTION, retBuffer);
           
               if( (CurrMenu >= 0) )
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                   {
                       ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                       return ALT;
                   }
                   case TAB:
                       next = 2;
                       smloop = 0;
                       ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                   break;
                   case SHIFT_TAB:
                       next = 0;
                       smloop = 0;
                       ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                   default:
                       if(retVal >= 0)
                       {
                           if(*NonMemberCount > 0)
                           {
                               char buffer[40];    

                               AddItem(Member, MemberCount, retBuffer);
                               ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                               DeleteItem(NonMember, NonMemberCount, retBuffer);
                               //ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, iCur, DRAW_ONLY|NO_HILIGHT, NULL);
                               sprintf(buffer, "Count : %03d", *NonMemberCount);
                               PrintText(buffer, LEFT, 28, 9, _TBRIGHTWHITE, _TBLUE);
                               sprintf(buffer, "Count : %03d", *MemberCount);
                               PrintText(buffer, LEFT, 65, 9, _TBRIGHTWHITE, _TBLUE);
                               if(((retVal - 1) == 0) || (retVal == 0))
                                   iCur = 0;
                               else
                                   if(retVal == (*NonMemberCount))
                                       iCur = (retVal - 1);
                                   else
                                       iCur = retVal;
                           }
                       }
                   break;
               }
           }
           smloop = 1;
       }

       if(next == 2)
       {
           iCur = 0;
           while(smloop == 1)
           {
               // non-member list box
               retVal = ListBox( 10, 42, 30, 10, Member, *MemberCount, iCur, HILIGHT|COPY_SELECTION, retBuffer);
           
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                   {
                       ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                       return ALT;
                   }
                   case TAB:
                       next = 0;
                       smloop = 0;
                       ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                   break;
                   case SHIFT_TAB:
                       next = 1;
                       smloop = 0;
                       ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                   default:
                       if(retVal >= 0)
                       {
                           if(*MemberCount > 0)
                           {
                               char buffer[40];
    
                               DeleteItem(Member, MemberCount, retBuffer);
                               //ListBox( 10, 42, 30, 10, Member, *MemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                               AddItem(NonMember, NonMemberCount, retBuffer);
                               ListBox( 10, 5, 30, 10, NonMember, *NonMemberCount, 0, DRAW_ONLY|NO_HILIGHT, NULL);
                               sprintf(buffer, "Count : %03d", *MemberCount);
                               PrintText(buffer, LEFT, 65, 9, _TBRIGHTWHITE, _TBLUE);
                               sprintf(buffer, "Count : %03d", *NonMemberCount);
                               PrintText(buffer, LEFT, 28, 9, _TBRIGHTWHITE, _TBLUE);
                               if(((retVal - 1) == 0) || (retVal == 0))
                                   iCur = 0;
                               else
                                   if(retVal == (*MemberCount))
                                       iCur = (retVal - 1);
                                   else
                                       iCur = retVal;
                           }
                       }
                   break;
               }
           }
           smloop = 1;
       }
   }
                       
   return ESCAPE;
}

